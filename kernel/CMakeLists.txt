# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(KERNEL_NAME kernel)
set(KERNEL_NAME ${KERNEL_NAME} PARENT_SCOPE)

message(STATUS "Include ${KERNEL_NAME}")

include(Build)

set(KERNEL_SRC
        src/main.c
)

build_binary(${KERNEL_NAME} ${TEMP_OUTPUT} "${KERNEL_SRC}")
target_compile_options(${KERNEL_NAME} PUBLIC
        -target x86_64-pc-none-elf # targeting
        -march=x86-64              # generate instructions
        -D${LAON_KERNEL_PLATFORM}  # platform define
)
target_link_options(${KERNEL_NAME} PUBLIC
        "SHELL:-z max-page-size=0x200000" # 2MB boundary TODO page permission
        -N                                          # writable text section
        -m elf_${LAON_KERNEL_PLATFORM}              # 64bit build
        -e main                                     # entry point
        -lclang_rt.builtins-${LAON_KERNEL_PLATFORM} # compiler-rt
)
add_custom_command(TARGET ${KERNEL_NAME} # remove unused section TODO
        POST_BUILD
        COMMENT "Optimization..."
        COMMAND ${LAON_OBJCOPY} --remove-section=.comment
        "${TEMP_OUTPUT}/${KERNEL_NAME}.elf"
        VERBATIM
)

if (LAON_KERNEL_BUILD_MODE MATCHES PIC)
    # position independent executable
    target_compile_options(${KERNEL_NAME} PUBLIC -mcmodel=kernel -fpie)
    target_link_options(${KERNEL_NAME} PUBLIC -pie)
elseif (LAON_KERNEL_BUILD_MODE MATCHES RELOCATE)
    target_compile_options(${KERNEL_NAME} PUBLIC -mcmodel=large -fno-pic)
    target_link_options(${KERNEL_NAME} PUBLIC "SHELL:-z notext" -shared)
else ()
    # TODO
endif ()

target_include_directories(${KERNEL_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/src
)
