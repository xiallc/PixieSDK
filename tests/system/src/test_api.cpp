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

#include <algorithm>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>

#include <pixie_crate.hpp>
#include <pixie_log.hpp>
#include <pixie_util.hpp>

#include "simulation.hpp"

#include "args.hxx"

/*
 * Localize the log and error
 */
using xia_log = xia::log;
using error = xia::pixie::error::error;

/*
 * Command processor.
 */
using options = std::vector<std::string>;
using commands = std::vector<options>;

struct command_def
{
    std::vector<size_t> counts;
    std::string help;
};

struct cmd_handler {
    std::string cmd;
    void (*func)(xia::pixie::crate::crate& crate, options& cmd);
};

static void help(xia::pixie::crate::crate& crate, options& cmd);
static void boot(xia::pixie::crate::crate& crate, options& cmd);
static void crate_report(xia::pixie::crate::crate& crate, options& cmd);
static void adj_off(xia::pixie::crate::crate& crate, options& cmd);
static void set_dacs(xia::pixie::crate::crate& crate, options& cmd);
static void adc_acq(xia::pixie::crate::crate& crate, options& cmd);
static void adc_save(xia::pixie::crate::crate& crate, options& cmd);
static void bl_acq(xia::pixie::crate::crate& crate, options& cmd);
static void bl_save(xia::pixie::crate::crate& crate, options& cmd);
static void hist_start(xia::pixie::crate::crate& crate, options& cmd);
static void hist_resume(xia::pixie::crate::crate& crate, options& cmd);
static void hist_save(xia::pixie::crate::crate& crate, options& cmd);
static void list_start(xia::pixie::crate::crate& crate, options& cmd);
static void list_resume(xia::pixie::crate::crate& crate, options& cmd);
static void list_save(xia::pixie::crate::crate& crate, options& cmd);
static void run_active(xia::pixie::crate::crate& crate, options& cmd);
static void run_end(xia::pixie::crate::crate& crate, options& cmd);
static void par_write(xia::pixie::crate::crate& crate, options& cmd);
static void par_read(xia::pixie::crate::crate& crate, options& cmd);
static void var_write(xia::pixie::crate::crate& crate, options& cmd);
static void var_read(xia::pixie::crate::crate& crate, options& cmd);
static void stats(xia::pixie::crate::crate& crate, options& cmd);
static void import(xia::pixie::crate::crate& crate, options& cmd);
static void export_(xia::pixie::crate::crate& crate, options& cmd);
static void test(xia::pixie::crate::crate& crate, options& cmd);
static void wait(xia::pixie::crate::crate& crate, options& cmd);

static const std::map<std::string, command_def> command_defs =
{
    { "help",        { { 0 },       "command help" } },
    { "crate",       { { 0 },       "report the crate" } },
    { "boot",        { { 0 },       "boots the module(s)" } },
    { "adj-off",     { { 1 },       "adjust the module's offsets" } },
    { "set-dacs",    { { 1 },       "set the module's DACs" } },
    { "adc-acq",     { { 1 },       "acquire a module's ADC trace" } },
    { "adc-save",    { { 1, 2, 3 }, "save a module's ADC trace to a file" } },
    { "bl-acq",      { { 1 },       "acquire the module's baselines" } },
    { "bl-save",     { { 1, 2 },    "save the module's baselines" } },
    { "run-active",  { { 1 },       "does the module have an active run?" } },
    { "run-end",     { { 1 },       "end the module's run" } },
    { "hist-start",  { { 1 },       "start module histograms" } },
    { "hist-resume", { { 1 },       "resume module histograms" } },
    { "hist-save",   { { 1, 2, 3 }, "save a module's histogram to a file" } },
    { "list-start",  { { 1 },       "start module list mode" } },
    { "list-resume", { { 1 },       "resume module list mode" } },
    { "list-save",   { { 3 },       "save a module's histogram to a file" } },
    { "par-read",    { { 2, 3 },    "read module/channel parameter" } },
    { "par-write",   { { 3, 4 },    "write module/channel parameter" } },
    { "var-read",    { { 2, 3, 4 }, "read module/channel variable" } },
    { "var-write",   { { 3, 4, 5 }, "write module/channel variable" } },
    { "stats",       { { 2, 3 },    "module/channel stats" } },
    { "import",      { { 1 },       "import a JSON configuration file" } },
    { "export",      { { 1 },       "export a configuration to a JSON file" } },
    { "test",        { { 3 },       "start a test" } },
    { "wait",        { { 1 },       "wait a number of msecs" } }
};

static const std::vector<cmd_handler> cmd_handlers = {
    { "help",        help },
    { "crate",       crate_report },
    { "boot",        boot },
    { "adj-off",     adj_off },
    { "set-dacs",    set_dacs },
    { "adc-acq",     adc_acq },
    { "adc-save",    adc_save },
    { "bl-acq",      bl_acq },
    { "bl-save",     bl_save },
    { "hist-start",  hist_start },
    { "hist-resume", hist_resume },
    { "hist-save",   hist_save },
    { "list-start",  list_start },
    { "list-resume", list_resume },
    { "list-save",   list_save },
    { "run-active",  run_active },
    { "run-end",     run_end },
    { "par-write",   par_write },
    { "par-read",    par_read },
    { "var-write",   var_write },
    { "var-read",    var_read },
    { "stats",       stats },
    { "import",      import },
    { "export",      export_ },
    { "test",        test },
    { "wait",        wait },
};

static std::string adc_prefix = "adc-trace";
static std::string histogram_prefix = "histo";
static std::string baseline_prefix = "baseline";

static bool
check_number(const std::string& opt)
{
    return
        std::regex_match(opt,
                         std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

template<typename T> static T
get_value(const std::string& opt)
{
    check_number(opt);
    std::istringstream iss(opt);
    T value;
    iss >> value;
    return value;
}

template<typename T> static std::vector<T>
get_values(const std::string& opt, const size_t modules)
{
    std::vector<T> values;
    if (opt == "all") {
        values.resize(modules);
        std::iota(values.begin(), values.end(), 0);
    } else {
        xia::util::strings sc;
        xia::util::split(sc, opt, ',');
        for (auto& slots : sc) {
            xia::util::strings sd;
            xia::util::split(sd, slots, '-');
            if (sd.size() == 1) {
                values.push_back(get_value<T>(sd[0]));
            } else if (sd.size() == 2) {
                auto start = get_value<T>(sd[0]);
                auto end = get_value<T>(sd[1]);
                if (start > end) {
                    throw std::runtime_error(
                        "invalid slots, start before end: " + opt
                    );
                }
                for (T s = start; s <= end; ++s) {
                    values.push_back(s);
                }
            } else {
                throw std::runtime_error("invalid slots: " + opt);
            }
        }
    }
    return values;
}

static void
module_check(xia::pixie::crate::crate& crate,
             std::vector<size_t> mod_nums) {
    for (auto mod_num : mod_nums) {
        if (mod_num > crate.num_modules) {
            throw std::runtime_error(
                std::string("invalid module number: " + std::to_string(mod_num))
            );
        }
        if (!crate.modules[mod_num]->online()) {
            throw std::runtime_error(
                std::string("module offline: " + std::to_string(mod_num))
            );
        }
    }
}

template<typename V>
static void
output_value(const std::string& name, V value)
{
    xia::util::ostream_guard oguard(std::cout);
    std::cout << name << " = " << value;
    if (!std::is_same<V, double>::value) {
        std::cout << std::hex << " (0x" << value << ')';
    }
    std::cout << std::endl;
}

static bool
make_command_sets(const std::vector<std::string>& cmd_strings, commands& cmds)
{
    options option;
    for (auto opt : cmd_strings) {
        if (option.empty()) {
            auto search = command_defs.find(opt);
            if (search == command_defs.end()) {
                std::cerr << "error: invalid command: " << opt
                          << std::endl;
                return false;
            }
        }
        option.push_back(opt);
        auto search = command_defs.find(option[0]);
        auto counts = search->second.counts;
        auto min = std::min_element(counts.begin(), counts.end());
        auto max = std::max_element(counts.begin(), counts.end());
        if (*min != *max && option.size() > *min) {
            search = command_defs.find(opt);
            if (search != command_defs.end()) {
                if (option.size() > 1) {
                    option.pop_back();
                }
                cmds.push_back(option);
                option.clear();
                option.push_back(opt);
            }
        }
        if (option.size() > *max) {
          cmds.push_back(option);
          option.clear();
        }
    }
    if (!option.empty()) {
        auto search = command_defs.find(option[0]);
        auto counts = search->second.counts;
        auto min = std::min_element(counts.begin(), counts.end());
        if (option.size() <= *min) {
            std::cerr << "error: malformed command:";
            for (auto& opt : option) {
                std::cerr << ' ' << opt;
            }
            std::cerr << std::endl;
            return false;
        }
        cmds.push_back(option);
    }
    return true;
}

static bool
make_command_sets(args::PositionalList<std::string>& cmd, commands& cmds)
{
    std::vector<std::string> cmd_strings;
    for (auto opt : args::get(cmd)) {
        cmd_strings.push_back(opt);
    }
    return make_command_sets(cmd_strings, cmds);
}

static bool
make_command_sets(std::string cmd_file, commands& cmds)
{
    std::ifstream file(cmd_file, std::ios::binary);
    if (!file) {
        std::cerr << "error: cannot open command file: " << cmd_file
                  << std::endl;
        return false;
    }
    std::vector<std::string> cmd_strings;
    for (std::string line; std::getline(file, line); ) {
        xia::util::ltrim(line);
        if (!line.empty() && line[0] != '#') {
            xia::util::strings ss;
            xia::util::split(ss, line);
            for (auto& s : ss) {
                cmd_strings.push_back(s);
            }
        }
    }
    file.close();
    return make_command_sets(cmd_strings, cmds);
}

template<typename W> void
module_threads(xia::pixie::crate::crate& crate,
               std::vector<size_t>& mod_nums,
               std::vector<W>& workers,
               std::string error_message)
{
    if (workers.size() != mod_nums.size()) {
        throw std::runtime_error("workers and modules counts mismatch");
    }
    using promise_error =std::promise<error::code>;
    using future_error = std::future<error::code>;
    std::vector<promise_error> promises(mod_nums.size());
    std::vector<future_error> futures;
    std::vector<std::thread> threads;
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto module = crate.modules[mod_nums[m]];
        auto& worker = workers[m];
        futures.push_back(future_error(promises[m].get_future()));
        threads.push_back(
            std::thread(
                [m, &promises, module, &worker] {
                    try {
                        worker.worker(*module);
                        promises[m].set_value(error::code::success);
                    } catch (xia::pixie::error::error& e) {
                        promises[m].set_value(e.type);
                    } catch (...) {
                        try {
                            promises[m].set_exception(std::current_exception());
                        } catch (...) { }
                    }
                }));
    }
    error::code first_error = error::code::success;
    for (size_t t = 0; t < threads.size(); ++t) {
        error::code e = futures[t].get();
        if (first_error == error::code::success) {
            first_error = e;
        }
        threads[t].join();
    }
    if (first_error != error::code::success) {
        throw error(first_error, error_message);
    }
}

template<typename W> void
set_seconds(std::vector<W>& workers, const size_t secs)
{
    for (auto& w : workers) {
        w.seconds = secs;
    }
}

template<typename W> void
performance_stats(std::vector<W>& workers, const size_t secs)
{
    size_t total = 0;
    for (auto& w : workers) {
        total += w.total;
    }
    total *= sizeof(xia::pixie::hw::word);
    std::cout << "data received: " << total
              << " bytes, rate: "
              << xia::util::humanize(double(total) / secs, " bytes/sec")
              << std::endl;
    xia_log(xia_log::info) << "data received: " << total
                           << " bytes, rate: "
                           << double(total) / secs << " bytes/sec";
}

static void
help(xia::pixie::crate::crate& , options& )
{
  std::cout << "Command help:" << std::endl;
  auto mi = std::max_element(command_defs.begin(),
                             command_defs.end(),
                             [](auto& a, auto& b) {
                                 return (std::get<0>(a).size() <
                                         std::get<0>(b).size());
                             });
  auto max = std::get<0>(*mi).size();
  for (auto& cmd_def : command_defs) {
      std::cout << std::setw(max + 1) << std::get<0>(cmd_def)
                << " " << std::get<1>(cmd_def).help
                << std::endl;
  }
}

static void
boot(xia::pixie::crate::crate& crate, options& )
{
    xia::util::timepoint tp;
    std::cout << "booting crate" << std::endl;
    tp.start();
    crate.boot();
    tp.end();
    std::cout << "boot time=" << tp << std::endl;
}

static void
crate_report(xia::pixie::crate::crate& crate, options& )
{
  std::cout << crate << std::endl;
}

static void
adj_off(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    crate[mod_num].adjust_offsets();
}

static void
set_dacs(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    crate[mod_num].set_dacs();
}

static void
adc_acq(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    crate[mod_num].get_traces();
}

static void
adc_save(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    xia::pixie::channel::range channels;
    size_t length = xia::pixie::hw::max_adc_trace_length;
    if (cmd.size() == 3) {
        auto value = get_value<size_t>(cmd[2]);
        if (value > crate[mod_num].num_channels) {
            length = value;
        } else {
            channels.resize(1);
            channels[0] = value;
        }
    } else if (cmd.size() == 4) {
        channels.resize(1);
        channels[0] = get_value<size_t>(cmd[2]);
        length = get_value<size_t>(cmd[3]);
    }
    if (channels.empty()) {
        channels.resize(crate[mod_num].num_channels);
        xia::pixie::channel::range_set(channels);
    }
    for (auto channel : channels) {
        xia::pixie::hw::adc_trace adc_trace(length);
        crate[mod_num].read_adc(channel, adc_trace, false);
        std::ostringstream name;
        name << std::setfill('0') << adc_prefix
             << '-' << std::setw(2) << mod_num
             << '-' << std::setw(2) << channel << ".bin";
        std::ofstream out(name.str(), std::ios::binary);
        out.write(reinterpret_cast<char*>(adc_trace.data()),
                  adc_trace.size() * sizeof(xia::pixie::hw::adc_trace::value_type));
    }
}

static void
bl_acq(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].acquire_baselines();
    }
}

static void
bl_save(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        if (cmd.size() == 3) {
            channels.resize(1);
            channels[0] = get_value<size_t>(cmd[2]);
        }
        if (channels.empty()) {
            channels.resize(crate[mod_num].num_channels);
            xia::pixie::channel::range_set(channels);
        }
        xia::pixie::channel::baseline::channels_values baselines(channels.size());
        crate[mod_num].bl_get(channels, baselines, false);
        for (size_t channel = 0; channel < channels.size(); ++channel) {
            std::ostringstream name;
            name << std::setfill('0') << baseline_prefix
                 << '-' << std::setw(2) << mod_num
                 << '-' << std::setw(2) << channels[channel] << ".txt";
            std::ofstream out(name.str(), std::ios::binary);
            for (auto& value : baselines[channel]) {
                out << "timestamp=" << std::get<0>(value)
                    << ",baseline=" << std::get<1>(value)
                    << std::endl;
            }
        }
    }
}

static void
run_active(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        std::cout << "module=" << mod_num << " run-active="
                  << std::boolalpha
                  << crate[mod_num].run_active()
                  << std::endl;
    }
}

static void
hist_start(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    using namespace xia::pixie::hw::run;
    crate[mod_num].start_histograms(run_mode::new_run);
}

static void
hist_resume(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    using namespace xia::pixie::hw::run;
    crate[mod_num].start_histograms(run_mode::resume);
}

static void
hist_save(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_num = get_value<size_t>(cmd[1]);
    using namespace xia::pixie::hw::run;
    xia::pixie::channel::range channels;
    size_t length = xia::pixie::hw::max_histogram_length;
    if (cmd.size() == 3) {
        auto value = get_value<size_t>(cmd[2]);
        if (value > crate[mod_num].num_channels) {
            length = value;
        } else {
            channels.resize(1);
            channels[0] = value;
        }
    } else if (cmd.size() == 4) {
        channels.resize(1);
        channels[0] = get_value<size_t>(cmd[2]);
        length = get_value<size_t>(cmd[3]);
    }
    if (channels.empty()) {
        channels.resize(crate[mod_num].num_channels);
        xia::pixie::channel::range_set(channels);
    }
    for (auto channel : channels) {
        xia::pixie::hw::words histogram(length);
        crate[mod_num].read_histogram(channel, histogram);
        std::ostringstream name;
        name << std::setfill('0') << histogram_prefix
             << '-' << std::setw(2) << mod_num
             << '-' << std::setw(2) << channel << ".bin";
        std::ofstream out(name.str(), std::ios::binary);
        out.write(reinterpret_cast<char*>(histogram.data()),
                  histogram.size() * sizeof(xia::pixie::hw::word));
    }
}

static void
list_start(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::new_run);
    }
}

static void
list_resume(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::resume);
    }
}

struct list_save_worker
{
    std::string name;
    size_t total;
    size_t seconds;

    list_save_worker();
    void worker(xia::pixie::module::module& module);

};

list_save_worker::list_save_worker()
    : total(0),
      seconds(0)
{
}

void
list_save_worker::worker(xia::pixie::module::module& module)
{
    name += '-' + std::to_string(module.number) + ".lmd";
    std::ofstream out(name, std::ios::binary);
    if (!out) {
        throw std::runtime_error(
            std::string("list mode file open: ") + name +
            ": " + std::strerror(errno)
        );
    }
    const size_t poll_period_usecs = 100 * 1000;
    total = 0;
    xia::util::timepoint duration(true);
    while (duration.msecs() < (seconds * 1000)) {
        size_t data_available = module.read_list_mode_level();
        if (data_available > 0) {
            xia::pixie::hw::words lm;
            module.read_list_mode(lm);
            total += lm.size();
            out.write(reinterpret_cast<char*>(lm.data()),
                      lm.size() * sizeof(xia::pixie::hw::word));
        }
        if (data_available == 0) {
            xia::pixie::hw::wait(poll_period_usecs);
        }
    }
}

static void
list_save(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    auto secs = get_value<size_t>(cmd[2]);
    auto name = cmd[3];
    module_check(crate, mod_nums);
    if (secs == 0) {
        throw std::runtime_error(
            std::string("list mode save period is 0")
        );
    }
    auto saves = std::vector<list_save_worker>(mod_nums.size());
    for (auto& s : saves) {
        s.name = name;
    };
    set_seconds(saves, secs);
    module_threads(crate, mod_nums, saves, "list mode save error; see log");
    performance_stats(saves, secs);
}

static void
run_end(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_end();
    }
}

static void
par_write(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        if (cmd.size() == 4) {
            auto value =
                get_value<xia::pixie::param::value_type>(cmd[3]);
            crate[mod_num].write(cmd[2], value);
        } else {
            auto channel_num = get_value<size_t>(cmd[2]);
            auto value = get_value<double>(cmd[4]);
            crate[mod_num].write(cmd[3], channel_num, value);
        }
    }
}

static void
par_read(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        int reg_name;
        if (cmd.size() == 3) {
            reg_name = 2;
            std::cout << "# module param read: " << mod_num
                      << ": " << cmd[reg_name] << std::endl;
            if (cmd[reg_name] == "all") {
                for (auto& par : xia::pixie::param::get_module_param_map()) {
                    try {
                        output_value(par.first, crate[mod_num].read(par.second));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(cmd[reg_name], crate[mod_num].read(cmd[reg_name]));
            }
        } else {
            reg_name = 3;
            auto channel_num = get_value<size_t>(cmd[2]);
            std::cout << "# channel param read: " << mod_num << ':' << channel_num
                      << ": " << cmd[reg_name] << std::endl;
            if (cmd[reg_name] == "all") {
                for (auto& par : xia::pixie::param::get_channel_param_map()) {
                    try {
                        output_value(par.first,
                                     crate[mod_num].read(par.second, channel_num));
                    } catch (error& e) {
                        if (e.type != error::code::channel_param_disabled &&
                            e.type != error::code::channel_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(cmd[reg_name],
                             crate[mod_num].read(cmd[reg_name], channel_num));
            }
        }
    }
}

static void
var_write(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    int reg_name;
    size_t channel = 0;
    size_t offset = 0;
    int val;
    switch (cmd.size()) {
    case 4:
        reg_name = 2;
        val = 3;
        break;
    case 5:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        val = 4;
        break;
    case 6:
    default:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        val = 4;
        offset = get_value<size_t>(cmd[5]);
        break;
    }
    for (auto mod_num : mod_nums) {
        auto value = get_value<xia::pixie::param::value_type>(cmd[val]);
        crate[mod_num].write_var(cmd[reg_name], value, channel, offset);
    }
}

static void
var_read(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    bool is_module = false;
    int reg_name;
    size_t channel = 0;
    size_t offset = 0;
    switch (cmd.size()) {
    case 3:
        is_module = true;
        reg_name = 2;
        break;
    case 4:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        break;
    case 5:
    default:
        channel = get_value<size_t>(cmd[2]);
        reg_name = 3;
        offset = get_value<size_t>(cmd[4]);
        break;
    }
    for (auto mod_num : mod_nums) {
        if (cmd[reg_name] == "all") {
            if (is_module) {
                std::cout << "# module var read: " << mod_num
                          << ": " << cmd[reg_name] << std::endl;
                for (auto& var : crate[mod_num].module_var_descriptors) {
                    try {
                        output_value(var.name, crate[mod_num].read_var(var.par));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                std::cout << "# channel var read: " << mod_num << ':' << channel
                          << ": " << cmd[reg_name] << std::endl;
                for (auto& var : crate[mod_num].channel_var_descriptors) {
                    try {
                        output_value(var.name,
                                     crate[mod_num].read_var(var.par,
                                                             channel,
                                                             offset));
                    } catch (error& e) {
                        if (e.type != error::code::channel_param_disabled &&
                            e.type != error::code::channel_param_writeonly) {
                            throw;
                        }
                    }
                }
            }
        } else {
            output_value(cmd[reg_name],
                         crate[mod_num].read_var(cmd[reg_name],
                                                 channel,
                                                 offset));
        }
    }
}

static void
stats(xia::pixie::crate::crate& crate, options& cmd)
{
    auto mod_nums = get_values<size_t>(cmd[1], crate.num_modules);
    for (auto mod_num : mod_nums) {
        std::string stat;
        xia::pixie::channel::range channels;
        if (cmd.size() == 3) {
            stat = cmd[2];
            channels.resize(crate[mod_num].num_channels);
            xia::pixie::channel::range_set(channels);
        } else {
            channels.resize(1);
            channels[0] = get_value<size_t>(cmd[2]);
            stat = cmd[3];
        }
        xia::pixie::stats::stats stats(crate[mod_num].configs);
        crate[mod_num].read_stats(stats);
        if (stat == "pe") {
            std::cout << "module " << mod_num
                      << ": processed-events="
                      << stats.mod.processed_events()
                      << std::endl;
        } else if (stat == "icr") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": input-count-rate="
                          << stats.chans[channel].input_count_rate()
                          << std::endl;
            }
        } else if (stat == "ocr") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": output-count-rate="
                          << stats.chans[channel].output_count_rate()
                          << std::endl;
            }
        } else if (stat == "rt") {
            std::cout << "module " << mod_num
                      << ": real-time="
                      << stats.mod.real_time()
                      << std::endl;
        } else if (stat == "lt") {
            for (auto channel : channels) {
                std::cout << "module " << mod_num << " chan " << channel
                          << ": live-time="
                          << stats.chans[channel].live_time()
                          << std::endl;
            }
        }
    }
}

static void
import(xia::pixie::crate::crate& crate, options& cmd)
{
    xia::util::timepoint tp;
    xia::pixie::module::number_slots modules;
    tp.start();
    crate.import_config(cmd[1], modules);
    tp.end();
    std::cout << "Modules imported: " << modules.size()
              << " time=" << tp
              << std::endl;
}

static void
export_(xia::pixie::crate::crate& crate, options& cmd)
{
    xia::util::timepoint tp;
    tp.start();
    crate.export_config(cmd[1]);
    tp.end();
    std::cout << "Modules export time=" << tp
              << std::endl;
}

struct test_fifo_worker {
    size_t total;
    size_t seconds;

    test_fifo_worker();
    void worker(xia::pixie::module::module& module);
};

test_fifo_worker::test_fifo_worker()
    : total(0),
      seconds(0)
{
}

void
test_fifo_worker::worker(xia::pixie::module::module& module)
{
    try {
        module.start_test(xia::pixie::module::module::test::lm_fifo);
        const size_t poll_period_usecs = 100 * 1000;
        xia::util::timepoint duration(true);
        while (duration.msecs() < (seconds * 1000)) {
            size_t data_available = module.read_list_mode_level();
            if (data_available > 0) {
                xia::pixie::hw::words lm;
                module.read_list_mode(lm);
                total += lm.size();
            }
            if (data_available == 0) {
                xia::pixie::hw::wait(poll_period_usecs);
            }
        }
        module.end_test();
    } catch (...) {
        module.end_test();
        throw;
    }
}

static void
test(xia::pixie::crate::crate& crate, options& cmd)
{
    xia::pixie::module::module::test mode =
        xia::pixie::module::module::test::off;
    if (cmd[1] == "lmfifo") {
        mode = xia::pixie::module::module::test::lm_fifo;
    }
    if (mode == xia::pixie::module::module::test::off) {
        throw std::runtime_error(std::string("invalid test mode: " + cmd[1]));
    }
    auto mod_nums = get_values<size_t>(cmd[2], crate.num_modules);
    auto secs = get_value<size_t>(cmd[3]);
    auto tests = std::vector<test_fifo_worker>(mod_nums.size());
    set_seconds(tests, secs);
    module_threads(crate, mod_nums, tests, "fifo test error; see log");
    performance_stats(tests, secs);
}

static void
wait(xia::pixie::crate::crate& , options& cmd)
{
    auto msecs = get_value<size_t>(cmd[1]);
    std::cout << "waiting " << msecs << " msecs" << std::endl;
    xia::pixie::hw::wait(msecs * 1000);
}

static void
initialize(xia::pixie::crate::crate& crate, bool reg_trace)
{
    xia::util::timepoint tp;
    std::cout << "crate: initialize" << std::endl;
    tp.start();
    crate.initialize(reg_trace);
    tp.end();
    std::cout << "modules: detected=" << crate.modules.size()
              << " time=" << tp
              << std::endl;
}

static bool
process_command_sets(xia::pixie::crate::crate& crate, commands& cmds)
{
    for (auto& cmd : cmds) {
        for (const auto& handler : cmd_handlers) {
            if (handler.cmd == cmd[0]) {
                handler.func(crate, cmd);
                break;
            }
        }
    }
    return true;
}

void
load_crate_firmware(const std::string& file,
                    xia::pixie::firmware::crate& firmwares)
{
    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw std::runtime_error(
            std::string("crate firmware file open: ") + file +
            ": " + std::strerror(errno)
        );
    }
    for (std::string line; std::getline(input, line); ) {
        if (!line.empty()) {
            auto fw = xia::pixie::firmware::parse(line, ',');
            if (xia::pixie::firmware::check(firmwares, fw)) {
                std::string what("duplicate firmware option: ");
                what += line;
                throw std::runtime_error(what);
            }
            xia::pixie::firmware::add(firmwares, fw);
        }
    }
}

int
main(int argc, char* argv[])
{
    args::ArgumentParser parser("Tests C++ API");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args::Group option_group(parser, "Options");
    args::HelpFlag
        help(option_group,
             "help",
             "Display this help menu",
             {'h', "help"});
    args::Flag
        debug_flag(option_group,
                   "debug_flag",
                   "Enable debug log level",
                   {'d', "debug"}, false);
    args::Flag
        throw_unhandled_flag(option_group,
                             "throw_unhandled_flag",
                             "Throw an unhandled exception, it will detail the exception",
                             {'t', "throw-unhandled"}, false);
    args::Flag
        reg_trace(option_group,
                  "reg_trace",
                  "Registers debugging traces in the API.", {'R', "reg-trace"});
    args::Flag
        simulate(option_group,
                 "simulate",
                 "Simulate the crate and modules",
                 {'S', "simulate"}, false);
    args::ValueFlag<size_t>
        num_modules_flag(option_group,
                         "num_modules_flag",
                         "Number of modules to report",
                         {'n', "num-modules"}, 0);
    args::ValueFlagList<std::string>
        fw_file_flag(option_group,
                     "fw_file_flag",
                     "Firmware file(s) to load. Can be repeated. "
                     "Takes the form rev:mod-rev-num:type:name"
                     "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
                     {'F', "firmware"});
    args::ValueFlag<std::string>
        module_defs(option_group,
                    "module_file_flag",
                    "Crate simulation module definition file to load. "
                    "The file contains the module to simulate.",
                    {'M', "modules"});
    args::ValueFlagList<std::string>
        crate_file_flag(option_group,
                        "crate_file_flag",
                        "Crate firmware file to load. "
                        "The file contain the list of firmware files.",
                        {'C', "crate"});
    args::ValueFlag<std::string>
        log_file_flag(option_group,
                      "log_file_flag",
                      "Log file. Use `stdout` for the console.",
                      {'l', "log"});
    args::ValueFlagList<std::string>
        slot_map_flag(option_group,
                      "slot_map_flag",
                      "A list of slots used to define the slot to index mapping.",
                      {'s', "slot_map"});
    args::ValueFlag<std::string>
        cmd_file_flag(option_group,
                      "cmd_file_flag",
                      "Command file to execue.",
                      {'c', "cmd"});

    args::Group command_group(parser, "Commands");
    args::PositionalList<std::string>
        cmd_flag(command_group,
                 "commands",
                 "Commands to be performed in order. "
                 "The command `help` lists available command.");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << help.what() << std::endl;
        std::cout << parser;
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << e.what() << std::endl;
        std::cout << parser;
        return EXIT_FAILURE;
    }

    try {
        xia::util::timepoint run;
        run.start();

        std::string log;
        if (log_file_flag) {
            log = args::get(log_file_flag);
        } else {
            log = "test-api-log.txt";
        }

        auto log_level = xia_log::info;
        if (args::get(debug_flag)) {
            log_level = xia_log::debug;
        }
        xia::logging::start("log", log, log_level, false);

        size_t num_modules = args::get(num_modules_flag);

        xia::pixie::crate::crate crate_hw;
        xia::pixie::sim::crate crate_sim;

        xia::pixie::crate::crate* crate_selection = &crate_hw;

        if (simulate) {
            if (!module_defs) {
                throw
                    std::runtime_error("simulation requires a module definition");
            }
            xia_log(xia_log::info) << "simulation: " << args::get(module_defs);
            xia::pixie::sim::load_module_defs(args::get(module_defs));
            crate_selection = &crate_sim;
        }

        xia::pixie::crate::crate& crate = *crate_selection;

        if (fw_file_flag) {
            for (const auto& firmware: args::get(fw_file_flag)) {
                auto fw = xia::pixie::firmware::parse(firmware, ':');
                if (xia::pixie::firmware::check(crate.firmware, fw)) {
                    std::string what("duplicate firmware: ");
                    what += firmware;
                    throw std::runtime_error(what);
                }
                xia::pixie::firmware::add(crate.firmware, fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& firmware: args::get(crate_file_flag)) {
                load_crate_firmware(firmware, crate.firmware);
            }
        }

        initialize(crate, reg_trace);

        if (num_modules != 0 && crate.num_modules != num_modules) {
            throw std::runtime_error("invalid number of modules detected: " \
                                     "found " +
                                     std::to_string(crate.num_modules));
        }

        if (slot_map_flag) {
            xia::pixie::module::number_slots slot_map;
            for (const auto& slot : args::get(slot_map_flag)) {
                std::vector<size_t> slots =
                    get_values<size_t>(slot, crate.num_modules);
                for (auto s : slots) {
                    slot_map.emplace_back(std::make_pair(slot_map.size(), s));
                }
            }
            crate.assign(slot_map);
        }

        std::cout << "modules: online=" << crate.modules.size()
                  << " offline=" << crate.offline.size()
                  << std::endl;

        crate.set_firmware();
        crate.probe();

        commands cmds;

        if (cmd_file_flag) {
            if (!make_command_sets(args::get(cmd_file_flag), cmds)) {
                return EXIT_FAILURE;
            }
        }
        if (cmd_flag) {
            if (!make_command_sets(cmd_flag, cmds)) {
                return EXIT_FAILURE;
            }
        }

        if (!process_command_sets(crate, cmds)) {
            return EXIT_FAILURE;
        }

        run.end();
        std::cout << "run time=" << run << std::endl;
    } catch (xia::pixie::error::error& e) {
        xia_log(xia_log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia_log::error) << "unknown error: " << e.what();
        std::cerr <<  "error: unknown error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        xia_log(xia_log::error) << "unknown error: unhandled exception";
        std::cerr <<  "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
