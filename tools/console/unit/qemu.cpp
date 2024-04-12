// SPDX-License-Identifier: MIT

module;

export module test.process.qemu;

import std;

import test.process.process;

export struct qemu_option {
  std::string path = "qemu-system-x86_64";
  std::string options{};
  std::string os_file{};
};

export class qemu_wrapper : public process {
public:
  explicit qemu_wrapper(const qemu_option &option) : option(option) {
    if (!std::filesystem::exists(option.os_file)) {
      throw process_error("ISO file not found!");
    }
  }

  void run() {
    const auto call =
        option.path +
        std::regex_replace(generic_argument, std::regex("\\$ISO_PATH"),
                           option.os_file) +
        option.options;

    process::run(call);
  }

  std::string get_device() {
    std::regex pattern("/dev/[a-zA-Z0-9]+");
    std::smatch matches;
    std::string line;

    for (int count = 0; count < max_try; ++count) {
      line = process::get_line();

      if (std::regex_search(line, matches, pattern)) {
        return matches[0];
      }
    }

    return "";
  }

private:
  const qemu_option &option;

  static constexpr auto max_try = 5;
  static constexpr auto generic_argument =
      " -cpu qemu64,+la57 -smp 4 -m 6G -name test-console -display none "
      "-drive index=0,media=disk,format=raw,file=\"$ISO_PATH\" "
      "-serial pty";
};

export class qemu_qmp {
public:
  explicit qemu_qmp() {}


};
