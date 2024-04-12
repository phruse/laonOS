// SPDX-License-Identifier: MIT

#include "serial.h"

#include <stdlib.h>
#include <utilities.h>

enum {
  COM_PORT = 0x3f8,
  COM_INTERRUPT = (COM_PORT + 1),
  COM_FIFO_CONTROL = (COM_PORT + 2),
  COM_LINE_CONTROL = (COM_PORT + 3),
  COM_MODEM_CONTROL = (COM_PORT + 4),
  COM_STATUS = (COM_PORT + 5),
};

// interrupt
#define NO_INTERRUPT_BIT 0x0
// FIFO
#define FIFO_DISABLE_BIT (1 << 0)
#define TRIGGER_LEVEL_3_BIT ((1 << 6) | (1 << 7))
// line
#define DIVISOR_LATCH_ACCESS_BIT (1 << 7)
#define WORD_8_BIT ((1 << 0) | (1 << 1))
// moden
#define IRQ_BIT (1 << 3)
#define LOOPBACK_BIT ((1 << 1) | (1 << 4))
#define OUT_BIT (1 << 2)
#define READY_BIT ((1 << 0) | (1 << 1))
// status
#define DATA_READY_BIT (1 << 0)
#define EMPTY_BUFFER_BIT (1 << 5)
// baud rate
#define BAUD 115200
#define RATE_LOW ((115200 / BAUD) & 0xff)
#define RATE_HIGH (uint8_t)((115200 / BAUD) >> 8)

bool serial_init(void) {
  outb(COM_INTERRUPT, NO_INTERRUPT_BIT);

  // rate setting
  outb(COM_LINE_CONTROL, DIVISOR_LATCH_ACCESS_BIT);
  outb(COM_PORT + 0, RATE_LOW);       // MIN
  outb(COM_PORT + 1, RATE_HIGH);      // MAX
  outb(COM_LINE_CONTROL, WORD_8_BIT); // 8BIT

  // general setting
  outb(COM_FIFO_CONTROL, FIFO_DISABLE_BIT | TRIGGER_LEVEL_3_BIT);
  outb(COM_MODEM_CONTROL, IRQ_BIT | READY_BIT);

  // test
  outb(COM_MODEM_CONTROL, OUT_BIT | LOOPBACK_BIT);
  uint8_t test_value = (uint8_t)(rand() % 0xFF);
  outb(COM_PORT + 0, test_value);
  if (inb(COM_PORT) != test_value) {
    return false;
  }

  // reset
  outb(COM_MODEM_CONTROL, OUT_BIT | READY_BIT);
  return true;
}

bool is_writable(void) { return (inb(COM_STATUS) & EMPTY_BUFFER_BIT) == 0; }
bool is_readable(void) { return (inb(COM_STATUS) & DATA_READY_BIT) == 0; }

void serial_write(char character) {
  while (is_writable())
    ;
  outb(COM_PORT, character);
}

char serial_read(void) {
  while (is_readable())
    ;
  return inb(COM_PORT);
}
