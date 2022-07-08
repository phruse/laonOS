// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STANDARD_INCLUDE_UTILITIES_H
#define LAONOS_LOADER_STANDARD_INCLUDE_UTILITIES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * out wrapper
 *
 * @param port port
 * @param value data to send
 */
inline static void outb(uint16_t port, uint8_t value) {
  asm("outb %0, %1"
  :
  : "a"(value), "Nd"(port));
}

/**
 * in wrapper
 *
 * @param port port
 * @return 8-bit value
 */
inline static uint8_t inb(uint16_t port) {
  uint8_t temp;
  asm("inb %1, %0"
  : "=a"(temp)
  : "Nd"(port));
  return temp;
}

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
#define ERROR_BOUNDARY(function, message) \
  if (!(function)) {                      \
    status_print(puts(message), false);   \
    return;                               \
  } else {                                \
    status_print(puts(message), true);    \
  }

#endif //LAONOS_LOADER_STANDARD_INCLUDE_UTILITIES_H
