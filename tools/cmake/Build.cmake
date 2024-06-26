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

    set(SYM_DIR ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${target}.sym)
    set(LAON_OBJCOPY_EXTRACT
            ${LAON_OBJCOPY} --only-keep-debug
            "${output}/${target}.elf" ${SYM_DIR}
            )
    set(LAON_OBJCOPY_REMOVE
            ${LAON_OBJCOPY} --strip-debug
            "${output}/${target}.elf"
            )
    add_custom_command(TARGET ${target}
            POST_BUILD
            BYPRODUCTS ${SYM_DIR}
            COMMENT "Extracting debugging symbols..."
            COMMAND ${CMAKE_COMMAND} -E remove ${SYM_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
            COMMAND "$<$<CONFIG:Debug>:${LAON_OBJCOPY_EXTRACT}>"
            COMMAND "$<$<CONFIG:Release>:${LAON_OBJCOPY_REMOVE}>"
            COMMAND_EXPAND_LISTS
            VERBATIM
            )

    first_letter_uppercase(${target} UPPERCASE_TARGET_NAME)
    get_output_dir(${target} TARGET_DIR)

    message(STATUS "${UPPERCASE_TARGET_NAME} build output to: ${TARGET_DIR}")

endfunction()

function(build_iso target config)
    build_iso_dir(${target} ${config} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endfunction()

function(build_iso_dir target config output_dir)
    add_custom_target(${target} ALL
            BYPRODUCTS
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${target}.iso"
            COMMENT "Creation ISO image..."
            COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CMAKE_BINARY_DIR}/image/boot/grub" # clean old folder
            COMMAND ${CMAKE_COMMAND} -E copy
            "${config}"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg" # copy grub setting
            COMMAND ${LAON_GRUB_DIR} -o
            "${output_dir}/${target}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
            )
endfunction()

function(build_iso_secure target config)
    add_custom_target(${target} ALL
            BYPRODUCTS
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${target}.iso"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg"
            COMMENT "Creation ISO image..."
            COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CMAKE_BINARY_DIR}/image/boot/grub" # clean old folder
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/${config}"
            "${CMAKE_BINARY_DIR}/image/boot/grub/grub.cfg" # copy grub setting
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_BINARY_DIR}/extract"
            COMMAND ${LAON_GRUB_DIR} -o
            "${CMAKE_BINARY_DIR}/${target}.iso"
            "${CMAKE_BINARY_DIR}/image" # create image
            COMMAND ${LAON_XORRISO_DIR} -osirrox on -indev
            "${CMAKE_BINARY_DIR}/${target}.iso"
            -extract . "${CMAKE_BINARY_DIR}/extract" # extract ISO
            COMMAND chmod -R +w "${CMAKE_BINARY_DIR}/extract"
            COMMAND cd "${CMAKE_BINARY_DIR}/extract"
            COMMAND find ./* -type f | xargs sha1sum | sed 's/.\\\//\\\//'
            > "${CMAKE_BINARY_DIR}/extract/sha1.txt" # hash
            COMMAND ${LAON_GRUB_DIR} -o
            "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${target}.iso"
            "${CMAKE_BINARY_DIR}/extract"
            COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CMAKE_BINARY_DIR}/extract" # clean
            )
endfunction()
