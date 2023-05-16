# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(KERNEL_NAME kernel)
set(KERNEL_NAME ${KERNEL_NAME} PARENT_SCOPE)

message(STATUS "Include ${KERNEL_NAME}")

include(Build)

set(KERNEL_SRC
        kernel.c
        )

build_binary(${KERNEL_NAME} ${TEMP_OUTPUT} "${KERNEL_SRC}")
target_compile_options(${KERNEL_NAME} PUBLIC
        -target x86_64-pc-none-elf # targeting
        -march=x86-64              # generate instructions
        -mcmodel=kernel            # no memory restriction
        -fpie                      # position independent executable
        -D${KERNEL_PLATFORM}       # platform define
        )
target_link_options(${KERNEL_NAME} PUBLIC
        -N                                     # writable text section
        -m elf_${KERNEL_PLATFORM}              # 64bit build
        -e kmain                               # entry point
        -pie                                   # position independent executable
        -lclang_rt.builtins-${KERNEL_PLATFORM} # compiler-rt
        "SHELL:-z common-page-size=0x200000"      # 2MB boundary TODO page permission
        )
add_custom_command(TARGET ${KERNEL_NAME} # remove unused section TODO
        POST_BUILD
        COMMENT "Optimization..."
        COMMAND ${LAON-OS_OBJCOPY} --remove-section=.comment
        "${TEMP_OUTPUT}/${KERNEL_NAME}.elf"
        VERBATIM
        )

target_include_directories(${KERNEL_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        c/include
        )
