// SPDX-License-Identifier: MIT
#include "info.h"

.code32

.global start
.extern main // C boot entrypoint

.align 8
.section ".multiboot2" // multiboot2 header
multiboot2_start:
  // magic
  .long 0xe85250d6
  // architecture (32-bit mode of i386)
  .long 0
  // header_length
  .long multiboot2_end - multiboot2_start
  // checksum
  .long 0x100000000 - (0xe85250d6 + 0 + (multiboot2_end - multiboot2_start))
  .word 0
  .word 0
  .long 8
multiboot2_end:

.section ".text"

start:
  // interrupt blocking
  cli

  mov $os_stack, %esp
  // set base pointer register (legacy)
  mov %esp, %ebp

  push %ebx // boot information
  push %eax // magic value
  call main

  hlt       //make idle state to the processor.

.align 16
.section ".os_stack", "aw", @nobits
.rept OS_STACK_SIZE
.byte 0
.endr
os_stack:
