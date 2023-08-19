# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(TEST_SERVER_NAME test-server)
set(TEST_SERVER_NAME ${TEST_SERVER_NAME} PARENT_SCOPE)

set(LIBCXX_BUILD /Users/itaeyeon/llvm-project/build)

# module enable START

if (CMAKE_VERSION VERSION_LESS "3.27.0")
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
else ()
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
endif ()
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

include(FetchContent)
fetchcontent_declare(
        std
        URL "file://${LIBCXX_BUILD}/modules/c++/v1/"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
fetchcontent_getproperties(std)
if (NOT std_POPULATED)
    fetchcontent_populate(std)
    add_subdirectory(${std_SOURCE_DIR} ${std_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# module enable END

message(STATUS " Include ${TEST_SERVER_NAME} ")

set(TEST_SERVER_SRC
        src/argument.cppm
        src/tab.cppm

        src/process/qemu.cppm
        src/process/process.cppm

        src/connect/tty.cppm
        src/connect/call.cppm
        src/connect/protocol.cppm
)

add_executable(${TEST_SERVER_NAME})
target_link_options(${TEST_SERVER_NAME} PRIVATE -fuse-ld=lld)
target_include_directories(${TEST_SERVER_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/share
)
target_sources(
        ${TEST_SERVER_NAME}
        PRIVATE
        main.cpp

        PRIVATE FILE_SET CXX_MODULES
        BASE_DIRS
        FILES
        ${TEST_SERVER_SRC}
)

target_compile_options(${TEST_SERVER_NAME} PRIVATE
        -W -Wall -Werror -Wpedantic
        $<$<CONFIG:Debug>:-Wno-error=unused-parameter>

        -fprebuilt-module-path=${CMAKE_BINARY_DIR}/_deps/std-build/CMakeFiles/std.dir/
        -fprebuilt-module-path=${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/test-server.dir

        -nostdinc++
        -isystem ${LIBCXX_BUILD}/modules/c++/v1
)
target_link_options(${TEST_SERVER_NAME} PRIVATE
        -nostdlib++
        -L${LIBCXX_BUILD}/lib
        -Wl,-rpath,${LIBCXX_BUILD}/lib
)
target_link_libraries(${TEST_SERVER_NAME} PRIVATE
        std c++
)

