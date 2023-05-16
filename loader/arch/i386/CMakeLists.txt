# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include(arch/i386/src/CMakeLists.txt)     # LOADER_ARCH_SRC
include(arch/i386/std/CMakeLists.txt) # LOADER_ARCH_STANDARD_SRC

set(LOADER_ARCH_INCLUDE arch/i386/include/src arch/i386/include)
