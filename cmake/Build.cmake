# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

include_guard(DIRECTORY)

function(build_binary target output src)
    include(StringUtility)

    add_executable(${target} ${src})
    set_target_properties(${target} PROPERTIES
            PREFIX ""
            SUFFIX ""
            OUTPUT_NAME "${target}.elf"
            RUNTIME_OUTPUT_DIRECTORY ${output}
    )
    target_compile_definitions(${target} PUBLIC
            $<$<CONFIG:Debug>:_DEBUG>
    )

    set(SYM_PATH ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${target}.sym)
    set(OBJCOPY_EXTRACT
            ${OBJCOPY} --only-keep-debug "${output}/${target}.elf" ${SYM_PATH})
    set(OBJCOPY_REMOVE
            ${OBJCOPY} --strip-debug "${output}/${target}.elf")
    add_custom_command(TARGET ${target}
            POST_BUILD
            BYPRODUCTS ${SYM_PATH}
            COMMENT "Extracting debugging symbols..."
            COMMAND ${CMAKE_COMMAND} -E remove ${SYM_PATH}
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
            COMMAND "$<$<CONFIG:Debug>:${OBJCOPY_EXTRACT}>"
            COMMAND "$<$<CONFIG:Debug>:${OBJCOPY_REMOVE}>"
            COMMAND_EXPAND_LISTS
            VERBATIM
    )

    first_letter_uppercase(${target} UPPERCASE_TARGET_NAME)
    get_output_path(${target} TARGET_PATH)

    message(STATUS "${UPPERCASE_TARGET_NAME} build output to: ${TARGET_PATH}")

endfunction()
