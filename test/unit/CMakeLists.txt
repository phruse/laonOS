# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

message(STATUS "Include test/unit")
set(TEST_UNIT_PRE_HEADER ${CMAKE_SOURCE_DIR}/test/unit/include/test.h)

function(target_link_test target)
    set(TEST_UNIT_NAME_PATH ${CMAKE_SOURCE_DIR}/test/unit)

    target_sources(${target} PRIVATE
            ${TEST_UNIT_NAME_PATH}/src/test.c
            ${TEST_UNIT_NAME_PATH}/src/check.c
            ${TEST_UNIT_NAME_PATH}/src/call.c
            )
    target_include_directories(${target} PRIVATE
            ${TEST_UNIT_NAME_PATH}/include
            )
    target_compile_definitions(${target} PRIVATE TEST_ENABLE)
endfunction()
