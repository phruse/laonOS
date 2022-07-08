# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_NAME loader)
set(LOADER_NAME ${LOADER_NAME} PARENT_SCOPE)

message(STATUS "Include ${LOADER_NAME}")

include(Build)

include(standard/CMakeLists.txt)

set(LOADER_SRC
        loader.asm
        loader.c
        source/init.c
        source/elf64.c
        source/init.asm
        ${LOADER_STANDARD_SRC}
)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)

build_binary(${LOADER_NAME} ${TEMP_OUTPUT} "${LOADER_SRC}")
set_target_properties(${LOADER_NAME} PROPERTIES
        LINK_DEPENDS ${LINKER_SCRIPT}
)
set(LOADER_C_FLAG
        -target i686-pc-none-elf      # targeting
        -march=i686                   # generate instructions
        -Wno-language-extension-token # asm
        -mcmodel=kernel               # memory restriction, 2G code section
)
target_compile_options(${LOADER_NAME} PUBLIC
        "$<$<COMPILE_LANGUAGE:C>:${LOADER_C_FLAG}>"
)
target_link_options(${LOADER_NAME} PUBLIC
        -T ${LINKER_SCRIPT}             # linker init
        -m elf_i386                     # 32bit build
        "SHELL:-z max-page-size=0x1000" # file size optimization
)
target_link_options(${LOADER_NAME} PUBLIC
        -lclang_rt.builtins-i386      # compiler-rt
)
target_include_directories(${LOADER_NAME} PUBLIC
        standard/include
        include
        external
)
add_custom_command(TARGET ${LOADER_NAME} # defined OS_KERNEL_SIZE
        PRE_BUILD
        COMMENT "Preprocessing..."
        COMMAND python3 ${CMAKE_SOURCE_DIR}/cmake/FileSize.py
            ${TEMP_OUTPUT}/${KERNEL_NAME}.elf
            ${CMAKE_SOURCE_DIR}/loader/include/info.h
        VERBATIM
)
