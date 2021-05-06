#ifndef PIXIE_BUFFER_H
#define PIXIE_BUFFER_H

/*----------------------------------------------------------------------
* Copyright (c) 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/
/// @file test_pixie_buffer.cpp
/// @brief
/// @author Chris Johns
/// @date May 5, 2021

#include <atomic>
#include <deque>
#include <forward_list>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <vector>

#include <pixie_error.hpp>
#include <pixie_hw.hpp>

namespace xia
{
namespace buffer
{
/*
 * Local error
 */
typedef pixie::error::error error;

/*
 * Buffer lock types
 */
typedef std::mutex lock_type;
typedef std::lock_guard<lock_type> lock_guard;

/*
 * A buffer is still based on the hardware words. A generic container is more
 * work than it is worth at this point in time.
 */
typedef pixie::hw::word buffer_value;
typedef pixie::hw::word_ptr buffer_value_ptr;
typedef pixie::hw::words buffer;
typedef buffer* buffer_ptr;
typedef std::shared_ptr<buffer> handle;

/*
 * Pool of buffers
 */
struct pool
{
    pool();
    ~pool();

    void create(const size_t number, const size_t size);
    void destroy();

    handle request();

    bool empty() const {
        return count_.load() == 0;
    }

    bool full() const {
        return number != 0 && count_.load() == number;
    }

    size_t count() const {
        return count_.load();
    }

    size_t number;
    size_t size;

    void output(std::ostream& out);

private:

    struct releaser;
    void release(buffer_ptr buf);

    std::atomic_size_t count_;

    std::forward_list<buffer_ptr> buffers;

    lock_type lock;
};

/*
 * Queue of buffers
 */
struct queue
{
    typedef std::deque<handle> handles;

    queue();

    void push(handle buf);
    handle pop();

    void copy(buffer& to);
    void copy(buffer_value_ptr to, const size_t count);

    void compact();

    bool empty() const {
        return count_.load() == 0;
    }

    size_t size() const {
        return size_.load();
    }
    size_t count() const {
        return count_.load();
    }

    void output(std::ostream& out);

private:
    void copy_unprotected(buffer_value_ptr to, const size_t count);

    handles buffers;
    lock_type lock;
    std::atomic_size_t size_;
    std::atomic_size_t count_;
};

}
}

std::ostream& operator<<(std::ostream& out, xia::buffer::pool& pool);
std::ostream& operator<<(std::ostream& out, xia::buffer::queue& queue);

#endif  // PIXIE_BUFFER_H
