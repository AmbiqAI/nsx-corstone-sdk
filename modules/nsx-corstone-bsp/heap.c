/* SPDX-License-Identifier: BSD-3-Clause */
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

extern char __end__[];
extern char __HeapLimit[];

/* Newlib's bare-metal heap extends upward to the reserved stack boundary. */
void *_sbrk(ptrdiff_t increment)
{
    static uintptr_t current = (uintptr_t)__end__;
    uintptr_t previous = current;
    if (increment >= 0) {
        uintptr_t bytes = (uintptr_t)increment;
        if (bytes > (uintptr_t)__HeapLimit - current) {
            errno = ENOMEM;
            return (void *)-1;
        }
        current += bytes;
    } else {
        uintptr_t bytes = (uintptr_t)0 - (uintptr_t)increment;
        if (bytes > current - (uintptr_t)__end__) {
            errno = ENOMEM;
            return (void *)-1;
        }
        current -= bytes;
    }
    return (void *)previous;
}
