// SPDX-License-Identifier: MIT

#include "loader.h"

#include <stdio.h>
#include <utilities.h>

#include "init.h"

void loader_main(uint32_t magic_value, uintptr_t info_address) {
  clear_screen();

  printf("> laonOS %s loader\n", OS_VERSION);

  elf64_t kernel_file;

  ERROR_BOUNDARY(check_bootloader(magic_value),
                 "Bootloader check")
  ERROR_BOUNDARY(gdt_init(),
                 "Set GDT")
  ERROR_BOUNDARY(multiboot2_init(info_address, &kernel_file),
                 "Multiboot2 init")
  ERROR_BOUNDARY(x86_64_init(&kernel_file),
                 "Setup long-mode")
}
