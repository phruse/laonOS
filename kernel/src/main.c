// SPDX-License-Identifier: MIT

#include "main.h"
int a  = 0;

 void main(void) {
  // change stack area
  // remove unused page table
  asm volatile("hlt");
  while (a++)
    ;
}
