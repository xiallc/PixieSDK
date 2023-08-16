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

/** @file time.hpp
 * @brief Defines utility functions and data structures related to timepoints useful throughout the SDK.
 */

#ifndef PIXIESDK_UTIL_TIME_HPP
#define PIXIESDK_UTIL_TIME_HPP

#include <atomic>
#include <chrono>
#include <string>

namespace xia {
/**
 * @brief Utilities to accomplish various needs in the SDK.
 */
namespace util {
namespace time {

/**
 * @brief Timepoint measures a period of time between two points.
 */
struct timepoint {
    using marker = std::chrono::time_point<std::chrono::steady_clock>;

    std::atomic_bool active;
    std::atomic_bool suspended;
    std::atomic_bool captured;

    marker start_mark;
    marker end_mark;

    timepoint(bool autostart = false);
    timepoint(const timepoint& tp);
    ~timepoint();

    bool running() const {
        return active.load() && !suspended.load();
    }

    void reset();
    void start();
    void end();
    void stop() {
        end();
    }
    void restart();
    void pause();
    void resume();

    uint64_t secs();
    uint64_t msecs();
    uint64_t usecs();

    operator std::string();
    std::string output();

    timepoint& operator=(const timepoint& tp);

private:
    void lock();
    void unlock();

    std::atomic_bool locked;
};

/**
 * @brief The datetime clock we use is the system clock.
 */
using datetime_timepoint = std::chrono::time_point<std::chrono::system_clock>;

/**
 * @brief Return time in the ISO-8601 format
 */
std::string datetime_iso8601(datetime_timepoint tp);

/**
 * @brief Return the current time in the ISO-8601 format
 */
std::string datetime_iso8601();

/**
 * @brief Make an ISO-8601 format string suitable as a file name.
 */
void datetime_iso8601_as_filename(std::string& datetime);

} // namespace time
} // namespace util
} // namespace xia

/**
 * @brief Output stream operator for timepoint objects.
 */
std::ostream& operator<<(std::ostream& out, xia::util::time::timepoint& timepoint);

#endif  //PIXIESDK_UTIL_TIME_HPP
