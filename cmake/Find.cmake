# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include_guard(DIRECTORY)

macro(find_build_tools)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        message(FATAL_ERROR "Windows not tested.")
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        if(NOT DEFINED CMAKE_C_COMPILER)
            # clang does not need cross compiler build
            set(CMAKE_C_COMPILER "clang")
            set(CMAKE_C_COMPILER_ID "Clang")
        endif()
        if(NOT DEFINED LAON-OS_CLANG_LIB_DIR)
            # compiler runtime library dir
            set(LAON-OS_CLANG_LIB_DIR "/lib/clang/13.0.1/lib/linux")
        endif()
        if(NOT DEFINED CMAKE_ASM_NASM_COMPILER)
            # NASM dir
            set(CMAKE_ASM_NASM_COMPILER "nasm")
        endif()
        if(NOT DEFINED CMAKE_LINKER)
            # linker dir
            set(CMAKE_LINKER "ld.lld")
        endif()
        if(NOT DEFINED LAON-OS_GRUB_DIR)
            # tool for creating grub image
            set(LAON-OS_GRUB_DIR "grub-mkrescue")
        endif()
        if(NOT DEFINED LAON-OS_XORRISO_DIR)
            # iso tool
            set(LAON-OS_XORRISO_DIR "xorriso")
        endif()
        if(NOT DEFINED LAON-OS_OBJCOPY)
            # debug symbol tool
            set(LAON-OS_OBJCOPY "llvm-objcopy-13")
        endif()
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(LAON-OS_HOMEBREW_DIR "/opt/homebrew/opt")

        if(NOT DEFINED CMAKE_C_COMPILER)
            # clang does not need cross compiler build
            set(CMAKE_C_COMPILER "${LAON-OS_HOMEBREW_DIR}/llvm/bin/clang")
            set(CMAKE_C_COMPILER_ID "Clang")
        endif()
        if(NOT DEFINED LAON-OS_CLANG_LIB_DIR)
            # compiler runtime library dir
            set(LAON-OS_CLANG_LIB_DIR "/usr/local/lib/clang/linux")
        endif()
        if(NOT DEFINED CMAKE_ASM_NASM_COMPILER)
            # NASM dir
            set(CMAKE_ASM_NASM_COMPILER "${LAON-OS_HOMEBREW_DIR}/nasm/bin/nasm")
        endif()
        if(NOT DEFINED CMAKE_LINKER)
            # linker dir
            set(CMAKE_LINKER "${LAON-OS_HOMEBREW_DIR}/llvm/bin/ld.lld")
        endif()
        if(NOT DEFINED LAON-OS_GRUB_DIR)
            # tool for creating grub image
            set(LAON-OS_GRUB_DIR "grub-mkrescue")
        endif()
        if(NOT DEFINED LAON-OS_XORRISO_DIR)
            # iso tool
            set(LAON-OS_XORRISO_DIR
                    "${LAON-OS_HOMEBREW_DIR}/xorriso/bin/xorriso")
        endif()
        if(NOT DEFINED LAON-OS_OBJCOPY)
            # debug symbol tool
            set(LAON-OS_OBJCOPY "${LAON-OS_HOMEBREW_DIR}/llvm/bin/llvm-objcopy")
        endif()
    endif()

    # tool check
    message(CHECK_START "Finding tools")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    program_check(${CMAKE_C_COMPILER} "Clang")
    program_check(${CMAKE_LINKER} "LD")
    program_check(${CMAKE_ASM_NASM_COMPILER} "NASM")
    program_check(${LAON-OS_XORRISO_DIR} "xorriso")
    program_check(${LAON-OS_GRUB_DIR} "grub-mkrescue")
    program_check(${LAON-OS_OBJCOPY} "llvm-objcopy")

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "all tools found")

endmacro()

macro(find_build_library)
    message(CHECK_START "Finding librares")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    library_check("clang_rt.builtins-i386" "clang_rt-i386")
    library_check("clang_rt.builtins-x86_64" "clang_rt-x86_64")

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(CHECK_PASS "all librares found")
endmacro()

function(program_check path name)
    message(CHECK_START "Finding ${name}")
    find_program(IS_${name} ${path})
    print_check(${IS_${name}} ${name})
endfunction()

macro(library_check path name)
    message(CHECK_START "Finding ${name}")
    find_library(IS_${name} ${path}
            HINT ${LAON-OS_CLANG_LIB_DIR})
    print_check(${IS_${name}} ${name})
endmacro()

function(print_check is_founded name)
    if(NOT ${is_founded} MATCHES "-NOTFOUND$")
        message(CHECK_PASS "found")
    else()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required ${name} found.")
    endif()
endfunction()
