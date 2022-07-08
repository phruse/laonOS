// SPDX-License-Identifier: MI

#include <stdio.h>
#include <utilities.h>

void status_print(size_t length, bool status) {
  for (size_t i = length; i < 80 - 7; ++i) {
    putchar(' ');
  }

  if (status) {
    printf("[ OK ]\n");
  } else {
    printf("[fail]\n");
  }
}
