// SPDX-License-Identifier: MIT

#include "init.h"

#include <macro.h>
#include <stdio.h>
#include <string.h>
#include <utilities.h>

#include "elf64.h"
#include "info.h"
#include "multiboot.h"

#define MINIMUM_MEMORY 65536 // kb

/* multiboot2 */

static bool load_module(elf64_t *file, const multiboot_module_t *module,
                        int64_t relocation);

bool multiboot2_init(uintptr_t info_address, elf64_t *kernel_file) {
  if (info_address & (MULTIBOOT_ALIGN - 1u)) // check 8B aligned
  {
    printf("Error: Unaligned multiboot2 information: 0x%x\n", info_address);
    return false;
  }

  puts("- Parsing multiboot2 structure\n");

  uintptr_t memory_upper;
  const multiboot_module_t *kernel_module = 0;

  multiboot_tag_t *tag = (multiboot_tag_t *)(info_address + MULTIBOOT_ALIGN);
  for (; tag->type != MULTIBOOT_TYPE_END;
       tag = (multiboot_tag_t *)((uint8_t *)tag +
                                 ALIGN(tag->size, MULTIBOOT_ALIGN))) {
    switch (tag->type) {
    case MULTIBOOT_TYPE_MODULE:
      if (strcmp(((multiboot_module_t *)tag)->name, OS_KERNEL_NAME) == 0) {
        kernel_module = (multiboot_module_t *)tag;
      }
      break;
    case MULTIBOOT_TYPE_BASIC_MEMINFO:
      memory_upper = ((multiboot_meminfo_t *)tag)->memory_upper;
      if (memory_upper <= MINIMUM_MEMORY) { // check memory 64M+
        printf("Error: Less than minimum memory: %uKB\n", memory_upper);
        return false;
      }
      break;
    default:;
    }
  }

  if (kernel_module != 0) {
    return load_module(kernel_file, kernel_module, OS_KERNEL_VME);
  } else {
    printf("Error: Kernel module not found: %s\n", OS_KERNEL_NAME);
    return false;
  }
}

static bool load_module(elf64_t *file, const multiboot_module_t *module,
                        int64_t relocation) {
  file->file_start = module->module_start;
  file->file_end = module->module_end;

  relocation += module->module_start; // TODO

  bool status_init = elf64_init_executable(file);
  file->entry += relocation;

  bool status_relocate = true;
  if (status_init && file->is_shared) {
    status_relocate = elf64_relocate_executable(file, relocation);
  }

  status_print(printf("* Loading %s module at 0x%x-0x%x", module->name,
                      file->file_start, file->file_end),
               status_init && status_relocate);

  return status_init && status_relocate;
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
