cmake_minimum_required(VERSION 3.20)

set(LOADER_TEST_NAME ${LOADER_NAME}-test)
set(TEST_NAME ${laonOS_NAME}-${LOADER_NAME}-test)

# loader-test
loader_build(${LOADER_TEST_NAME})
target_sources(
        ${LOADER_TEST_NAME}
        PRIVATE
        test/std/stdio.test.c
)
target_link_libraries(${LOADER_TEST_NAME} ${TEST_UNIT_NAME_NAME})
target_link_test(${LOADER_TEST_NAME})
target_precompile_headers(${LOADER_TEST_NAME} PRIVATE ${TEST_UNIT_PRE_HEADER})

# ISO build
build_iso(${TEST_NAME} "loader/configs/grub-test.cfg")
add_dependencies(${TEST_NAME} ${LOADER_TEST_NAME}) # loader

# test
add_test(NAME "${LOADER_NAME} test"
        COMMAND ${LAON_CONSOLE} unit ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${TEST_NAME}.iso
)
