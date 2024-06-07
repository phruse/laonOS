// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_BOOT_INCLUDE_SRC_MODULE_H
#define LAONOS_BOOT_INCLUDE_SRC_MODULE_H

#include <stdbool.h>
#include <stdint.h>

#include "multiboot.h"

#pragma pack(push, 1)

typedef struct {
  uintptr_t module_start;  // module start
  uintptr_t module_end;    // module end
  uintptr_t virtual_start; // module virtual memory
  uint64_t entry;          // entry address
  bool is_shared;          // is shared object
  bool is_kernel;          // is kernel module, allways index 0
} module_t;

typedef struct {
  uint16_t tag_id;
  uint16_t flag;
  uint32_t data_size;
} module_tag_t;

#pragma pack(pop)

/**
 * module initialized check
 *
 * @return if initialized return true
 */
bool is_module(const module_t *module);

/**
 * converting multiboot_module_t to module_t
 *
 * @param multiboot_module
 * @param module
 */
void multiboot_converter(const multiboot_module_t *multiboot_module,
                         module_t *module);

#endif // LAONOS_BOOT_INCLUDE_SRC_MODULE_H
