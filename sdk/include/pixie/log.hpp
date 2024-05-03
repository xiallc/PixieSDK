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

/** @file log.hpp
 * @brief Defines logging infrastructure components.
 */

#ifndef PIXIE_LOG_H
#define PIXIE_LOG_H

#include <atomic>
#include <list>
#include <memory>
#include <sstream>

#include <pixie/os_compat.h>

#include <pixie/mib.hpp>

namespace xia {
/**
 * @brief Components for logging to a file or stream.
 */
namespace logging {
/**
 * @brief A log handle contains the log mib and outputter outputs that
 * log stream. Destruct them last.
 *
 * The MIB header has to be included in this header before the static
 * below is declared. This ensures the MIB is created before the log
 * MIB is created and destructed after the log MIB is destroyed.
 */
struct log_mib;
using log_mib_ptr = std::unique_ptr<log_mib>;
struct outputter;
using outputters = std::list<outputter>;
using outputters_ptr = std::unique_ptr<outputters>;
struct log_handle {
    log_mib_ptr mibs;
    outputters_ptr outputs;
    log_handle();
    ~log_handle();
};
using log_handle_ptr = std::shared_ptr<log_handle>;
PIXIE_EXPORT log_handle_ptr PIXIE_API make_log_handle();
static log_handle_ptr logs = make_log_handle();
};  // namespace logging

/**
 * @brief A class to provide logging support throughout the SDK.
 *
 * This class lives in the pixie namespace to make the references in the code
 * simpler.
 *
 * The macro xia_log provides conditional use of log instances. This improves
 * runtime performance.
 */
class log {
public:
    /**
     * The log level defines what type of messages get processed.
     *
     * The levels are ordered from high priority to lower priority with `off`
     * being always first and 0.
     */
    enum level { off = 0, error, warning, info, debug, max_level };

    log(level level__) : level_(level__) {}
    ~log();

    template<typename T>
    std::ostringstream& operator<<(T item) {
        output << item;
        return output;
    }

    log::level get_level() const {
        return level_.load();
    }

    const std::string get_entry() const {
        return output.str();
    }

private:
    std::atomic<level> level_;
    std::ostringstream output;
};

namespace logging {
/*
  * Start and stop a log output stream.
  */
void start(const std::string name, const std::string file, bool append = true);
void stop(const std::string name);

/*
 * Output control.
 */
void set_level(log::level level);
void set_level_stamp(const std::string name, bool level);
void set_datetime_stamp(const std::string name, bool datetime);
void set_line_numbers(const std::string name, bool line_numbers);

/*
 * Check the currently active logging level
 */
bool level_logging(log::level level);
log::level get_logging_level(void);

/*
 * Guard to temporally change the logging level
 */
struct log_level_guard {
    log::level level;
    log_level_guard(log::level new_level) {
        level = xia::logging::get_logging_level();
        if (!xia::logging::level_logging(new_level)) {
            xia::logging::set_level(new_level);
        }
    }
    ~log_level_guard() {
        xia::logging::set_level(level);
    }
};

/*
 * Parse and return the log.command status returned with a MIB node
 * get operation.
 */
struct log_mib_status {
    enum mode_type { follow, hold };
    size_t count;
    size_t entries;
    size_t mib_count;
    size_t in;
    size_t at;
    bool on;
    mode_type mode;

    log_mib_status();
    log_mib_status(const std::string& status);

    void parse(const std::string& status);

    void clear();
};

/**
 * @brief Outputs a memory segment as hex values.
 * @param[in] level The logging level for the output.
 * @param[in] label A label that can be used to name the output for finding it in the logs.
 * @param[in] addr The address of the memory to display.
 * @param[in] length The number of elements to display.
 * @param[in] size The size of the data element.
 * @param[in] line_length Number of elements per line.
 * @param[in] offset The printed offset.
 */
void memdump(log::level level, const std::string label, const void* addr, size_t length,
             size_t size = 1, size_t line_length = 16, size_t offset = 0);
}  // namespace logging
}  // namespace xia

/*
 * Use the following macro to control logging via the preprocessor.
 */
#define xia_log(_level) \
          if (xia::logging::level_logging(_level)) xia::log(_level)

#endif  // PIXIE_LOG_H
