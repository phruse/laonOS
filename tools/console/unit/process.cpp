// SPDX-License-Identifier: MIT

module;

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

export module test.process.process;

import std;

export class process_error : public std::exception {
public:
  explicit process_error(std::string &&msg)
      : format(msg + ": " + std::strerror(errno)) {}

  [[nodiscard]] const char *what() const noexcept override {
    return format.c_str();
  }

private:
  const std::string format;
};

export class process {
public:
  void run(const std::string &command) {
    if (pid == 0) {
      return;
    }

    if (pid = pipe(fd); pid < 0) {
      throw process_error("Pipe creation fail!");
    }

    if (pid = fork(); pid < 0) {
      throw process_error("Fork fail!");
    } else if (pid == 0) {
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      close(fd[1]);

      execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
      exit(1);
    }

    close(fd[1]);
  }

  std::string get_line() {
    if (pid == -1) {
      return "";
    }

    char buf[1024];
    auto output = fdopen(fd[0], "r");
    if (fgets(buf, sizeof buf, output)) {
      return buf;
    }
    fclose(output);

    return "";
  }

  auto kill() const {
    if (pid == -1) {
      return false;
    }

    ::kill(pid, SIGKILL);
    return true;
  }

  ~process() { kill(); }

private:
  pid_t pid = -1;
  int fd[2] = {};
};
