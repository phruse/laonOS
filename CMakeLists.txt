# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

set(laonOS_NAME laonOS)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf) # NASM elf format build

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(${laonOS_NAME}
        LANGUAGES C ASM_NASM
        VERSION 0.1
        DESCRIPTION "OS that various pH coexists."
)

message(NOTICE "Hello!")
message("")
message(NOTICE "Project ${CMAKE_PROJECT_NAME}")
message(NOTICE "Build Version ${CMAKE_PROJECT_VERSION}")
message("")

include(Find)
find_build_tools() # tools check
message("")
find_build_library() # library check

# reset
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_RELEASE "")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release)
else ()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/debug)
endif()

# default LINK setting
set(CMAKE_C_LINK_EXECUTABLE
"<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET>"
)
# default COMPILE setting
set(CMAKE_C_COMPILE_OBJECT
"<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")

# default build flags
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(OS_FLAGS_DEBUG
        -g # debug
        -O0
)
set(OS_C_FLAGS_RELEASE
        -O2 # optimization 2
)
set(OS_ASM_FLAG
        -Ox # optimization
)
set(OS_C_FLAGS
        -Wall               # enable all warnings
        -Werror             # warnings become error
        -Wextra             # additional warning flags
        -Wpedantic          # strict ISO C
        -ffreestanding      # freestanding build
        -D_FORTIFY_SOURCE=2 # prevent buffer overflows
        -mno-red-zone       # avoid interrupt handler problem TODO

        $<$<CONFIG:Debug>:-Wno-error=unused-variable>
        $<$<PLATFORM_ID:Darwin>:-Wno-error=unused-command-line-argument>
)
set(OS_CXX_FLAGS
        -fuse-cxa-atexit        # do not add a unload destructor
        -fno-threadsafe-statics # do not add a guard variable
        -fno-exceptions         # exceptions need C++ runtime.
        -fno-rtti               # same reason, and not required to OS
)
set(OS_LINKER_FLAGS
        -n                              # read only text section
        -no-pie                         # no pie code
        -static                         # link only static library
        -nostdlib                       # block linking std library
        -L${LAON-OS_CLANG_LIB_DIR}      # compiler-rt dir
)
add_compile_options(
    "$<$<CONFIG:Debug>:${OS_FLAGS_DEBUG}>"
    "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:${OS_C_FLAGS_RELEASE}>"
    "$<$<AND:$<COMPILE_LANGUAGE:ASM_NASM>,$<CONFIG:Release>>:${OS_ASM_FLAG}>"
    "$<$<COMPILE_LANGUAGE:C>:${OS_C_FLAGS}>"
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

message("")
add_subdirectory(kernel)
message("")
add_subdirectory(loader)
message("")

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
                "${CMAKE_SOURCE_DIR}/loader/grub/grub.cfg"
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
            "${CMAKE_BINARY_DIR}/extract" # TODO replace mkisofs
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
                "${CMAKE_SOURCE_DIR}/loader/grub/grub_debug.cfg"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg" # copy grub setting
            COMMAND ${LAON-OS_GRUB_DIR} -o
                "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
    )
endif()

# kernel
add_dependencies(${laonOS_NAME} ${KERNEL_NAME})
# loader
add_dependencies(${laonOS_NAME} ${LOADER_NAME})

#TODO analytics, install
