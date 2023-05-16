// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STANDARD_INCLUDE_UTILITIES_H
#define LAONOS_LOADER_STANDARD_INCLUDE_UTILITIES_H

#include <stdbool.h>

#ifdef i386
#include "platform/i386/utilities.h"
#endif

/**
 * status print helper
 *
 * @param length printed length
 * @param status status
 */
void status_print(size_t length, bool status);

/**
 * error boundary macro
 */
#define ERROR_BOUNDARY(function, message)                                      \
  if (!(function)) {                                                           \
    status_print(puts(message), false);                                        \
    while (1);                                                                 \
  } else {                                                                     \
    status_print(puts(message), true);                                         \
  }

#endif // LAONOS_LOADER_STANDARD_INCLUDE_UTILITIES_H
