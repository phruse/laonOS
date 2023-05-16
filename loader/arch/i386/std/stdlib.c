// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <cpuid.h>

static uint64_t get_tsc(void) {
  uint32_t low;
  uint32_t high;
  asm volatile("rdtsc" : "=a"(low), "=d"(high));
  return low | ((uint64_t)high << 32);
}

int init_seed(void) {
  uint64_t seed = 1;
  for (int j = 0; j < 9; ++j) {
    uint64_t delta = 1;
    for (int i = 0; 9 > i; ++i) {
      uint64_t time_1 = get_tsc();
      uint64_t time_2 = get_tsc();
      delta *= 10;
      delta += (int)(time_2 - time_1) % 9;
    }
    seed |= delta;
  }

  return (int)seed;
}
