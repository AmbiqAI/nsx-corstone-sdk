/* SPDX-License-Identifier: BSD-3-Clause */
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "uart_stdout.h"

extern void *_sbrk(ptrdiff_t increment);
static volatile unsigned initialized = 0xA5;
static volatile unsigned char scratch[400 * 1024];

int main(void)
{
    UartStdOutInit();
    if (initialized != 0xA5 || scratch[sizeof(scratch) - 1] != 0)
        return 1;
    scratch[0] = 1;
    char *start = _sbrk(0);
    if (_sbrk(16) != start || _sbrk(-16) != start + 16)
        return 2;
    errno = 0;
    if (_sbrk(PTRDIFF_MAX) != (void *)-1 || errno != ENOMEM)
        return 3;
    errno = 0;
    if (_sbrk(PTRDIFF_MIN) != (void *)-1 || errno != ENOMEM || _sbrk(0) != start)
        return 4;
    puts("Corstone-300 startup and heap passed");
    return 0;
}
