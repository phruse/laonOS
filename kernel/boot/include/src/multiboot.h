// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_BOOT_INCLUDE_SRC_MULTIBOOT_H
#define LAONOS_BOOT_INCLUDE_SRC_MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MAGIC 0x36d76289

#define MULTIBOOT_ALIGN 8
#define MULTIBOOT_TYPE_END 0
#define MULTIBOOT_TYPE_MODULE 3
#define MULTIBOOT_TYPE_BASIC_MEMINFO 4

typedef struct {
  uint32_t type;
  uint32_t size;
} multiboot_tag_t;

typedef struct {
  uint32_t type;
  uint32_t size;
  uint32_t module_start;
  uint32_t module_end;
  char name[];
} multiboot_module_t;

typedef struct {
  uint32_t type;
  uint32_t size;
  uint32_t memory_lower;
  uint32_t memory_upper;
} multiboot_meminfo_t;

#endif // LAONOS_BOOT_INCLUDE_SRC_MULTIBOOT_H
