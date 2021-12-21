// SPDX-License-Identifier: MIT

#include <cpuid.h>
#include <stdbool.h>
#include <stdint.h>

inline static void gdt_set(uint32_t gdt_address) {
    asm volatile("lgdt (%0)" ::"a"(gdt_address));
}

// reset EFLAGS
inline static void grub_reset_flag() {
    asm volatile(
            "push $0;"
            "popf;");
}

inline static bool is_long_mod_support() {
    uint32_t edx, tmp_null;
    __get_cpuid(0x80000001, &tmp_null,
                &tmp_null, &tmp_null, &edx);

    if (edx & bit_LM) {
        return true;
    } else {
        return false;
    }
}

inline static bool is5_level_support() {
    uint32_t ecx, tmp_null;
    __get_cpuid_count(7, 0, &tmp_null,
                      &tmp_null, &ecx, &tmp_null);

    if (ecx & (1 << 16)) {
        return true;
    } else {
        return false;
    }
}

inline static void page_init(const int *table_address, bool is5_level_support) {
    asm volatile(
            // disable paging
            "mov %%cr0, %%ebx; and $~(1 << 31), %%ebx; mov %%ebx, %%cr0;"
            // enable PAE(Physical Address Extension)
            "mov %%cr4, %%edx; or $(1 << 5), %%edx; mov %%edx, %%cr4;"
            // set page map level 4 or 5 table
            "mov %[pml_table], %%eax; mov %%eax, %%cr3;"
            //enable long mode
            "mov $0xc0000080, %%ecx; rdmsr; or $(1 << 8), %%eax; wrmsr;"
            ::[pml_table] "r"(table_address)
            : "%cr0", "%cr3", "%cr4", "%ebx", "%edx", "%eax", "%ecx");

    if (is5_level_support) {
        asm volatile(
                "mov %%cr4, %%eax;"
                "or $(1 << 12), %%eax;"
                "mov %%eax, %%cr4;" ::
                        : "%cr4", "%eax");
    }
}

inline static void x86_64_enter_kernel(uint64_t entry_address) {
    // enable paging
    asm volatile goto("mov %%cr0, %%eax;"
                      // set PG(paging), disable NW, CD
                      "or $((1 << 31) | (1 << 29) | (1 << 30)), %%eax;"
                      "xor $((1 << 29) | (1 << 30)), %%eax;"
                      "mov %%eax, %%cr0;"
                      "mov $0x10, %%ax;"// init
                      "mov %%ax, %%ss;"
                      "mov %%ax,%%ds;"
                      "mov %%ax, %%es;"
                      "jmp $0x08, $%l[jump_64];" ::
                              : "%cr0", "%eax", "%ax"
                      : jump_64);
jump_64:
    asm volatile(".code64 \n\t push %%rax; mov %1, +4(%%rsp); pop %%rax;"
                 "jmp *%%rax;" ::
                         "a"((uint32_t) (entry_address & 0xFFFFFFFFLL)),
                 "b"((uint32_t) ((entry_address >> 32) & 0xFFFFFFFFLL)));
}
