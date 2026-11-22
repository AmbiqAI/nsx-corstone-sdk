# SPDX-License-Identifier: BSD-3-Clause
set(driver "${NSX_CORSTONE_SOURCES}/ethos-u-core-driver")
if(NSX_SOC_FAMILY STREQUAL "corstone300")
    set(arch ETHOSU55)
    set(ip u55)
    set(device ethosu_device_u55_u65.c)
elseif(NSX_SOC_FAMILY STREQUAL "corstone320")
    set(arch ETHOSU85)
    set(ip u85)
    set(device ethosu_device_u85.c)
else()
    message(FATAL_ERROR "Unsupported Corstone NPU for '${NSX_SOC_FAMILY}'")
endif()
add_library(nsx_corstone_npu INTERFACE)
add_library(nsx::corstone_npu ALIAS nsx_corstone_npu)
target_link_libraries(nsx_corstone_npu INTERFACE nsx::corstone_soc)
target_compile_definitions(nsx_corstone_npu INTERFACE ${arch} ETHOSU_ARCH=${ip})
target_include_directories(nsx_corstone_npu INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include" "${driver}/include")
target_sources(nsx_corstone_npu INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/npu.c"
    "${driver}/src/ethosu_driver.c" "${driver}/src/ethosu_pmu.c" "${driver}/src/${device}")
