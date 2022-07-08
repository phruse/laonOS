// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <string.h>

#define VGA_VIDEO_ADDRESS 0xb8000

// video memory pointer 4B text + 4B style
static volatile char *video_mem = (volatile char *) VGA_VIDEO_ADDRESS;

static void cursor_position();

static void putchar__(int character);

static void line_push();

inline static size_t
puts__(const char *text);

// converting unsigned long to char*
static int number_to_chars(char *buffer, uint32_t number, int divisor);

void putchar(const int character) {
  putchar__(character);
  cursor_position();
}

size_t puts(const char *text) {
  size_t temp = puts__(text);
  cursor_position();

  return temp;
}

static void putchar__(const int character) {
  if (character == '\n' || character == '\r') {// newline
    video_mem += 160 - ((uint32_t) (video_mem -
        (volatile char *) VGA_VIDEO_ADDRESS) % 160);
  } else {
    *video_mem++ = (char) character;
    *video_mem++ = 0x0F;
  }

  if (video_mem == (volatile char *) (VGA_VIDEO_ADDRESS + 80 * 24 * 2)) {
    line_push();
  }
}

inline static size_t
puts__(const char *text) {
  size_t length = 0;

  // write text
  while (*text) {
    putchar__(*text++);
    length++;
  }

  return length;
}

size_t printf(const char *format, ...) {
  int32_t **argument = (int32_t **) &format + 1;
  char buffer[20];
  int temp_char;
  size_t length = 0;

  while ((temp_char = (int) *format++) != '\0') {
    if (temp_char == '%') {
      int is_fill_zero = 0;
      int temp_int = 0;
      int number;

      temp_char = (int) *format++;// get format ex) d, s, 0, 9...

      if (temp_char == '0') {// fill '0'
        is_fill_zero = 1;
        temp_char = (int) *format++;
      }

      if (temp_char >= '0' && temp_char <= '9') {// minimum number of digits.
        temp_int = temp_char - '0';
        temp_char = (int) *format++;
      }

      if (!argument) {// null pointer
        argument++;
        continue;
      }

      switch (temp_char) {
      case 'd':                       // int
        if (*(int *) argument < 0) {// check negative
          number = number_to_chars(buffer,
                                   -*(int *) argument++,
                                   10);
          if (is_fill_zero || temp_int == 0) {
            putchar__('-');
            length++;
          } else {
            *buffer = '-';
          }
          number++;
          goto print_number;
        }
      case 'u':// unsigned int
        number = number_to_chars(buffer,
                                 *(unsigned int *) argument++,
                                 10);
        goto print_number;
      case 'x':// hex
        number = number_to_chars(buffer,
                                 *(uint32_t *) argument++,
                                 16);
      print_number:
        if ((temp_int -= number) > 0) {// fill
          while (temp_int-- > 0) {
            putchar__(is_fill_zero ? '0' : ' ');
            length++;
          }
        }
        if (*buffer != '-')// first letter check
          length += puts__((const char *) &buffer + 1);
        else
          length += puts__(buffer);
        break;
      case 's':// string
        length += puts__((const char *) *argument++);
        break;
      default:putchar__(*(int *) argument++);
        length++;
        break;
      }

    } else {// normal text
      putchar__(temp_char);
      length++;
    }
  }

  cursor_position();

  putchar__('\0');
  video_mem -= 2;

  return length;
}

static int number_to_chars(char *buffer, uint32_t number, int divisor) {
  int counter = 0;
  int temp;
  char *start = buffer;
  char *end;

  do {
    temp = (int) (number % divisor);              // one-digit number
    *buffer++ = (char) ((temp < 10) ? temp + '0' :// number
                        temp + 'a' - 10); // hex
    counter++;
  } while (number /= divisor);
  *buffer++ = ' ';// negative

  end = buffer - 1;
  while (start < end) {// reverse
    temp = (unsigned char) *start;
    *start++ = *end;
    *end-- = (char) temp;
  }
  *buffer = '\0';

  return counter;
}

// set cursor position
static void cursor_position() {
  uint32_t position = (uint32_t) (video_mem -
      (volatile char *) VGA_VIDEO_ADDRESS) / 2;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t) (position & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t) ((position >> 8) & 0xFF));
}

// clear console screen
void clear_screen() {
  video_mem = (volatile char *) VGA_VIDEO_ADDRESS;

  // init screen
  for (size_t i = 0; i < 80 * 25 * 2; i += 2) {
    video_mem[i] = '\0';
    video_mem[i + 1] = 0x0F;// black background + white text
  }

  // enable cursor
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0));
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0));

  cursor_position();
}

static void line_push() {
  char *temp_video_mem = (char *) VGA_VIDEO_ADDRESS;

  for (size_t i = 0; i < 24; ++i) {
    for (size_t j = strcpy_s(temp_video_mem,
                             80 * 2, temp_video_mem + 80 * 2);
         j < 80 * 2; j += 2) {
      temp_video_mem[j] = '\0';
    }
    temp_video_mem += 80 * 2;
  }

  video_mem -= 80 * 2;
}
