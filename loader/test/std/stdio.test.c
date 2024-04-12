// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include "test.h"

TEST_FILE(stdio);

TEST(putchar_print) {
}

TEST_I(puts_print) {
}

TEST_I(printf_format) {
  int value1 = rand();
  EXPECT_TRUE(0);
  EXPECT_TRUE(0);

  printf("%d", value1);
  ASSERT_TRUE(0);
}
