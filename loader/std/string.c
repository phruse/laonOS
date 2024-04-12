// SPDX-License-Identifier: MIT

#include <string.h>

#include <stdlib.h>

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

const char *strstr(const char *str, const char *find_str) {
  if (*find_str == '\0') {
    return str;
  }

  int i = 0;
  while (*str != '\0') {
    if (*(find_str + i) == '\0') {
      return str - i;
    }
    i = *(str++) == *(find_str + i) ? i + 1 : 0;
  }

  return NULL;
}

size_t strlen(const char *str) {
  size_t i;
  for (i = 0; str[i] != '\0'; ++i)
    ;
  return i;
}
