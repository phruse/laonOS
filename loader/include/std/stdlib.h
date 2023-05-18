// SPDX-License-Identifier: MIT

// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STD_INCLUDE_STDLIB_H
#define LAONOS_LOADER_STD_INCLUDE_STDLIB_H

#include <stddef.h>
#include <stdint.h>

#define RAND_MAX 32767

#define FREE_MIN 0x200000
#define FREE_MAX 0x300000

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

/**
 * not stable. use only unit test
 *
 * @param size size of memory bytes
 * @return allocated memory
 */
void *malloc(uint16_t size);

/**
 * not stable. use only unit test
 *
 * @param ptr pointer for remove
 */
void free(void *ptr);

#endif // LAONOS_LOADER_STD_INCLUDE_STDLIB_H
