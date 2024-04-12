// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_STD_MACRO_H
#define LAONOS_LOADER_INCLUDE_STD_MACRO_H

/**
 * unused error ignore macro
 */
#define UNUSED(x) (void)(x)

#define ALIGN(var, align) ({((var) + ((align)-1u)) & ~((align)-1u);})

#endif // LAONOS_LOADER_INCLUDE_STD_MACRO_H
