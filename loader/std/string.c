// SPDX-License-Identifier: MIT

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
