// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_BOOT_INCLUDE_STD_MACRO_H
#define LAONOS_BOOT_INCLUDE_STD_MACRO_H

/**
 * unused error ignore macro
 */
#define UNUSED(x) (void)(x)

#define ALIGN(var, align) ({((var) + ((align)-1u)) & ~((align)-1u);})

#endif // LAONOS_BOOT_INCLUDE_STD_MACRO_H
