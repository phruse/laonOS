// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_SRC_MAIN_H
#define LAONOS_LOADER_INCLUDE_SRC_MAIN_H

#include <stdint.h>

/**
 * loader c entry point
 *
 * @param magic_value magic value that checks bootloader.
 * @param info_address pointer of multiboot information structure.
 */
_Noreturn void main(uint32_t magic_value, uintptr_t info_address);

#endif // LAONOS_LOADER_INCLUDE_SRC_MAIN_H
