# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include(arch/armv7a/src/CMakeLists.txt)     # BOOT_ARCH_SRC
# include(arch/armv7a/std/CMakeLists.txt) # BOOT_ARCH_STANDARD_SRC

set(BOOT_ARCH_INCLUDE arch/armv7a/include/src arch/armv7a/include)
