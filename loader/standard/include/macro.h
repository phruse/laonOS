// SPDX-License-Identifier: MIT

#pragma once

/**
 * unused error ignore macro
 */
#define UNUSED(x) (void)(x)

#define ALIGN(var, align) ((var + (align - 1u)) & ~(align - 1u))
