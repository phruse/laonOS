// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_superuser : 1;
    uint8_t cache_policy : 1;
    uint8_t cache_disable : 1;
    uint8_t accessed : 1;
    uint8_t : 6;// ignore
    uint32_t base_address : 28;
    uint32_t : 23;// ignore
    uint8_t execute_disable : 1;
} paging_table;

typedef struct {
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_superuser : 1;
    uint8_t cache_policy : 1;
    uint8_t cache_disable : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t page_size : 1;
    uint8_t global : 1;
    uint8_t : 3;// ignore
    uint32_t base_address : 28;
    uint32_t : 23;// ignore
    uint8_t execute_disable : 1;
} paging_directory_table;

#pragma pack(pop)
