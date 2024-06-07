// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <macro.h>
#include <string.h>

int atoi(const char *str) {
  bool minus = false;
  if (*str == '-') {
    minus = true;
    str++;
  }

  int result = 0;
  for (size_t i = 0; str[i] != '\0'; i++) {
    result = result * 10 + (str[i] - '0');
  }

  return minus ? -result : result;
}

// rand

static uint32_t memory = 0;
extern int init_seed(void);

void srand(uint32_t seed) { memory = seed; }

int rand(void) {// feat: secure random _rdrand32_step
  // reference: C standard
  if (memory == 0) {
    srand(init_seed());
  }

  memory = memory * 1103515245 + 12345;
  return (int) (memory / 65536) % RAND_MAX;
}
