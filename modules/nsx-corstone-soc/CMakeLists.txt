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
include(GNUInstallDirs)
set(device "${NSX_CORSTONE_SOURCES}/Cortex_DFP/Device/${device_name}")
# Usage requirements for libraries that compile for the board's CPU.
add_library(nsx_corstone_cpu INTERFACE)
add_library(nsx::corstone_cpu ALIAS nsx_corstone_cpu)
set(core_headers "${NSX_CORSTONE_SOURCES}/CMSIS_6/CMSIS/Core/Include")
set(cpu_headers "${CMAKE_INSTALL_INCLUDEDIR}/nsx/modules/nsx-corstone-soc")
set_target_properties(nsx_corstone_cpu PROPERTIES EXPORT_NAME corstone_cpu)
target_include_directories(nsx_corstone_cpu INTERFACE
    "$<BUILD_INTERFACE:${core_headers}>"
    "$<BUILD_INTERFACE:${device}/Include>"
    "$<INSTALL_INTERFACE:${cpu_headers}>")
target_compile_definitions(nsx_corstone_cpu INTERFACE ${device_name})
target_compile_options(nsx_corstone_cpu INTERFACE -mcpu=${cpu} -mthumb -mfloat-abi=hard)
target_link_options(nsx_corstone_cpu INTERFACE -mcpu=${cpu} -mthumb -mfloat-abi=hard)

install(TARGETS nsx_corstone_cpu EXPORT nsxTargets)
install(DIRECTORY "${core_headers}/" "${device}/Include/" DESTINATION "${cpu_headers}")
install(FILES "${NSX_CORSTONE_SOURCES}/CMSIS_6/LICENSE" DESTINATION "${cpu_headers}" RENAME LICENSE-CMSIS_6)
install(FILES "${NSX_CORSTONE_SOURCES}/Cortex_DFP/LICENSE" DESTINATION "${cpu_headers}" RENAME LICENSE-Cortex_DFP)

add_library(nsx_corstone_soc INTERFACE)
add_library(nsx::corstone_soc ALIAS nsx_corstone_soc)
target_link_libraries(nsx_corstone_soc INTERFACE nsx::sdk_corstone nsx::corstone_cpu)
# Attach reset and system objects directly to each final ELF.
target_sources(nsx_corstone_soc INTERFACE
    "${device}/Source/startup_${device_name}.c" "${device}/Source/system_${device_name}.c")
