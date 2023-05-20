// SPDX-License-Identifier: MIT

#include <utilities.h>
#include <stdio.h>

void status_print(size_t length, bool status) {
  for (size_t i = length; i < CONSOLE_WIDTH - 7; ++i) {
    putchar(' ');
  }

  if (status) {
    printf("[ OK ]\n");
  } else {
    printf("[fail]\n");
  }
}
