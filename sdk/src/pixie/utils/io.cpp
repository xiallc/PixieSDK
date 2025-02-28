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

#include <pixie/os_compat.h>

#include <fcntl.h>

#ifdef XIA_PIXIE_WINDOWS
#include <io.h>
#else
#include <sys/stat.h>
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

void memdump(const void* addr, size_t length, write_string_func writer, std::string label,
             size_t size, size_t line_length, size_t offset) {
    if (length > 0) {
        std::ostringstream out;
        const uint8_t* addr8 = static_cast<const uint8_t*>(addr);
        size_t b = 0;
        out << std::hex << std::setfill('0') << label << std::endl;

        while (true) {
            std::vector<char> data(line_length);
            if (((b % line_length) == 0) || (b >= length)) {
                if (b != 0) {
                    size_t line = b % line_length;
                    if (line != 0) {
                        size_t remaining = line_length - line;
                        remaining = (remaining * 2) + (remaining / size);
                        out << std::setfill(' ') << std::setw(remaining) << ' '
                            << std::setfill('0');
                    } else {
                        line = line_length;
                    }
                    out << ' ';
                    for (size_t c = 0; c < line; c++) {
                        if ((data[c] < 0x20) || (data[c] > 0x7e)) {
                            out << '.';
                        } else {
                            out << data[c];
                        }
                    }
                    if (b >= length) {
                        break;
                    }
                    out << std::endl;
                }
                out << std::setw(8) << (uint32_t) (offset + b);
            }

            if ((b & (line_length - 1)) == (line_length >> 1)) {
                out << "-";
            } else {
                out << " ";
            }

            uint8_t d8 = 0;
            uint16_t d16 = 0;
            uint32_t d32 = 0;
            uint64_t d64 = 0;

            switch (size) {
                case sizeof(uint8_t):
                default:
                    d8 = *(addr8 + b);
                    out << std::setw(2) << (uint32_t) d8;
                    data[(b % line_length) + 0] = d8;
                    break;
                case sizeof(uint16_t):
                    d16 = *((const uint16_t*) (addr8 + b));
                    out << std::setw(4) << d16;
                    data[(b % line_length) + 0] = (uint8_t) (d16 >> 8);
                    data[(b % line_length) + 1] = (uint8_t) d16;
                    break;
                case sizeof(uint32_t):
                    d32 = *((const uint32_t*) (addr8 + b));
                    out << std::setw(8) << d32;
                    for (int i = sizeof(uint32_t); i > 0; --i) {
                        data[(b % line_length) + i] = (uint8_t) d32;
                        d32 >>= 8;
                    }
                    break;
                case sizeof(uint64_t):
                    d64 = *((const uint64_t*) (addr8 + b));
                    out << std::setw(16) << d64;
                    for (int i = sizeof(uint64_t); i > 0; --i) {
                        data[(b % line_length) + i] = (uint8_t) d64;
                        d64 >>= 8;
                    }
                    break;
            }
            b += size;
        }
        writer(out.str());
    }
}

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

bufferstream::bufferstream()
    : open(false), max_size(0), page_size(default_page_size) {
}

bufferstream::bufferstream(size_t max_size_)
    : open(false), max_size(max_size_), page_size(default_page_size) {
}

bufferstream::~bufferstream() {
}

void bufferstream::create(size_t max_size_, size_t page_size_) {
    if (max_size_ == 0 && page_size_ == 0) {
        throw error(
            error::code::invalid_value, "bufferstream: max size and page size are 0");
    }
    /*
     * No need to cheak max size and page size alignment as page size
     * is only used if max size is 0.
     */
    lock_guard guard(lock);
    if (open) {
        throw error(error::code::file_open_failure, "bufferstream: already open");
    }
    max_size = max_size_;
    page_size = page_size_;
    reset();
    open = true;
}

void bufferstream::destroy() {
    lock_guard guard(lock);
    if (open) {
        buffer.clear();
        buffer.shrink_to_fit();
        open = false;
    }
}

void bufferstream::set_page_size(size_t page_size_) {
    lock_guard guard(lock);
    page_size = page_size_;
}

void bufferstream::clear() {
    lock_guard guard(lock);
    if (!open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    reset();
}

bool bufferstream::full() {
    lock_guard guard(lock);
    if (!open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    if (max_size != 0) {
        return buffer.size() == max_size;
    }
    return false;
}

size_t bufferstream::size() {
    lock_guard guard(lock);
    if (!open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    return buffer.size();
}

size_t bufferstream::available() {
    lock_guard guard(lock);
    if (!open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    return buffer.capacity() - buffer.size();
}

void bufferstream::push(std::string& data) {
    auto size = data.size();
    push(reinterpret_cast<const_data_type_ptr>(data.c_str()), size);
}

void bufferstream::push(const_data_type_ptr data, size_t& size) {
    lock_guard guard(lock);
    if (!open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    extend(size);
    if (size != 0) {
        auto buf_size = buffer.size();
        buffer.resize(buf_size + size);
        std::memcpy(buffer.data() + buf_size, data, size);
    }
}

void bufferstream::reset() {
    buffer.clear();
    if (max_size != 0) {
        buffer.reserve(max_size);
    } else {
        buffer.reserve(page_size);
    }
}

void bufferstream::extend(size_t& size) {
    if (max_size != 0) {
        if (buffer.size() == max_size) {
            size = 0;
            return;
        }
    }
    auto remaining = buffer.capacity() - buffer.size();
    if (size > remaining) {
        auto need = size - remaining;
        if (max_size != 0) {
            size -= need;
        } else {
            auto pages = ((need - 1) / page_size) + 1;
            buffer.reserve(buffer.capacity() + pages * page_size);
        }
    }
}

bufferstream::reader::reader(bufferstream& bufstream_)
    : bufstream(bufstream_), pos(0) {
}

bufferstream::reader::~reader() {
}

size_t bufferstream::reader::read(std::string& data, size_t size) {
    data.clear();
    data.resize(size);
    return read(reinterpret_cast<data_type_ptr>(data.data()), size);
}

size_t bufferstream::reader::read(char* data, size_t size) {
    if (data == nullptr) {
        throw error(error::code::invalid_value, "bufferstream::reader: null pointer in read");
    }
    return read(reinterpret_cast<data_type_ptr>(data), size);
}

size_t bufferstream::reader::read(data_type_ptr data, size_t size) {
    lock_guard guard(bufstream.lock);
    if (!bufstream.open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    auto buf_size = bufstream.buffer.size();
    if (pos > buf_size) {
        pos = buf_size;
    }
    auto end = pos + size;
    if (end > buf_size) {
        end = buf_size;
    }
    size = end - pos;
    if (size > 0) {
        std::memcpy(data, bufstream.buffer.data() + pos, size);
        pos += size;
    }
    return size;
}

size_t bufferstream::reader::seek(size_t pos_) {
    lock_guard guard(bufstream.lock);
    if (!bufstream.open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    auto buf_size = bufstream.buffer.size();
    pos = pos_;
    if (pos > buf_size) {
        pos = buf_size;
    }
    return pos;
}

size_t bufferstream::reader::tell() {
    lock_guard guard(bufstream.lock);
    if (!bufstream.open) {
        throw error(error::code::file_open_failure, "bufferstream: not created");
    }
    auto buf_size = bufstream.buffer.size();
    if (pos > buf_size) {
        pos = buf_size;
    }
    return pos;
}

bool bufferstream::reader::full() {
    return bufstream.full();
}

size_t bufferstream::reader::size() {
    return bufstream.size();
}

size_t bufferstream::reader::available() {
    return bufstream.available();
}
} // namespace io
} // namespace util
} // namespace xia
