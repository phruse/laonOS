// SPDX-License-Identifier: MIT

#include <macro.h>
#include <stdint.h>
#include <string.h>

int strcmp(const char *string1, const char *string2) {
    while (*string1) {
        if (*string1 != *string2) {
            break;
        }
        string1++;
        string2++;
    }

    return *(const unsigned char *) string1 - *(const unsigned char *) string2;
}

size_t strcpy_s(char *dest, size_t size, const char *src) {
    size_t i;
    for (i = 0; src[i] != '\0' && i < size; ++i) {
        dest[i] = src[i];
    }

    return i;
}

size_t strlen(const char *string) {
    size_t i;
    for (i = 0; string[i] != '\0'; ++i)
        ;
    return i;
}

void *memset(void *ptr, int number, size_t size) {
    asm("cld;"
        "rep; stosb;" ::"D"(ptr),
        "c"(size), "a"(number));

    return ptr;
}

void *memcpy(void *dest, const void *src, size_t size) {
    if ((uint32_t) dest == ALIGN((uint32_t) dest, 16)) {
        asm volatile(
                "cld;"
                "rep; movsb;" ::"D"(dest),
                "S"(src),
                "c"(size));
    } else {
        for (size_t i = 0; i <= size; ++i) {
            ((int *) dest)[i] = ((int *) src)[i];
        }
    }

    return dest;
}
