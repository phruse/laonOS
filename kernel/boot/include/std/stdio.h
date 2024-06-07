// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_BOOT_INCLUDE_STD_STDIO_H
#define LAONOS_BOOT_INCLUDE_STD_STDIO_H

#include "utilities.h"

#include <stdarg.h>
#include <stddef.h>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25
#define BUFFER_SIZE 1024

typedef struct {
  char *stream;
  size_t size;
} FILE;

extern FILE *stdout;

/**
 * initialize screen
 */
void init_screen(void);

/**
 * clear console screen
 */
void clear_screen(void);

/**
 * flush stream
 *
 * @param file flushed file
 */
void fflush(FILE *file);

/**
 * putchar to file.
 * @param file file descriptor
 * @param character character to print.
 */
void fputchar(int character, FILE *file);

/**
 * puts to file.
 * '\n' not include
 *
 * @param text text to print.
 * @param file file descriptor
 * @return length of printed string
 */
size_t fputs(const char *text, FILE *file);

/**
 * print to file according to the format string.
 *
 * @param file file descriptor
 * @param format format string
 * @param ... additional format string
 * @return length of printed string
 */
size_t fprintf(FILE *file, const char *format, ...);

/**
 * flush stdout
 */
void flush(void);

/**
 * putchar for debugging.
 *
 * @param character character to print.
 */
void putchar(int character);

/**
 * puts for debugging.
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

/**
 *
 * @return raed charactor
 */
char getchar(void);

#endif // LAONOS_BOOT_INCLUDE_STD_STDIO_H
