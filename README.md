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

NSX glue is BSD-3-Clause. The files under
`modules/nsx-corstone-sdk/upstream` retain their Apache-2.0 licenses and
copyright notices. Paths beneath each upstream directory match its source tree.
Two files carry marked local changes. `drivers/uart/src/uart_cmsdk_apb.c`
bounds console line input and `targets/corstone-300/retarget.c` returns a
completed line to newlib. The revision table identifies their upstream baselines.

| Upstream | Revision |
| --- | --- |
| [CMSIS_6](https://github.com/ARM-software/CMSIS_6) | 8753b993c7544f0329b2ccc61bfbc967fdb3b112 |
| [Cortex_DFP](https://github.com/ARM-software/Cortex_DFP) | cd38dbb875f7195e70460eee51faa7ab180c24a1 |
| [ethos-u-core-platform](https://git.gitlab.arm.com/artificial-intelligence/ethos-u/ethos-u-core-platform) | 8927abdff4441e8598dbbb3f7ee03bf6bc2a8d7c |

## Verify the firmware

With Arm GNU and newlib installed, cross-link the regression firmware:

```sh
cmake -S tests/bringup -B build -DCMAKE_SYSTEM_NAME=Generic \
  -DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
cmake --build build
```

CI runs this cross-build. To qualify execution, load `build/bringup` into the
Corstone-300/U55 FVP with 512 KiB ITCM and DTCM. Capture UART0 and check for
`Corstone-300 startup and heap passed` and `Application exit code: 0.`. The
probe exercises initialized data, a 400 KiB BSS, heap growth and shrink, and
refusal of allocations beyond either bound.
