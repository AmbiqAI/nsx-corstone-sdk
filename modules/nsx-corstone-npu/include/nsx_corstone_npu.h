/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef NSX_CORSTONE_NPU_H
#define NSX_CORSTONE_NPU_H

#ifdef __cplusplus
extern "C" {
#endif
struct ethosu_driver;
/* Initialize once during single-threaded startup. Returns NULL on failure.
 * Keep the data cache disabled while using this bare-metal backend. */
struct ethosu_driver *nsx_corstone_npu_init(void);
#ifdef __cplusplus
}
#endif
#endif
