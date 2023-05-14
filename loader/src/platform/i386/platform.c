// SPDX-License-Identifier: MIT

#include "platform.h"

#include <macro.h>
#include <stdio.h>
#include <string.h>

#include "inline.h"
#include "platform/i386/gdt.h"

/* gdt */

// segment descriptor list
segment_t segments[3];
// GDT descriptor
gdt_t gdt_ptr;

static void add_segment(int index, uint8_t access_byte, uint8_t flags);

bool platform_pre_init(void) {
  // data segment 0
  gdt_ptr.size = (sizeof(segment_t) * 3) - 1;
  gdt_ptr.offset = (uintptr_t)&segments;

  // 0x9A Pr=1(valid), Privl=0(kernel privilege), Ex=1(code), RW=1(read)
  // 0x92 Pr=1(valid), Privl=0(kernel privilege), Ex=0(data), RW=1(R/W)
  // 0xaf Gr=1(4K block), Sz=0(32-bit protected mode), L=1(long mode)
  // limit and base not used in x64
  add_segment(0, 0, 0);       // null descriptor
  add_segment(1, 0x9a, 0xaf); // code 0x08
  add_segment(2, 0x92, 0xaf); // data 0x10

  // set gdt
  gdt_set((uintptr_t)&gdt_ptr);

  return true;
}

// add segment descriptor
static void add_segment(int index, uint8_t access_byte, uint8_t flags) {
  segment_t *segment = &segments[index];

  segment->low.base = 0;
  segment->low.limit = 0;

  segment->high.base_low = 0;
  segment->high.base_high = 0;
  segment->high.access_byte = access_byte;
  segment->high.flags = flags;
}

/* long mode */

// page table array

static uintptr_t create_page_tables(uintptr_t address, bool is_5_level_support);
inline static void add_page_table(uintptr_t address, size_t index,
                                  uint64_t value);

bool platform_post_init(const elf64_t *kernel_file) {
  if (!is_long_mod_support()) {
    printf("Error: CPU not supported 64-bit(long) mode.\n");
    return false;
  }

  // init paging
  bool level = is_5_level_support();
  const uintptr_t page_address =
      create_page_tables(kernel_file->file_end, level);
  page_init(page_address, level);
  if (level) {
    puts("- Enable 5-level paging.\n");
  }

  status_print(puts("Setup long-mode"), true);

  x86_64_enter_kernel(kernel_file->entry);

  return true;
}

static uintptr_t create_page_tables(uintptr_t address,
                                    bool is_5_level_support) {
  const int table_count = is_5_level_support ? 2 : 1;

  address = ALIGN(address, 0x1000);
  memset((uintptr_t *)address, 0, 0x1000 * (table_count + 2));

  // page directory
  add_page_table(address, 0, 0x83);

  // page directory pointer
  address += 0x1000;
  add_page_table(address, 0, address - 0x1000 | 3);
  add_page_table(address, 480, address - 0x1000 | 3);

  // page map level-4, page map level-5
  for (int i = 0; i < table_count; ++i) {
    address += 0x1000;
    add_page_table(address, 0, address - 0x1000 | 3);
    add_page_table(address, 511, address - 0x1000 | 3);
  }

  return address;
}

inline static void add_page_table(uintptr_t address, size_t index,
                                  uint64_t value) {
  ((uint64_t *)address)[index] = value;
}
