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

# @file FindBroadcomAPI.cmake
# @brief Find the Broadcom PCI/PCIe library

#
# PLX_LIBRARY_DIR - Location of the PlxApi* files.
# PLX_INCLUDE_DIR - Location of the Plx header files.
# PLX_STATIC_LIB - List of the static library.
# PLX_SHARED_LIB - List of the shared library.
# PLX_FOUND - true if at least one of the libraries and the includes was found.
#

find_path(PLX_LIBRARY_DIR
        NAMES PlxApi.a PlxApi.so PlxApi.lib
        HINTS $ENV{PLX_SDK_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk C:/PlxApi
        PATH_SUFFIXES PlxApi/Library Linux/PlxApi/Library PlxApi/Release)

find_path(PLX_INCLUDE_DIR
        NAMES Plx.h PlxTypes.h PlxApi.h
        HINTS $ENV{PLX_SDK_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk
        PATH_SUFFIXES Include)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BroadcomAPI DEFAULT_MSG PLX_LIBRARY_DIR PLX_INCLUDE_DIR)

if (BroadcomAPI_FOUND)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(PLX_SHARED_LIB -l:PlxApi.so)
        set(PLX_STATIC_LIB -l:PlxApi.a)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(PLX_SHARED_LIB PlxApi.dll)
        set(PLX_STATIC_LIB PlxApi.lib)
    endif ()
endif (BroadcomAPI_FOUND)

mark_as_advanced(PLX_SHARED_LIB PLX_STATIC_LIB PLX_LIBRARY_DIR PLX_INCLUDE_DIR)