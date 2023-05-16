# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_NAME loader)
set(LOADER_NAME ${LOADER_NAME} PARENT_SCOPE)

message(STATUS "Include ${LOADER_NAME}")

include(Build)

include(src/CMakeLists.txt)
include(std/CMakeLists.txt)
include(arch/${LOADER_PLATFORM}/CMakeLists.txt)

set(LOADER_CODE
        src/loader.c
        ${LOADER_ASM_BOOT}
        ${LOADER_SRC}
        ${LOADER_ARCH_SRC}
        ${LOADER_STANDARD_SRC}
        ${LOADER_ARCH_STANDARD_SRC}
        )
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/configs/linker_script.ld)

build_binary(${LOADER_NAME} ${TEMP_OUTPUT} "${LOADER_CODE}")
set_target_properties(${LOADER_NAME} PROPERTIES
        LINK_DEPENDS ${LINKER_SCRIPT}
        )
set(LOADER_C_FLAG
        -target ${LOADER_PLATFORM}-pc-none-elf # targeting
        -march=${LOADER_PLATFORM}              # generate instructions
        -Wno-language-extension-token          # asm
        -mcmodel=kernel                        # memory restriction, 2G code section
        -mrdrnd                                # random
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
        ${LOADER_ARCH_INCLUDE}
        include/src
        include/std
        )

# make image file

# image
# |-boot
#   |- grub
#     |- grub.cfg
#   |- loader.elf
#   |- kernel.m

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_target(${laonOS_NAME} ALL
            BYPRODUCTS
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg"
            COMMENT "Creation ISO image..."
            COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CMAKE_BINARY_DIR}/image/boot/grub" # clean old folder
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/loader/configs/grub.cfg"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg" # copy grub setting
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_BINARY_DIR}/extract"
            COMMAND ${LAON-OS_GRUB_DIR} -o
            "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
            COMMAND ${LAON-OS_XORRISO_DIR} -osirrox on -indev
            "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso"
            -extract / "${CMAKE_BINARY_DIR}/extract" # extract ISO
            COMMAND chmod -R +w "${CMAKE_BINARY_DIR}/extract"
            COMMAND cd "${CMAKE_BINARY_DIR}/extract"
            COMMAND find ./* -type f | xargs sha1sum | sed 's/.\\\//\\\//'
            > "${CMAKE_BINARY_DIR}/extract/sha1.txt" # hash
            COMMAND ${LAON-OS_GRUB_DIR} -o
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/extract"
            COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CMAKE_BINARY_DIR}/extract" # clean
            )
else ()
    add_custom_target(${laonOS_NAME} ALL
            BYPRODUCTS
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg"
            COMMENT "Creation ISO image..."
            COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CMAKE_BINARY_DIR}/image/boot/grub" # clean old folder
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/loader/configs/grub-debug.cfg"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg" # copy grub setting
            COMMAND ${LAON-OS_GRUB_DIR} -o
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
            )
endif ()

# kernel
add_dependencies(${laonOS_NAME} ${KERNEL_NAME})
# loader
add_dependencies(${laonOS_NAME} ${LOADER_NAME})
