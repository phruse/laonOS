// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_BOOT_INCLUDE_STD_UTILITIES_H
#define LAONOS_BOOT_INCLUDE_STD_UTILITIES_H

#include <stdbool.h>
#include <stddef.h>

#ifdef i386
#include "std/utilities.h"
#endif

/**
 * status print helper
 *
 * @param length printed length
 * @param status status
 */
void just_print(size_t length, bool status);

/**
 * error boundary macro
 */
#define ERROR_BOUNDARY(function, message)                                      \
  if (!(function)) {                                                           \
    just_print(puts(message), false);                                        \
    while (1)                                                                  \
      ;                                                                        \
  } else {                                                                     \
    just_print(puts(message), true);                                         \
  }

#endif // LAONOS_BOOT_INCLUDE_STD_UTILITIES_H
