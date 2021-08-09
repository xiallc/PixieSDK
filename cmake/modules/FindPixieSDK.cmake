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

# @file FindPixieSDK.cmake
# @brief Find the PixieSDK installed on the user's system.

# XIA_LIBRARY_DIR
# XIA_INCLUDE_DIR
# XIA_LIBRARIES

find_path(XIA_LIBRARY_DIR
        NAMES libPixieSDK.so
        HINTS $ENV{XIA_PIXIE_SDK}
        PATHS /usr/local/xia/pixie_sdk
        PATH_SUFFIXES lib
        DOC "Path to pixie library.")

find_path(XIA_INCLUDE_DIR
        NAMES pixie16app_defs.h pixie16sys_defs.h
        HINTS $ENV{XIA_PIXIE_SDK}
        PATHS /usr/local/xia/pixie_sdk
        PATH_SUFFIXES include
        DOC "Path to XIA includes.")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PIXIE_SDK DEFAULT_MSG XIA_LIBRARY_DIR)

if (PIXIE_SDK_FOUND)
    set(XIA_LIBRARIES -lPixieSDK)
endif ()