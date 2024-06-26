# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

include_guard(DIRECTORY)

macro(find_build_tools)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        message(FATAL_ERROR "Windows not tested.")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        if (NOT DEFINED CMAKE_C_COMPILER)
            # clang does not need cross compiler build
            set(CMAKE_C_COMPILER "clang")
            set(CMAKE_C_COMPILER_ID "Clang")
        endif ()
        if (NOT DEFINED LAON_CLANG_LIB_DIR)
            # compiler runtime library dir
            set(LAON_CLANG_LIB_DIR "/lib/clang/14.0.0/lib/linux") # TODO version
        endif ()
        if (NOT DEFINED CMAKE_ASM_NASM_COMPILER)
            set(CMAKE_ASM_NASM_COMPILER "nasm")
        endif ()
        if (NOT DEFINED CMAKE_LINKER)
            # linker dir
            set(CMAKE_LINKER "ld.lld")
        endif ()
        if (NOT DEFINED LAON_GRUB_DIR)
            # tool for creating grub image
            set(LAON_GRUB_DIR "grub-mkrescue")
        endif ()
        if (NOT DEFINED LAON_XORRISO_DIR)
            # iso tool
            set(LAON_XORRISO_DIR "xorriso")
        endif ()
        if (NOT DEFINED LAON_OBJCOPY)
            # debug symbol tool
            set(LAON_OBJCOPY "llvm-objcopy")
        endif ()
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(LAON_HOMEBREW_DIR "/opt/homebrew/opt")

        if (NOT DEFINED CMAKE_C_COMPILER)
            # clang does not need cross compiler build
            set(CMAKE_C_COMPILER "${LAON_HOMEBREW_DIR}/llvm/bin/clang")
            set(CMAKE_C_COMPILER_ID "Clang")
        endif ()
        if (NOT DEFINED LAON_CLANG_LIB_DIR)
            # compiler runtime library dir
            set(LAON_CLANG_LIB_DIR "/usr/local/lib/clang/linux")
        endif ()
        if (NOT DEFINED CMAKE_ASM_NASM_COMPILER)
            set(CMAKE_ASM_NASM_COMPILER "nasm")
        endif ()
        if (NOT DEFINED CMAKE_LINKER)
            # linker dir
            set(CMAKE_LINKER "${LAON_HOMEBREW_DIR}/llvm/bin/ld.lld")
        endif ()
        if (NOT DEFINED LAON_GRUB_DIR)
            # tool for creating grub image
            set(LAON_GRUB_DIR "grub-mkrescue")
        endif ()
        if (NOT DEFINED LAON_XORRISO_DIR)
            # iso tool
            set(LAON_XORRISO_DIR
                    "${LAON_HOMEBREW_DIR}/xorriso/bin/xorriso")
        endif ()
        if (NOT DEFINED LAON_OBJCOPY)
            # debug symbol tool
            set(LAON_OBJCOPY "${LAON_HOMEBREW_DIR}/llvm/bin/llvm-objcopy")
        endif ()
    endif ()

    if (NOT DEFINED LAON_LIBCXX_BUILD)
        set(LLVM_BUILD_DIR ${CMAKE_SOURCE_DIR}/externs/llvm-project)
        file(MAKE_DIRECTORY ${LLVM_BUILD_DIR} ${LLVM_BUILD_DIR}/build)

        set(LLVM_BUILD_LIST_CONTENT "
        cmake_minimum_required(VERSION 3.20)
        project(llvm)

        include(ExternalProject)
        ExternalProject_Add(llvm-project
                GIT_REPOSITORY https://github.com/llvm/llvm-project.git
                GIT_TAG llvmorg-18.1.3
                SOURCE_SUBDIR runtimes
                PREFIX \"\"
                INSTALL_COMMAND \"\"
                CMAKE_ARGS
                -DLIBCXX_ENABLE_STD_MODULES=ON
                -DCMAKE_CXX_STANDARD=23
                -DLLVM_ENABLE_RUNTIMES=libcxx$<SEMICOLON>libcxxabi$<SEMICOLON>libunwind
        )

        add_custom_target(trigger_llvm)
        add_dependencies(trigger_llvm llvm-project)
        ")

        file(WRITE ${LLVM_BUILD_DIR}/CMakeLists.txt "${LLVM_BUILD_LIST_CONTENT}")

        execute_process(COMMAND ${CMAKE_COMMAND} .. -G ${CMAKE_GENERATOR}
                WORKING_DIRECTORY ${LLVM_BUILD_DIR}/build
        )
        execute_process(COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${LLVM_BUILD_DIR}/build
        )

        set(LAON_LIBCXX_BUILD
                "${LLVM_BUILD_DIR}/build/llvm-project-prefix/src/llvm-project-build")
    endif ()

    # tool check
    message(CHECK_START "Finding tools")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    program_check(${CMAKE_C_COMPILER} "Clang")
    program_check(${CMAKE_LINKER} "LD")
    program_check(${CMAKE_ASM_NASM_COMPILER} "nasm")
    program_check(${LAON_XORRISO_DIR} "xorriso")
    program_check(${LAON_GRUB_DIR} "grub-mkrescue")
    program_check(${LAON_OBJCOPY} "llvm-objcopy")

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
            HINT ${LAON_CLANG_LIB_DIR})
    print_check(${IS_${name}} ${name})
endmacro()

function(print_check is_founded name)
    if (NOT ${is_founded} MATCHES "-NOTFOUND$")
        message(CHECK_PASS "found")
    else ()
        message(CHECK_FAIL "not found")
        message(FATAL_ERROR "No required ${name} found.")
    endif ()
endfunction()
