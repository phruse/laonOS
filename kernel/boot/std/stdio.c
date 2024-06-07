// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void __putchar(int character);
extern char __getchar(void);

extern void cursor_position(void);
extern void line_push(void);

static char __stdout_stream[BUFFER_SIZE];
static FILE __stdout = {.stream = __stdout_stream, .size = 0};
FILE *stdout = &__stdout;

// converting unsigned long to char*
static int number_to_chars(char *buffer, uint32_t number, int divisor);

void write_file(FILE *file, char c) {
  if (file != NULL) {
    file->stream[file->size++] = c;
  }
}

void init_screen(void) {
  clear_screen();
}

void fputchar(int character, FILE *file) { write_file(file, (char)character); }

void putchar(int character) { fputchar(character, stdout); }

size_t fputs(const char *text, FILE *file) {
  size_t length = 0;

  // write text
  while (*text) {
    write_file(file, *text++);
    length++;
  }

  return length;
}

size_t puts(const char *text) { return fputs(text, stdout); }

void fflush(FILE *file) {
  for (size_t i = 0; i < file->size; ++i) {
    __putchar(file->stream[i]);
  }
  __putchar('\0');

  file->size = 0;
  cursor_position();
}

void flush(void) { fflush(stdout); }

static size_t vfprintf(FILE *file, const char *format, va_list arg) {
  int32_t **argument = (int32_t **)arg;
  char buffer[20];
  int temp_char;
  size_t length = 0;

  while ((temp_char = (int)*format++) != '\0') {
    if (temp_char == '%') {
      int is_fill_zero = 0;
      int temp_int = 0;
      int number;

      temp_char = (int)*format++; // get format ex) d, s, 0, 9...

      if (temp_char == '0') { // fill '0'
        is_fill_zero = 1;
        temp_char = (int)*format++;
      }

      if (temp_char >= '0' && temp_char <= '9') { // minimum number of digits.
        temp_int = temp_char - '0';
        temp_char = (int)*format++;
      }

      if (!argument) { // null pointer
        argument++;
        continue;
      }

      switch (temp_char) {
      case 'd':                     // int
        if (*(int *)argument < 0) { // check negative
          number = number_to_chars(buffer, -*(int *)argument++, 10);
          if (is_fill_zero || temp_int == 0) {
            fputchar('-', file);
            length++;
          } else {
            *buffer = '-';
          }
          number++;
          goto print_number;
        }
      case 'u': // unsigned int
        number = number_to_chars(buffer, *(unsigned int *)argument++, 10);
        goto print_number;
      case 'x': // hex
        number = number_to_chars(buffer, *(uint32_t *)argument++, 16);
      print_number:
        if ((temp_int -= number) > 0) { // fill
          while (temp_int-- > 0) {
            fputchar(is_fill_zero ? '0' : ' ', file);
            length++;
          }
        }
        if (*buffer != '-') // first letter check
          length += fputs((const char *)&buffer + 1, file);
        else
          length += fputs(buffer, file);
        break;
      case 'c': // char
        fputchar(*(char *)argument++, file);
        length++;
        break;
      case 's': // string
        length += fputs((const char *)*argument++, file);
        break;
      default:
        fputchar(*(int *)argument++, file);
        length++;
        break;
      }

    } else { // normal text
      fputchar(temp_char, file);
      length++;
    }
  }

  return length;
}

size_t fprintf(FILE *file, const char *format, ...) {
  va_list args;
  va_start(args, format);
  size_t size = vfprintf(file, format, args);
  va_end(args);
  return size;
}

size_t printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  size_t size = vfprintf(stdout, format, args);
  va_end(args);

  flush();

  return size;
}

char getchar(void) { return __getchar(); }

static int number_to_chars(char *buffer, uint32_t number, int divisor) {
  int counter = 0;
  int temp;
  char *start = buffer;
  char *end;

  do {
    temp = (int)(number % divisor);               // one-digit number
    *buffer++ = (char)((temp < 10) ? temp + '0' : // number
                           temp + 'a' - 10);      // hex
    counter++;
  } while (number /= divisor);
  *buffer++ = ' '; // negative

  end = buffer - 1;
  while (start < end) { // reverse
    temp = (unsigned char)*start;
    *start++ = *end;
    *end-- = (char)temp;
  }
  *buffer = '\0';

  return counter;
}
