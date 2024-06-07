// SPDX-License-Identifier: MIT

#include "call.h"

#include <stdio.h>
#include <string.h>

#define READ_SIZE 512

// make extended string
#define MAKE_STRING(NAME, CALL, PREFIX)                                        \
  char NAME[sizeof(PREFIX) + 2];                                               \
  strcpy_s(NAME, sizeof(PREFIX), PREFIX);                                      \
  (NAME)[sizeof(PREFIX) - 1] = CALL;                                           \
  (NAME)[sizeof(PREFIX)] = '\0'

extern void __flush(void);

void response(call_code_e call) {
  MAKE_STRING(send, call, TEST_SEND);
  printf("%s", send);
  wait_response();

  MAKE_STRING(ok, call, TEST_OK);
  char buffer[READ_SIZE];
  for (size_t i = 0; i < READ_SIZE; ++i) {
    buffer[i] = getchar();

    if (strstr(buffer, ok) != NULL) {
      break;
    }
  }
}

void wait_response(void) {
  __flush();
  while (getchar() != '\n')
    ;
}

void read(char *buff, size_t size) {
  for (size_t count = 0; count < size; ++count) {
    buff[count] = getchar();
    if (count > 1 && strcmp(&buff[count - 2], END) == 0) {
      break;
    }
  }
}
