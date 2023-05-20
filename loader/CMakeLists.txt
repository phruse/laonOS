# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(LOADER_NAME loader)
set(LOADER_NAME ${LOADER_NAME} PARENT_SCOPE)

message(STATUS "Include ${LOADER_NAME}")

include(Build)

include(src/CMakeLists.txt)
include(std/CMakeLists.txt)
include(arch/${LAON_LOADER_PLATFORM}/CMakeLists.txt)

set(LOADER_SOURCE
        src/loader.c
        ${LOADER_ASM_BOOT}
        ${LOADER_SRC}
        ${LOADER_ARCH_SRC}
        ${LOADER_STANDARD_SRC}
        ${LOADER_ARCH_STANDARD_SRC}
        )
set(LOADER_INCLUDE
        ${LOADER_ARCH_INCLUDE}
        include/src
        include/std
        )
set(LOADER_C_FLAGS
        -target ${LAON_LOADER_PLATFORM}-pc-none-elf # targeting
        -march=${LAON_LOADER_PLATFORM}              # generate instructions
        -Wno-language-extension-token               # asm
        -mcmodel=kernel                             # memory restriction, 2G code section
        -D${LAON_LOADER_PLATFORM}                   # platform define
        )
set(LOADER_LINKER_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/configs/linker_script.ld
        )
set(LOADER_LINKER_FLAGS
        -T ${LOADER_LINKER_SCRIPT}             # linker init
        -m elf_${LAON_LOADER_PLATFORM}  # 32bit build
        "SHELL:-z max-page-size=0x1000" # file size optimization
        -lclang_rt.builtins-i386        # compiler-rt
        )

function(loader_build target)
    build_binary(${target} ${TEMP_OUTPUT} "${LOADER_SOURCE}")
    target_include_directories(${target} PUBLIC ${LOADER_INCLUDE})
    set_target_properties(${target} PROPERTIES LINK_DEPENDS ${LOADER_LINKER_SCRIPT})
    target_compile_options(${target} PUBLIC "$<$<COMPILE_LANGUAGE:C>:${LOADER_C_FLAGS}>")
    target_link_options(${target} PUBLIC ${LOADER_LINKER_FLAGS})
endfunction()

loader_build(${LOADER_NAME})

# test
if (LAON_TEST)
    include(test/CMakeLists.txt)
endif ()

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
            COMMAND ${LAON_GRUB_DIR} -o
            "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
            COMMAND ${LAON_XORRISO_DIR} -osirrox on -indev
            "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.iso"
            -extract / "${CMAKE_BINARY_DIR}/extract" # extract ISO
            COMMAND chmod -R +w "${CMAKE_BINARY_DIR}/extract"
            COMMAND cd "${CMAKE_BINARY_DIR}/extract"
            COMMAND find ./* -type f | xargs sha1sum | sed 's/.\\\//\\\//'
            > "${CMAKE_BINARY_DIR}/extract/sha1.txt" # hash
            COMMAND ${LAON_GRUB_DIR} -o
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
            COMMAND ${LAON_GRUB_DIR} -o
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
            )
endif ()

# kernel
add_dependencies(${laonOS_NAME} ${KERNEL_NAME})
# loader
add_dependencies(${laonOS_NAME} ${LOADER_NAME})
