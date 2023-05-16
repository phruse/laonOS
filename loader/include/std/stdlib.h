// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STD_INCLUDE_STDLIB_H
#define LAONOS_LOADER_STD_INCLUDE_STDLIB_H

#include <stdint.h>

#define RAND_MAX 32767

/**
 * set seed
 * @param seed new seed
 */
void srand(uint32_t seed);

/**
 * generate a random value
 * @return [0, RAND_MAX] range random value
 */
int rand(void);

#endif // LAONOS_LOADER_STD_INCLUDE_STDLIB_H
