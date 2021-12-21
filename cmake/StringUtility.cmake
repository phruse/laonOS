# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include_guard(DIRECTORY)

function(first_letter_uppercase str out_str)

    string(SUBSTRING ${str} 0 1 first_letter)
    string(TOUPPER ${first_letter} first_letter)
    string(LENGTH ${str} length)
    execute_process(COMMAND expr ${length} - 1 OUTPUT_VARIABLE length)
    string(SUBSTRING ${str} 1 ${length} letters)
    set(${out_str} "${first_letter}${letters}" PARENT_SCOPE)

endfunction()

function(get_output_path target output_path)

    get_property(path TARGET ${target} PROPERTY RUNTIME_OUTPUT_DIRECTORY)
    get_property(name TARGET ${target} PROPERTY OUTPUT_NAME)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(${output_path} "${path}\\${name}" PARENT_SCOPE)
    else()
        set(${output_path} "${path}/${name}" PARENT_SCOPE)
    endif()

endfunction()
