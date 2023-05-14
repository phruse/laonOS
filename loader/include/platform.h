// SPDX-License-Identifier: MIT

// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_PLATFORM_H
#define LAONOS_LOADER_INCLUDE_PLATFORM_H

#include <stdbool.h>

#include "elf64.h"

/**
 * pre initialize
 *
 * @return if successful return true and otherwise return false
 */
bool platform_pre_init(void);

/**
 * post initialize
 *
 * @return if successful return true and otherwise return false
 */
bool platform_post_init(const elf64_t *kernel_file);

#endif // LAONOS_LOADER_INCLUDE_PLATFORM_H
