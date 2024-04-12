# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_ARCH_SRC
        arch/armv7a/src/platform.c
)
set(LOADER_ASM_BOOT arch/armv7a/src/boot.asm)
