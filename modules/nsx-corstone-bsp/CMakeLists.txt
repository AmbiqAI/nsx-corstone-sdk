# SPDX-License-Identifier: BSD-3-Clause
if(NOT NSX_BOARD STREQUAL "corstone300_fvp_u55")
    message(FATAL_ERROR "No qualified Corstone BSP for '${NSX_BOARD}'")
endif()
set(platform "${NSX_CORSTONE_SOURCES}/ethos-u-core-platform")
set(UART0_BASE 0x49303000)
set(UART0_BAUDRATE 115200)
set(SYSTEM_CORE_CLOCK 25000000)
configure_file("${platform}/drivers/uart/uart_config.h.in" uart_config.h @ONLY)
add_library(nsx_corstone_bsp INTERFACE)
add_library(nsx::bsp ALIAS nsx_corstone_bsp)
set_target_properties(nsx_corstone_bsp PROPERTIES
    NSX_BSP_CONTRACT_VERSION 2 NSX_BSP_DEBUG_ITM 0 NSX_BSP_DEBUG_UART 1)
target_link_libraries(nsx_corstone_bsp INTERFACE nsx::corstone_soc)
target_include_directories(nsx_corstone_bsp INTERFACE
    "${CMAKE_CURRENT_BINARY_DIR}" "${platform}/drivers/uart/include")
target_sources(nsx_corstone_bsp INTERFACE
    "${platform}/drivers/uart/src/uart_cmsdk_apb.c" "${platform}/targets/corstone-300/retarget.c")
set(script "${CMAKE_CURRENT_SOURCE_DIR}/corstone300.ld")
target_link_options(nsx_corstone_bsp INTERFACE "--specs=nosys.specs" "LINKER:-T,${script}")
set_property(TARGET nsx_corstone_bsp PROPERTY INTERFACE_LINK_DEPENDS "${script}")
