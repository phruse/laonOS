// SPDX-License-Identifier: MIT

#include "test.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utilities.h>

#include "call.h"
#include "protocol.h"
#include "serial.h"

extern test_case_t __start_test_list;
extern test_case_t __stop_test_list;

#define RESPONSE_CASE(CALL_CODE, FUNCTION)                                     \
  case CALL_CODE:                                                              \
    (FUNCTION);                                                                \
    response(CALL_CODE);                                                       \
    break

#define RESPONSE_PRE_CASE(CALL_CODE, FUNCTION)                                 \
  case CALL_CODE:                                                              \
    response(CALL_CODE);                                                       \
    FUNCTION;                                                                  \
    break

static void call_count(void);
static void call_test(void);
static void call_info(void);
inline static void qemu_reboot(void);
inline static void qemu_exit(void);

static int get_int(void);
static test_case_t *get_test();

_Noreturn void test_start(void) {
  printf(TEST_READY);

  while (1) {
    char code = serial_read();

    switch (code) {
      RESPONSE_CASE(CALL_COUNT, call_count());
      RESPONSE_CASE(CALL_TEST, call_test());
      RESPONSE_CASE(CALL_INFO, call_info());
      RESPONSE_PRE_CASE(CALL_REBOOT, qemu_reboot());
      RESPONSE_PRE_CASE(CALL_EXIT, qemu_exit());
    default:
    }

    wait_response();
  }
}

static void call_count(void) {
  size_t test_count =
      ((uintptr_t)(&__stop_test_list) - (uintptr_t)(&__start_test_list)) /
      sizeof(test_case_t);

  printf("%d", test_count);
}

static void call_test(void) {
  test_case_t *current_test = get_test();
  if (current_test < &__stop_test_list && current_test != NULL) {
    current_test->call();
  }
  flush_check();
}

static void call_info(void) {
  test_case_t *current_test = get_test();
  printf(TEST_INFO_FORMAT, current_test->name, current_test->group,
         current_test->file_name, current_test->is_isolation);
}

inline static void qemu_reboot(void) { asm("jmp 0xffff"); }

inline static void qemu_exit(void) { asm("cli; hlt"); }

static int get_int(void) {
  char buff[8];
  for (size_t i = 0; i < sizeof buff; ++i) {
    buff[i] = serial_read();

    if (buff[i] == '\0') {
      break;
    }
  }

  return atoi(buff);
}

static test_case_t *get_test(void) {
  return &__start_test_list + get_int() - 1;
}
