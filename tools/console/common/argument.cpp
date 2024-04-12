// SPDX-License-Identifier: MIT

module;

export module test.argument;

import std;

export class argument_message : public std::exception {
public:
  explicit argument_message(std::string msg, std::string arg_name)
      : message(std::move(msg)), name(std::move(std::move(arg_name))),
        format(message + ": " + name + "\n\nPlease use -h | --help") {}

  [[nodiscard]] const char *what() const noexcept override {
    return format.c_str();
  }

  [[nodiscard]] const std::string &get_name() const { return name; }
  [[nodiscard]] const std::string &get_message() const { return message; }

  static auto warning(std::string_view &&str) {
    std::cout << "Warning: " << std::move(str) << "\n";
  }

private:
  const std::string message, name, format;
};

using option_data = std::optional<std::string>;
using option_map = std::unordered_map<std::string, option_data>;
export class argument_options : public option_map {
public:
  static constexpr auto empty = "empty";

  auto gets(const std::initializer_list<std::string> &arguments,
            bool additional = true) {
    std::string return_string{};
    auto duple_counter = 0ul;

    for (const auto &arg : arguments) {
      if (option_map::contains(arg)) {
        if (additional) {
          if (const auto &value = option_map::operator[](arg); value) {
            return_string = *value;
          } else {
            throw argument_message("This option requires an additional value",
                                   arg);
          }
        } else {
          return_string = empty;
        }

        option_map::erase(arg);
        duple_counter++;
      }
    }

    if (duple_counter > 1) {
      argument_message::warning(
          std::format("Acronym option is ignored: {}", arguments));
    }

    return std::make_pair(!return_string.empty(), return_string);
  }

  auto get(const std::string &argument) { return gets({argument}); }

  auto get_raw(const std::string &argument) {
    auto return_string = option_map::operator[](argument);
    if (option_map::count(argument) == 1) {
      option_map::erase(option_map::find(argument), option_map::end());
    }

    return return_string;
  }

  auto check_unused() {
    if (!option_map::empty()) {
      std::string keys{};
      for (auto const &kv : static_cast<option_map>(*this))
        keys += kv.first + ", ";

      keys.pop_back();
      keys.pop_back();

      argument_message::warning("Unknown arguments: " + keys);
    }
  }
};

export class argument {
  using command_callback = std::function<std::string(argument_options &)>;
  using string_deque = std::deque<std::string_view>;
  using option_result = std::pair<std::string, bool>;

public:
  argument(int argc, char *argv[])
      : argument_list(argument_convert(argc, argv)) {}

  void add_command(const std::initializer_list<std::string> &&command_list,
                   const command_callback &callback) noexcept {
    for (const auto &str : command_list) {
      call_list.emplace(str, callback);
    }
  }

  [[nodiscard]] int run() {
    // path
    argument_list.pop_front();

    // command
    const auto &command_name = std::string(argument_list.front());
    if (!call_list.contains(command_name)) {
      throw argument_message("Command not found", command_name);
    }
    argument_list.pop_front();

    std::size_t counter = 0;
    argument_options options;
    option_result previous;
    for (const auto &arg :
         argument_list | std::views::transform(remove_front_hyphens)) {
      auto &[str, is_option] = arg;
      counter++;

      if (is_option) {
        options.emplace(str, std::nullopt);
        previous = arg;
        continue;
      }

      if (previous.second) {
        options[previous.first] = std::make_optional(str);
        previous = {};
        continue;
      }

      options["argument" + std::to_string(counter)] = std::make_optional(str);
    }

    if (const auto &out = call_list[command_name](options); out != "") {
      throw argument_message(out, command_name);
    } else {
      return 0;
    }
  }

private:
  string_deque argument_list;
  std::unordered_map<std::string, command_callback> call_list;

  static string_deque argument_convert(std::size_t size, char *array[]) {
    string_deque span_view;
    std::ranges::transform(std::span<char *>{array, size},
                           std::back_inserter(span_view),
                           [](auto ptr) { return std::string_view(ptr); });
    return span_view;
  }

  static option_result remove_front_hyphens(std::string_view &input) {
    std::regex regex{"^(-{1,2})"};
    auto trans = std::regex_replace(input.data(), regex, "");
    return {trans, trans != input};
  }
};
