# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_ARCH_SRC
        arch/x86/src/platform.c
        arch/x86/src/serial.c
        )
set(LOADER_ASM_BOOT arch/x86/src/boot.asm)
