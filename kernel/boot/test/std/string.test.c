// SPDX-License-Identifier: MIT

#include <string.h>

#include "test.h"

#define SAMPLE_STR "Hello, World!"

TEST_FILE(std);

TEST(strcmp) {
  ASSERT_FALSE(strcmp("laonOS", "loanoS") == 0);
  ASSERT_TRUE(strcmp("OS", "OS") == 0);
}

TEST(strcpy_s) {
  char *src = SAMPLE_STR;
  char dest[16] = {
      0,
  };

  strcpy_s(dest, 13, src);
  ASSERT_TRUE(strcmp(src, dest) == 0);
}

TEST(strstr) { ASSERT_TRUE(strstr(SAMPLE_STR, "World")[0] == 'W'); }

TEST(strlen) { ASSERT_TRUE(strlen(SAMPLE_STR) == 13); }

TEST(memset) {
  char dest[16];
  memset(dest, 0, 16 * sizeof(char));

  for (int i = 0; i < 16; ++i) {
    ASSERT_TRUE(dest[i] == 0);
  }
}

TEST(memcpy) {
  char *src = SAMPLE_STR;
  char dest[16] = {
      0,
  };

  memcpy(dest, src, 16 * sizeof(char));
  ASSERT_TRUE(strcmp(src, dest) == 0);
}
