// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_KERNEL_INCLUDE_SRC_MAIN_H
#define LAONOS_KERNEL_INCLUDE_SRC_MAIN_H

#include <stdint.h>

/**
 * exokernel c entry point
 *
 * @param magic_value magic value for boot check.
 * @param info_address pointer of module information structure.
 */
 void main();

#endif // LAONOS_KERNEL_INCLUDE_SRC_MAIN_H
