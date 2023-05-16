// SPDX-License-Identifier: MIT

#include "init.h"

#include <macro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utilities.h>

#include "elf64.h"
#include "info.h"
#include "multiboot.h"

/* multiboot2 */
static bool check_memory(multiboot_meminfo_t *tag);
static void insert_module(multiboot_module_t *tag, module_t modules[]);

static bool load_module(module_t *module, int64_t relocation, bool is_aslr);
static uintptr_t aslr(const module_t *module);

bool multiboot2_init(uintptr_t info_address, module_t modules[]) {
  if (info_address & (MULTIBOOT_ALIGN - 1u)) // check 8B aligned
  {
    printf("Error: Unaligned multiboot2 information: 0x%x\n", info_address);
    return false;
  }

  puts("- Parsing multiboot2 structure\n");

  multiboot_tag_t *tag = (multiboot_tag_t *)(info_address + MULTIBOOT_ALIGN);
  for (; tag->type != MULTIBOOT_TYPE_END;
       tag = (multiboot_tag_t *)((uint8_t *)tag +
                                 ALIGN(tag->size, MULTIBOOT_ALIGN))) {
    switch (tag->type) {
    case MULTIBOOT_TYPE_MODULE:
      insert_module((multiboot_module_t *)tag, modules);
      break;
    case MULTIBOOT_TYPE_BASIC_MEMINFO:
      if (!check_memory((multiboot_meminfo_t *)tag)) {
        return false;
      }
      break;
    default:;
    }
  }

  if (is_module(modules) && modules->is_kernel == true) {
    return load_module(modules, OS_MEMORY_VIRTUAL, true);
  } else {
    printf("Error: Kernel module not found: %s\n", OS_KERNEL_NAME);
    return false;
  }
}

static bool check_memory(multiboot_meminfo_t *tag) {
  uintptr_t memory_upper = ((multiboot_meminfo_t *)tag)->memory_upper;
  if (memory_upper <= (OS_MEMORY_MINIMUM / 1048576)) { // check memory 64M+
    printf("Error: Less than minimum memory: %uKB\n", memory_upper);
    return false;
  } else
    return true;
}

static void insert_module(multiboot_module_t *tag, module_t modules[]) {
  static size_t module_counter = 1; // 0 is kernel module

  if (!is_module(modules) &&
      strcmp(((multiboot_module_t *)tag)->name, OS_KERNEL_NAME) == 0) {
    multiboot_converter((multiboot_module_t *)tag, modules);
    modules->is_kernel = true;
  } else if (module_counter < OS_MODULE_MAX) {
    multiboot_converter((multiboot_module_t *)tag, &modules[module_counter]);
    module_counter++;
  }
}

static bool load_module(module_t *module, int64_t relocation, bool is_module) {
  if (is_module) {
    uintptr_t aslr_address = aslr(module);
    relocation += aslr_address;
    module->virtual_start = aslr_address;
  }

  relocation += module->module_start;
  module->virtual_start += module->module_start;

  bool status_init = elf64_init_executable(module);
  module->entry += relocation;

  bool status_relocate = true;
  if (status_init && module->is_shared) {
    status_relocate = elf64_relocate_executable(module, relocation);
  }

  return status_init && status_relocate;
}

static uint32_t aslr(const module_t *module) {
  int32_t random = rand();
  if (random < 0)
    return false;

  return ALIGN(
             random * 0x10000 %
                 (OS_MEMORY_FREE - (module->module_end - module->module_start)),
             OS_MEMORY_ALIGN) -
         -OS_MEMORY_ALIGN;
}

bool check_bootloader(const uint32_t magic_value) {
  // check eax magic number <- booting to multiboot2
  if (magic_value != MULTIBOOT_MAGIC) {
    printf("Error: Please boot with multiboot2. Invalid number: 0x%x\n",
           magic_value);
    return false;
  }
  return true;
}
