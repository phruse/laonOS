// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdlib.h>

static uint32_t memory = 0;

extern int init_seed(void);

void srand(uint32_t seed) { memory = seed; }

int rand(void) { // TODO secure random _rdrand32_step
  // Reference: C standard
  if (memory == 0) {
    srand(init_seed());
  }

  memory = memory * 1103515245 + 12345;
  return (int)(memory / 65536) % RAND_MAX;
}
