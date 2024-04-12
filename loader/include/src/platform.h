// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_SRC_PLATFORM_H
#define LAONOS_LOADER_INCLUDE_SRC_PLATFORM_H

#include <stdbool.h>

#include "module.h"

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
bool platform_post_init(const module_t modules[]);

#endif // LAONOS_LOADER_INCLUDE_SRC_PLATFORM_H
