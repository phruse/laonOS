// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t limit;
    uint16_t base;
} segment_descriptor_low_t;
typedef struct {
    uint8_t base_low;
    uint8_t access_byte;
    uint8_t flags;
    uint8_t base_high;
} segment_descriptor_high_t;

/**
 * segment descriptor struct
 */
typedef struct {
    segment_descriptor_low_t low;
    segment_descriptor_high_t high;
} segment_t;

/**
 * GDT descriptor struct
 */
typedef struct {
    uint16_t size;
    uint32_t offset;
} gdt_t;

#pragma pack(pop)
