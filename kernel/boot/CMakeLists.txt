# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(BOOT_NAME boot)
set(BOOT_NAME ${BOOT_NAME} PARENT_SCOPE)

message(STATUS "Include ${BOOT_NAME}")

include(Build)

include(src/CMakeLists.txt)
include(std/CMakeLists.txt)
include(arch/${LAON_BOOT_PLATFORM}/CMakeLists.txt)

set(BOOT_SOURCE
        src/main.c
        ${BOOT_ASM_BOOT}
        ${BOOT_SRC}
        ${BOOT_ARCH_SRC}
        ${BOOT_STANDARD_SRC}
        ${BOOT_ARCH_STANDARD_SRC}
)
set(BOOT_INCLUDE
        ${BOOT_ARCH_INCLUDE}
        include/src
        include/std
)

if (LAON_BOOT_PLATFORM MATCHES x86)
    set(LAON_BOOT_PLATFORM_TARGET i386)
endif ()

set(BOOT_C_FLAGS
        -target ${LAON_BOOT_PLATFORM_TARGET}-pc-none-elf  # targeting
        -march=${LAON_BOOT_PLATFORM_TARGET}               # generate instructions
        -mcmodel=kernel                                     # memory restriction, 2G code section
        -D${LAON_BOOT_PLATFORM_TARGET}                    # platform define
        -Wno-gnu-statement-expression
)
set(BOOT_LINKER_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/configs/linker_script.ld
)
set(BOOT_LINKER_FLAGS
        -T ${BOOT_LINKER_SCRIPT}             # linker init
        -m elf_${LAON_BOOT_PLATFORM_TARGET}  # 32bit build
        "SHELL:-z max-page-size=0x1000" # file size optimization
        -lclang_rt.builtins-i386        # compiler-rt
        -e start
)

function(boot_build target)
    build_binary(${target} ${TEMP_OUTPUT} "${BOOT_SOURCE}")
    target_include_directories(${target} PUBLIC ${BOOT_INCLUDE})
    set_target_properties(${target} PROPERTIES LINK_DEPENDS ${BOOT_LINKER_SCRIPT})
    target_compile_options(${target} PUBLIC ${BOOT_C_FLAGS})
    target_link_options(${target} PUBLIC ${BOOT_LINKER_FLAGS})
endfunction()

boot_build(${BOOT_NAME})

# test
if (LAON_TEST)
    include(test/CMakeLists.txt)
endif ()

# make image file

# image
# |-boot
#   |- grub
#     |- grub.cfg
#   |- boot.elf
#   |- kernel.m

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    build_iso_secure(${laonOS_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/configs/grub.cfg")
else ()
    build_iso(${laonOS_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/configs/grub-debug.cfg")
endif ()

add_dependencies(${laonOS_NAME} ${KERNEL_NAME}) # kernel
add_dependencies(${laonOS_NAME} ${BOOT_NAME}) # boot
