/* SPDX-License-Identifier: BSD-3-Clause */
#include "nsx_corstone_npu.h"
#include "ethosu_driver.h"
#if defined(ARMCM55)
#include "ARMCM55.h"
#define NPU_BASE 0x48102000u
#define NPU_IRQ ((IRQn_Type)56)
#elif defined(ARMCM85)
#include "ARMCM85.h"
#define NPU_BASE 0x50004000u
#define NPU_IRQ ((IRQn_Type)16)
#endif

static struct ethosu_driver driver;

void CorstoneNPU_Handler(void)
{
    ethosu_irq_handler(&driver);
}

struct ethosu_driver *nsx_corstone_npu_init(void)
{
    static bool initialized;
    if (SCB->CCR & SCB_CCR_DC_Msk)
        return NULL;
    if (!initialized) {
        if (ethosu_init(&driver, (void *)NPU_BASE, NULL, 0, 1, 1) != 0)
            return NULL;
        NVIC_ClearPendingIRQ(NPU_IRQ);
        NVIC_EnableIRQ(NPU_IRQ);
        initialized = true;
    }
    return &driver;
}

/* Mask only this peripheral while the driver cancels a reserved job. */
uint32_t nsx_corstone_npu_mask_irq(void)
{
    uint32_t enabled = NVIC_GetEnableIRQ(NPU_IRQ);
    NVIC_DisableIRQ(NPU_IRQ);
    return enabled;
}

void nsx_corstone_npu_restore_irq(uint32_t enabled, bool cancelled)
{
    if (cancelled)
        NVIC_ClearPendingIRQ(NPU_IRQ);
    if (enabled)
        NVIC_EnableIRQ(NPU_IRQ);
}
