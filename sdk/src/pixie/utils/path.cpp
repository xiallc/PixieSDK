/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file path.cpp
 * @brief Defines utility functions and data structures related to filepaths useful throughout the SDK.
 */

#include <cstring>

#include <pixie/error.hpp>
#include <pixie/utils/path.hpp>

#include <pixie/os_compat.hpp>

#ifdef XIA_PIXIE_WINDOWS
#include <isakbosman/dirent.h>
#else
#include <dirent.h>
#endif

namespace xia {
namespace util {
namespace path {
void find_files(const std::string path, path::strings& files_, const std::string& ext,
                size_t depth) {
    if (depth > 100) {
        throw std::runtime_error("file find path too deep: " + path);
    }
    if (depth == 0) {
        files_.clear();
    }
    DIR* dir = nullptr;
    try {
        dir = ::opendir(path.c_str());
        if (dir == nullptr) {
            throw std::runtime_error("file find path: " + path + ": " + std::strerror(errno));
        }
        while (true) {
            struct dirent* ent = ::readdir(dir);
            if (ent == nullptr) {
                break;
            }
            std::string name = ent->d_name;
            if (ent->d_type == DT_REG) {
                if (name.size() > ext.size() &&
                    name.compare(name.size() - ext.size(), ext.size(), ext) == 0) {
                    files_.push_back(path + '/' + name);
                }
            } else if (ent->d_type == DT_DIR && name != "." && name != "..") {
                std::string child = path + '/' + name;
                find_files(child, files_, ext, depth + 1);
            }
        }
        ::closedir(dir);
    } catch (...) {
        if (dir != nullptr) {
            ::closedir(dir);
        }
        throw;
    }
}

const std::string basename(const std::string& name) {
    size_t b = name.find_last_of('/');
    if (b != std::string::npos) {
        return name.substr(b + 1);
    }
    return name;
};

const std::string dirname(const std::string& name) {
    size_t b = name.find_last_of('/');
    if (b != std::string::npos) {
        return name.substr(0, b);
    }
    return name;
};

const std::string extension(const std::string& name) {
    size_t d = name.find_last_of('.');
    if (d != std::string::npos) {
        return name.substr(d + 1);
    }
    return name;
}
}
}
}