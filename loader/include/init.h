// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_INIT_H
#define LAONOS_LOADER_INCLUDE_INIT_H

#include <stdbool.h>
#include <stdint.h>

#include "gdt.h"
#include "elf64.h"

/**
 * GDT initialize
 *
 * @return if successful return true and otherwise return false
 */
bool gdt_init(void);

/**
 * multiboot2 initialize
 *
 * @param info_address multiboot2 structure address
 * @return if successful return true and otherwise return false
 */
bool multiboot2_init(uintptr_t info_address, elf64_t *kernel_file);

/**
 * check booting to multiboot2
 *
 * @param magic_value magic value
 * @return if booting to multiboot2 return true and otherwise return false
 */
bool check_bootloader(uint32_t magic_value);

/**
 * setup long mode
 *
 * @return if successful return true and otherwise return false
 */
bool x86_64_init(elf64_t *kernel_file);

#endif //LAONOS_LOADER_INCLUDE_INIT_H
