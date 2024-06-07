import std;

import test.connect.tty;
import test.connect.protocol;
import test.argument;

import test.process.qemu;

constexpr auto os_path = "bin/debug/laonOS-boot-test.iso";

int main(int argc, char *argv[]) {
  std::set_terminate([]() {
    try {
      std::rethrow_exception(std::current_exception());
    } catch (const std::exception &err) {
      std::cerr << err.what() << std::endl;
    }

    std::abort();
  });

  auto arg = argument{argc, argv};

  arg.add_command({"-h", "--help"}, [](auto &) {
    constexpr auto info_string =
        "OVERVIEW: test console\n\n"
        "USAGE: test-console mode [options...]\n\n"
        "MODS:\n"
        "unit|u [OS file]\n"
        "\tRun the unit test using QEMU. Output in TAB14 format.\n\n"

        "OPTIONS:\n"
        "--isolation|-I\n"
        "--range|-r (start),(end)\n"
        "--group|-g (group name),...\n"
        "--name|-n (test name),...\n\n"
        "--qemu-path (path)\n"
        "--qemu-options (arg)";

    std::cout << info_string << std::endl;

    return "";
  });

  arg.add_command({"unit", "u"}, [](auto &options) -> std::string {
    qemu_option option{};

    if (const auto &file = options.get_raw("argument1"); file) {
      option.os_file = *file;
    } else if (std::filesystem::exists(os_path)) {
      option.os_file = os_path;
    } else {
      return "No input OS file";
    }

    if (const auto &[is, qemu_path] = options.get("qemu-path"); is) {
      option.path = qemu_path;
    }
    if (const auto &[is, qemu_options] = options.get("qemu-options"); is) {
      option.options = qemu_options;
    }

    auto is_ranges = false;
    std::array<std::size_t, 2> ranges{0};
    if (const auto [is, range_string] = options.gets({"range", "r"}); is) {
      auto counter = 0ul;

      using std::operator""sv;
      for (const auto &range : std::views::split(range_string, ","sv)) {
        if (counter > 1) {
          return "Too many range arguments";
        }

        auto i = 0;
        auto result =
            std::from_chars(range.data(), range.data() + range.size(), i);
        if (result.ec == std::errc::invalid_argument) {
          return "Range must be integer";
        }

        ranges[counter] = i;
        counter++;
      }

      if (ranges[0] <= 0 || ranges[1] <= 0) {
        return "Range value must be greater than 0";
      }

      is_ranges = true;
    }

    auto string_argument_helper =
        [&](const std::initializer_list<std::string> &names) {
          std::vector<std::string> list{};

          if (const auto &[is, string] = options.gets(names); is) {
            using std::operator""sv;
            std::ranges::transform(string | std::views::split(","sv),
                                   std::back_inserter(list),
                                   [](const auto &str) {
                                     return std::string{str.begin(), str.end()};
                                   });

            return std::make_pair(true, list);
          }

          return std::make_pair(false, list);
        };

    const auto &[is_group, groups] = string_argument_helper({"group", "g"});
    const auto &[is_name, names] = string_argument_helper({"name", "n"});

    auto make_isolation = false;
    if (const auto [_, isolation_string] =
            options.gets({"isolation", "I"}, false);
        isolation_string == argument_options::empty) {
      make_isolation = true;
    }

    options.check_unused();

    qemu_wrapper qemu_wrapper{option};
    qemu_wrapper.run();

    tty connection{qemu_wrapper};
    test_runner<tab> runner{connection};

    auto builder = runner.build();

    if (make_isolation) {
      builder.make_isolation();
    }
    if (is_ranges) {
      builder.add_range(std::make_pair(ranges[0], ranges[1]));
    }
    if (is_group) {
      builder.add_group(groups);
    }
    if (is_name) {
      builder.add_name(names);
    }
    if (!(is_ranges || is_group || is_name)) {
      builder.add_all();
    }

    if (auto result = builder.run(); result < 0) {
      return "\nFail test!";
    }

    return "";
  });

  try {
    return arg.run();
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    return -1;
  }
}
