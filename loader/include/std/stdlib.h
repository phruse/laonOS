// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_STD_STDLIB_H
#define LAONOS_LOADER_INCLUDE_STD_STDLIB_H

#include <stddef.h>
#include <stdint.h>

#define RAND_MAX 32767

#define FREE_MIN 0x200000
#define FREE_MAX 0x300000

/**
 * string to int
 *
 * @param str string to convert
 * @return converted int
 */
int atoi(const char *str);

/**
 * set seed
 *
 * @param seed new seed
 */
void srand(uint32_t seed);

/**
 * generate a random value
 *
 * @return [0, RAND_MAX] range random value
 */
int rand(void);

#endif // LAONOS_LOADER_INCLUDE_STD_STDLIB_H
