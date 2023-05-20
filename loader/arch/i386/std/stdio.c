// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <string.h>

#include "serial.h"

#define VGA_VIDEO_ADDRESS 0xb8000

// video memory pointer 4B text + 4B style
static volatile char *video_mem = (volatile char *)VGA_VIDEO_ADDRESS;
static bool is_serial = false;

void cursor_position(void);
void line_push(void);

// clear console screen
void clear_screen(void) {
  // init serial
  is_serial = serial_init();

  // init screen
  video_mem = (volatile char *)VGA_VIDEO_ADDRESS;
  for (size_t i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT * 2; i += 2) {
    video_mem[i] = '\0';
    video_mem[i + 1] = 0x0F; // black background + white text
  }

  // enable cursor
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0));
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0));

  cursor_position();
}

void __putchar(const int character) {
  if (is_serial && character != '\0') {
    serial_write((char)character);
  }

  if (character == '\n' || character == '\r') { // newline
    video_mem +=
        160 -
        ((uint32_t)(video_mem - (volatile char *)VGA_VIDEO_ADDRESS) % 160);
  } else if (character == '\0') {
    *video_mem = (char)character;
  } else {
    *video_mem++ = (char)character;
    *video_mem++ = 0x0F;
  }

  if (video_mem ==
      (volatile char *)(VGA_VIDEO_ADDRESS +
                        CONSOLE_WIDTH * (CONSOLE_HEIGHT - 1) * 2)) {
    line_push();
  }
}

size_t __puts(const char *text) {
  size_t length = 0;

  // write text
  while (*text) {
    __putchar(*text++);
    length++;
  }

  return length;
}

// set cursor position
void cursor_position(void) {
  uint32_t position =
      (uint32_t)(video_mem - (volatile char *)VGA_VIDEO_ADDRESS) / 2;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(position & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

void line_push(void) {
  char *temp_video_mem = (char *)VGA_VIDEO_ADDRESS;

  for (size_t i = 0; i < CONSOLE_HEIGHT - 1; ++i) {
    for (size_t j = strcpy_s(temp_video_mem, CONSOLE_WIDTH * 2,
                             temp_video_mem + CONSOLE_WIDTH * 2);
         j < CONSOLE_WIDTH * 2; j += 2) {
      temp_video_mem[j] = '\0';
    }
    temp_video_mem += CONSOLE_WIDTH * 2;
  }

  video_mem -= CONSOLE_WIDTH * 2;
}
