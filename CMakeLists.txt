# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(laonOS_NAME laonOS)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools/cmake")

project(${laonOS_NAME}
        LANGUAGES C CXX ASM
        VERSION 0.1
        DESCRIPTION "OS that various pH coexists."
)

message("")
message(NOTICE "Project ${CMAKE_PROJECT_NAME}")
message(NOTICE "Build Version ${CMAKE_PROJECT_VERSION}")
message("")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release)
else ()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/debug)
endif ()

include(Setting) # load setting
include(Tools)

find_build_tools() # tools check
message("")
find_build_library() # library check
message("")

# default build flags
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

if (LAON_TEST)
    include(CTest)
    enable_testing()
    add_subdirectory(tools/console)
endif ()

# reset
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp")
set(CMAKE_ASM_FLAGS_DEBUG "")
set(CMAKE_ASM_FLAGS_MINSIZEREL "")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "")
set(CMAKE_ASM_FLAGS_RELEASE "")

# default LINK setting
set(CMAKE_C_LINK_EXECUTABLE
        "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET>"
)
# default COMPILE setting
set(CMAKE_C_COMPILE_OBJECT
        "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")

set(OS_FLAGS_DEBUG
        -g # debug
        -O0
)
set(OS_C_FLAGS_RELEASE
        -O2 # optimization 2
)
set(OS_ASM_FLAG_RELEASE
        -Ox # optimization
)
set(OS_C_FLAGS
        -Wall               # enable all warnings
        -Werror             # warnings become error
        -Wextra             # additional warning flags
        -Wpedantic          # strict ISO C
        -ffreestanding      # freestanding build
        -D_FORTIFY_SOURCE=2 # prevent buffer overflows
        -mno-red-zone       # avoid interrupt handler problem
        -Wno-language-extension-token # asm

        $<$<CONFIG:Debug>:-Wno-error=unused-variable>
        $<$<PLATFORM_ID:Darwin>:-Wno-unused-command-line-argument>
)
set(OS_CXX_FLAGS
        -fuse-cxa-atexit        # do not add a unload destructor
        -fno-threadsafe-statics # do not add a guard variable
        -fno-exceptions         # exceptions need C++ runtime.
        -fno-rtti               # same reason, and not required to OS
)
set(OS_LINKER_FLAGS
        -nostdlib                       # block linking std library
        -L${LAON_CLANG_LIB_DIR}         # compiler-rt dir
)
add_compile_options(
        ${OS_C_FLAGS}

        "$<$<CONFIG:Debug>:${OS_FLAGS_DEBUG}>"
        "$<$<CONFIG:Release>:${OS_C_FLAGS_RELEASE}>"

        #        "$<$<COMPILE_LANGUAGE:CXX>:${OS_CXX_FLAGS}>"

        "$<$<PLATFORM_ID:Linux>:-D__linux__>"
        "$<$<PLATFORM_ID:Darwin>:-D__APPLE__>"
)
add_compile_definitions(
        OS_VERSION="${CMAKE_PROJECT_VERSION}"
)
add_link_options(
        "$<$<CONFIG:Debug>:${OS_FLAGS_DEBUG}>"
        "$<$<CONFIG:Release>:${OS_C_FLAGS_RELEASE}>"
        "${OS_LINKER_FLAGS}"
)

set(TEMP_OUTPUT ${CMAKE_BINARY_DIR}/image/boot)

if (LAON_TEST)
    add_subdirectory(test/unit)
endif ()

message("")
add_subdirectory(kernel)
message("")
add_subdirectory(kernel/boot)
message("")

#TODO analytics, install
