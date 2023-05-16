# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_SRC
        src/init.c
        src/elf64.c
        src/module.c
        ${LOADER_SRC_PLATFORM}
        )
