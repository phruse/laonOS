// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_TEST_UNIT_INCLUDE_CHECK_H
#define LAONOS_TEST_UNIT_INCLUDE_CHECK_H

#include "protocol.h"
#include <stdio.h>

void flush_check(void);

/**
 * assert receive function.
 *
 * @param code assert type for debugging.
 * @param line line where the assertion fails.
 */
void assert_response(assert_code_e code, unsigned line);

/**
 * expect receive function.
 *
 * @param code expect type for debugging.
 * @param line line where the assertion fails.
 */
void expect_response(expect_code_e code, unsigned line);

/**
 * basic assert
 */
#define ASSERT(CASE, CODE)                                                     \
  if (!(CASE)) {                                                               \
    assert_response(CODE, __LINE__);                                           \
    return;                                                                    \
  }                                                                            \
  (void)0

/**
 * case must be ture.
 */
#define ASSERT_TRUE(CASE) ASSERT(CASE, ASSERT_NOT_TRUE)

/**
 * basic expect
 */
#define EXPECT(CASE, CODE)                                                     \
  if (!(CASE))                                                                 \
    expect_response(CODE, __LINE__);                                           \
  (void)0

/**
 * case should be ture.
 */
#define EXPECT_TRUE(CASE) EXPECT(CASE, EXPECT_NOT_TRUE)

#endif // LAONOS_TEST_UNIT_INCLUDE_CHECK_H
