// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_SRC_SERIAL_H
#define LAONOS_LOADER_INCLUDE_SRC_SERIAL_H

#include <stdbool.h>

/**
 * serial communication initialization
 *
 * @return if the serial port is working returns true
 */
bool serial_init(void);

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
