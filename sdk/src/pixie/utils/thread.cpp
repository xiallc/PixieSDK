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

/** @file thread.cpp
 * @brief Defines utility functions and data structures related to threads useful throughout the SDK.
 */

#include <thread>

#include <pixie/error.hpp>
#include <pixie/utils/thread.hpp>

namespace xia {
namespace util {
namespace thread {
worker::worker(const function func)
    : body(func), future(promise.get_future()), running(false), has_error(false) {}

worker::worker(worker&& orig)
    : body(std::move(orig.body)), future(promise.get_future()),
      thread(std::move(orig.thread)), running(orig.running.load()),
      has_error(orig.has_error.load()) {}

worker::worker() : future(promise.get_future()), running(false), has_error(false) {}

worker::~worker() {
}

void worker::start() {
    /*
     * The func is copied onto the thread's stack so it is contained
     * in the thread's context. This avoids any potential issue
     * related to the life-cycle of this object and the thread.
     */
    thread = std::thread([&self = *this, func = body]() {
        try {
            self.running = true;
            func();
            self.promise.set_value(error::code::success);
        } catch (xia::pixie::error::error& e) {
            self.has_error = true;
            self.promise.set_value(e.type);
        } catch (...) {
            self.has_error = true;
            try {
                self.promise.set_exception(std::current_exception());
            } catch (...) {
            }
        }
        self.running = false;
    });
}

pixie::error::code wait_until_finished(
    workers& threads, size_t msecs, const std::string error_message) {
    waiter_func waiter =
        [msecs](util::thread::workers::size_type ) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms * msecs);
        };
    finished_func finished;
    error_func error;
    return wait_until_finished(
        threads, waiter, finished, error, error_message);
}

pixie::error::code wait_until_finished(
    workers& threads, waiter_func& wait,
    finished_func& thread_finished, error_func& thread_error,
    const std::string& error_message) {
    const auto zero = std::chrono::seconds(0);
    error::code first_error = error::code::success;
    size_t finished = 0;
    while (finished != threads.size()) {
        finished = threads.size();
        for (workers::size_type t = 0; t < threads.size(); ++t) {
            auto& thread = threads[t];
            if (thread.future.valid()) {
                if (thread.future.wait_for(zero) == std::future_status::ready) {
                    if (thread_finished) {
                        thread_finished(t);
                    }
                    error::code e = thread.future.get();
                    if (thread_error && e != error::code::success) {
                        thread_error(t, e);
                    }
                    if (first_error == error::code::success) {
                        first_error = e;
                    }
                    thread.thread.join();
                } else {
                    --finished;
                }
            }
        }
        auto running = threads.size() - finished;
        if (running != 0) {
            wait(running);
        }
    }
    if (first_error != error::code::success && !error_message.empty()) {
        throw pixie::error::error(first_error, error_message);
    }
    return first_error;
}
}
}
}
