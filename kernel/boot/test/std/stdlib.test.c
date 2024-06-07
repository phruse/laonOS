// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "test.h"

TEST_FILE(std);

TEST(atoi) {
  ASSERT_TRUE(atoi("0") == 0);
  ASSERT_TRUE(atoi("4255") == 4255);
  ASSERT_TRUE(atoi("343235") == 343235);
  ASSERT_TRUE(atoi("-4324546") == -4324546);
  ASSERT_TRUE(atoi("132352356") == 132352356);
}

TEST(rand) {
  int value1 = rand();
  int value2 = rand();
  int value3 = rand();
  int value4 = rand();

  ASSERT_FALSE(value1 == value3);
  ASSERT_FALSE(value2 == value4);
}
