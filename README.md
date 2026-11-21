# NSX Corstone SDK

Firmware inputs for Arm Corstone FVP boards in NSX. The first board is
`corstone300_fvp_u55`, a Cortex-M55 application using the MPS3 UART and
512 KiB each of secure instruction and data TCM. The firmware provider,
SoC startup and board support are separate CMake modules.

| Board scope | CPU | NPU on reference model | Status |
| --- | --- | --- | --- |
| Corstone-300 | Cortex-M55 | Ethos-U55 variant | Initial GCC CPU/console implementation |
| Corstone-310 | Cortex-M85 | Ethos-U55 | Candidate |
| Corstone-320 | Cortex-M85 | Ethos-U85 | Candidate |

Each board must qualify its startup, memory map, UART, model arguments and
guest exit separately. NPU driver and inference qualification are separate.
Install the model under [Arm's FVP terms](https://support.arm.com/tools-and-software/fixed-virtual-platforms/iot-fvps).

## Source notices

NSX glue is BSD-3-Clause. The unmodified files under
`modules/nsx-corstone-sdk/upstream` retain their Apache-2.0 licenses and
copyright notices. Paths beneath each upstream directory match its source tree.

| Upstream | Revision |
| --- | --- |
| [CMSIS_6](https://github.com/ARM-software/CMSIS_6) | 8753b993c7544f0329b2ccc61bfbc967fdb3b112 |
| [Cortex_DFP](https://github.com/ARM-software/Cortex_DFP) | cd38dbb875f7195e70460eee51faa7ab180c24a1 |
| [ethos-u-core-platform](https://git.gitlab.arm.com/artificial-intelligence/ethos-u/ethos-u-core-platform) | 8927abdff4441e8598dbbb3f7ee03bf6bc2a8d7c |
