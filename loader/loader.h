// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

/**
 * loader c entry point
 *
 * @param magic_value magic value that checks bootloader.
 * @param info_address pointer of multiboot information structure.
 */
void loader_main(uint32_t magic_value, uint32_t info_address);
