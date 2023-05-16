# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include(std/src/platform/CMakeLists.txt)

set(LOADER_STANDARD_SRC
        std/src/stdio.c
        std/src/stdlib.c
        std/src/string.c
        std/src/utilities.c
        ${LOADER_STD_SRC_PLATFORM}
        )
