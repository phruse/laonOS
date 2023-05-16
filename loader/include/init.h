// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_INIT_H
#define LAONOS_LOADER_INCLUDE_INIT_H

#include <stdbool.h>
#include <stdint.h>

#include "module.h"

/**
 * multiboot2 initialize
 *
 * @param info_address multiboot2 structure address
 * @return if successful return true and otherwise return false
 */
bool multiboot2_init(uintptr_t info_address, module_t modules[]);

/**
 * check booting to multiboot2
 *
 * @param magic_value magic value
 * @return if booting to multiboot2 return true and otherwise return false
 */
bool check_bootloader(uint32_t magic_value);

#endif // LAONOS_LOADER_INCLUDE_INIT_H
