/* SPDX-License-Identifier: MIT */

ENTRY(main) /* set loader.asm's main to entry */

SECTIONS {
    . = 1M; /* load from 1MB position */

    _loader_start = .;

    .text ALIGN(4K) :
    {
        KEEP(*(.multiboot2)) /* multiboot2 header */
        *(.text)
    }
    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    }
    .data ALIGN(4K) :
    {
        *(.data)
    }
    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
        KEEP(*(.loader_stack)) /* loader stack */
    }

    _loader_end = .;
}
