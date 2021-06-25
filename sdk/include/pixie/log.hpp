#ifndef PIXIE_LOG_H
#define PIXIE_LOG_H

/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
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

#include <atomic>
#include <list>
#include <memory>
#include <sstream>

#include <pixie/os_compat.hpp>

namespace xia
{
namespace logging
{
/*
 * An outputter outputs a log stream. Destruct them last.
 */
struct outputter;
typedef std::list<outputter> outputters;
typedef std::shared_ptr<outputters> outputters_ptr;
PIXIE_EXPORT outputters_ptr PIXIE_API make_outputters();
static outputters_ptr outputs_ptr = make_outputters();
};

/**
 * Log class.
 *
 * This class lives in the pixie namespace to make the references in the code
 * simpler.
 */
class log
{
    friend logging::outputter;
public:
    /*
     * A log level.
     *
     * The levels are ordered from high priority to lower priority with `off`
     * always being first and 0.
     */
    enum level {
        off = 0,
        error,
        warning,
        info,
        debug,
        max_level
    };

    log(level level__)
        : level_(level__) {
    }
    ~log();

    template <typename T>
    std::ostringstream& operator<<(T item) {
        output << item;
        return output;
    }

    log::level get_level() const{
        return level_.load();
    }

private:
    std::atomic<level> level_;
    std::ostringstream output;
};

namespace logging
{
 /*
  * Start and stop a log output stream.
  */
void start(const std::string name,
           const std::string file,
           log::level level = log::warning,
           bool append = true);
void stop(const std::string name);

/*
 * Output control.
 */
void set_level(const std::string name, log::level level);
void set_level_stamp(const std::string name, bool level);
void set_datetime_stamp(const std::string name, bool datetime);
void set_line_numbers(const std::string name, bool line_numbers);

/*
 * Level active
 */
bool level_logging(log::level level);

/**
 * Hex display memory.
 *
 * @param addr The address of the memory to display.
 * @param length The number of elements to display.
 * @param size The size of the data element.
 * @param line_length Number of elements per line.
 * @param offset The printed offset.
 *
 * From https://git.rtems.org/rtems-tools/tree/rtemstoolkit/rtems-utils.cpp#n39
 */
void memdump(log::level level,
             const std::string label,
             const void* addr,
             size_t length,
             size_t size = 1,
             size_t line_length = 16,
             size_t offset = 0);
}
}

#endif  // PIXIE_LOG_H
