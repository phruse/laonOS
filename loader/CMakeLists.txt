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
        src/main.c
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

if (LAON_LOADER_PLATFORM MATCHES x86)
    set(LAON_LOADER_PLATFORM_TARGET i386)
endif ()

set(LOADER_C_FLAGS
        -target ${LAON_LOADER_PLATFORM_TARGET}-pc-none-elf  # targeting
        -march=${LAON_LOADER_PLATFORM_TARGET}               # generate instructions
        -Wno-language-extension-token                       # asm
        -mcmodel=kernel                                     # memory restriction, 2G code section
        -D${LAON_LOADER_PLATFORM_TARGET}                    # platform define
        -Wno-gnu-statement-expression
)
set(LOADER_LINKER_SCRIPT
        ${CMAKE_CURRENT_SOURCE_DIR}/configs/linker_script.ld
)
set(LOADER_LINKER_FLAGS
        -T ${LOADER_LINKER_SCRIPT}             # linker init
        -m elf_${LAON_LOADER_PLATFORM_TARGET}  # 32bit build
        "SHELL:-z max-page-size=0x1000" # file size optimization
        -lclang_rt.builtins-i386        # compiler-rt
        -e start
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
    build_iso_secure(${laonOS_NAME} "loader/configs/grub.cfg")
else ()
    build_iso(${laonOS_NAME} "loader/configs/grub-debug.cfg")
endif ()

add_dependencies(${laonOS_NAME} ${KERNEL_NAME}) # kernel
add_dependencies(${laonOS_NAME} ${LOADER_NAME}) # loader
