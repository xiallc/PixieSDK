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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace xia {
namespace util {
namespace path {
#ifdef XIA_PIXIE_WINDOWS
const char path_sep =  '\\';
#else
const char path_sep =  '/';
#endif

void find_files(
    const std::string path, paths& files_, const std::string& ext, size_t depth) {
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
    size_t b = name.find_last_of(path_sep);
    if (b != std::string::npos) {
        return name.substr(b + 1);
    }
    return name;
};

const std::string dirname(const std::string& name) {
    size_t b = name.find_last_of(path_sep);
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

const std::string join(const std::string path_, const paths& paths_) {
    auto pi = std::rbegin(paths_);
    while (pi != std::rend(paths_)) {
        auto& p = *pi++;
#ifdef XIA_PIXIE_WINDOWS
        if (p.length() > 1 && p[1] == ':') {
            break;
        }
#else /* XIA_PIXIE_WINDOWS */
        if (!p.empty() && p[0] == '/') {
            break;
        }
#endif /* XIA_PIXIE_WINDOWS */
    }
    std::string ret;
    if (pi == std::rend(paths_)) {
        ret = path_ + path_sep;
    }
    do {
        auto& p = *--pi;
        if (!ret.empty() && ret.back() != path_sep && (p.empty() || p[0] != path_sep)) {
            ret += path_sep;
        }
        if (!p.empty()) {
            ret += p;
        }
    } while (pi != std::rbegin(paths_));
    return ret;
}

bool exists(const std::string& path) {
    bool r;
#ifdef XIA_PIXIE_WINDOWS
    DWORD attr = GetFileAttributes(path.c_str());
    r = attr != INVALID_FILE_ATTRIBUTES;
#else  /* XIA_PIXIE_WINDOWS */
    struct stat sb;
    r = ::stat(path.c_str(), &sb) == 0;
#endif /* XIA_PIXIE_WINDOWS */
    return r;
}

bool isfile(const std::string& path) {
    bool r;
#ifdef XIA_PIXIE_WINDOWS
    DWORD attr = GetFileAttributes(path.c_str());
    /*
     * If not a file it must be a directory or a device.
     */
    constexpr auto not_mask =
        FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE |
        FILE_ATTRIBUTE_INTEGRITY_STREAM | FILE_ATTRIBUTE_VIRTUAL |
        FILE_ATTRIBUTE_NO_SCRUB_DATA;
    r = attr != INVALID_FILE_ATTRIBUTES && (attr & not_mask) == 0;
#else  /* XIA_PIXIE_WINDOWS */
    struct stat sb;
    r = ::stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode);
#endif /* XIA_PIXIE_WINDOWS */
    return r;
}

bool isdir(const std::string& path) {
    bool r;
#ifdef XIA_PIXIE_WINDOWS
    DWORD attr = GetFileAttributes(path.c_str());
    r = attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else  /* XIA_PIXIE_WINDOWS */
    struct stat sb;
    r = ::stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
#endif /* XIA_PIXIE_WINDOWS */
    return r;
}
}
}
}
