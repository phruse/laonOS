// SPDX-License-Identifier: MIT

module;

export module test.format;

import std;

export using format_detail_pair =
    std::vector<std::pair<std::string, std::string>>;

export struct format_data {
  format_data(const bool is_success, const std::string &&info_str,
              const format_detail_pair &&details_str)
      : sucess(is_success), info(std::move(info_str)),
        details(std::move(details_str)) {}

  const bool sucess;
  const std::string &info;
  const format_detail_pair &details;
};

export class format {
public:
  virtual void start(std::size_t count) = 0;
  virtual void test(const format_data &data) = 0;
  virtual void end() = 0;
};

export template <class T>
concept formatter = std::derived_from<T, format>;

export class tab : public format {
public:
  void start(std::size_t count) override {
    constexpr auto tap_start = "TAP version 14\n";
    std::cout << tap_start << std::format("1..{}\n", count) << std::endl;
  };

  void test(const format_data &data) override {
    counter++;

    std::stringstream ss;

    if (data.sucess) {
      ss << "ok ";
    } else {
      ss << "not ok ";
    }

    ss << counter;
    ss << " - " << data.info << '\n';

    ss << "  ---\n";
    for (const auto &[key, value] : data.details) {
      auto new_value = std::regex_replace(value, std::regex("\\\n"), "\n    ");
      ss << "  " << std::format("{}: {}", key, quoted(new_value)) << '\n';
    }
    ss << "  ...\n";

    std::cout << ss.str() << std::endl;
  };

  void end() override{};

private:
  std::size_t counter = 0;

  static std::string quoted(const std::string &str) {
    return str.find_first_not_of(" ") == std::string::npos ? "'" + str + "'"
                                                           : str;
  }
};
