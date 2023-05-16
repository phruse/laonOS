// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_LOADER_H
#define LAONOS_LOADER_LOADER_H

#include <stdint.h>

/**
 * loader c entry point
 *
 * @param magic_value magic value that checks bootloader.
 * @param info_address pointer of multiboot information structure.
 */
_Noreturn void lmain(uint32_t magic_value, uintptr_t info_address);

#endif // LAONOS_LOADER_LOADER_H
