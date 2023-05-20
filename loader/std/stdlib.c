// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <macro.h>
#include <string.h>

// rand

static uint32_t memory = 0;
extern int init_seed(void);

void srand(uint32_t seed) { memory = seed; }

int rand(void) { // feat: secure random _rdrand32_step
  // reference: C standard
  if (memory == 0) {
    srand(init_seed());
  }

  memory = memory * 1103515245 + 12345;
  return (int)(memory / 65536) % RAND_MAX;
}

// malloc, free

#define ITERATOR_INFO(INFO) for (; INFO->next != NULL; INFO = INFO->next)

typedef struct __attribute__((packed)) free_list { // 8bytes
  struct free_list *next;
  uint16_t size;
  uint16_t is_free;
} allocate_info_t;

uintptr_t *free_start = (uintptr_t *)FREE_MIN;
static const size_t info_offset = sizeof(allocate_info_t);

inline static void *get_pointer(allocate_info_t *ptr);
inline static allocate_info_t *get_info(void *ptr);

inline static void swap_info(allocate_info_t *a, allocate_info_t *b);
inline static void alloc_info(allocate_info_t *info, uint16_t size);
inline static void split_info(allocate_info_t *info, uint16_t size);

void *malloc(uint16_t size) {
  if (size == 0)
    return NULL;

  size = ALIGN(size, 4);
  size_t min_size = SIZE_MAX;

  allocate_info_t *start = (allocate_info_t *)free_start;
  allocate_info_t *select = NULL;

  // find size
  ITERATOR_INFO(start) {
    size_t info_size = start->size;
    // size and free check
    if (info_size >= size && start->is_free) {
      // exactly size
      if (info_size == size) {
        select = start;
        break;
      } else if (min_size > info_size) {
        min_size = info_size;
        select = start;
      }
    }
  }

  // free memory
  if (select != NULL) {
    split_info(select, size);
    alloc_info(select, size);
    return get_pointer(select);
  }

  // init
  if (start == (allocate_info_t *)free_start) {
    alloc_info(start, size);
    return get_pointer(start);
  }

  // next memory
  void *new_ptr = (uint8_t *)start + start->size + info_offset;
  if ((uintptr_t)new_ptr > FREE_MAX) {
    return NULL;
  }

  allocate_info_t *new = new_ptr;
  start->next = new;

  alloc_info(new, size);
  return get_pointer(new);
}

void free(void *ptr) {
  // clear
  allocate_info_t *allocate = get_info(ptr);
  memset(ptr, 0, allocate->size);
  allocate->is_free = true;

  // marge
  allocate_info_t *info = (allocate_info_t *)free_start;
  allocate_info_t *previous = NULL;
  ITERATOR_INFO(info) {
    if (previous != NULL && previous->is_free && info->is_free) {
      swap_info(previous, info);
      previous->size += info->size + info_offset;
    } else {
      previous = info;
    }
  }
}

// return actual pointer
inline static void *get_pointer(allocate_info_t *ptr) {
  return (uint8_t *)ptr + info_offset;
}

// return info data
inline static allocate_info_t *get_info(void *ptr) {
  return (allocate_info_t *)((uint8_t *)ptr - info_offset);
}

inline static void swap_info(allocate_info_t *a, allocate_info_t *b) {
  a->next = b->next;
}

inline static void alloc_info(allocate_info_t *info, uint16_t size) {
  info->size = size;
  info->is_free = false;
}

inline static void split_info(allocate_info_t *info, uint16_t size) {
  // split if last over 4bytes
  if (info->size - size >= 4 + info_offset) {
    allocate_info_t *new =
        (allocate_info_t *)((uint8_t *)info + size + info_offset);

    swap_info(new, info);
    info->next = new;

    new->size = info->size - size - info_offset;
    info->size = size;

    new->is_free = true;
  }
}
