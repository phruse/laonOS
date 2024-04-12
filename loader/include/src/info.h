// SPDX-License-Identifier: MIT

#pragma once
#ifndef LAONOS_LOADER_INCLUDE_SRC_INFO_H
#define LAONOS_LOADER_INCLUDE_SRC_INFO_H

#define OS_KERNEL_NAME "KERNEL"

#define OS_MODULE_MAX 10

#define OS_MEMORY_FREE 0x80000000                  // 2GB
#define OS_MEMORY_VIRTUAL_BEGIN 0xfffffff800000000 // -2GB
#define OS_MEMORY_MINIMUM 0x4000000                // 64MB

#ifdef i386
#define OS_MEMORY_ALIGN 0x200000 // 2MB
#endif

#endif // LAONOS_LOADER_INCLUDE_SRC_INFO_H
