// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STD_INCLUDE_PLATFORM_I386_UTILITIES_H
#define LAONOS_LOADER_STD_INCLUDE_PLATFORM_I386_UTILITIES_H

#include <stdint.h>

/**
 * out wrapper
 *
 * @param port port
 * @param value data to send
 */
inline static void outb(uint16_t port, uint8_t value) {
  asm("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * in wrapper
 *
 * @param port port
 * @return 8-bit value
 */
inline static uint8_t inb(uint16_t port) {
  uint8_t temp;
  asm("inb %1, %0" : "=a"(temp) : "Nd"(port));
  return temp;
}

#endif // LAONOS_LOADER_STD_INCLUDE_PLATFORM_I386_UTILITIES_H
