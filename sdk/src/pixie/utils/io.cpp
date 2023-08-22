/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2023 XIA LLC, All rights reserved.
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

/** @file io.cpp
 * @brief Defines utility functions and data structures related to I/O useful throughout the SDK.
 */

#include <cstring>

#include <pixie/error.hpp>
#include <pixie/utils/io.hpp>

#include <pixie/os_compat.hpp>

#include <fcntl.h>

#ifdef XIA_PIXIE_WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

/*
 * Platform mapping of system level calls
 *
 * 1. POSIX system direcly map to the system calls with a macro
 *
 * 2. Windows generates a warning if the compiler sees the POSIX names so
 *    mask them behind inline functions. Make these functions make POSIX
 *    where possible. Windows does not have ssize_t so use int.
 *
 * 3. The Windows functions reside in the global namespace
 *
 * Open group POSIX functions are:
 *
 *    int close(int fildes);
 *    int creat(const char *path, mode_t mode);
 *    int open(const char *path, int oflag, ...);
 *    off_t lseek(int fildes, off_t offset, int whence);
 *    ssize_t read(int fildes, void *buf, size_t nbyte);
 *    ssize_t write(int fildes, const void *buf, size_t nbyte);
 *
 * Note:
 *
 *   a) The mode on creat is the permissions
 *   b) The open call with O_CREAT requires permissions
 */
#ifdef XIA_PIXIE_WINDOWS
#define S_IRUSR  S_IREAD  /* Read user */
#define S_IWUSR  S_IWRITE /* Write user */
#define S_IXUSR  0        /* Execute user */
#define S_IRGRP  0        /* Read group */
#define S_IWGRP  0        /* Write group */
#define S_IXGRP  0        /* Execute group */
#define S_IROTH  0        /* Read others */
#define S_IWOTH  0        /* Write others */
#define S_IXOTH  0        /* Execute others */
static inline int os_close(int fh) {
    return _close(fh);
}
static inline int os_creat(const char* path, int mode) {
    return _creat(path, mode);
}
static inline int os_open(const char* path, int oflag) {
    return _open(path, oflag);
}
static inline int64_t os_lseek(int fildes, int64_t offset, int mode) {
    return _lseeki64(fildes, offset, mode);
}
static inline int os_read(int fildes, void *buf, size_t nbyte) {
    return _read(fildes, buf, static_cast<unsigned int>(nbyte));
}
static inline int os_write(int fildes, const void* buf, size_t nbyte) {
    return _write(fildes, buf, static_cast<unsigned int>(nbyte));
}
#else
#define os_close close
#define os_creat creat
#define os_open  open
static inline int os_lseek(int fildes, int64_t offset, int mode) {
    return lseek(fildes, static_cast<off_t>(offset), mode);
}
#define os_read  read
#define os_write write
#endif

namespace xia {
namespace util {
namespace io {

using error = pixie::error::error;

static int open_flags_mapper(file::flag flag) {
    switch (flag) {
        case file::flag::ro:
            return O_RDONLY;
        case file::flag::wr:
            return O_WRONLY;
        case file::flag::wr_trunc:
            return O_WRONLY | O_TRUNC;
        case file::flag::rw_trunc:
            return O_RDWR | O_TRUNC;
        case file::flag::rw:
        default:
            return O_RDWR;
    }
}

static int seek_flags_mapper(file::seek_mode mode) {
    switch (mode) {
        case file::seek_mode::set:
            return SEEK_SET;
        case file::seek_mode::cur:
            return SEEK_CUR;
        case file::seek_mode::end:
        default:
            return SEEK_END;
    }
}

template<typename T = int>
void error_check(T r, const char* func) {
    if (r < 0) {
        throw error(
            error::code::file_not_found,
            "io: file: " + std::string(func) + ": " + std::strerror(errno));
    }
}

file::file() : handle(-1) {}

file::~file() {
    close();
}

void file::close() {
    int fh = handle.load();
    if (std::atomic_exchange(&handle, -1) >= 0) {
        try {
            error_check(::os_close(fh), "close");
        } catch(...) {
            /* do nothing, file already closed */
        }
    }
}

void file::create(const std::string& filename, flag flag) {
    /*
     * There is no mode control at the moment.
     */
    int mode;
    switch (flag) {
        case flag::ro:
            mode = S_IRUSR;
            break;
        case flag::wr:
            mode = S_IWUSR;
            break;
        case flag::rw:
        default:
            mode = S_IRUSR | S_IWUSR | S_IXUSR;
    }
    handle = ::os_creat(filename.c_str(), mode);
    error_check<std::atomic_int&>(handle, "create");
}

void file::open(const std::string& filename, flag flag) {
    handle = ::os_open(filename.c_str(), open_flags_mapper(flag));
    error_check<std::atomic_int&>(handle, "open");
}

int64_t file::seek(int64_t offset, seek_mode mode) {
    if (valid()) {
        auto r = ::os_lseek(handle, offset, seek_flags_mapper(mode));
        error_check<int64_t>(r, "lseek");
        return static_cast<int64_t>(r);
    }
    throw error(error::code::file_not_found, "io: file: lseek: not open");
}

size_t file::read(value_ptr data, size_t size) {
    if (valid()) {
        auto r = ::os_read(handle, data, size);
        error_check(r, "read");
        return static_cast<size_t>(r);
    }
    throw error(error::code::file_not_found, "io: file: read: not open");
}

size_t file::write(const_value_ptr data, size_t size) {
    if (valid()) {
        auto r = ::os_write(handle, data, size);
        error_check(r, "write");
        return static_cast<size_t>(r);
    }
    throw error(error::code::file_not_found, "io: file: write: not open");
}
} // namespace io
} // namespace util
} // namespace xia
