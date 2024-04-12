// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_STD_STRING_H
#define LAONOS_LOADER_INCLUDE_STD_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
 * find string
 *
 * @param str source string
 * @param find_str find string
 * @return first occurrence in str
 */
const char *strstr(const char *str, const char *find_str);

/**
 * split string
 *
 * @param str source string
 * @param delim delimiter
 * @return
 */
char *strtok(char *str, const char *delim);

/**
 * return the length of the string.
 *
 * @param str input string
 * @return length of string
 */
size_t strlen(const char *str);

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
 * copy memory
 *
 * @param dest destination address
 * @param src source address
 * @param size copy size
 * @return input destination
 */
void *memcpy(void *dest, const void *src, size_t size);

#endif // LAONOS_LOADER_INCLUDE_STD_STRING_H
