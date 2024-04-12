// SPDX-License-Identifier: MIT

module;

#include "protocol.h"
#include <errno.h>

export module test.connect.call;

import std;

export class basic_call {
public:
  virtual std::string_view call(call_code_e code, std::string &&option) = 0;
  virtual void wait_call() = 0;

  virtual std::string_view call(call_code_e code) {
    return call(code, std::string{});
  }
};

export class call_error : public std::exception {
public:
  explicit call_error(std::string &&msg)
      : format("Failed to " + msg + ": " + std::strerror(errno)) {}

  [[nodiscard]] const char *what() const noexcept override {
    return format.c_str();
  }

private:
  const std::string format;
};
