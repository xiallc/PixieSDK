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
#include <pixie/os_compat.hpp>
#include <pixie/utils/io.hpp>

namespace xia {
namespace logging {
/*
 * Local error type
 */
using error = xia::pixie::error::error;

/*
 * A single logging level.
 */
static std::atomic<log::level> log_level(xia::log::warning);

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

    outputter(const std::string& name, const std::string& filename, bool append);
    outputter(outputter&& op);
    ~outputter();

    void write(const log& entry);
    void write(const log::level entry_level, const std::string& entry);

private:
    std::shared_ptr<std::ofstream> outfile;
    std::ostream out;
};

static outputters_ptr outputs;

PIXIE_EXPORT outputters_ptr PIXIE_API make_outputters() {
    if (!outputs) {
        outputs = std::make_shared<outputters>();
    }
    return outputs;
}

static const char* level_label[log::max_level] = {"[OFF  ] ", "[ERROR] ", "[WARN ] ", "[INFO ] ",
                                                  "[DEBUG] "};

outputter::outputter(const std::string& name_, const std::string& filename_, bool append)
    : name(name_), filename(filename_), counter(0), linefeed(true), flush(false),
      show_level(true), show_counts(false), show_datetime(true), count_length(5), out(nullptr) {
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
      show_counts(op.show_counts), show_datetime(op.show_datetime), count_length(op.count_length),
      outfile(op.outfile), out(op.out.rdbuf()) {
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
    log::level current_level = log_level.load();
    log::level entry_level = entry.get_level();
    if (current_level != log::off && current_level >= entry_level) {
        write(entry_level, entry.output.str());
    }
}

void outputter::write(const log::level entry_level, const std::string& entry) {
    std::lock_guard<lock_type> guard(lock);
    log::level current_level = log_level.load();

    if (entry_level == log::level::off || current_level < entry_level) {
        return;
    }

    ++counter;

    if (show_level) {
        out << level_label[entry_level];
    }

    if (show_counts) {
        out << std::setw(count_length) << counter << ' ';
    }

    if (show_datetime) {
        using us = std::chrono::microseconds;
        auto now = std::chrono::system_clock::now();
        auto as_time_t = std::chrono::system_clock::to_time_t(now);
        const auto now_us = std::chrono::duration_cast<us>(now.time_since_epoch());
        char timeBuffer[80];
        std::strftime(timeBuffer, 80, "%FT%T", localtime(&as_time_t));
        out << timeBuffer << std::setfill('0') << '.' << std::setw(6) << now_us.count() % 1000000
            << ' ';
    }

    out << entry;

    if (linefeed) {
        out << std::endl;
    }

    if (flush) {
        out << std::flush;
    }
}

static void write(const log& entry) {
    for (auto& output : *outputs) {
        output.write(entry);
    }
}

static void write(const log::level entry_level, const std::string& entry) {
    for (auto& output : *outputs) {
        output.write(entry_level, entry);
    }
}

void start(const std::string name, const std::string file, bool append) {
    /*
     * If the log exists quietly return. Could be the API init call is
     * called again.
     */
    for (auto& output : *outputs) {
        if (output.name == name) {
            return;
        }
    }
    outputs->push_back(outputter(name, file, append));
}

void stop(const std::string name) {
    for (auto it = outputs->begin(); it != outputs->end(); ++it) {
        if ((*it).name == name) {
            outputs->erase(it);
            return;
        }
    }
    throw error(error::code::internal_failure, "invalid log output name in stop");
}

void set_level(log::level level) {
    log_level = level;
}

void set_level_stamp(const std::string name, bool level) {
    for (auto& output : *outputs) {
        if (output.name == name) {
            output.show_level = level;
            return;
        }
    }
    throw error(error::code::internal_failure, "invalid log output name in set level stamp");
}

void set_datetime_stamp(const std::string name, bool datetime) {
    for (auto& output : *outputs) {
        if (output.name == name) {
            output.show_datetime = datetime;
            return;
        }
    }
    throw error(error::code::internal_failure, "invalid log output name in set datetime stamp");
}

void set_line_numbers(const std::string name, bool line_numbers) {
    for (auto& output : *outputs) {
        if (output.name == name) {
            output.show_counts = line_numbers;
            return;
        }
    }
    throw error(error::code::internal_failure, "invalid log output name in set line numbers");
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
