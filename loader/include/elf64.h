// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define ELF_MAGIC 0x464c457f
#define ELF_IDENT_CLASS_64 2
#define ELF_ISA_X86_64 0x3E
#define ELF_ORIGIN_VERSION 1

#define ELF_SECTION_BSS ".bss"
#define ELF_SECTION_RELA ".rela"

#define ELF_SHT_RELA 0x4
#define ELF_SHT_DYNSYM 0xb

enum {
    R_x86_64_NONE = 0,

    R_x86_64_64 = 1,
    R_X86_64_RELATIVE = 8
};

#pragma pack(push, 1)

typedef struct {
    uint32_t magic;
    uint8_t class;
    uint8_t endianness;
    uint8_t version;
    uint8_t abi;
    uint8_t abi_version;
    uint32_t : 32;// not used
    uint16_t : 16;
    uint8_t : 8;
} elf64_identify_t;

typedef struct {
    elf64_identify_t identify;
    uint16_t type;
    uint16_t isa;
    uint32_t version;
    uint64_t entry;
    uint64_t program_entry;
    uint64_t section_entry;
    uint32_t flag;
    uint16_t size;
    uint16_t program_size;
    uint16_t program_number;
    uint16_t section_size;
    uint16_t section_number;
    uint16_t section_index;
} elf64_file_t;

typedef struct {
    uint32_t name;
    uint32_t type;
    uint64_t flags;
    uint64_t address;
    uint64_t offset;
    uint64_t size;
    uint32_t link;
    uint32_t info;
    uint64_t align;
    uint64_t entsize;
} elf64_section_t;

typedef struct {
    uint64_t offset;
    uint32_t type;
    uint32_t symbol;
    int64_t addend;
} elf64_rela_t;

typedef struct {
    uint32_t name;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
    uint64_t value;
    uint64_t size;
} elf64_sym_t;

#pragma pack(pop)

typedef struct {
    elf64_file_t *file_ptr;
    uint32_t file_end;
    uint32_t relocate;
    uint64_t entry;
} elf64_t;

/**
 * init the elf64 executable file
 *
 * @param file_ptr file memory address
 * @param file elf64 file
 * @return
 */
bool elf64_init_executable(uint32_t file_ptr, uint32_t file_end, elf64_t *file);

/**
 * relocate the relocatable elf64 executable file
 *
 * @param header elf64 file header
 * @param location address to relocate
 * @return
 */
bool elf64_relocate_executable(const elf64_t *file, uint64_t location);
