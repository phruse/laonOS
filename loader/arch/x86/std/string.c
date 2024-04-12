// SPDX-License-Identifier: MIT

#include <macro.h>
#include <stddef.h>
#include <stdint.h>

#include "stdio.h"

void *memset(uintptr_t *ptr, int number, size_t size) {
  asm("cld;"
      "rep; stosb;" ::"D"(ptr),
      "c"(size), "a"(number));

  return ptr;
}

void *memcpy(uintptr_t *dest, const uintptr_t *src, size_t size) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-to-int-cast"
  if ((uintptr_t)dest == ALIGN((uintptr_t)dest, 16)) {
#pragma clang diagnostic pop
    asm volatile("cld;"
                 "rep; movsb;" ::"D"(dest),
                 "S"(src), "c"(size));
  } else {
    for (size_t i = 0; i <= size; ++i) {
      dest[i] = src[i];
    }
  }

  return dest;
}
