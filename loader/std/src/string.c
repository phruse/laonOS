// SPDX-License-Identifier: MIT

#include <macro.h>
#include <stdint.h>
#include <string.h>

int strcmp(const char *first_str, const char *second_str) {
  while (*first_str) {
    if (*first_str != *second_str) {
      break;
    }
    first_str++;
    second_str++;
  }

  return *(const unsigned char *)first_str - *(const unsigned char *)second_str;
}

size_t strcpy_s(char *dest, size_t size, const char *src) {
  size_t i;
  for (i = 0; src[i] != '\0' && i < size; ++i) {
    dest[i] = src[i];
  }

  return i;
}

size_t strlen(const char *string) {
  size_t i;
  for (i = 0; string[i] != '\0'; ++i)
    ;
  return i;
}

#ifdef i386

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

#endif
