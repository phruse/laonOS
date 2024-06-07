// SPDX-License-Identifier: MIT

module;

#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

#include "protocol.h"

export module test.connect.tty;
export import test.connect.call;
export import test.process.qemu;

import std;

export class tty : public basic_call {
public:
  explicit tty(qemu_wrapper &qemu_process)
      : device(open(qemu_process.get_device().c_str(),
                    O_RDWR | O_NOCTTY | O_SYNC)),
        offset(0), qemu(qemu_process) {
    if (device == -1) {
      throw call_error("open device");
    }

    FD_ZERO(&set);
    FD_SET(device, &set);

    timeout.tv_sec = 8;
    timeout.tv_usec = 0;

    wait_call();
  }

  ~tty() {
    call(CALL_EXIT);
    close();
  }

  void close() const {
    if (device == 0) {
      return;
    }

    ::close(device);
  }

  auto tty_read(std::string &&end, std::size_t max_get = 32) {
    for (auto get_count = 0ul; get_count < max_get; ++get_count) {
      std::string buffer(256, '\0');

      if (auto check = select(device + 1, &set, nullptr, nullptr, &timeout);
          check < 1)
        throw call_error("read due to timeout");

      long read_bytes = read(device, &buffer[0], buffer.size());

      if (read_bytes == -1) {
        throw call_error("read from device");
      }
      if (read_bytes > 0) {
        out += trim(buffer);
      }

      if (auto find = out.find(std::forward<decltype(end)>(end), offset);
          find != std::string::npos) {
        auto pre = offset;
        offset = find + std::size(end);
        return std::string_view{out.c_str() + pre, find - pre};
      }
    }

    throw "Failed to read block (out of range)";
  }

  void tty_write(std::string &&data) const {
    data += '\n';

    if (auto read_bytes = write(device, &data, std::size(data));
        read_bytes != static_cast<ssize_t>(std::size(data))) {
      qemu.kill();
      throw call_error("write from device");
    }
  }

  std::string_view call(int code, std::string &&option) override {
    constexpr auto max_try = 8;
    std::string_view receive{};

    for (auto try_count = 1l; try_count <= max_try; ++try_count) {
      try {
        tty_write(static_cast<char>(code) + option);
        receive = tty_read(std::string(TEST_SEND) + static_cast<char>(code));
        break;
      } catch (...) {
        if (try_count == max_try) {
          qemu.kill();
          throw call_error("call");
        }
      }
    }

    tty_write(std::string(TEST_OK) + static_cast<char>(code));
    return receive;
  }

  void wait_call() override { tty_read(std::string(TEST_READY)); }

  using basic_call::call;

private:
  const int device = 0;
  std::string out{};
  std::size_t offset{};

  fd_set set;
  timeval timeout;
  qemu_wrapper &qemu;

  inline std::string &trim(std::string &str) {
    str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
    return str;
  }
};
