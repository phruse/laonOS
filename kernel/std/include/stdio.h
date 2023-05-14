// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_STANDARD_INCLUDE_STDIO_H
#define LAONOS_LOADER_STANDARD_INCLUDE_STDIO_H

#include <stddef.h>
#include <utilities.h>

/**
 * clear console screen
 */
void clear_screen(void);

/**
 * vga text mode putchar for debugging.
 *
 * @param character character to print.
 */
void putchar(int character);

/**
 * vga text mode puts for debugging.
 * '\n' not include
 *
 * @param text text to print.
 * @return length of printed string
 */
size_t puts(const char *text);

/**
 * print to console according to the format string.
 *
 * @param format format string
 * @param ... additional format string
 * @return length of printed string
 */
size_t printf(const char *format, ...);

#endif // LAONOS_LOADER_STANDARD_INCLUDE_STDIO_H
