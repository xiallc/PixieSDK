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

/** @file log.cpp
 * @brief Implements logging infrastructure components.
 */

#include <cerrno>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <pixie/error.hpp>
#include <pixie/log.hpp>
#include <pixie/os_compat.h>
#include <pixie/utils/io.hpp>

namespace xia {
namespace logging {
/*
 * Local error type
 */
using error = xia::pixie::error::error;

/*
 * Log clock.
 */
using log_time = std::chrono::time_point<std::chrono::system_clock>;

/*
 * A single logging level.
 */
static std::atomic<log::level> log_level(xia::log::warning);

/*
 * Log entry, the values for finding and formatted entry
 */
struct log_entry {
    static constexpr size_t entry_text_size = 128;
    using entry_text = char[entry_text_size];
    log::level level;
    size_t counter;
    log_time time;
    entry_text entry;

    log_entry();

    void set(
        const log::level level, const size_t counter, const log_time& time,
        const std::string& entry);

    void clear();
};

/*
 * Log MIB holds the entries the MIB window over those entries
 */
struct log_mib {
    enum struct mode { follow, hold };
    static constexpr size_t max_mib_entries = 1000;
    static constexpr size_t max_mibs = 50;

    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;
    using log_entries = std::array<log_entry, max_mib_entries>;
    using mibs_type = std::array<mib::node, max_mibs>;

    lock_type lock;
    size_t counter;
    log_entries entries;
    size_t entry_count;
    bool on;
    mode update;
    size_t in;
    size_t at;
    mibs_type mibs;
    size_t mib_count;

    mib::node command;
    bool show_help;

    log_mib();
    ~log_mib();

    void add(const log& entry);
    void add(const log::level entry_level, const std::string& entry);

    void get(size_t index, std::string& entry);

    void command_handler(std::string& cmd);

    void flush();

    size_t at_in_distance() const;
    size_t in_at_distance() const;

    size_t next_entry(size_t index) const;
};

/*
 * Synchronous outputter.
 *
 * Can be made asynchronous with a fixed size queue and worker thread.
 */
struct outputter {
    using lock_type = std::mutex;

    const std::string name;
    const std::string filename;

    std::mutex lock;
    int counter;

    /*
     * Output controls.
     */
    bool linefeed;
    bool flush;

    /*
     * Formatters
     */
    bool show_level;
    bool show_counts;
    bool show_datetime;

    /*
     * Format modifiers
     */
    int count_length;

    outputter(
        const std::string& name, const std::string& filename, bool append);
    outputter(outputter&& op);
    ~outputter();

    void write(const log& entry);
    void write(const log::level entry_level, const std::string& entry);

private:
    std::shared_ptr<std::ofstream> outfile;
    std::ostream out;
};

/*
 * The log handle is shared by each module that includes the log. This
 * holds the log mib and outputters open until all code modules have
 * destructed.
 */
static log_handle_ptr log_handles;

log_handle::log_handle()
    : mibs(std::make_unique<log_mib>()),
      outputs(std::make_unique<outputters>()) {
}

log_handle::~log_handle() {
}

PIXIE_EXPORT log_handle_ptr PIXIE_API make_log_handle() {
    if (!log_handles) {
        log_handles = std::make_shared<log_handle>();
    }
    return log_handles;
}

static const char* level_label[log::max_level] = {
    "[OFF  ]", "[ERROR]", "[WARN ]", "[INFO ]", "[DEBUG]"};

static void log_format_level(std::ostream& out, log::level level) {
    out << level_label[level];
}

static void log_format_counter(
    std::ostream& out, int counter, int count_length) {
    out << std::setw(count_length) << counter;
}

static void log_format_time(std::ostream& out, log_time& time) {
    using us = std::chrono::microseconds;
    auto as_time_t = std::chrono::system_clock::to_time_t(time);
    const auto time_us =
        std::chrono::duration_cast<us>(time.time_since_epoch());
    char timeBuffer[80];
    std::strftime(timeBuffer, 80, "%FT%T", localtime(&as_time_t));
    out << timeBuffer << std::setfill('0') << '.'
        << std::setw(6) << time_us.count() % 1000000;
}

outputter::outputter(
    const std::string& name_, const std::string& filename_, bool append)
    : name(name_), filename(filename_), counter(0), linefeed(true),
      flush(false), show_level(true), show_counts(false), show_datetime(true),
      count_length(5), out(nullptr) {
    if (filename.empty() || filename == "stdout") {
        out.rdbuf(std::cout.rdbuf());
    } else {
        outfile = std::make_shared<std::ofstream>();
        std::ios::openmode mode = std::ios_base::out;
        if (append) {
            mode |= std::ios::app;
        } else {
            mode |= std::ios::trunc;
        }
        outfile->open(filename, mode);
        if (!*outfile) {
            std::ostringstream what;
            what << "opening: " << filename << ": " << std::strerror(errno);
            throw error(error::code::file_create_failure, what);
        }
        out.rdbuf(outfile->rdbuf());
    }
    out << std::setfill(' ') << std::dec;
    if (outfile) {
        write(log::info, "begin " + name);
    }
}

outputter::outputter(outputter&& op)
    : name(op.name), filename(op.filename), counter(op.counter),
      linefeed(op.linefeed), flush(op.flush), show_level(op.show_level),
      show_counts(op.show_counts), show_datetime(op.show_datetime),
      count_length(op.count_length), outfile(op.outfile),
      out(op.out.rdbuf()) {
    op.outfile.reset();
}

outputter::~outputter() {
    if (outfile) {
        write(log::info, "end " + name);
        outfile->close();
        outfile.reset();
    } else {
        std::cout << std::flush;
    }
}

void outputter::write(const log& entry) {
    write(entry.get_level(), entry.get_entry());
}

void outputter::write(const log::level entry_level, const std::string& entry) {
    std::lock_guard<lock_type> guard(lock);
    log::level current_level = log_level.load();

    ++counter;

    if (entry_level == log::level::off || current_level < entry_level) {
        return;
    }

    if (show_level) {
        log_format_level(out, entry_level);
        out << ' ';
    }

    if (show_counts) {
        log_format_counter(out, counter, count_length);
        out << ' ';
    }

    if (show_datetime) {
        auto time = std::chrono::system_clock::now();
        log_format_time(out, time);
        out << ' ';
    }

    out << entry;

    if (linefeed) {
        out << std::endl;
    }

    if (flush) {
        out << std::flush;
    }
}


log_mib_status::log_mib_status() {
    clear();
}

log_mib_status::log_mib_status(const std::string& status) {
    parse(status);
}

void log_mib_status::parse(const std::string& status) {
    clear();
    util::string::strings fields;
    util::string::split(fields, status);
    for (auto& field : fields) {
        util::string::strings parts;
        util::string::split(parts, field, ':');
        if (parts.size() != 2) {
            throw error(
                error::code::invalid_value, "invalid log mib status");
        }
        if (parts[0] == "on") {
            if (parts[1] == "true") {
                on = true;
            } else if (parts[1] == "false") {
                on = false;
            } else {
                throw error(
                    error::code::invalid_value, "invalid log mib status");
            }
        } else if (parts[0] == "mode") {
            if (parts[1] == "follow") {
                mode = follow;
            } else if (parts[1] == "hold") {
                mode = hold;
            } else {
                throw error(
                    error::code::invalid_value, "invalid log mib status");
            }
        } else {
            std::istringstream iss(parts[1]);
            iss.exceptions(std::ifstream::failbit);
            size_t val;
            try {
                iss >> std::setbase(0) >> val;
            } catch (...) {
                throw error(
                    error::code::invalid_value, "invalid log mib status");
            }
            if (parts[0] == "count") {
                count = val;
            } else if (parts[0] == "mib-count") {
                mib_count = val;
            } else if (parts[0] == "entries") {
                entries = val;
            } else if (parts[0] == "in") {
                in = val;
            } else if (parts[0] == "at") {
                at = val;
            } else {
                throw error(
                    error::code::invalid_value, "invalid log mib status");
            }
        }
    }
}

void log_mib_status::clear() {
    count = 0;
    entries = 0;
    mib_count = 0;
    in = 0;
    at = 0;
    on = false;
    mode = follow;
}

log_entry::log_entry()  {
    clear();
}

void log_entry::set(
    const log::level level_, const size_t counter_, const log_time& time_,
    const std::string& entry_) {
    level = level_;
    counter = counter_;
    time = time_;
    /*
     * Since C++11 a string object has a null terminating character but lets
     * handle this locally.
     */
    auto time = std::chrono::system_clock::now();
    std::ostringstream oss;
    oss << level_label[level]
        << ' '
        << std::setw(6) << counter
        << ' ';
    log_format_time(oss, time);
    oss << ' '
        << entry_;
    size_t len = oss.str().size();
    if (len >= sizeof(entry)) {
        len = sizeof(entry) - 1;
    }
    std::memcpy(entry, oss.str().data(), len);
    entry[len + 1] = '\0';
}

void log_entry::clear() {
    level = log::level::off;
    counter = 0;
    std::memset(entry, 0, sizeof(entry));
}

log_mib::log_mib() :
    counter(0), entry_count(0), on(false), update(log_mib::mode::follow),
    in(0), at(0), mib_count(0), show_help(false) {
    size_t count = 0;
    for (auto& m : mibs) {
        m = mib::node("log." + std::to_string(count), mib::type::string);
        m.set_hint(mib::hint::fmt_no_quotes);
        m.disable();
        mib::event_func getter =
            [&self = *this, count = count](
                mib::event , mib::type , mib::data_type& data) {
                self.get(count, data.s);
            };
        m.set_event_func(mib::event::get, getter);
        ++count;
    }
    command = mib::node("log.command", mib::type::string);
    mib::event_func getter =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            std::ostringstream oss;
            oss << std::boolalpha
                << "count:" << self.counter
                << " mib-count:" << self.mib_count
                << " entries:" << self.max_mib_entries
                << " in:" << self.in
                << " at:" << self.at
                << " on:" << self.on
                << " mode:";
            switch (self.update) {
            case mode::follow:
                oss << "follow";
                break;
            case mode::hold:
                oss << "hold";
                break;
            }
            if (self.show_help) {
                self.show_help = false;
                oss << " help: help, on, off, flush, follow, "
                    << "hold, end, page-back, page-forward";
            }
            data.s = oss.str();
        };
    command.set_event_func(mib::event::get, getter);
    mib::event_func setter =
        [&self = *this](mib::event , mib::type , mib::data_type& data) {
            self.command_handler(data.s);
        };
    command.set_event_func(mib::event::set, setter);
}

log_mib::~log_mib() {
}

void log_mib::add(const log& entry) {
    add(entry.get_level(), entry.get_entry());
}

void log_mib::add(const log::level entry_level, const std::string& entry) {
    lock_guard guard(lock);
    log::level current_level = log_level.load();
    if (entry_level != log::level::off && current_level >= entry_level) {
        ++counter;
        auto time = std::chrono::system_clock::now();
#if LOG_MIB_TRACE
        std::cout << "[] in=" << in << " at=" << at
                  << " what=\"" << entry << '"' << std::endl;
#endif
        entries[in].set(entry_level, counter, time, entry);
        in = next_entry(in);
        if (update == mode::follow || at == in) {
            at = next_entry(at);
        }
        if (mib_count < max_mibs) {
            if (on) {
                mibs[mib_count].enable();
            }
            ++mib_count;
        }
    }
}

void log_mib::get(size_t index, std::string& entry) {
    const size_t available = at_in_distance();
    size_t window_size = std::min(counter, max_mibs);
    if (window_size > available) {
        window_size = available;
    }
    size_t entry_index =
        ((max_mib_entries - window_size) + index + at) % max_mib_entries;
#if LOG_MIB_TRACE
    std::cout << ")) E mode=" << int(update)
              << " index=" << index
              << " mib_count=" << mib_count
              << " entry_index=" << entry_index
              << " in=" << in
              << " at=" << at
              << " counter=" << counter
              << " available=" << available
              << " window_size=" << window_size
              << " counter=" << counter
              <<" max_mibs=" << max_mibs
              << std::endl;
#endif
    if (on && entry_index < counter && index < window_size) {
        entry = entries[entry_index].entry;
    } else {
        entry.clear();
    }
}

void log_mib::command_handler(std::string& cmd) {
    lock_guard guard(lock);
    if (cmd == "help") {
        show_help = true;
    } else if (cmd == "on") {
        on = true;
        for (size_t c = 0; c < mib_count; ++c) {
            mibs[c].enable();
        }
    } else if (cmd == "off") {
        on = false;
        for (auto& m : mibs) {
            m.disable();
        }
    } else if (cmd == "flush") {
        flush();
    } else if (cmd == "follow") {
        update = mode::follow;
        at = in;
    } else if (cmd == "hold") {
        update = mode::hold;
    } else if (cmd == "end") {
        update = mode::hold;
        at = in;
    } else if (cmd == "page-back") {
        update = mode::hold;
        const size_t available = at_in_distance();
        if (available >= mib_count) {
            if (at > mib_count) {
                at -= mib_count;
            } else {
                at = max_mib_entries - (mib_count - at);
            }
        }
    } else if (cmd == "page-forward") {
        update = mode::hold;
        const size_t available = in_at_distance();
        if (available >= mib_count) {
            if ((at + mib_count) < max_mib_entries) {
                at += mib_count;
            } else {
                at = mib_count - (max_mib_entries - at);
            }
        }
    }
}

void log_mib::flush() {
    for (size_t c = 0; c < mib_count; ++c) {
      mibs[c].disable();
    }
    for (auto& entry : entries) {
        entry.clear();
    }
    counter = 0;
    entry_count = 0;
    in = 0;
    at = 0;
    mib_count = 0;
}

size_t log_mib::at_in_distance() const {
    size_t distance;
    if (in < at) {
        distance = at - in;
    } else {
        distance = max_mib_entries - (at - in);
    }
    if (distance > counter) {
        distance = counter;
    }
    return distance;
}

size_t log_mib::in_at_distance() const {
    return max_mib_entries - at_in_distance();
}

size_t log_mib::next_entry(size_t index) const {
    ++index;
    return index < entries.size() ? index : 0;
}

static void write(const log& entry) {
    if (log_handles) {
        for (auto& output : *(log_handles->outputs)) {
            output.write(entry);
        }
        log_handles->mibs->add(entry);
    }
}

static void write(const log::level entry_level, const std::string& entry) {
    if (log_handles) {
        for (auto& output : *(log_handles->outputs)) {
            output.write(entry_level, entry);
        }
        log_handles->mibs->add(entry_level, entry);
    }
}

void start(const std::string name, const std::string file, bool append) {
    if (log_handles) {
        /*
         * If the log exists quietly return. Could be the API init call is
         * called again.
         */
        for (auto& output : *(log_handles->outputs)) {
            if (output.name == name) {
                return;
            }
        }
        log_handles->outputs->push_back(outputter(name, file, append));
    }
}

void stop(const std::string name) {
    if (log_handles) {
        for (auto it = log_handles->outputs->begin();
             it != log_handles->outputs->end(); ++it) {
            if ((*it).name == name) {
                log_handles->outputs->erase(it);
                return;
            }
        }
    }
    throw error(
        error::code::internal_failure, "invalid log output name in stop");
}

void set_level(log::level level) {
    log_level = level;
}

void set_level_stamp(const std::string name, bool level) {
    if (log_handles) {
        for (auto& output : *(log_handles->outputs)) {
            if (output.name == name) {
                output.show_level = level;
                return;
            }
        }
    }
    throw error(
        error::code::internal_failure,
        "invalid log output name in set level stamp");
}

void set_datetime_stamp(const std::string name, bool datetime) {
    if (log_handles) {
        for (auto& output : *(log_handles->outputs)) {
            if (output.name == name) {
                output.show_datetime = datetime;
                return;
            }
        }
    }
    throw error(
        error::code::internal_failure,
        "invalid log output name in set datetime stamp");
}

void set_line_numbers(const std::string name, bool line_numbers) {
    if (log_handles) {
        for (auto& output : *(log_handles->outputs)) {
            if (output.name == name) {
                output.show_counts = line_numbers;
                return;
            }
        }
    }
    throw error(
        error::code::internal_failure,
        "invalid log output name in set line numbers");
}

bool level_logging(log::level level) {
    log::level current_level = log_level.load();
    if ((current_level != log::off && current_level >= level) ||
        (current_level == log::off && level == log::off)) {
        return true;
    }
    return false;
}

log::level get_logging_level(void) {
    return log_level.load();
}

void memdump(log::level level, const std::string label, const void* addr, size_t length,
             size_t size, size_t line_length, size_t offset) {
    if (level_logging(level)) {
        util::io::write_string_func writer = [level](const std::string& out) {
            write(level, out);
        };
        util::io::memdump(addr, length, writer, label, size, line_length, offset);
    }
}
}  // namespace logging

log::~log() {
    logging::write(*this);
}

};  // namespace xia
