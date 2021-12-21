// SPDX-License-Identifier: MIT

#include "kernel.h"

volatile int a = 10000;

void dwdw() {
    while (1) {
        a++;
    }
}

void kmain() {
    static int c = 9, q = 0, r;
    while (1) {
        a++;
        c = a;
        q = c;
        r = a;
    }
    dwdw();
}
