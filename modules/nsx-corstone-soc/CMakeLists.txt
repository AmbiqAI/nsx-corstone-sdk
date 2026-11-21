# SPDX-License-Identifier: BSD-3-Clause
if(NOT NSX_SOC_FAMILY STREQUAL "corstone300")
    message(FATAL_ERROR "No qualified startup for Corstone SoC '${NSX_SOC_FAMILY}'")
endif()
set(device "${NSX_CORSTONE_SOURCES}/Cortex_DFP/Device/ARMCM55")
add_library(nsx_corstone_soc INTERFACE)
add_library(nsx::corstone_soc ALIAS nsx_corstone_soc)
target_link_libraries(nsx_corstone_soc INTERFACE nsx::sdk_corstone)
target_include_directories(nsx_corstone_soc INTERFACE
    "${NSX_CORSTONE_SOURCES}/CMSIS_6/CMSIS/Core/Include" "${device}/Include")
target_compile_definitions(nsx_corstone_soc INTERFACE ARMCM55)
target_compile_options(nsx_corstone_soc INTERFACE -mcpu=cortex-m55 -mthumb -mfloat-abi=hard)
target_link_options(nsx_corstone_soc INTERFACE -mcpu=cortex-m55 -mthumb -mfloat-abi=hard)
# Attach reset and system objects directly to each final ELF.
target_sources(nsx_corstone_soc INTERFACE
    "${device}/Source/startup_ARMCM55.c" "${device}/Source/system_ARMCM55.c")
