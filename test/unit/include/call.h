// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_TEST_UNIT_INCLUDE_CALL_H
#define LAONOS_TEST_UNIT_INCLUDE_CALL_H

#include "protocol.h"

#include <stddef.h>

/**
 * respond to the server in an appropriate protocol.
 *
 * @param call code of call
 */
void response(call_code_e call);

/**
 * wait response with flush
 */
void wait_response(void);

/**
 * read until END.
 *
 * @param buff string buffer
 * @param size size of buffer
 */
void read(char *buff, size_t size);

#endif// LAONOS_TEST_UNIT_INCLUDE_CALL_H
