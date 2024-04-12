// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_TEST_UNIT_INCLUDE_PROTOCOL_H
#define LAONOS_TEST_UNIT_INCLUDE_PROTOCOL_H

#define END "END"

#define TEST_PREFIX "__test"
#define TEST_READY TEST_PREFIX "__ready__"
#define TEST_SEND TEST_PREFIX "__send__"
#define TEST_OK TEST_PREFIX "__ok__"

#define TEST_RESULT_FORMAT "%d %d"     // code, line
#define TEST_INFO_FORMAT "%s %s %s %d" // name, group, file, isolation

#define TEST_RESULT_SPLIT "__split__"

#define TEST_ASSERT_ENTRY 0
#define TEST_EXPECT_ENTRY 100

#define TEST_DUMP_BEGIN 0x100000
#define TEST_DUMP_END 0x100100

#pragma pack(push, 1)
typedef struct {
  void (*const call)(void);
  const char *name;
  const char *group;
  const char *file_name;
  bool is_isolation;
} test_case_t;
#pragma pack(pop)

typedef enum {
  CALL_COUNT = 'c',
  CALL_TEST = 't',
  CALL_REBOOT = 'b',
  CALL_INFO = 'i',
  CALL_EXIT = 'e'
} call_code_e;

static const char *const assert_code_map[] = {"assert not-true",
                                              "assert not memory same"};
typedef enum {
  ASSERT_NOT_TRUE = TEST_ASSERT_ENTRY + 1,
  ASSERT_NOT_MEMORY_SAME = TEST_ASSERT_ENTRY + 2,
} assert_code_e;

static const char *const expect_code_map[] = {"expect not-true"};
typedef enum {
  EXPECT_NOT_TRUE = TEST_EXPECT_ENTRY + 1,
} expect_code_e;

#endif // LAONOS_TEST_UNIT_INCLUDE_PROTOCOL_H
