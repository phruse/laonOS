# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

message(STATUS "Include ${TEST_CLIENT_NAME}")

function(target_link_test target)
    set(TEST_CLIENT_PATH ${CMAKE_SOURCE_DIR}/test/client)

    target_sources(${target} PRIVATE
            ${TEST_CLIENT_PATH}/src/test.c
            ${TEST_CLIENT_PATH}/src/check.c
            ${TEST_CLIENT_PATH}/src/call.c
            )
    target_include_directories(${target} PRIVATE
            ${TEST_CLIENT_PATH}/include
            ${CMAKE_SOURCE_DIR}/share
            )
    target_compile_definitions(${target} PRIVATE TEST_ENABLE)
    add_dependencies(${target} ${TEST_SERVER_NAME})
endfunction()
