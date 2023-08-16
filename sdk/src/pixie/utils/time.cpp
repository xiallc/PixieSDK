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

/** @file time.cpp
 * @brief Defines utility functions and data structures related to timepoints useful throughout the SDK.
 */

#include <algorithm>
#include <iomanip>
#include <sstream>

#include <pixie/utils/time.hpp>

namespace xia {
namespace util {
namespace time {
timepoint::timepoint(bool autostart)
    : active(false), suspended(false), captured(false), locked(false) {
    if (autostart) {
        start();
    }
}

timepoint::timepoint(const timepoint& tp)
    : active(tp.active.load()), suspended(tp.suspended.load()), captured(tp.captured.load()),
      start_mark(tp.start_mark), end_mark(tp.end_mark),
      locked(false) {
}

timepoint::~timepoint() {
    lock();
}

void timepoint::reset() {
    lock();
    active = false;
    suspended = false;
    captured = false;
    unlock();
}

void timepoint::start() {
    lock();
    if (!active.load() && !captured.load()) {
        active = true;
        start_mark = std::chrono::steady_clock::now();
    }
    unlock();
}

void timepoint::end() {
    lock();
    if (!active.load() && !captured.load()) {
        active = false;
        suspended = false;
        captured = true;
        end_mark = std::chrono::steady_clock::now();
    }
    unlock();
}

void timepoint::pause() {
    if (active.load()) {
        suspended = true;
    }
}

void timepoint::resume() {
    suspended = false;
}

void timepoint::restart() {
    lock();
    active = true;
    suspended = false;
    captured = false;
    start_mark = std::chrono::steady_clock::now();
    unlock();
}

uint64_t timepoint::secs() {
    return msecs() / 1000;
}

uint64_t timepoint::msecs() {
    lock();
    if (active.load() && !suspended.load() && !captured.load()) {
        end_mark = std::chrono::steady_clock::now();
    }
    uint64_t period = 0;
    if (active.load() || captured.load()) {
        using namespace std::literals;
        period = (end_mark - start_mark) / 1ms;
    }
    unlock();
    return period;
}

uint64_t timepoint::usecs() {
    lock();
    if (active && !suspended.load() && !captured.load()) {
        end_mark = std::chrono::steady_clock::now();
    }
    uint64_t period = 0;
    if (active.load() || captured.load()) {
        using namespace std::literals;
        period = (end_mark - start_mark) / 1us;
    }
    unlock();
    return period;
}


timepoint::operator std::string() {
    return output();
}

std::string timepoint::output() {
    lock();
    if (active.load() && !captured.load()) {
        end_mark = std::chrono::steady_clock::now();
    }
    if (!active.load() && !captured.load()) {
        unlock();
        return std::string("n/a");
    }
    const auto duration = end_mark - start_mark;
    unlock();
    using namespace std::literals;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << duration / 1h << ':' << std::setw(2)
        << (duration / 1min) % 60 << ':' << std::setw(2) << (duration / 1s) % 60 << '.'
        << std::setw(6) << (duration / 1us) % 1000000;
    return oss.str();
}

void timepoint::lock() {
    while (true) {
        bool is_locked = locked.exchange(true);
        if (!is_locked) {
            return;
        }
    }
}

void timepoint::unlock() {
    locked = false;
}

timepoint& timepoint::operator=(const timepoint& tp) {
    lock();
    active = tp.active.load();
    suspended = tp.suspended.load();
    captured = tp.captured.load();
    start_mark = tp.start_mark;
    end_mark = tp.end_mark;
    unlock();
    return *this;
}

std::string datetime_iso8601(datetime_timepoint tp) {
    auto secs = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    auto ns =
        std::chrono::time_point_cast<std::chrono::nanoseconds>(tp) -
        std::chrono::time_point_cast<std::chrono::nanoseconds>(secs);
    std::time_t time = std::time_t{secs.time_since_epoch().count()};
    char buf[sizeof "YYYY-MM-DDTHH:MM:SS.MMMZ"];
    struct tm tm;
#if defined(_WIN64) || defined(_WIN32)
    ::gmtime_s(&tm, &time);
#else
    ::gmtime_r(&time, &tm);
#endif
    auto end = std::strftime(buf, sizeof buf, "%FT%T.", &tm);
    snprintf(buf + end, sizeof(buf) - end, "%03dZ", int((ns.count() / 1000000) % 1000));
    return std::string(buf);
}

std::string datetime_iso8601() {
    return datetime_iso8601(std::chrono::system_clock::now());
}

void datetime_iso8601_as_filename(std::string& datetime) {
    std::replace(datetime.begin(), datetime.end(), ':', '-');
}
}
}
}

std::ostream& operator<<(std::ostream& out, xia::util::time::timepoint& timepoint) {
    out << timepoint.output();
    return out;
}