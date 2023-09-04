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

/** @file thread.hpp
 * @brief Defines utility functions and data structures related to threads and
 *        workers.
 */

#ifndef PIXIESDK_UTIL_THREAD_HPP
#define PIXIESDK_UTIL_THREAD_HPP

#include <functional>
#include <future>
#include <thread>
#include <vector>

#include <pixie/error.hpp>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace thread {
/**
 * Thread errors
 */
using error = pixie::error::error;

/**
 * @brief Thread worker that runs a function in a thread context
 */
struct worker {
    using function = std::function<void()>;
    using promise_error = std::promise<error::code>;
    using future_error = std::future<error::code>;

    function body;

    promise_error promise;
    future_error future;
    std::thread thread;

    std::atomic_bool running;
    std::atomic_bool has_error;

    /**
     * Construct setting the thread body.
     */
    worker(const function func);
    worker(worker&& orig);
    worker();
    ~worker();

    /**
     * Start the thread with the set function. Construct a worker with
     * a function or set it before calling start.
     */
    void start();
};

using workers = std::vector<worker>;

using waiter_func = std::function<void(workers::size_type)>;
using finished_func = std::function<void(workers::size_type)>;
using error_func = std::function<void(workers::size_type, error::code ec)>;

/**
 *
 */
pixie::error::code wait_until_finished(
    workers& threads, size_t msecs, const std::string error_message = "");

/**
 *
 */
pixie::error::code wait_until_finished(
    workers& threads, waiter_func& wait,
    finished_func& thread_finished, error_func& thread_error,
    const std::string& error_message);

/**
 *
 */
bool has_error(workers& threads);

} // namespace thread
} // namespace util
} // namespace xia

#endif  //PIXIESDK_UTIL_THREAD_HPP
