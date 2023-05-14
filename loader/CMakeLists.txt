# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_NAME loader)
set(LOADER_NAME ${LOADER_NAME} PARENT_SCOPE)

message(STATUS "Include ${LOADER_NAME}")

include(Build)

include(src/CMakeLists.txt)
include(std/src/CMakeLists.txt)

set(LOADER_CODE
        loader.asm
        loader.c
        ${LOADER_SRC}
        ${LOADER_STANDARD_SRC}
)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)

build_binary(${LOADER_NAME} ${TEMP_OUTPUT} "${LOADER_CODE}")
set_target_properties(${LOADER_NAME} PROPERTIES
        LINK_DEPENDS ${LINKER_SCRIPT}
)
set(LOADER_C_FLAG
        -target ${LOADER_PLATFORM}-pc-none-elf # targeting
        -march=${LOADER_PLATFORM}              # generate instructions
        -Wno-language-extension-token          # asm
        -mcmodel=kernel                        # memory restriction, 2G code section
)
target_compile_options(${LOADER_NAME} PUBLIC
        "$<$<COMPILE_LANGUAGE:C>:${LOADER_C_FLAG}>"
        -D${LOADER_PLATFORM}            # platform define
)
target_link_options(${LOADER_NAME} PUBLIC
        -T ${LINKER_SCRIPT}             # linker init
        -m elf_${LOADER_PLATFORM}       # 32bit build
        "SHELL:-z max-page-size=0x1000" # file size optimization
)
target_link_options(${LOADER_NAME} PUBLIC
        -lclang_rt.builtins-i386      # compiler-rt
)
target_include_directories(${LOADER_NAME} PUBLIC
        std/include
        include
        external
)
