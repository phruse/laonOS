# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include(arch/x86/src/CMakeLists.txt)     # BOOT_ARCH_SRC
include(arch/x86/std/CMakeLists.txt) # BOOT_ARCH_STANDARD_SRC

set(BOOT_ARCH_INCLUDE arch/x86/include/src arch/x86/include)
