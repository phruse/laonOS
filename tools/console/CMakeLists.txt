# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(TEST_CONSOLE_NAME test-console)
set(TEST_CONSOLE_NAME ${TEST_CONSOLE_NAME} PARENT_SCOPE)

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
        URL "file://${LAON_LIBCXX_BUILD}/modules/c++/v1/"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
fetchcontent_getproperties(std)

if (NOT std_POPULATED)
    fetchcontent_populate(std)
    add_subdirectory(${std_SOURCE_DIR} ${std_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# module enable END

message(STATUS "Include test/console")

set(TEST_SERVER_SRC
        common/argument.cpp

        unit/format.cpp
        unit/qemu.cpp
        unit/process.cpp
        unit/tty.cpp
        unit/call.cpp
        unit/protocol.cpp
)

add_executable(${TEST_CONSOLE_NAME})
add_dependencies(${TEST_CONSOLE_NAME} llvm-project)
target_link_options(${TEST_CONSOLE_NAME} PRIVATE -fuse-ld=lld)
target_include_directories(${TEST_CONSOLE_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/test/unit/include
)
target_sources(
        ${TEST_CONSOLE_NAME}
        PRIVATE
        main.cpp

        PRIVATE FILE_SET CXX_MODULES
        BASE_DIRS .
        FILES
        ${TEST_SERVER_SRC}
)

target_compile_options(${TEST_CONSOLE_NAME} PRIVATE
        -W -Wall -Werror -Wpedantic
        $<$<CONFIG:Debug>:-Wno-error=unused-parameter>

        -fprebuilt-module-path=${CMAKE_BINARY_DIR}/_deps/std-build/CMakeFiles/std.dir/
        -fprebuilt-module-path=${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${TEST_CONSOLE_NAME}.dir

        -nostdinc++
        -isystem ${LAON_LIBCXX_BUILD}/modules/c++/v1
)
target_link_options(${TEST_CONSOLE_NAME} PRIVATE
        -nostdlib++
        -L${LAON_LIBCXX_BUILD}/lib
        -Wl,-rpath,${LAON_LIBCXX_BUILD}/lib
)
target_link_libraries(${TEST_CONSOLE_NAME} PRIVATE
        std c++
)
