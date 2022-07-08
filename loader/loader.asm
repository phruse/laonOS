; SPDX-License-Identifier: MIT

[bits 32] ;run in 32bit mode(protected mode)

global main
extern loader_main ;C loader entrypoint

OS_STACK_SIZE equ 2048      ;OS's stack size (2kb)
TBALE equ 1048576

align 8
section .multiboot2 ;multiboot2 header
multiboot2_start:
  ;magic
  dd 0xe85250d6
  ;architecture (32-bit mode of i386)
  dd 0
  ;header_length
  dd multiboot2_end - multiboot2_start
  ;checksum
  dd 0x100000000 - (0xe85250d6 + 0 + (multiboot2_end - multiboot2_start))
  dw 0
  dw 0
  dd 8
multiboot2_end:

section .text

main:
  cli               ;interrupt blocking

  mov esp, os_stack ;set stack pointer register
  mov ebp, esp      ;set base pointer register (legacy)

  push ebx          ;passing boot information as loader_main argument
  push eax          ;passing magic value as loader_main argument
  call loader_main

  hlt	            ;make idle state to the processor.

;allocatable(alloc), writable(write) don't stored this section(nobits)
section .loader_stack nobits alloc noexec write align=16
  resb OS_STACK_SIZE		;create stack
os_stack:
