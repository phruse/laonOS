cmake_minimum_required(VERSION 3.20)

set(BOOT_TEST_NAME ${BOOT_NAME}-test)
set(TEST_NAME ${laonOS_NAME}-${BOOT_NAME}-test)

# boot-test
boot_build(${BOOT_TEST_NAME})
target_sources(
        ${BOOT_TEST_NAME}
        PRIVATE
        test/std/stdlib.test.c
        test/std/string.test.c
)
target_link_libraries(${BOOT_TEST_NAME} ${TEST_UNIT_NAME_NAME})
target_link_test(${BOOT_TEST_NAME})
target_precompile_headers(${BOOT_TEST_NAME} PRIVATE ${TEST_UNIT_PRE_HEADER})

# ISO build
build_iso(${TEST_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/configs/grub-test.cfg")
add_dependencies(${TEST_NAME} ${BOOT_TEST_NAME}) # boot

# test
add_test(NAME "${BOOT_NAME} test"
        COMMAND ${TEST_CONSOLE_NAME} unit ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${TEST_NAME}.iso
)
