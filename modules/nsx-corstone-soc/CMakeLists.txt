# SPDX-License-Identifier: BSD-3-Clause
if(NSX_SOC_FAMILY STREQUAL "corstone300")
    set(device_name ARMCM55)
    set(cpu cortex-m55)
elseif(NSX_SOC_FAMILY MATCHES "^corstone(310|320)$")
    set(device_name ARMCM85)
    set(cpu cortex-m85)
else()
    message(FATAL_ERROR "No qualified startup for Corstone SoC '${NSX_SOC_FAMILY}'")
endif()
set(device "${NSX_CORSTONE_SOURCES}/Cortex_DFP/Device/${device_name}")
add_library(nsx_corstone_soc INTERFACE)
add_library(nsx::corstone_soc ALIAS nsx_corstone_soc)
target_link_libraries(nsx_corstone_soc INTERFACE nsx::sdk_corstone)
target_include_directories(nsx_corstone_soc INTERFACE
    "${NSX_CORSTONE_SOURCES}/CMSIS_6/CMSIS/Core/Include" "${device}/Include")
target_compile_definitions(nsx_corstone_soc INTERFACE ${device_name})
target_compile_options(nsx_corstone_soc INTERFACE -mcpu=${cpu} -mthumb -mfloat-abi=hard)
target_link_options(nsx_corstone_soc INTERFACE -mcpu=${cpu} -mthumb -mfloat-abi=hard)
# Attach reset and system objects directly to each final ELF.
target_sources(nsx_corstone_soc INTERFACE
    "${device}/Source/startup_${device_name}.c" "${device}/Source/system_${device_name}.c")
