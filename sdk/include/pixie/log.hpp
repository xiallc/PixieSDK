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

#include <pixie/os_compat.hpp>

namespace xia {
/**
 * @brief Components for logging to a file or stream.
 */
namespace logging {
/**
 * @brief An outputter outputs a log stream. Destruct them last.
 */
struct outputter;
using outputters = std::list<outputter>;
using outputters_ptr = std::shared_ptr<outputters>;
PIXIE_EXPORT outputters_ptr PIXIE_API make_outputters();
static outputters_ptr outputs_ptr = make_outputters();
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
    friend logging::outputter;

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
