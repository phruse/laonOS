// SPDX-License-Identifier: MIT

module;

#include "protocol.h"

export module test.connect.protocol;
export import test.format;

import std;

import test.connect.call;
import test.argument;

export template <std::size_t size = 255> class test_info {
public:
  test_info() = default;
  explicit test_info(const std::string_view &str, std::size_t index)
      : index(index) {
    set(str);
  }

  auto operator<=>(const test_info &rhs) const {
    return get_index() <=> rhs.get_index();
  }

  operator std::string() const {
    return std::format("{}, {}", underscore_to_space(get_group()),
                       underscore_to_space(get_name()));
  }

  void set(const std::string_view &str) noexcept {
    std::sscanf(str.data(), TEST_INFO_FORMAT, name, group, file,
                reinterpret_cast<int *>(&isolation));
  }

  [[nodiscard]] auto get_index() const noexcept { return index; }

  [[nodiscard]] auto get_name() const noexcept {
    return std::string_view{name};
  }
  [[nodiscard]] auto get_group() const noexcept {
    return std::string_view{group};
  }
  [[nodiscard]] auto get_file_name() const noexcept {
    return std::string_view{file};
  }

  [[nodiscard]] auto is_isolation() const noexcept { return isolation; }
  auto make_isolation() noexcept { isolation = true; }

private:
  std::size_t index = 0;

  char name[size]{};
  char group[size]{};
  char file[size]{};
  bool isolation = false;

  static auto underscore_to_space(const std::string_view &input) {
    return std::regex_replace(input.data(), std::regex("-"), " ");
  }
};

using info_data = test_info<225>;
using info_vector = std::vector<test_info<225>>;

export class test_result {
public:
  test_result() = default;
  explicit test_result(const std::string_view &str) { set(str); }

  void set(const std::string_view &str) noexcept {
    bool first = true;
    constexpr auto prefix = std::string_view{TEST_RESULT_SPLIT};
    for (const auto &data : std::views::split(str, prefix)) {
      auto word = std::string_view{data};

      if (data.empty() || first) {
        first = false;
        output = word;
        continue;
      }

      auto code = 0, line = 0;
      std::sscanf(word.data(), TEST_RESULT_FORMAT,
                  reinterpret_cast<int *>(&code),
                  reinterpret_cast<int *>(&line));
      code--;

      if (code < TEST_EXPECT_ENTRY) {
        success = false;
        code -= TEST_ASSERT_ENTRY;

        if ((sizeof(assert_code_map) / sizeof(*assert_code_map)) >=
            static_cast<unsigned long>(code))
          results.emplace_back(assert_code_map[code], line);
      } else {
        code -= TEST_EXPECT_ENTRY;

        if ((sizeof(expect_code_map) / sizeof(*expect_code_map)) >=
            static_cast<unsigned long>(code))
          results.emplace_back(expect_code_map[code], line);
      }
    }
  }

  [[nodiscard]] auto &get_results() const { return results; }
  [[nodiscard]] auto get_output() const { return output; }

  [[nodiscard]] auto is_success() const { return success; }

private:
  std::list<std::pair<std::string, std::size_t>> results{};
  std::optional<std::string_view> output{};

  bool success = true;
};

struct test_option {
  bool isolation = false;
};

class test_builder {
  using info_reference_set =
      std::set<std::reference_wrapper<const info_data>, std::less<info_data>>;
  using runner_delegate = std::function<int(test_builder &builder)>;

public:
  test_builder(const info_vector &info_data, const runner_delegate &runner_func)
      : info(info_data), runner(runner_func) {}

  [[nodiscard]] auto &get_order() const { return order; }

  auto run() { return runner(*this); }

  auto &add_all() {
    std::ranges::for_each(info, [&](const auto &data) { order.insert(data); });
    return *this;
  }

  auto &add_range(std::pair<std::size_t, std::size_t> range) {
    if (range.second > info.size()) {
      argument_message::warning(std::format("Out of range, {} to {} (max {}).",
                                            range.first, range.second,
                                            info.size()));
    }

    for (auto i = range.first - 1; i < range.second; ++i) {
      order.emplace(info[i]);
    }

    return *this;
  }

  auto &add_group(const std::vector<std::string> &groups) {
    string_filter(
        groups, [&](auto &data, auto &str) { return data.get_group() == str; });

    return *this;
  }

  auto &add_group(const std::initializer_list<std::string> &&groups) && {
    return add_group(std::move(groups));
  }

  auto &add_name(const std::vector<std::string> &names) {
    string_filter(
        names, [&](auto &data, auto &str) { return data.get_name() == str; });

    return *this;
  }

  auto &add_name(const std::initializer_list<std::string> &&names) && {
    return add_name(std::move(names));
  }

  auto &make_isolation() noexcept {
    options.isolation = true;
    return *this;
  }

  const auto &get_option() noexcept { return options; }

private:
  const info_vector &info;
  test_option options{};

  info_reference_set order{};
  runner_delegate runner{};

  inline void string_filter(const std::vector<std::string> &list,
                            const auto &checker) {
    auto ignore_list = list;
    std::ranges::for_each(
        info | std::views::filter([&](auto &data) {
          return std::ranges::any_of(list, [&](auto &str) {
            auto is = checker(data, str);
            if (is) {
              ignore_list.erase(
                  std::remove(ignore_list.begin(), ignore_list.end(), str),
                  ignore_list.end());
            }

            return is;
          });
        }),
        [&](auto &data) { order.emplace(data); });

    if (!ignore_list.empty()) {
      argument_message::warning(
          std::format("Some values are ignored. {}", ignore_list));
    }
  }
};

export template <class Formatter>
  requires formatter<Formatter>
class test_runner {
public:
  test_runner(basic_call &test_caller) : caller(test_caller) {
    auto count_str = caller.call(CALL_COUNT);
    if (auto result = std::from_chars(
            count_str.data(), count_str.data() + count_str.size(), counter);
        result.ec == std::errc::invalid_argument) {
      throw call_error("get test count");
    }

    for (auto i = 0u; i < counter; ++i) {
      auto [string, _] = call_number(CALL_INFO, i);
      info.emplace_back(string, i);
    }
  }

  auto build() {
    return test_builder{info, [&](auto &builder) {
                          const auto &order = builder.get_order();
                          formatter.start(order.size());

                          auto return_value = 0;
                          return_value += run(builder, false);
                          return_value += run(builder, true);

                          formatter.end();
                          return return_value;
                        }};
  }

private:
  basic_call &caller;
  Formatter formatter;

  std::size_t counter;
  info_vector info{};

  inline auto run(test_builder builder, bool is_isolation) {
    const auto &order = builder.get_order();
    const auto &options = builder.get_option();

    auto return_value = 0;

    for (const auto &info : order | std::views::filter([&](const auto &data) {
                              return is_isolation ? data.get().is_isolation()
                                                  : !data.get().is_isolation();
                            })) {
      const auto &info_data = info.get();
      const auto [return_result, duration] =
          call_number(CALL_TEST, info_data.get_index());

      const test_result result{return_result};

      format_detail_pair details{};
      details.emplace_back("name", info_data.get_name());
      details.emplace_back("group", info_data.get_group());
      details.emplace_back("file", info_data.get_file_name());
      details.emplace_back("isolation", is_isolation ? "true" : "false");
      details.emplace_back("duration", std::to_string(duration) + "ms");

      if (auto output = result.get_output(); output) {
        details.emplace_back(
            "output",
            std::format("'{}'", std::regex_replace(std::string{*output},
                                                   std::regex("\\\n"), "\\n")));
      }

      auto at_counter = 0l;
      details.insert_range(
          details.end(),
          result.get_results() |
              std::ranges::views::transform([&at_counter](const auto &data) {
                return std::make_pair(std::format("at {}", ++at_counter),
                                      std::format("\ntype: {}\nline: {}",
                                                  data.first,
                                                  std::to_string(data.second)));
              }));

      formatter.test(
          format_data{result.is_success(), info_data, std::move(details)});

      if (!result.is_success()) {
        return_value--;
      }

      if (is_isolation || options.isolation) {
        caller.call(CALL_REBOOT);
        caller.wait_call();
      }
    }

    return return_value;
  }

  [[nodiscard]] std::pair<std::string_view, long long>
  call_number(call_code_e code, std::size_t index) const {
    auto begin = std::chrono::steady_clock::now();
    auto test_result = caller.call(code, std::to_string(index + 1) + '\0');
    auto end = std::chrono::steady_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
            .count();

    return {test_result, duration};
  }
};
