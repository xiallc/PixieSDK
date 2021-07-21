# SPDX-License-Identifier: Apache-2.0

# Copyright 2021 XIA LLC, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# @file xia_configure_target.cmake
# @brief Defines a function used to configure targets on Windows and Linux

function(xia_configure_target)
    set(options USE_PLX FORCE_DEBUG CONFIG_OBJ)
    set(oneValueArgs TARGET)
    set(multiValueArgs COMPILE_DEFS WIN_COMPILE_OPTIONS LINUX_COMPILE_OPTIONS LIBS LINUX_LIBS WIN_LIBS)
    cmake_parse_arguments(XIA_CT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (${XIA_CT_USE_PLX})
        target_include_directories(${XIA_CT_TARGET} PUBLIC ${PLX_INCLUDE_DIR})
        if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
            target_link_directories(${XIA_CT_TARGET} PUBLIC ${PLX_LIBRARY_DIR})
        endif ()
        if (NOT ${XIA_CT_CONFIG_OBJ})
            target_link_libraries(${XIA_CT_TARGET} PUBLIC ${PLX_SHARED_LIB})
        endif ()
        target_compile_definitions(${XIA_CT_TARGET} PUBLIC PLX_LITTLE_ENDIAN)
    endif ()

    target_compile_definitions(${XIA_CT_TARGET} PUBLIC PCI_CODE ${XIA_CT_COMPILE_DEFS})
    if (NOT ${XIA_CT_CONFIG_OBJ})
        target_link_libraries(${XIA_CT_TARGET} PUBLIC ${XIA_CT_LIBS})
    endif ()

    if ((${CMAKE_BUILD_TYPE} MATCHES "Debug") OR ${XIA_CT_FORCE_DEBUG})
        target_compile_definitions(${XIA_CT_TARGET} PUBLIC PRINT_DEBUG_MSG)
    endif ()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        if (${XIA_CT_USE_PLX})
            target_compile_definitions(${XIA_CT_TARGET} PUBLIC PLX_LINUX)
            if (NOT ${XIA_CT_CONFIG_OBJ})
                target_link_libraries(${XIA_CT_TARGET} PUBLIC dl m pthread)
            endif ()
        endif ()
        if (NOT ${XIA_CT_CONFIG_OBJ})
            target_link_libraries(${XIA_CT_TARGET} PUBLIC ${XIA_CT_LINUX_LIBS})
        endif ()
        target_compile_options(${XIA_CT_TARGET} PUBLIC -g -Wall -Wextra ${XIA_CT_LINUX_COMPILE_OPTIONS})
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        if (${XIA_CT_USE_PLX})
            target_compile_definitions(${XIA_CT_TARGET} PUBLIC PLX_WIN)
        endif ()
        if (NOT ${XIA_CT_CONFIG_OBJ})
            target_link_libraries(${XIA_CT_TARGET} PUBLIC winmm ${XIA_CT_WIN_LIBS})
        endif ()
        target_compile_options(${XIA_CT_TARGET} PUBLIC ${XIA_CT_WIN_COMPILE_OPTIONS})
        target_compile_definitions(${XIA_CT_TARGET} PUBLIC _CRT_SECURE_NO_WARNINGS)
    endif ()
endfunction()
