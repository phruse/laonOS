// SPDX-License-Identifier: MIT

#include "elf64.h"
#include "module.h"

#include <macro.h>
#include <stdio.h>
#include <string.h>

inline static elf64_section_t *elf64_section(elf64_file_t *file_ptr,
                                             uint64_t section_offset);

inline static char *elf64_shstrtab(elf64_file_t *file_ptr);

static elf64_section_t *elf64_section_name(elf64_file_t *file_ptr,
                                           const char *name);

bool elf64_init_executable(module_t *module) {
  uint32_t file_size = module->module_end - module->module_start;
  uint32_t align_size = ALIGN(file_size, 64);

  uintptr_t location = module->module_start;
  module->module_start = location;
  module->module_end = location + align_size;

  elf64_file_t *header = (elf64_file_t *)location;
  module->entry = header->entry;
  module->is_shared = header->type == ELF_ET_DYN;

  // check ELF magic
  if (header->identify.magic != ELF_MAGIC) {
    printf("Error: Invalid magic number: 0x%x\n", header->identify.magic);
    return false;
  }

  // check 64bit binary
  if (header->identify.class != ELF_IDENT_CLASS_64 &&
      header->isa != ELF_ISA_X86_64) {
    printf("Error: Not 64-bit ELF file: class %u\n", header->identify.class);
    return false;
  }

  // check ELF version
  if (header->identify.version != ELF_ORIGIN_VERSION &&
      header->version != ELF_ORIGIN_VERSION) {
    printf("Warning: Invalid version: %u\n", header->version);
  }

  const elf64_section_t *section = elf64_section_name(header, ELF_SECTION_BSS);
  if (section && section->type == ELF_SHT_NOTE) {
    // init .bss section
    memset((uintptr_t *)(location + section->offset), 0, (size_t)section->size);
  } else {
    printf("Warning: Not found .bss section!\n");
  }

  return true;
}

#define OFFSET(DATA) (file_address + DATA)

bool elf64_relocate_executable(const module_t *module, uint64_t location) {
  elf64_file_t *header = (elf64_file_t *)module->module_start;
  const uint64_t file_address = module->module_start;

  const elf64_section_t *relocate_section =
      elf64_section_name(header, ELF_SECTION_RELA);
  const elf64_section_t *dynsym_section =
      elf64_section(header, relocate_section->link);

  // check section
  if (!relocate_section || !dynsym_section ||
      relocate_section->type != ELF_SHT_RELA ||
      dynsym_section->type != ELF_SHT_DYNSYM) {
    printf("Note: Not found relocation data.\n");
    return true;
  }

  const elf64_rela_t *rela = (elf64_rela_t *)OFFSET(relocate_section->offset);
  const elf64_sym_t *symbols = (elf64_sym_t *)OFFSET(dynsym_section->offset);
  const elf64_sym_t *symbol;

  uint64_t temp_value;
  size_t size = (size_t)(relocate_section->size / relocate_section->entsize);
  for (size_t i = 0; i < size; ++i) {
    symbol = &symbols[rela->symbol];           // get symbol
    temp_value = symbol->value + rela->addend; // pre calculate

    switch (rela->type) {
    case R_AMD64_NONE:
      break;
    case R_AMD64_GLOB_DAT:
    case R_AMD64_JUMP_SLOT:
    case R_AMD64_64:
    case R_AMD64_RELATIVE:
      *(uint64_t *)OFFSET(rela->offset) = location + temp_value;
      break;
    default:
      printf("Error: Not supported relocation: %d\n", rela->type);
      return false;
    }
  }

  return true;
}

inline static elf64_section_t *elf64_section(elf64_file_t *file_ptr,
                                             uint64_t section_offset) {
  return &((elf64_section_t *)((uint64_t)file_ptr +
                               file_ptr->section_entry))[section_offset];
}

// get section str table
inline static char *elf64_shstrtab(elf64_file_t *file_ptr) {
  return (char *)file_ptr +
         elf64_section(file_ptr, file_ptr->section_index)->offset;
}

static elf64_section_t *elf64_section_name(elf64_file_t *file_ptr,
                                           const char *name) {
  elf64_section_t *section = elf64_section(file_ptr, 0);
  const char *strtab = elf64_shstrtab(file_ptr);
  char temp_char[10];
  size_t name_size = strlen(name);

  for (size_t index = 0; index < file_ptr->section_number; ++index) {
    if (section->type == 0) { // unused section header table
      section++;
      continue;
    }

    // compare only name length
    strcpy_s(temp_char, name_size, strtab + section->name);
    temp_char[name_size] = '\0';
    if (strcmp(temp_char, name) == 0) {
      return section;
    }

    section++;
  }

  return NULL; // not found
}
