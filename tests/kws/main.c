/* SPDX-License-Identifier: BSD-3-Clause */
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "nsx_corstone_npu.h"
#include "ethosu_driver.h"
#include "pmu_ethosu.h"
#include "uart_stdout.h"

#define NPU_MODEL __attribute__((section(".npu_model"), aligned(32)))
#include "payload.h"
static uint8_t arena[ARENA_SIZE] __attribute__((section(".npu_arena"), aligned(32)));
/* LiteRT reference kernels, uncompiled KWS model, 490 signed int8 zeros. */
static const uint8_t expected[] = {0xff,0x80,0x80,0x80,0x80,0xff,0x80,0x80,0x80,0x80,0x80,0x81};

int main(void)
{
    UartStdOutInit();
    struct ethosu_driver *drv = nsx_corstone_npu_init();
    if (!drv || ethosu_request_power(drv) != 0)
        return 2;
    memset(arena, 0, sizeof(arena));
    uint64_t bases[] = {(uintptr_t)weights, (uintptr_t)arena};
    size_t sizes[] = {sizeof(weights), sizeof(arena)};
    ETHOSU_PMU_Enable(drv);
    ETHOSU_PMU_Set_EVTYPER(drv, 0, ETHOSU_PMU_NPU_ACTIVE);
    ETHOSU_PMU_EVCNTR_ALL_Reset(drv);
    ETHOSU_PMU_CYCCNT_Reset(drv);
    ETHOSU_PMU_CNTR_Enable(drv, 0x80000001u);
    int status = ethosu_invoke(drv, commands, sizeof(commands), bases, sizes, 2);
    uint32_t active = ETHOSU_PMU_Get_EVCNTR(drv, 0);
    uint64_t cycles = ETHOSU_PMU_Get_CCNTR(drv);
    ethosu_release_power(drv);
    printf("NPU status=%d active=%" PRIu32 " total=%" PRIu64 " output=", status, active, cycles);
    for (size_t i = 0; i < sizeof(expected); ++i)
        printf("%02x", arena[OUTPUT_OFFSET + i]);
    puts("");
    if (status || !active || cycles < active)
        return 2;
#ifdef KWS_NEGATIVE_CONTROL
    /* Exercise the guest mismatch path after a successful dispatch. */
    arena[OUTPUT_OFFSET] ^= 1;
#endif
    return memcmp(arena + OUTPUT_OFFSET, expected, sizeof(expected)) != 0;
}
