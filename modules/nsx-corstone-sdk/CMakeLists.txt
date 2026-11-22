# SPDX-License-Identifier: BSD-3-Clause
add_library(nsx_sdk_corstone INTERFACE)
add_library(nsx::sdk_corstone ALIAS nsx_sdk_corstone)
set(NSX_CORSTONE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/upstream" CACHE INTERNAL "Vendored Corstone firmware inputs" FORCE)
