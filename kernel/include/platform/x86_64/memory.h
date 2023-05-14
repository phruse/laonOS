// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_KERNEL_SRC_PLATFORM_X86_64_MEMORY_H
#define LAONOS_KERNEL_SRC_PLATFORM_X86_64_MEMORY_H

#include <stdbool.h>

/**
 * remove low page table and init temp kernel page table
 *
 * @return if successful return true and otherwise return false
 */
bool memory_clean_up(void);

#endif // LAONOS_KERNEL_SRC_PLATFORM_X86_64_MEMORY_H
