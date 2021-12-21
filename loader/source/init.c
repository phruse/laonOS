// SPDX-License-Identifier: MIT

#include "init.h"

#include <macro.h>
#include <multiboot2.h>
#include <stdio.h>
#include <string.h>
#include <utilities.h>

#include "elf64.h"
#include "info.h"
#include "inline.h"
#include "page.h"

#define MINIMUM_MEMORY 65536     // kb
#define PAGE_TABLE_SPACE 0x100000// byte

/* gdt */

static void segment_add(int index, uint8_t access_byte, uint8_t flags);

bool gdt_init(void) {
    // data segment 0
    gdt_ptr.size = (sizeof(segment_t) * 3) - 1;
    gdt_ptr.offset = (uint32_t) &segments;

    // 0x9A Pr=1(valid), Privl=0(kernel privilege), Ex=1(code), RW=1(read)
    // 0x92 Pr=1(valid), Privl=0(kernel privilege), Ex=0(data), RW=1(R/W)
    // 0xaf Gr=1(4K block), Sz=0(32-bit protected mode), L=1(long mode)
    // limit and base not used in x64
    segment_add(0, 0, 0);      // null descriptor
    segment_add(1, 0x9a, 0xaf);// code 0x08
    segment_add(2, 0x92, 0xaf);// data 0x10

    // set gdt
    gdt_set((uint32_t) &gdt_ptr);
    grub_reset_flag();

    return true;
}

// add segment descriptor
static void segment_add(int index, uint8_t access_byte, uint8_t flags) {
    segment_t *segment = &segments[index];

    segment->low.base = 0;
    segment->low.limit = 0xfff;

    segment->high.base_low = 0;
    segment->high.base_high = 0;
    segment->high.access_byte = access_byte;
    segment->high.flags = flags;
}

/* multiboot2 */

typedef struct multiboot_tag multiboot_tag_t;
typedef struct multiboot_tag_module multiboot_tag_module_t;

static bool
load_module(elf64_t *file, const multiboot_tag_module_t *module, uint64_t location);

bool multiboot2_init(const uint32_t info_address, elf64_t *kernel_file) {
    if (info_address & (MULTIBOOT_TAG_ALIGN - 1u))// check 8B aligned
    {
        printf("Error: Unaligned multiboot2 information: 0x%x\n",
               info_address);
        return false;
    }

    puts("- Parsing multiboot2 structure\n");

    unsigned int memory_upper;
    multiboot_tag_module_t *kernel_module = 0;

    multiboot_tag_t *tag =
            (multiboot_tag_t *) (info_address + MULTIBOOT_TAG_ALIGN);
    for (; tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag_t *) ((uint8_t *) tag +
                                    ALIGN(tag->size, MULTIBOOT_TAG_ALIGN))) {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_MODULE:
                if (strcmp(((multiboot_tag_module_t *) tag)->cmdline,
                           OS_KERNEL_CMDLINE) == 0) {
                    kernel_module = (multiboot_tag_module_t *) tag;
                }
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                memory_upper =
                        ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper;
                if (memory_upper <= (MINIMUM_MEMORY)) {// check memory 64M+
                    printf("Error: Less than minimum memory: %uKB\n",
                           memory_upper);
                    return false;
                }
                break;
        }
    }

    if (kernel_module != 0) {
        size_t kernel_size = (kernel_module->mod_end - kernel_module->mod_start);
        if (kernel_size != OS_KERNEL_SIZE) {
            printf("Warning: Invalid kernel size: %u-%u\n", kernel_size,
                   OS_KERNEL_SIZE);
        }

        return load_module(kernel_file, kernel_module,
                           OS_KERNEL_VME);
    } else {
        printf("Error: Kernel module not found: %s\n",
               OS_KERNEL_CMDLINE);
        return false;
    }
}

static bool
load_module(elf64_t *file, const multiboot_tag_module_t *module, const uint64_t location) {
    file->relocate = ALIGN(module->mod_end, 0x100000) + PAGE_TABLE_SPACE;
    bool status1 = elf64_init_executable(module->mod_start,
                                         module->mod_end, file);
    bool status2 = true;
    if (status1) {
        status2 = elf64_relocate_executable(file,
                                            location + (uint32_t) file->file_ptr);
    }

    status_print(printf("* Loading %s module at 0x%x-0x%x",
                        module->cmdline, file->file_ptr, file->file_end),
                 status1 && status2);
    return status1 && status2;
}

bool check_bootloader(const uint32_t magic_value) {
    //check eax magic number <- booting to multiboot2
    if (magic_value != MULTIBOOT2_BOOTLOADER_MAGIC) {
        printf("Error: Please boot with multiboot2. Invalid number: 0x%x\n",
               magic_value);
        return false;
    }
    return true;
}

/* long mode */

static void page_create(uint32_t address);

extern int *pml4_table;

bool x86_64_init(elf64_t *kernel_file) {
    if (!is_long_mod_support()) {
        printf("Error: CPU not supported 64-bit(long) mode.\n");
        return false;
    }

    {// init paging
        bool level = is5_level_support();
        page_create(kernel_file->relocate);
        page_init(pml4_table, level);
    }

    status_print(puts("Setup long-mode"), true);

    x86_64_enter_kernel(kernel_file->entry);

    return true;
}

static void page_create(uint32_t address) {
    address -= PAGE_TABLE_SPACE;
    memset((void *) address, 0, PAGE_TABLE_SPACE);

    paging_table **pml5_tables = (paging_table **) address;

    paging_table *pml5_start = pml5_tables[0];
    paging_table *pml5_end = pml5_tables[512 - 1];

    address += sizeof(paging_table) * 512;
    paging_table **pml4 = (paging_table **) address;
}
