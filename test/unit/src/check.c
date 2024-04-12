// SPDX-License-Identifier: MIT

#include "check.h"

#include <stdio.h>

static char __testout_stream[BUFFER_SIZE];
static FILE __testout = {.stream = __testout_stream, .size = 0};
static FILE *testout = &__testout;

void flush_check(void) { fflush(testout); }

void assert_response(assert_code_e code, unsigned line) {
  fprintf(testout, TEST_RESULT_SPLIT TEST_RESULT_FORMAT, code, line);
}

void expect_response(expect_code_e code, unsigned line) {
  fprintf(testout, TEST_RESULT_SPLIT TEST_RESULT_FORMAT, code, line);
}
