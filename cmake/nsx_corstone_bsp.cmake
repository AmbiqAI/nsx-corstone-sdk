# SPDX-License-Identifier: BSD-3-Clause
function(nsx_validate_bsp out_target)
    get_target_property(provider nsx::bsp ALIASED_TARGET)
    get_target_property(version ${provider} NSX_BSP_CONTRACT_VERSION)
    if(NOT version STREQUAL "2" OR NOT NSX_BSP_CONTRACT_REQUIRED EQUAL 2)
        message(FATAL_ERROR "Corstone requires BSP contract 2")
    endif()
    set(${out_target} "${provider}" PARENT_SCOPE)
endfunction()
