// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STANDARD_INCLUDE_STRING_H
#define LAONOS_LOADER_STANDARD_INCLUDE_STRING_H

#include <stdbool.h>
#include <stddef.h>

/**
 * compare two strings
 *
 * @param first_str string to compared
 * @param second_str string to compared
 * @return if both strings are equal return true and otherwise return false
 */
int strcmp(const char *first_str, const char *second_str);

/**
 * copy string
 *
 * @param dest destination
 * @param src source
 * @return number of copied string (not standard)
 */
size_t strcpy_s(char *dest, size_t size, const char *src);

/**
 * return the length of the string.
 *
 * @param string input string
 * @return length of string
 */
size_t strlen(const char *string);

/**
 * set memory
 *
 * @param ptr address to fill
 * @param number number to be set
 * @param size number of bytes
 * @return input prt
 */
void *memset(uintptr_t *ptr, int number, size_t size);

/**
 * copy memory
 *
 * @param dest destination address
 * @param src source address
 * @param size copy size
 * @return input destination
 */
void *memcpy(uintptr_t *dest, const uintptr_t *src, size_t size);

#endif //LAONOS_LOADER_STANDARD_INCLUDE_STRING_H
