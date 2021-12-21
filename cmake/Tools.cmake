# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include_guard(DIRECTORY)

macro(find_build_tools)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        message(FATAL_ERROR "Windows not tested.")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR
            ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        if(NOT DEFINED CMAKE_C_COMPILER)
            # clang does not need cross compiler build
            set(CMAKE_C_COMPILER "clang")
            set(CMAKE_C_COMPILER_ID "Clang")
        endif()
        if(NOT DEFINED CLANG_LIB_PATH)
            # compiler runtime library path
            set(CLANG_LIB_PATH "/lib/clang/13.0.1/lib/linux")
        endif()
        if(NOT DEFINED CMAKE_ASM_NASM_COMPILER)
            # NASM path
            set(CMAKE_ASM_NASM_COMPILER "nasm")
        endif()
        if(NOT DEFINED LINKER_PATH)
            # linker path
            set(LINKER_PATH "ld.lld")
        endif()
        if(NOT DEFINED GRUB_PATH)
            # tool for creating grub image
            set(GRUB_PATH "grub-mkrescue")
        endif()
        if(NOT DEFINED XORRISO_PATH)
            # iso tool
            set(XORRISO_PATH "xorriso")
        endif()
        if(NOT DEFINED OBJCOPY)
            # debug symbol tool
            set(OBJCOPY "llvm-objcopy-13")
        endif()
    endif()

    # tool check

    message(CHECK_START "Finding tools")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    message(CHECK_START "Finding Clang")
    find_program(IS_CLANG_EXIST ${CMAKE_C_COMPILER})
    if(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND IS_CLANG_EXIST)
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required tools Clang found.")
    endif()

    message(CHECK_START "Finding LD")
    find_program(IS_LINKER_EXIST ${LINKER_PATH})
    if(IS_LINKER_EXIST)
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required tools LD found.")
    endif()

    message(CHECK_START "Finding NASM")
    find_program(IS_NASM_EXIST ${CMAKE_ASM_NASM_COMPILER})
    if(IS_NASM_EXIST)
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required tools NASM found.")
    endif()

    message(CHECK_START "Finding xorriso")
    find_program(IS_XORRISO_EXIST ${XORRISO_PATH})
    if(IS_XORRISO_EXIST)
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required tools xorriso found.")
    endif()

    message(CHECK_START "Finding grub-mkrescue")
    find_program(IS_GRUB_EXIST ${GRUB_PATH})
    if(IS_GRUB_EXIST)
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required tools grub-mkrescue found.")
    endif()

    message(CHECK_START "Finding llvm-objcopy")
    find_program(IS_OBJCOPY_EXIST ${OBJCOPY})
    if(IS_OBJCOPY_EXIST)
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required tools llvm-objcopy found.")
    endif()

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "all tools found")

endmacro()
