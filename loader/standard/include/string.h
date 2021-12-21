// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * compare two strings
 *
 * @param string1 string to compared
 * @param string2 string to compared
 * @return if both strings are equal return true and otherwise return false
 */
int strcmp(const char *string1, const char *string2);

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
void *memset(void *ptr, int number, size_t size);

/**
 *
 * @param dest
 * @param src
 * @param size
 * @return
 */
void *memcpy(void *dest, const void *src, size_t size);
