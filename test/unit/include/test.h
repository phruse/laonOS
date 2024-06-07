// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_TEST_UNIT_INCLUDE_TEST_H
#define LAONOS_TEST_UNIT_INCLUDE_TEST_H

#include <stdint.h>

#include "check.h"

/**
 * test starting point.
 */
_Noreturn void test_start(void);

/**
 * test starting macro.
 */
#define TEST_START test_start()

#define TEST_BREAK(NAME)

/**
 * test file macro
 * must exists in front of test file
 *
 * @param GROUP case group name
 */
#define TEST_FILE(GROUP)                                                       \
  static const char __file_name[] = __FILE__;                                  \
  static char __group_name[] = #GROUP

/**
 * basic test macro template.
 */
#define TEST_BASIC(NAME, OPTION)                                               \
  void NAME##_test(void);                                                      \
  const char NAME##_name[] = #NAME;                                            \
  __attribute__((section("test_list")))                                        \
  const test_case_t NAME##_info = {.call = &NAME##_test,                       \
                                   .name = NAME##_name,                        \
                                   .group = __group_name,                      \
                                   .file_name = __file_name,                   \
                                   OPTION};                                    \
  void NAME##_test(void)

/**
 * test case macro.
 *
 * @param NAME case name
 */
#define TEST(NAME) TEST_BASIC(NAME, )

/**
 * test case macro with isolated running.
 *
 * @param NAME case name
 */
#define TEST_I(NAME) TEST_BASIC(NAME, .is_isolation = true)

#endif // LAONOS_TEST_UNIT_INCLUDE_TEST_H
