// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_SRC_SERIAL_H
#define LAONOS_LOADER_INCLUDE_SRC_SERIAL_H

#include <stdbool.h>
#include <stddef.h>

/**
 * serial communication initialization
 *
 * @return if the serial port is working returns true
 */
bool serial_init(void);

/**
 * ready to write
 *
 * @return if writable returns true
 */
bool is_writable(void);

/**
 * ready to read
 *
 * @return if readable returns true
 */
bool is_readable(void);

/**
 * writing one character to serial port
 *
 * @param character character to write
 */
void serial_write(char character);

/**
 * reading one character to serial port
 *
 * @return read character
 */
char serial_read(void);

#endif // LAONOS_LOADER_INCLUDE_SRC_SERIAL_H
