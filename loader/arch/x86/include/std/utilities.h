// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_ARCH_I386_INCLUDE_STD_UTILITIES_H
#define LAONOS_LOADER_ARCH_I386_INCLUDE_STD_UTILITIES_H

#include <stdint.h>

/**
 * outb wrapper
 *
 * @param port port
 * @param value data to send
 */
inline static void outb(uint16_t port, uint8_t value) {
  asm("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * inb wrapper
 *
 * @param port port
 * @return 8-bit value
 */
inline static uint8_t inb(uint16_t port) {
  uint8_t temp;
  asm("inb %1, %0" : "=a"(temp) : "Nd"(port));
  return temp;
}

/**
 * outw wrapper
 *
 * @param port port
 * @param value data to send
 */
inline static void outw(uint16_t port, uint16_t value) {
  asm("outw %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * inw wrapper
 *
 * @param port port
 * @return 8-bit value
 */
inline static uint32_t inw(uint16_t port) {
  uint8_t temp;
  asm("inw %1, %0" : "=a"(temp) : "Nd"(port));
  return temp;
}

#endif // LAONOS_LOADER_ARCH_I386_INCLUDE_STD_UTILITIES_H
