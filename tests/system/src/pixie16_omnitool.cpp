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

/**
 * @brief A C++ Tool for Pixie16
 */

#include <algorithm>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>

#include <pixie/config.hpp>
#include <pixie/log.hpp>
#include <pixie/util.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/legacy.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/sim.hpp>

#include <pixie/pixie16/fpga_comms.hpp>
#include <pixie/pixie16/fpga_fippi.hpp>

#include <args/args.hxx>
#include <nolhmann/json.hpp>

/*
 * Localize the log and error
 */
using error = xia::pixie::error::error;

/*
 * JSON support
 */
using json = nlohmann::json;

/*
 * Process command line options
 */
using slot_range = std::vector<int>;
using module_range = std::vector<size_t>;
using channel_range = xia::pixie::channel::range;
using files = std::vector<std::string>;
using paths = std::vector<std::string>;

struct process_options {
    size_t num_modules;
    slot_range slots;
    std::string firmware_host_path;
    files firmware_crate_files;
    files firmware_files;
    bool reg_trace;
    bool verbose;
    std::ostream& out;

    process_options(std::ostream& out);
};

/**
 * Command context
 */
struct command;
struct command_context {
    xia::pixie::crate::module_crate& crate;
    process_options& opts;
    command& cmd;
    command_context(
        xia::pixie::crate::module_crate& crate_,
        process_options& opts_,
        command& cmd_) : crate(crate_), opts(opts_), cmd(cmd_) {}
};

/*
 * Module threading worker base
 */
struct module_thread_worker {
    int number;
    int slot;
    int pci_bus;
    int pci_slot;

    std::atomic_bool running;
    bool has_error;
    xia::util::timepoint period;
    size_t total;
    size_t last_total;

    module_thread_worker();

    virtual void worker(
        command_context& context,
        xia::pixie::module::module& module) = 0;
};

/*
 * Commands.
 */

/**
 * @brief Command Definition defines a command that runs a function
 *        when executed
 */
struct command_definition {
    using argument = std::string;
    using arguments = std::vector<argument>;
    using arguments_iter = arguments::iterator;
    using option = std::pair<argument, argument>;
    using options = std::vector<option>;
    using operation = argument;
    using operations = arguments;
    using handler = void (*)(command_context& context);

    std::string group;
    std::string name;

    handler call;
    operations boot;

    size_t min_args;
    size_t max_args;
    size_t forced;
    options opts;

    std::string help_cmd;
    std::string help;

    command_definition() = default;

    std::string formatted_help(const bool long_opt, const size_t max) const;
};

using command_definitions = std::vector<command_definition>;

/**
 * @brief A command is an instance of the command to executed
 */
struct command {
    using argument = command_definition::argument;
    using arguments = command_definition::arguments;
    using arguments_iter = command_definition::arguments_iter;
    using option = command_definition::option;
    using options = command_definition::options;

    const command_definition& def;
    options opts;
    arguments args;

    command(const command_definition& cmd);

    /**
     * @brief Parse the options and arguments from the list of arguments
     */
    void parse(arguments_iter& ai, arguments_iter& ae);

    /**
     * @brief Get the argument and return an empty string is the user
     * did not supply one.
     */
    std::string get_arg();

    /**
     * @brief Is there an argument available?
     */
    bool has_arg() const;

    /**
     * @brief Get the option if valid
     */
    std::string get_option(const argument& opt) const;

    /**
     * @brief Is the option true?
     */
    bool option_true(const std::string& opt) const;

    /**
     * @brief Run the command
     */
    void run(command_context& context);
};

using commands = std::vector<command>;

/**
 * @brief A batch of commands are executed in sequence
 */
struct command_batch {
    using operation_func = std::function<void(command_context& context)>;
    struct operation {
        bool run_once;
        operation_func handler;
        operation(operation_func& handler_);
        operation();
        void run(command_context& context);
    };
    using operations = std::map<std::string, operation>;

    commands cmds;
    paths path;

    operations ops;

    command_batch();

    /**
     * @brief Set a operation
     */
    void set_operation(const std::string& name, operation_func func);

    /**
     * @brief Parse a set of arguments into commands.
     *
     * This valids some basics of the commands before executing
     * them. It checks options are valid and argument counts are
     * within range.
     */
    void parse(command::arguments& cmds);

    /**
     * @brief Execute the batch of commands
     */
    void execute(xia::pixie::crate::module_crate& crate, process_options& opts);

    /**
     * @brief Run the operation
     */
    void run_operation(const std::string& name, command_context& context);

    /**
     * @brief Report the parsed commands
     */
    void report(std::ostream& out);
};

#define command_handler_decl(_name) static void _name(command_context& context)
#define command_opt_decl(_name) {"-" _name, ""}
#define command_opt_arg_decl(_name) {"-" _name, "true"}

/*
 * Process command arguments.
 */
using args_parser = args::ArgumentParser;
using args_group = args::Group;
using args_help_flag = args::HelpFlag;
using args_flag = args::Flag;
using args_int_flag = args::ValueFlag<int>;
using args_size_flag = args::ValueFlag<size_t>;
using args_string_flag = args::ValueFlag<std::string>;
using args_strings_flag = args::ValueFlagList<std::string>;
using args_positional_list = args::PositionalList<command::argument>;

/*
 * The commands are sorted alphabetical
 */

command_handler_decl(boot);
static const command_definition boot_cmd = {
    "Crate", "/crate/boot", boot,
    {"init", "probe"},
    0, 4, 0,
    {},
    "[modules(s) [comms] [fippi] [dsp]]",
    "Boots the module(s)"
};

command_handler_decl(crate_report);
static const command_definition crate_cmd = {
    "Crate", "/crate/crate", crate_report,
    {"init", "probe"},
    0, 0, 0,
    {},
    "",
    "Report the crate"
};

command_handler_decl(export_);
static const command_definition export_cmd = {
    "Crate", "/crate/export", export_,
    {"init", "probe"},
    1, 1, 0,
    {},
    "file",
    "Export a configuration to a JSON file"
};

command_handler_decl(fw_report);
static const command_definition fw_report_cmd = {
    "Crate", "/crate/firmware/report", fw_report,
    {"init", "probe"},
    1, 1, 0,
    {},
    "file",
    "Report on the system's firmware"
};

command_handler_decl(import);
static const command_definition import_cmd = {
    "Crate", "/crate/import", import,
    {"init", "probe"},
    1, 1, 0,
    {},
    "file",
    "Import a JSON configuration file"
};

command_handler_decl(report);
static const command_definition report_cmd = {
    "Crate", "/crate/report", report,
    {"init", "probe"},
    1, 1, 0,
    {},
    "file",
    "Report the crate's configuration"
};

static const command_definitions crate_commands = {
    boot_cmd,
    crate_cmd,
    export_cmd,
    fw_report_cmd,
    import_cmd,
    report_cmd,
};

command_handler_decl(adc_acq);
static const command_definition adc_acq_cmd = {
    "Module", "/module/adc-acq", adc_acq,
    {"init", "probe"},
    0, 1, 0,
    {},
    "[modules(s)]",
    "Acquire a module's ADC trace"
};

command_handler_decl(adc_save);
static const command_definition adc_save_cmd = {
    "Module", "/module/adc-save", adc_save,
    {"init", "probe"},
    0, 3, 0,
    {},
    "[modules(s) [channel(s) [length]]]",
    "Save a module's ADC trace to a file"
};

command_handler_decl(adj_off);
static const command_definition adj_off_cmd = {
    "Module", "/module/adj-off", adj_off,
    {"init", "probe"},
    0, 1, 0,
    {},
    "[modules(s)]",
    "Adjust the module's offsets"
};

command_handler_decl(bl_acq);
static const command_definition bl_acq_cmd = {
    "Module", "/module/bl-acq", bl_acq,
    {"init", "probe"},
    0, 1, 0,
    {},
    "[module(s)]",
    "Acquire module baselines"
};

command_handler_decl(bl_save);
static const command_definition bl_save_cmd = {
    "Module", "/module/bl-save", bl_save,
    {"init", "probe"},
    0, 2, 0,
    {},
    "[module(s) [channel(s)]]",
    "Save the module's baselines"
};

command_handler_decl(db);
static const command_definition db_cmd = {
    "Module", "/module/db", db,
    {"init", "probe"},
    1, 3, 0,
    {},
    "module(s) [channel(s)] [disable-swap]]",
    "Daughter board control"
};

command_handler_decl(hist_resume);
static const command_definition hist_resume_cmd = {
    "Module", "/module/hist-resume", hist_resume,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Resume module histograms"
};

command_handler_decl(hist_save);
static const command_definition hist_save_cmd = {
    "Module", "/module/hist-save", hist_save,
    {"init", "probe"},
    1, 2, 0,
    {command_opt_arg_decl("b")},
    "[-b bins] module(s) [channel(s)]",
    "Save a module's histogram to a file"
};

command_handler_decl(hist_start);
static const command_definition hist_start_cmd = {
    "Module", "/module/hist-start", hist_start,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Start module histograms"
};

command_handler_decl(list_mode);
static const command_definition list_mode_cmd = {
    "Module", "/module/list-mode", list_mode,
    {"init", "probe"},
    3, 3, 0,
    {},
    "module(s) secs file",
    "Run list mode saving the data to a file"
};

command_handler_decl(list_resume);
static const command_definition list_resume_cmd = {
    "Module", "/module/list-resume", list_resume,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Resume module list mode",
};

command_handler_decl(list_save);
static const command_definition list_save_cmd = {
    "Module", "/module/list-save", list_save,
    {"init", "probe"},
    3, 3, 0,
    {},
    "module(s) secs file",
    "Save a module's list-mode data to a file"
};

command_handler_decl(list_start);
static const command_definition list_start_cmd = {
    "Module", "/module/list-start", list_start,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Start module list mode"
};

command_handler_decl(lset_import);
static const command_definition lset_import_cmd = {
    "Module", "/module/lset-import", lset_import,
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) file [flush/sync]",
    "Import a legacy settings file to a module"
};

command_handler_decl(lset_load);
static const command_definition lset_load_cmd = {
    "Module", "/module/lset-load", lset_load,
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) file [flush/sync]",
    "Load a legacy settings file to a modul's DSP memory"
};

command_handler_decl(lset_report);
static const command_definition lset_report_cmd = {
    "Module", "/module/lset-report", lset_report,
    {"init", "probe"},
    2, 2, 0,
    {},
    "module(s) file",
    "Output a legacy settings fie in a readable format"
};

command_handler_decl(mod_offline);
static const command_definition mod_offline_cmd = {
    "Module", "/module/mod-offline", mod_offline,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Set a module offline"
};

command_handler_decl(mod_online);
static const command_definition mod_online_cmd = {
    "Module", "/module/mod-online", mod_online,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Set a module online"
};

command_handler_decl(override_fw);
static const command_definition override_fw_cmd = {
    "Module", "/module/firmware/override", override_fw,
    {"init", "probe"},
    2, 2, 0,
    {},
    "module fwfile",
    "Override a firmware file for a module"
};

command_handler_decl(par_read);
static const command_definition par_read_cmd = {
    "Module", "/module/par-read", par_read,
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) [channel(s)] param",
    "Read module/channel parameter"
};

command_handler_decl(par_write);
static const command_definition par_write_cmd = {
    "Module", "/module/par-write", par_write,
    {"init", "probe"},
    3, 4, 0,
    {},
    "module(s) [channel(s)] param value",
    "Write module/channel parameter"
};

command_handler_decl(reg_read);
static const command_definition reg_read_cmd = {
    "Module", "/module/reg-read", reg_read,
    {"init"},
    2, 2, 0,
    {command_opt_decl("s"), command_opt_decl("x")},
    "[-s] [-x] module/slot address/name/memory:name",
    "Read from a register in a module or slot (-s) memory address"
};

command_handler_decl(reg_write);
static const command_definition reg_write_cmd = {
    "Module", "/module/reg-write", reg_write,
    {"init"},
    3, 3, 0,
    {command_opt_decl("s")},
    "[-s] module/slot address/name/memory:name [value]",
    "Write to a register in a module or slot (-s) memory address"
};

command_handler_decl(run_active);
static const command_definition run_active_cmd = {
    "Module", "/module/run-active", run_active,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Does the module have an active run?"
};

command_handler_decl(run_end);
static const command_definition run_end_cmd = {
    "Module", "/module/run-end", run_end,
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "End module run(s)"
};

command_handler_decl(set_dacs);
static const command_definition set_dacs_cmd = {
    "Module", "/module/set-dacs", set_dacs,
    {"init", "probe"},
    1, 1, 0,
    {},
    "modules(s)",
    "Set the module's DAC"
};

command_handler_decl(stats);
static const command_definition stats_cmd = {
    "Module", "/module/stats", stats,
    {"init", "probe"},
    1, 2, 0,
    {command_opt_arg_decl("s")},
    "[-s stat (pe/ocr/rt/lt)] module(s) [channel(s)]",
    "module channel stats"
};

command_handler_decl(stats_rpt);
static const command_definition stats_rpt_cmd = {
    "Module", "/module/stats-rpt", stats_rpt,
    {"init", "probe"},
    2, 2, 0,
    {},
    "module(s) filename",
    "module channel stats"
};

command_handler_decl(test);
static const command_definition test_cmd = {
    "Module", "/module/test", test,
    {"init", "probe"},
    1, 1, 0,
    {command_opt_arg_decl("m")},
    "[-m mode (off/lmfifo)] module(s)",
    "Test control, default mode is 'off'"
};

command_handler_decl(var_read);
static const command_definition var_read_cmd = {
    "Module", "/module/var-read", var_read,
    {"init", "probe"},
    2, 4, 0,
    {},
    "module(s) [channel(s)] param [offset(s)]",
    "Read module/channel variable. A channel references a channel variable."
};

command_handler_decl(var_write);
static const command_definition var_write_cmd = {
    "Module", "/module/var-write", var_write,
    {"init", "probe"},
    3, 5, 0,
    {},
    "module(s) [channel(s)] param [offset(s)] value",
    "Write module/channel variable. A channel references a channel variable."
};

static const command_definitions module_commands = {
    adc_acq_cmd,
    adc_save_cmd,
    adj_off_cmd,
    bl_acq_cmd,
    bl_save_cmd,
    db_cmd,
    hist_resume_cmd,
    hist_save_cmd,
    hist_start_cmd,
    list_mode_cmd,
    list_resume_cmd,
    list_save_cmd,
    list_start_cmd,
    lset_import_cmd,
    lset_load_cmd,
    lset_report_cmd,
    mod_offline_cmd,
    mod_online_cmd,
    override_fw_cmd,
    par_read_cmd,
    par_write_cmd,
    reg_read_cmd,
    reg_write_cmd,
    run_active_cmd,
    run_end_cmd,
    set_dacs_cmd,
    stats_cmd,
    stats_rpt_cmd,
    test_cmd,
    var_read_cmd,
    var_write_cmd
};

command_handler_decl(help);
static const command_definition help_cmd = {
    "Utilities", "/util/help", help,
    {"none"},
    0, 1, 1,
    {command_opt_decl("l")},
    "[-l] [command]",
    "Command specific help. Add '-l' to list all commands"
};

command_handler_decl(wait);
static const command_definition wait_cmd = {
    "Utilities", "/util/wait", wait,
    {"none"},
    1, 1, 0,
    {},
    "msecs",
    "wait a number of msecs; add 's' for seconds and 'm' for minutes"
};

static const command_definitions util_commands = {
    help_cmd,
    wait_cmd
};

/*
 * Commands are registered at start up
 */
static command_definitions ominitool_commands;

/*
 * Command group labels.
 */
using command_group = std::vector<std::string>;

static command_group command_groups;

static std::string adc_prefix = "pixie16-omnitool-adc";
static std::string histogram_prefix = "pixie16-omnitool-mca";
static std::string baseline_prefix = "pixie16-omnitool-baseline";
static std::string list_mode_prefix = "pixie16-omnitool-list-mode";

static const json hardware = {
    {"memory",
     {{"sys",
       {{"cfg_datacs", 0x00},
        {"cfg_ctrlcs", 0x04},
        {"cfg_rdcs", 0x08},
        {"i2cm24c64", 0x10},
        {"pcf8574", 0x10},
        {"cfg_dcmrst", 0x14},
        {"cfg_dac", 0x18},
        {"csr", 0x48},
        {"request_hbr", 0x80},
        {"hbr_done", 0x84},
        {"wrt_ext_mem", 0xc0},
        {"ext_mem_test", 0xc4},
        {"wrt_dsp_mma", 0xc8},
        {"set_ext_fifo", 0xcc},
        {"wrt_dsp_dmac11", 0xd0},
        {"wrt_dsp_ii11", 0xd4},
        {"wrt_clr_extmem", 0xd8},
        {"wrt_dsp_c11", 0xdc},
        {"wrt_dsp_im11", 0xe0},
        {"wrt_dsp_ec11", 0xe4},
        {"rd_wrt_fifo_wml", 0xe8},
        {"set_exmem_fifo", 0xec},
        {"set_int_fifo", 0xf0},
        {"wrt_reset_rfcnt", 0xf4}}},
      {"fippi",
       {{"csrin", 0x00800400},
        {"coincpattern", 0x00800402},
        {"adcctrl", 0x00800404},
        {"hostclr", 0x00800408},
        {"adcspi", 0x0080040a},
        {"bitslip", 0x0080040c},
        {"hdr_ids", 0x00800412},
        {"adcfifoctrl", 0x00800418}}},
      {"dsp", {}},
      {"mca", {}},
      {"fifo", {}}}},
    {"bits",
     {{"hbr",
       {{"hdr_hbr", 0},
        {"hbr_dsp_ncs", 1},
        {"hbr_auto_inc", 2},
        {"hbr_read_again_read", 3}}},
      {"csr",
       {{"runena", 0},
        {"dspdownload", 1},
        {"pciactive", 2},
        {"pullup_ctrl", 3},
        {"dspreset", 4},
        {"extfifo_wml", 6},
        {"runactive", 13},
        {"clrextmem_active", 15},
        {"modcsrb_cpldpullup", 0},
        {"modcsrb_dirmod", 4},
        {"modcsrb_chassismaster", 6},
        {"cpldcsr_bpconnect", 12},
        {"cpldcsr_pullup", 13}}},
      {"cgancsra",
       {{"ccsra_ftrigsel", 0},
        {"ccsra_exttrigsel", 1},
        {"ccsra_good", 2},
        {"ccsra_chantrigsel", 3},
        {"ccsra_syncdataacq", 4},
        {"ccsra_polarity", 5},
        {"ccsra_vetoena", 6},
        {"ccsra_histoe", 7},
        {"ccsra_traceena", 8},
        {"ccsra_qdcena", 9},
        {"ccsra_cfdmode", 10},
        {"ccsra_globtrig", 11},
        {"ccsra_esumsena", 12},
        {"ccsra_chantrig", 13},
        {"ccsra_enarelay", 14}}}}}
};

void ominitool_register_commands(const command_definitions& cmds) {
    for (auto& cmd : cmds) {
        auto fi = std::find(command_groups.begin(), command_groups.end(), cmd.group);
        if (fi == command_groups.end()) {
            command_groups.push_back(cmd.group);
        }
        ominitool_commands.push_back(cmd);
    }
    std::sort(command_groups.begin(), command_groups.end());
    std::sort(
        ominitool_commands.begin(), ominitool_commands.end(), [](auto& a, auto& b) {
        return a.name < b.name;
    });
}

static bool starts_with(const std::string& s1, const std::string& s2) {
    return s2.size() <= s1.size() && s1.compare(0, s2.size(), s2) == 0;
}

static void string_replace(
    std::string& target, const char find, const char replace) {
    while (true) {
        auto pos = target.find_first_of(find);
        if (pos == std::string::npos) {
            break;
        }
        target.replace(pos, 1, 1, replace);
    }
}

static void tolower(std::string& s) {
    std::transform(
        s.cbegin(), s.cend(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

static bool check_number(const std::string& opt) {
    return std::regex_match(
        opt, std::regex(("((\\+|-)?[0-9]+)(.(([0-9]+)?))*")));
}

template<typename T>
static T get_value(const std::string& opt) {
    T value = 0;
    try {
        if (std::is_same<T, double>::value) {
            value = T(std::stod(opt, nullptr));
        } else {
            value = T(std::stoul(opt, nullptr, 0));
        }
    } catch (std::invalid_argument& ) {
        throw std::runtime_error("invalid number: " + opt);
    } catch (std::out_of_range& ) {
        throw std::runtime_error("number out of range: " + opt);
    }
    return value;
}

template<typename T>
static std::vector<T> get_values(
    const std::string& opt, const size_t max_count = 0, bool no_error = false) {
    std::vector<T> values;
    if (opt == "all") {
        if (max_count == 0) {
            throw std::runtime_error("range `all` invalid, max count is unknown");
        }
        values.resize(max_count);
        std::iota(values.begin(), values.end(), 0);
    } else {
        xia::util::strings sc;
        xia::util::split(sc, opt, ',');
        for (auto& slots : sc) {
            xia::util::strings sd;
            xia::util::split(sd, slots, '-');
            if (sd.size() == 1) {
                T val = get_value<T>(sd[0]);
                if (val < T(max_count)) {
                    values.push_back(val);
                } else if (!no_error) {
                    throw std::runtime_error(
                        "value out of range: " + std::to_string(val));
                }
            } else if (sd.size() == 2) {
                size_t start = get_value<T>(sd[0]);
                size_t end = get_value<T>(sd[1]);
                if (start > end) {
                    if (!no_error) {
                        throw std::runtime_error("invalid range: " + opt);
                    }
                    values.clear();
                    break;
                }
                if (start < max_count && end < max_count) {
                  for (T s = T(start); s <= T(end); ++s) {
                        values.push_back(s);
                    }
                } else if (!no_error) {
                    auto val = start < max_count ? end : start;
                    throw std::runtime_error(
                        "value out of range: " + std::to_string(val));
                }
            } else {
                if (!no_error) {
                    throw std::runtime_error("invalid range: " + opt);
                }
                values.clear();
                break;
            }
        }
    }
    return values;
}

static command_definitions::const_iterator no_command() {
    return ominitool_commands.end();
}

static bool valid_command(command_definitions::const_iterator cdi) {
    return cdi != no_command();
}

static command_definitions::const_iterator find_command(const command::argument& arg) {
    for (command_definitions::const_iterator cdi = ominitool_commands.begin();
         cdi != ominitool_commands.end();
         ++cdi) {
        const auto& def = *cdi;
        if (def.name == arg) {
            return cdi;
        }
    }
    return no_command();
}

static command_definitions::const_iterator find_command(
    const command::argument& arg, paths& path) {
    auto ci = find_command(arg);
    if (valid_command(ci)) {
        return ci;
    }
    for (auto& dir : path) {
        auto dci = find_command(dir + '/' + arg);
        if (valid_command(dci)) {
            return dci;
        }
    }
    return no_command();
}

static void channels_option(
    channel_range& channels, const command::argument& opt, size_t num_channels) {
    if (opt.empty()) {
        channels.resize(num_channels);
        xia::pixie::channel::range_set(channels);
    } else {
        auto chans = get_values<size_t>(opt, num_channels);
        for (auto c : chans) {
            channels.push_back(c);
        }
    }
}

static void modules_option(
    module_range& modules, const command::argument& opt, size_t num_modules) {
    if (opt.empty()) {
        modules.resize(num_modules);
        std::iota(modules.begin(), modules.end(), 0);
    } else {
        modules = get_values<size_t>(opt, num_modules);
    }
}

process_options::process_options(std::ostream& out_)
    : num_modules(-1), reg_trace(false), verbose(false), out(out_) {}

std::string command_definition::formatted_help(const bool long_opt, const size_t max) const {
    std::ostringstream oss;
    if (long_opt) {
        oss << ' '<< std::left << name << ": "
            << std::endl
            << "  " << help << std::endl;
        if (min_args == max_args) {
            if (min_args == 0) {
                oss << "   - no arguments";
            } else {
                oss << "   - requires " << min_args << " argument";
                if (min_args > 1) {
                    oss << 's';
                }
            }
        } else if (min_args < max_args) {
            oss << "   - requires " << min_args << " to " << max_args << " arguments ";
        } else {
            throw std::runtime_error("invalid command args range: " + name);
        }
        oss << std::endl
            << "     # " << name << ' ' << help_cmd << std::endl;
    } else {
        oss << ' ' << std::left << std::setw(max + 1) << name
            << " - " << help
            << std::endl;
    }
    return oss.str();
}

command_batch::operation::operation() : run_once(false) {}

command_batch::operation::operation(operation_func& handler_) : run_once(false), handler(handler_) {}

void command_batch::operation::run(command_context& context) {
    if (!run_once) {
        run_once = true;
        handler(context);
    }
}

command::command(const command_definition& cmd) : def(cmd) {}

void command::parse(command::arguments_iter& ai, command::arguments_iter& ae) {
    while (ai != ae) {
        auto& arg = *ai;
        if (arg[0] != '-') {
            break;
        }
        ++ai;
        auto is_arg_option = [&arg](auto& o) {
            return starts_with(arg, std::get<0>(o));
        };
        auto doi = std::find_if(def.opts.begin(), def.opts.end(), is_arg_option);
        if (doi == def.opts.end()) {
            throw std::runtime_error(
                "option (" + arg + ") argument not found: " + def.name);
        }
        auto& opt_switch = std::get<0>(*doi);
        auto opt_arg = !std::get<1>(*doi).empty();
        auto is_option = [&opt_switch](auto& o) {
            return opt_switch == std::get<0>(o);
        };
        auto oi = std::find_if(opts.begin(), opts.end(), is_option);
        if (oi != opts.end()) {
            throw std::runtime_error(
                "command option used more than once: " +
                opt_switch + " (" + def.name + ')');
        }
        if (opt_arg) {
            if (ai == ae || (*ai)[0] == '-') {
                throw std::runtime_error(
                    "option (" + opt_switch + ") argument not found: " + def.name);
            }
            opts.push_back({opt_switch, *ai++});
        } else {
            opts.push_back({arg, "true"});
        }
    }
    while (ai != ae) {
        auto& arg = *ai++;
        if (arg[0] == '-') {
            throw std::runtime_error("command option after argument: " + def.name);
        } else {
            args.push_back(arg);
        }
    }
    if (args.size() < def.min_args) {
        throw std::runtime_error("not enough arguments: " + def.name);
    }
    if (args.size() > def.max_args) {
        throw std::runtime_error("too many arguments: " + def.name);
    }
    while (args.size() < def.max_args) {
        args.push_back({});
    }
}

std::string command::get_arg() {
    if (args.empty()) {
        throw std::runtime_error("no arguments to get: " + def.name);
    }
    std::string arg = args.front();
    args.erase(args.begin());
    return arg;
}

bool command::has_arg() const {
    return !args.empty();
}

std::string command::get_option(const command::argument& opt) const {
    auto is_option = [&opt](auto& o) {
        return opt == std::get<0>(o);
    };
    auto oi = std::find_if(opts.begin(), opts.end(), is_option);
    if (oi == opts.end()) {
        auto odef = std::find_if(def.opts.begin(), def.opts.end(), is_option);
        if (odef == def.opts.end()) {
            throw std::runtime_error("invalid option to get: " + opt + " (" + def.name + ')');
        }
    }
    std::string opt_arg;
    if (oi != opts.end()) {
        opt_arg = std::get<1>(*oi);
    }
    return opt_arg;
}

bool command::option_true(const std::string& opt) const {
    return opt == "true";
}

void command::run(command_context& context) {
    def.call(context);
}

command_batch::command_batch() {
    set_operation("none", [](command_context& ){});
}

void command_batch::set_operation(const std::string& name, operation_func func) {
    auto check = ops.find(name);
    if (check != ops.end()) {
        throw std::runtime_error("operation aleady registered: " + name);
    }
    ops[name] = operation({func});
}

void command_batch::parse(command::arguments& args) {
    auto ai = args.begin();
    while (ai != args.end()) {
        auto the_cmd = find_command(*ai++, path);
        if (the_cmd == no_command()) {
            throw std::runtime_error("invalid command: " + *ai);
        }
        command cmd(*the_cmd);
        size_t forced = cmd.def.forced;
        auto ae = ai;
        while (ae != args.end()) {
            if (forced == 0) {
                auto next_cmd = find_command(*ae, path);
                if (next_cmd != no_command()) {
                    break;
                }
            } else {
                --forced;
            }
            ++ae;
        }
        cmd.parse(ai, ae);
        cmds.push_back(cmd);
    }
}

void command_batch::execute(
    xia::pixie::crate::module_crate& crate, process_options& opts) {
    for (auto& cmd : cmds) {
        command_context context(crate, opts, cmd);
        for (auto& op : cmd.def.boot) {
            run_operation(op, context);
        }
        cmd.run(context);
    }
}

void command_batch::run_operation(const std::string& name, command_context& context) {
    auto opi = ops.find(name);
    if (opi == ops.end()) {
        throw std::runtime_error("invalid operation: " + name);
    }
    auto& op = std::get<1>(*opi);
    op.run(context);
}

void command_batch::report(std::ostream& out) {
    out << "Total: " << cmds.size() << std::endl;
    size_t count = 0;
    for (auto& cmd : cmds) {
        out << std::setw(4) << ++count
            << ' ' << cmd.def.name
            << " (" << cmd.def.group << ')' << std::endl
            << "      args: total: " << cmd.args.size()
            << " (max=" << cmd.def.max_args
            << ", min=" << cmd.def.min_args
            << ", forced=" << cmd.def.forced << ')' << std::endl;
        size_t arg_count = 0;
        for (auto& arg : cmd.args) {
            out << "      " << std::setw(2) << ++arg_count << ": ";
            if (arg.empty()) {
                out << "  <empty>";
            } else {
                out << arg;
            }
            out << std::endl;
        }
        out << "      options: total: " << cmd.opts.size() << std::endl;
        size_t opt_count = 0;
        for (auto opt : cmd.opts) {
            out << "      " << std::setw(2) << ++opt_count
                << ": " << std::get<0>(opt) << ' ' << std::get<1>(opt) << std::endl;
        }
    }
}

void load_crate_firmwares(
    const std::string& file, xia::pixie::firmware::crate& fws) {
    std::ifstream input(file, std::ios::in | std::ios::binary);
    if (!input) {
        throw std::runtime_error(std::string("crate firmware file open: ") + file + ": " +
                                 std::strerror(errno));
    }
    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            auto fw = xia::pixie::firmware::parse(line, ',');
            if (xia::pixie::firmware::check(fws, fw)) {
                std::string what("duplicate firmware option: ");
                what += line;
                throw std::runtime_error(what);
            }
            xia::pixie::firmware::add(fws, fw);
        }
    }
}

static void firmware_load(command_context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    /*
     * Load command line first, then crate files and then fill in any
     * gaps with the host files.
     */
    for (auto& fwfile : opts.firmware_files) {
        auto fw = xia::pixie::firmware::parse(fwfile, ':');
        if (xia::pixie::firmware::check(crate->firmware, fw)) {
            std::string what("duplicate firmware on command line: ");
            what += fwfile;
            throw std::runtime_error(what);
        }
        xia::pixie::firmware::add(crate->firmware, fw);
    }
    for (auto& fwfile : opts.firmware_crate_files) {
        load_crate_firmwares(fwfile, crate->firmware);
    }
    load_firmwares(crate->firmware, opts.firmware_host_path.c_str());
}

static void initialize(command_context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    xia::util::timepoint tp;
    if (opts.verbose) {
        opts.out << "crate: initialize" << std::endl;
        tp.start();
    }
    crate->initialize(opts.reg_trace);
    if (opts.verbose) {
        tp.end();
        opts.out << "modules: detected=" << crate.num_modules
                 << " time=" << tp << std::endl;
    }
    if (opts.num_modules != 0 &&
        crate.num_modules != opts.num_modules) {
        throw std::runtime_error("invalid number of modules detected: "
                                 "found " +
                                 std::to_string(crate.num_modules));
    }
    if (!opts.slots.empty()) {
        xia::pixie::module::number_slots slot_map;
        for (auto s : opts.slots) {
            slot_map.emplace_back(std::make_pair(int(slot_map.size()), s));
        }
        crate.assign(slot_map);
    }
}

static void probe(command_context& context) {
    auto& crate = context.crate;
    auto& opts = context.opts;
    crate->set_firmware();
    crate->probe();
    if (opts.verbose) {
        opts.out << "modules: slots=" << crate->num_slots
                 << " present=" << crate->num_present
                 << " online=" << crate->num_online
                 << " offline=" << crate->num_offline << std::endl
                 << "module: assignment map: " << crate.format_module_map()
                 << std::endl;
    }
}

static void load_commands(const std::string& name, command::arguments& cmds) {
    std::ifstream in(name);
    if (!in) {
        throw std::runtime_error(
            std::string("command file open: ") + name + ": " +
            std::strerror(errno));
    }
    std::string line;
    while (getline(in, line)) {
        string_replace(line, '\r', ' ');
        string_replace(line, '\t', ' ');
        xia::util::strings sc;
        xia::util::split(sc, line);
        if (!sc.empty()) {
            cmds.insert(std::end(cmds), std::begin(sc), std::end(sc));
        }
    }
}

static void help_output(std::ostream& out) {
    auto ci = find_command("/util/help");
    if (valid_command(ci)) {
        const command_definition& cmd = *ci;
        out << "  COMMANDS:" << std::endl;
        out << "      " << cmd.name << " - " << cmd.help << std::endl
            << "        eg '-- help -l'" << std::endl
            << std::endl;
    } else {
        out << "  No help found; please report to XIA" << std::endl;
    }
}

static void module_check(xia::pixie::crate::module_crate& crate, std::vector<size_t> mod_nums) {
    for (auto mod_num : mod_nums) {
        if (mod_num > crate.num_modules) {
            throw std::runtime_error(
                std::string("invalid module number: " + std::to_string(mod_num)));
        }
        if (!crate[mod_num].online()) {
            throw std::runtime_error(
                std::string("module offline: " + std::to_string(mod_num)));
        }
    }
}

template<typename V>
static void output_value(std::ostream& out, const std::string& name, V value) {
    xia::util::ostream_guard oguard(out);
    out << name << " = " << value;
    if (!std::is_same<V, double>::value) {
        out << std::hex << " (0x" << value << ')';
    }
    out << std::endl;
}

template<typename W>
void module_threads(
    command_context& context, module_range& mod_nums, std::vector<W>& workers,
    std::string error_message, bool show_performance = true) {
    if (workers.size() != mod_nums.size()) {
        throw std::runtime_error("workers and modules counts mismatch");
    }
    auto& crate = context.crate;
    using promise_error = std::promise<error::code>;
    using future_error = std::future<error::code>;
    std::vector<promise_error> promises(mod_nums.size());
    std::vector<future_error> futures;
    std::vector<std::thread> threads;
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto& module = crate[mod_nums[m]];
        auto& worker = workers[m];
        futures.push_back(future_error(promises[m].get_future()));
        threads.push_back(std::thread([&context, m, &promises, &module, &worker] {
            try {
                worker.running = true;
                worker.worker(context, module);
                promises[m].set_value(error::code::success);
            } catch (xia::pixie::error::error& e) {
                promises[m].set_value(e.type);
            } catch (...) {
                try {
                    promises[m].set_exception(std::current_exception());
                } catch (...) {
                }
            }
            worker.running = false;
        }));
    }
    error::code first_error = error::code::success;
    size_t finished = 0;
    size_t show_secs = 5;
    xia::util::timepoint duration(true);
    xia::util::timepoint interval(true);
    while (finished != threads.size()) {
        finished = threads.size();
        for (size_t t = 0; t < threads.size(); ++t) {
            auto& future = futures[t];
            if (future.valid()) {
                auto zero = std::chrono::seconds(0);
                if (future.wait_for(zero) == std::future_status::ready) {
                    workers[t].period.stop();
                    error::code e = future.get();
                    if (e != error::code::success) {
                        context.opts.out << "module " << t << ": error: "
                                         << xia::pixie::error::api_result_text(e)
                                         << std::endl;
                    }
                    if (first_error == error::code::success) {
                        first_error = e;
                    }
                    threads[t].join();
                } else {
                    --finished;
                }
            }
        }
        xia::pixie::hw::wait(20 * 1000);
        if (show_performance && interval.secs() > show_secs) {
            auto secs = interval.secs();
            interval.restart();
            context.opts.out << "running: " << threads.size() - finished << std::endl;
            size_t all_total = 0;
            for (auto& w : workers) {
                if (w.period.secs() > 0) {
                    auto total = w.total;
                    auto bytes = (total - w.last_total) * sizeof(xia::pixie::hw::word);
                    auto rate = double(bytes) / secs;
                    all_total += total;
                    bytes = total * sizeof(xia::pixie::hw::word);
                    char active = w.running.load() ? '>' : ' ';
                    w.last_total = total;
                    std::ostringstream oss;
                    oss << ' ' << active << std::setw(2) << w.number
                        << ": total: " << std::setw(8)
                        << xia::util::humanize(bytes) << " rate: " << std::setw(8)
                        << xia::util::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                        << " slot=" << w.pci_slot;
                    context.opts.out << oss.str() << std::endl;
                    xia_log(xia::log::info) << oss.str();
                } else {
                    context.opts.out << ' ' << std::setw(2) << w.number
                                     << ": not running" << std::endl;
                }
            }
            all_total *= sizeof(xia::pixie::hw::word);
            std::ostringstream oss;
            oss << " all: total: " << std::setw(8) << xia::util::humanize(all_total)
                << " rate: " << std::setw(8)
                << xia::util::humanize(double(all_total) / duration.secs()) << " bytes/sec";
            context.opts.out << oss.str() << std::endl;
            xia_log(xia::log::info) << oss.str();
        }
    }
    if (first_error != error::code::success) {
        throw error(first_error, error_message);
    }
}

module_thread_worker::module_thread_worker()
    : number(-1), slot(-1), pci_bus(-1), pci_slot(-1), running(false), has_error(false),
      total(0), last_total(0) {}

template<typename W>
void set_num_slot(xia::pixie::crate::module_crate& crate, std::vector<size_t>& mod_nums,
                  std::vector<W>& workers) {
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto& module = crate[mod_nums[m]];
        auto& worker = workers[m];
        worker.number = module.number;
        worker.slot = int(module.slot);
        worker.pci_bus = module.pci_bus();
        worker.pci_slot = module.pci_slot();
    }
}

template<typename W>
void performance_stats(
    command_context& context, std::vector<W>& workers, bool show_workers = false) {
    size_t total = 0;
    size_t secs = 0;
    for (auto& w : workers) {
        if (w.period.secs() > secs) {
            secs = size_t(w.period.secs());
        }
        total += w.total;
        if (show_workers) {
            if (w.has_error) {
                std::stringstream he_oss;
                he_oss << "module: num:" << std::setw(2) << w.number
                       << " slot:" << std::setw(2)
                       << w.slot << ": has an error; check the log";
                context.opts.out << he_oss.str() << std::endl;
                xia_log(xia::log::info) << he_oss.str();
            }
            std::stringstream dr_oss;
            auto bytes = w.total * sizeof(xia::pixie::hw::word);
            auto rate = double(bytes) / w.period.secs();
            dr_oss << "module: num:" << std::setw(2) << w.number << " slot:" << std::setw(2)
                   << w.slot << ": data received: "
                   << std::setw(8) << xia::util::humanize(bytes)
                   << " bytes (" << std::setw(9) << bytes << "), rate: " << std::setw(8)
                   << xia::util::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                   << " slot=" << w.pci_slot;
            context.opts.out << dr_oss.str() << std::endl;
            xia_log(xia::log::info) << dr_oss.str();
        }
    }
    total *= sizeof(xia::pixie::hw::word);
    std::stringstream oss;
    oss << "data received: " << xia::util::humanize(total) << " bytes (" << total
        << "), rate: " << xia::util::humanize(double(total) / secs, " bytes/sec");
    context.opts.out << oss.str() << std::endl;
    xia_log(xia::log::info) << oss.str();
}

static void adc_acq(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].get_traces();
    }
}

static void adc_save(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();;
    auto len_opt = context.cmd.get_arg();;
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    size_t length = xia::pixie::hw::max_adc_trace_length;
    if (!len_opt.empty()) {
        length = get_value<size_t>(len_opt);
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        std::vector<xia::pixie::hw::adc_trace> traces;
        for (auto channel : channels) {
            xia::pixie::hw::adc_trace adc_trace(length);
            crate[mod_num].read_adc(channel, adc_trace, false);
            traces.push_back(adc_trace);
        }
        std::ostringstream name;
        name << std::setfill('0') << adc_prefix
             << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "bin,";
        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }
        for (unsigned int bin = 0; bin < length; bin++) {
            out << bin << ",";
            for (size_t idx = 0; idx < traces.size(); idx++) {
                if (idx != traces.size() - 1)
                    out << traces[idx][bin] << ",";
                else
                    out << traces[idx][bin] << std::endl;
            }
        }
    }
}

static void adj_off(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].adjust_offsets();
    }
}

static void bl_acq(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        if (crate[mod_num] < xia::pixie::hw::rev_H) {
            crate[mod_num].acquire_baselines();
        }
    }
}

static void bl_save(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        xia::pixie::channel::baseline::channels_values
            baselines(crate[mod_num].num_channels);
        crate[mod_num].bl_get(channels, baselines, false);

        std::ostringstream name;
        name << std::setfill('0') << baseline_prefix
             << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "sample, time,";

        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }

        for (unsigned int sample = 0; sample < baselines.front().size(); sample++) {
            out << sample << "," << std::get<0>(baselines.front()[sample]) << ",";
            for (unsigned int idx = 0; idx < channels.size(); idx++) {
                if (idx != channels.size() - 1)
                    out << std::get<1>(baselines[channels[idx]][sample]) << ",";
                else
                    out << std::get<1>(baselines[channels[idx]][sample]) << std::endl;
            }
        }
    }
}

static void boot(command_context& context) {
    auto& crate = context.crate;
    xia::pixie::crate::crate::boot_params boot_params;
    bool first = true;
    bool clear_boots = true;
    while (context.cmd.has_arg()) {
        auto opt = context.cmd.get_arg();
        if (opt.empty()) {
            break;
        }
        if (first) {
            first = false;
            using range_type = xia::pixie::crate::crate::boot_params::range_type;
            boot_params.slots = get_values<range_type>(opt, crate.num_modules);
            if (!boot_params.slots.empty()) {
                crate.modules_to_slots(boot_params.slots);
                continue;
            }
        }
        if (clear_boots) {
            clear_boots = false;
            boot_params.boot_comms = false;
            boot_params.boot_fippi = false;
            boot_params.boot_dsp = false;
        }
        if (opt == "comms" || opt == "sys" || opt == "system") {
            boot_params.boot_comms = true;
        } else if (opt == "fippi") {
            boot_params.boot_fippi = true;
        } else if (opt == "dsp") {
            boot_params.boot_dsp = true;
        } else {
            throw std::runtime_error("boot: invalid option: " + opt);
        }
    }
    xia::util::timepoint tp;
    if (context.opts.verbose) {
        bool dsp_boot_forced = boot_params.boot_comms || boot_params.boot_fippi;
        context.opts.out << std::boolalpha
                         << "booting crate: comms=" << boot_params.boot_comms
                         << " fippi=" << boot_params.boot_fippi
                         << " dsp=" << boot_params.boot_dsp
                         << " (forced=" << dsp_boot_forced
                         << ") slots";
        if (boot_params.slots.empty()) {
            context.opts.out << "=all";
        } else {
            char delimiter = '=';
            for (auto m : boot_params.slots) {
                context.opts.out << delimiter << m;
                delimiter = ',';
            }
        }
        context.opts.out << std::endl;
        tp.start();
    }
    crate->boot(boot_params);
    if (context.opts.verbose) {
        tp.end();
        context.opts.out << "boot time=" << tp << std::endl;
    }
}

static void crate_report(command_context& context) {
    auto& crate = context.crate;
    context.opts.out << *crate << std::endl;
}

static void db(command_context& context) {
    auto check_cmd = [](auto& str) {
      return str == "show" ||
        str == "disable-swap";
    };
    auto& crate = context.crate;
    command::argument mod_nums_opt;
    command::argument chans_opt;
    auto db_opt = context.cmd.get_arg();
    if (!check_cmd(db_opt)) {
        mod_nums_opt = db_opt;
        db_opt = context.cmd.get_arg();
    }
    if (!check_cmd(db_opt)) {
        chans_opt = db_opt;
        db_opt = context.cmd.get_arg();
    }
    std::string db_command = "show";
    if (!db_opt.empty()) {
        if (!check_cmd(db_opt)) {
            throw std::runtime_error("db: invalid command: " + db_opt);
        }
        db_command = db_opt;
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        if (crate[mod_num] < xia::pixie::hw::rev_H) {
            throw std::runtime_error(
                "db: module not rev H or later: " + std::to_string(mod_num));
        }
        if (!crate[mod_num].online()) {
            throw std::runtime_error("db: module off-line");
        }
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        for (auto channel : channels) {
            auto& fixture = crate[mod_num].channels[channel].fixture;
            if (!fixture) {
                throw std::runtime_error(
                    "db: no fixture: module: " + std::to_string(mod_num) +
                    " channel: " + std::to_string(channel));
            }
            if (db_command == "show") {
                context.opts.out << "DB: module:" << mod_num
                                 << " channel:" << channel << std::endl;
                fixture->report(context.opts.out, " ");
            } else if (db_command == "disable-swap") {
                fixture->set("ADC_SWAP_DISABLE", true);
            }
        }
    }
}

static void export_(command_context& context) {
    auto& crate = context.crate;
    auto file_opt = context.cmd.get_arg();
    xia::util::timepoint tp;
    tp.start();
    crate->export_config(file_opt);
    tp.end();
    context.opts.out << "Modules export time=" << tp << std::endl;
}

static void fw_report(command_context& context) {
    std::ostream* out= &std::cout;
    std::ofstream output_file;
    auto file_opt = context.cmd.get_arg();
    if (!file_opt.empty()) {
        output_file.open(file_opt);
        if (!output_file) {
            throw std::runtime_error(
                std::string("opening report: " + file_opt + ": " + std::strerror(errno)));
        }
        out = &output_file;
    }
    xia::pixie::firmware::system_fw_report(*out, xia::pixie::firmware::system_firmware_path);
}

static void help(command_context& context) {
    auto long_opt = context.cmd.get_option("-l");
    context.opts.out << "Command help:" << std::endl
                     << " 1. Command options and argument are required in the order help reports."
                     << std::endl
                     << " 2. Command options are '-' and flag, eg '-x', or option switch and argument, eg '-d 0'."
                     << std::endl
                     << " 3. 'modules(s)' and 'channel(s)' can be a number or series:"
                     << std::endl
                     << "     eg '0' or '3,4,6' or '3,4-5,10,20-22'" << std::endl;
    auto help_opt = context.cmd.get_arg();
    auto mi = std::max_element(
        ominitool_commands.begin(), ominitool_commands.end(),
        [](auto& a, auto& b) {
            return a.name.size() < b.name.size();
        });
    size_t max = (*mi).name.size();
    if (!help_opt.empty()) {
        auto is_command = [&help_opt](auto& cmd) {
            if (help_opt == cmd.name) { return true; }
            auto n = cmd.name.rfind('/');
            if (n != std::string::npos) {
                return cmd.name.compare(n + 1, cmd.name.size() - n, help_opt) == 0;
            }
            return false;
        };
        auto ci = std::find_if(
            ominitool_commands.begin(), ominitool_commands.end(), is_command);
        if (ci == ominitool_commands.end()) {
            context.opts.out << "error: command not found: " << help_opt << std::endl;
        } else {
            context.opts.out << (*ci).formatted_help(true, max);
        }
    } else {
        for (auto& group : command_groups) {
            context.opts.out << group << ':' << std::endl;
            for (auto& cmd : ominitool_commands) {
                if (cmd.group == group) {
                    context.opts.out << cmd.formatted_help(long_opt == "true", max);
                }
            }
        }
    }
}

static void hist_resume(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_histograms(run_mode::resume);
    }
}

static void hist_save(command_context& context) {
    auto& crate = context.crate;
    auto bins_opt = context.cmd.get_option("-b");
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        size_t length = crate[mod_num].channels[0].fixture->config.max_histogram_length;
        if (!bins_opt.empty()) {
            length = get_value<size_t>(bins_opt);
        }
        std::vector<xia::pixie::hw::words> histos;
        for (auto channel : channels) {
            xia::pixie::hw::words histogram(length);
            crate[mod_num].read_histogram(channel, histogram);
            histos.push_back(histogram);
        }
        std::ostringstream name;
        name << std::setfill('0') << histogram_prefix << '-' << std::setw(2) << mod_num << ".csv";
        std::ofstream out(name.str());
        out << "bin,";
        for (size_t idx = 0; idx < channels.size(); idx++) {
            if (idx != channels.size() - 1)
                out << "Chan" << channels[idx] << ",";
            else
                out << "Chan" << channels[idx] << std::endl;
        }
        for (unsigned int bin = 0; bin < length; bin++) {
            out << bin << ",";
            for (size_t idx = 0; idx < histos.size(); idx++) {
                if (idx != histos.size() - 1)
                    out << histos[idx][bin] << ",";
                else
                    out << histos[idx][bin] << std::endl;
            }
        }
    }
}

static void hist_start(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_histograms(run_mode::new_run);
    }
}

static void import(command_context& context) {
    auto& crate = context.crate;
    auto path_opt = context.cmd.get_arg();
    xia::util::timepoint tp;
    xia::pixie::module::number_slots modules;
    tp.start();
    crate->import_config(path_opt, modules);
    crate->initialize_afe();
    tp.end();
    context.opts.out << "Modules imported: " << modules.size()
                     << " time=" << tp << std::endl;
}

struct list_save_worker : public module_thread_worker {
    std::string name;
    size_t seconds;
    bool run_task;

    list_save_worker();
    void worker(
        command_context& context,
        xia::pixie::module::module& module);
};

list_save_worker::list_save_worker() : seconds(0) {}

void list_save_worker::worker(
    command_context& context, xia::pixie::module::module& module) {
    name += '-' + std::to_string(module.number) + ".lmd";
    std::ofstream out(name, std::ios::binary);
    if (!out) {
        throw std::runtime_error(
            std::string("list mode file open: ") + name + ": " +
            std::strerror(errno));
    }
    using namespace xia::pixie::hw::run;
    if (run_task) {
        module.start_listmode(run_mode::new_run);
    }
    xia::pixie::hw::words lm;
    const size_t poll_period_usecs = 100 * 1000;
    total = 0;
    period.start();
    while (period.secs() < seconds) {
        lm.clear();
        if (module.read_list_mode(lm) > 0) {
            total += lm.size();
            out.write(
                reinterpret_cast<char*>(lm.data()),
                lm.size() * sizeof(xia::pixie::hw::word));
        } else {
            xia::pixie::hw::wait(poll_period_usecs);
        }
    }
    if (run_task) {
        module.run_end();
        lm.clear();
        if (module.read_list_mode(lm) > 0) {
            total += lm.size();
            out.write(
                reinterpret_cast<char*>(lm.data()),
                lm.size() * sizeof(xia::pixie::hw::word));
        }
        context.opts.out << "list-mode: " << module.number
                         << ": " << module.run_stats.output() << std::endl;
        if (module.run_stats.hw_overflows != 0) {
            throw std::runtime_error(
                "list mode: EXT FIFO overflow (check workflow config)");
        }
        if (module.run_stats.overflows != 0) {
            throw std::runtime_error(
                "list mode: data FIFO overflow (check buffer sizes)");
        }
        if (module.run_stats.in != module.run_stats.out) {
            throw std::runtime_error(
                "list mode: data left in data FIFO");
        }
    }
    period.end();
}

static void list_mode_command(command_context& context, bool run_task) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto secs_opt = context.cmd.get_arg();
    auto name_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto secs = get_value<size_t>(secs_opt);
    auto name = name_opt;
    module_check(crate, mod_nums);
    if (secs == 0) {
        throw std::runtime_error(std::string("list mode run/save period is 0"));
    }
    auto saves = std::vector<list_save_worker>(mod_nums.size());
    set_num_slot(crate, mod_nums, saves);
    for (auto& s : saves) {
        s.name = name;
        s.seconds = secs;
        s.run_task = run_task;
    };
    module_threads(context, mod_nums, saves, "list mode command error; see log");
    performance_stats(context, saves);
}

static void list_mode(command_context& context) {
    list_mode_command(context, true);
}

static void list_resume(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        crate[mod_num].start_listmode(run_mode::resume);
    }
}

static void list_save(command_context& context) {
    list_mode_command(context, false);
}

static void list_start(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        using namespace xia::pixie::hw::run;
        if (crate[mod_num].read("SYNCH_WAIT") == 1) {
            for (size_t m = 0; m < crate.num_modules; ++m) {
                crate[m].start_listmode(run_mode::new_run);
            }
            break;
        }
        crate[mod_num].start_listmode(run_mode::new_run);
    }
}

static void lset_import(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto settings_opt = context.cmd.get_arg();
    auto action_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    std::string action;
    if (!action_opt.empty()) {
        if (action_opt == "flush" || action_opt == "sync") {
            action = action_opt;
        } else {
            throw std::runtime_error(
                std::string("invalid post settings import operation: " + action_opt));
        }
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::module::module& module = crate[mod_num];
        xia::pixie::legacy::settings settings(module);
        settings.load(settings_opt);
        settings.import(module);
        if (module.online() && !action.empty()) {
            if (action == "flush") {
                module.sync_vars();
            } else if (action == "sync") {
                module.sync_vars();
                module.sync_hw();
            }
        }
    }
}

static void lset_load(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto settings_opt = context.cmd.get_arg();
    auto action_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    std::string action;
    if (!action_opt.empty()) {
        if (action_opt == "flush" || action_opt == "sync") {
            action = action_opt;
        } else {
            throw std::runtime_error(
                std::string("invalid post settings import operation: " +
                            action_opt));
        }
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::module::module& module = crate[mod_num];
        xia::pixie::legacy::settings settings(module);
        settings.load(settings_opt);
        settings.write(module);
        if (module.online() && !action.empty()) {
            if (action == "flush") {
                module.sync_vars();
            } else if (action == "sync") {
                module.sync_vars();
                module.sync_hw();
            }
        }
    }
}

static void lset_report(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto settings_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::legacy::settings settings(crate[mod_num]);
        settings.load(settings_opt);
        context.opts.out << settings;
    }
}

static void mod_offline(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate->set_offline(crate[mod_num]);
    }
}

static void mod_online(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate->set_online(crate[mod_num]);
    }
}

static void override_fw(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto fwfile_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    if (mod_nums.size() > 1) {
        throw std::runtime_error("override_report: too many modules");
    }
    for (auto mod_num : mod_nums) {
        xia::pixie::firmware::override_default_fw(crate[mod_num].firmware, fwfile_opt);
    }
}

static void get_memory_reg(
    const std::string& mem_reg, std::string& mem, std::string& reg,
    const std::string& label) {
    reg = mem_reg;
    tolower(reg);
    auto has_mem_field = mem_reg.find(':') != std::string::npos;
    if (has_mem_field) {
        xia::util::strings mr;
        xia::util::split(mr, mem_reg, ':');
        if (mr.size() != 2) {
            throw std::runtime_error(
                label + ": invalid reg format: " + mem_reg);
        }
        const auto& mems = hardware["memory"];
        if (!mems.contains(mr[0])) {
            throw std::runtime_error(
                label + ": invalid memory: " + mr[0]);
        }
        mem = mr[0];
        reg = mr[1];
    } else {
        mem = "sys";
    }
}

static xia::pixie::hw::address get_address(
    const std::string& mem, const std::string& reg, const std::string& label) {
    xia::pixie::hw::address address = 0;
    bool number = true;
    try {
        address = std::stoul(reg, nullptr, 0);
    } catch (std::invalid_argument& ) {
        number = false;
    } catch (std::out_of_range& ) {
        throw std::runtime_error(label + ": reg value of range: " + reg);
    }
    const auto& mems = hardware["memory"];
    if (!mems.contains(mem)) {
        throw std::runtime_error(
            label + ": invalid memory type: " + mem);
    }
    if (!number) {
        const auto& mem_regs = mems[mem];
        if (!mem_regs.contains(reg)) {
            throw std::runtime_error(label + ": " + mem + " reg not found: " + reg);
        }
        address = mem_regs[reg];
    }
    return address;
}

static void reg_read(command_context& context) {
    const std::string label = "reg-read";
    auto& crate = context.crate;
    auto slot_opt = context.cmd.get_option("-s");
    auto hex_opt = context.cmd.get_option("-x");
    auto mod_slot_opt = context.cmd.get_arg();
    auto reg_opt = context.cmd.get_arg();
    size_t mod_slot = get_value<size_t>(mod_slot_opt);
    std::string mem;
    std::string reg;
    get_memory_reg(reg_opt, mem, reg, label);
    xia::pixie::hw::address address = get_address(mem, reg, label);
    if (slot_opt == "true") {
        auto mod = crate.find(mod_slot);
        mod_slot = mod->number;
    }
    auto& mod = crate[mod_slot];
    xia::pixie::hw::word value;
    if (mem == "sys") {
        value = mod.read_word(address);
    } else {
        xia::pixie::hw::fpga::comms comms(mod);
        if (!comms.done()) {
            throw std::runtime_error("reg-read: comms/sys FPGA not loaded");
        }
        if (mem == "fippi") {
            auto fippi = xia::pixie::hw::memory::fippi(mod);
            value = fippi.read(address);
        } else if (mem == "dsp") {
            auto dsp = xia::pixie::hw::memory::dsp(mod);
            value = dsp.read(address);
        }
    }
    if (hex_opt == "true") {
        context.opts.out << std::hex << "0x";
    }
    context.opts.out << value << std::endl;
    if (hex_opt == "true") {
        context.opts.out << std::dec;
    }
}

static void reg_write(command_context& context) {
    const std::string label = "reg-write";
    auto slot_opt = context.cmd.get_option("-s");
    auto& crate = context.crate;
    auto mod_slot_opt = context.cmd.get_arg();
    auto reg_opt = context.cmd.get_arg();
    auto value_opt = context.cmd.get_arg();
    size_t mod_slot = get_value<size_t>(mod_slot_opt);
    std::string mem;
    std::string reg;
    get_memory_reg(reg_opt, mem, reg, label);
    xia::pixie::hw::address address = get_address(mem, reg, label);
    if (slot_opt == "true") {
        auto mod = crate.find(mod_slot);
        mod_slot = mod->number;
    }
    auto& mod = crate[mod_slot];
    xia::pixie::hw::word value =
        get_value<xia::pixie::hw::word>(value_opt);
    if (mem == "sys") {
        mod.write_word(address, value);
    } else {
        xia::pixie::hw::fpga::comms comms(mod);
        if (!comms.done()) {
            throw std::runtime_error("reg-write: comms/sys FPGA not loaded");
        }
        if (mem == "fippi") {
            auto fippi = xia::pixie::hw::memory::fippi(mod);
            fippi.write(address, value);
        } else if (mem == "dsp") {
            auto dsp = xia::pixie::hw::memory::dsp(mod);
            dsp.write(address, value);
        }
    }
}

static void par_read(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    if (param_opt.empty()) {
        param_opt = chans_opt;
        chans_opt.clear();
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            context.opts.out << "# module param read: " << mod_num
                             << ": " << param_opt << std::endl;
            if (param_opt == "all") {
                for (auto& par : xia::pixie::param::get_module_param_map()) {
                    try {
                        output_value(
                            context.opts.out, par.first, crate[mod_num].read(par.second));
                    } catch (error& e) {
                        if (e.type != error::code::module_param_disabled &&
                            e.type != error::code::module_param_writeonly) {
                            throw;
                        }
                    }
                }
            } else {
                output_value(
                    context.opts.out, param_opt, crate[mod_num].read(param_opt));
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, crate[mod_num].num_channels);
            for (auto channel_num : channels) {
                context.opts.out << "# channel param read: " << mod_num
                                 << ':' << channel_num << ": "
                                 << param_opt << std::endl;
                if (param_opt == "all") {
                    for (auto& par : xia::pixie::param::get_channel_param_map()) {
                        try {
                            output_value(
                                context.opts.out, par.first, crate[mod_num].read(
                                    par.second, channel_num));
                        } catch (error& e) {
                            if (e.type != error::code::channel_param_disabled &&
                                e.type != error::code::channel_param_writeonly) {
                                throw;
                            }
                        }
                    }
                } else {
                    output_value(
                        context.opts.out, param_opt,
                        crate[mod_num].read(param_opt, channel_num));
                }
            }
        }
    }
}

static void par_write(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    auto value_opt = context.cmd.get_arg();
    if (value_opt.empty()) {
      value_opt = param_opt;
      param_opt = chans_opt;
      chans_opt.clear();
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto value = get_value<double>(value_opt);
    for (auto mod_num : mod_nums) {
        if (chans_opt.empty()) {
            bool bcast = crate[mod_num].write(param_opt, xia::pixie::param::value_type(value));
            if (bcast) {
                xia::pixie::crate::crate::user user(crate);
                for (size_t m = 0; m < crate.num_modules; ++m) {
                    if (mod_num != m) {
                        auto& module = crate[m];
                        if (module.online()) {
                            module.write(param_opt, xia::pixie::param::value_type(value));
                        }
                    }
                }
                break;
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, crate[mod_num].num_channels);
            auto& module = crate[mod_num];
            for (auto channel_num : channels) {
                module.write(param_opt, channel_num, value);
            }
        }
    }
}

static void report(command_context& context) {
    std::ostream* out= &context.opts.out;
    std::ofstream output_file;
    auto file_opt = context.cmd.get_arg();
    if (!file_opt.empty()) {
        output_file.open(file_opt);
        if (!output_file) {
            throw std::runtime_error(
                std::string("opening report: " + file_opt + ": " + std::strerror(errno)));
        }
        out = &output_file;
    }
    auto& crate = context.crate;
    crate->report(*out);
}

static void run_active(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        context.opts.out << "module=" << mod_num << " run-active=" << std::boolalpha
                         << crate[mod_num].run_active() << std::endl;
    }
}

static void run_end(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_end();
    }
}

static void set_dacs(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].set_dacs();
    }
}

static void stats(command_context& context) {
    auto& crate = context.crate;
    auto stat_opt = context.cmd.get_option("-s");
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    std::string stat = "all";
    if (!stat_opt.empty()) {
        if (stat_opt == "all" ||
            stat_opt == "pe" || stat_opt == "ocr" ||
            stat_opt == "rt" || stat_opt == "lt") {
            stat = stat_opt;
        } else {
            throw std::runtime_error("invalid stat type: " + stat_opt);
        }
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::channel::range channels;
        channels_option(channels, chans_opt, crate[mod_num].num_channels);
        xia::pixie::stats::stats stats(crate[mod_num]);
        crate[mod_num].read_stats(stats);
        if (stat == "all" || stat == "pe") {
            context.opts.out << "module " << mod_num
                             << ": processed-events="
                             << stats.mod.processed_events() << std::endl;
        }
        if (stat == "all" || stat == "icr") {
            for (auto channel : channels) {
                context.opts.out << "module " << mod_num << " chan " << channel
                                 << ": input-count-rate="
                                 << stats.chans[channel].input_count_rate()
                                 << std::endl;
            }
        }
        if (stat == "all" || stat == "ocr") {
            for (auto channel : channels) {
                context.opts.out << "module " << mod_num << " chan " << channel
                                 << ": output-count-rate="
                                 << stats.chans[channel].output_count_rate()
                                 << std::endl;
            }
        }
        if (stat == "all" || stat == "rt") {
            context.opts.out << "module " << mod_num
                             << ": real-time=" << stats.mod.real_time()
                             << std::endl;
        }
        if (stat == "all" || stat == "lt") {
            for (auto channel : channels) {
                context.opts.out << "module " << mod_num << " chan " << channel
                                 << ": live-time="
                                 << stats.chans[channel].live_time() << std::endl;
            }
        }
    }
}

static void stats_rpt(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto file_opt = context.cmd.get_arg();
    std::stringstream rpt;
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        xia::pixie::stats::stats stats(crate[mod_num]);
        crate[mod_num].read_stats(stats);
        stats.report(crate[mod_num], rpt);
        if (mod_num != mod_nums.back()) {
            rpt << ",\n";
        }
    }
    std::ofstream output(file_opt);
    output << rpt.str();
}

struct test_fifo_worker : public module_thread_worker {
    size_t length;

    test_fifo_worker();
    void worker(
        command_context& context,
        xia::pixie::module::module& module);
};

test_fifo_worker::test_fifo_worker() : length(0) {}

void test_fifo_worker::worker(
    command_context& , xia::pixie::module::module& module) {
    try {
        std::ofstream out(
            list_mode_prefix + "-control-task-11.lmd", std::ios::out | std::ios::binary);
        module.start_test(xia::pixie::module::module::test::lm_fifo);
        const size_t poll_period_usecs = 10 * 1000;
        period.start();
        while (total < length) {
            size_t data_available = module.read_list_mode_level();
            if (data_available > 0) {
                xia::pixie::hw::words lm;
                module.read_list_mode(lm);
                total += lm.size();
                out.write(reinterpret_cast<char*>(lm.data()), lm.size() * sizeof(uint32_t));
            }
            if (data_available == 0) {
                xia::pixie::hw::wait(poll_period_usecs);
            }
        }
        period.end();
        module.end_test();
        out.close();
    } catch (...) {
        period.end();
        has_error = true;
        module.end_test();
        throw;
    }
}

static void test(command_context& context) {
    auto& crate = context.crate;
    auto mode_opt = context.cmd.get_option("-m");
    auto mod_nums_opt = context.cmd.get_arg();
    xia::pixie::module::module::test mode = xia::pixie::module::module::test::off;
    if (!mode_opt.empty()) {
        if (mode_opt == "lmfifo") {
            mode = xia::pixie::module::module::test::lm_fifo;
        } else if (mode_opt != "off") {
            throw std::runtime_error(std::string("invalid test mode: " + mode_opt));
        }
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    size_t bytes = 500 * 1024 * 1000;
    auto tests = std::vector<test_fifo_worker>(mod_nums.size());
    set_num_slot(crate, mod_nums, tests);
    for (auto& t : tests) {
        t.length = (bytes) / sizeof(xia::pixie::hw::word);
    }
    context.opts.out << "Test: " << mode_opt << " (" << int(mode)
                     << ") length=" << xia::util::humanize(bytes)
                     << std::endl;
    module_threads(context, mod_nums, tests, "fifo test error; see log");
    performance_stats(context, tests, true);
}

static void var_read(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    auto offsets_opt = context.cmd.get_arg();
    if (!check_number(chans_opt)) {
        param_opt = chans_opt;
        chans_opt.clear();
    }
    if (offsets_opt.empty()) {
        offsets_opt = "0";
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        auto& mod = crate[mod_num];
        if (chans_opt.empty()) {
            const auto& desc =
                xia::pixie::param::lookup_module_descriptor(
                    param_opt, mod.module_var_descriptors);
            auto offsets = get_values<size_t>(offsets_opt, desc.size);
            if (param_opt == "all") {
                context.opts.out << "# module var read: " << mod_num << ": "
                                 << param_opt << std::endl;
                for (auto& var : mod.module_var_descriptors) {
                    for (auto offset : offsets) {
                        try {
                            output_value(
                                context.opts.out, var.name, mod.read_var(var.par, offset));
                        } catch (error& e) {
                            if (e.type != error::code::module_param_disabled &&
                                e.type != error::code::module_param_writeonly) {
                                throw;
                            }
                        }
                    }
                }
            } else {
                for (auto offset : offsets) {
                    output_value(
                        context.opts.out, param_opt, mod.read_var(param_opt, 0, offset));
                }
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, mod.num_channels);
            for (auto channel : channels) {
                const auto& desc =
                    xia::pixie::param::lookup_channel_descriptor(
                        param_opt, mod.channel_var_descriptors);
                auto offsets = get_values<size_t>(offsets_opt, desc.size);
                if (param_opt == "all") {
                    context.opts.out << "# channel var read: " << mod_num << ':'
                                     << channel << ": " << param_opt << std::endl;
                    for (auto& var : mod.channel_var_descriptors) {
                        for (auto offset : offsets) {
                            try {
                                output_value(
                                    context.opts.out, var.name, mod.read_var(
                                        var.par, channel, offset));
                            } catch (error& e) {
                                if (e.type != error::code::channel_param_disabled &&
                                    e.type != error::code::channel_param_writeonly) {
                                    throw;
                                }
                            }
                        }
                    }
                } else {
                    for (auto offset : offsets) {
                        output_value(
                            context.opts.out, param_opt, mod.read_var(
                                param_opt, channel, offset));
                    }
                }
            }
        }
    }
}

static void var_write(command_context& context) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto chans_opt = context.cmd.get_arg();
    auto param_opt = context.cmd.get_arg();
    auto offsets_opt = context.cmd.get_arg();
    auto value_opt = context.cmd.get_arg();
    if (!check_number(chans_opt)) {
        value_opt = param_opt;
        param_opt = chans_opt;
        chans_opt.clear();
    }
    if (value_opt.empty()) {
        value_opt = offsets_opt;
        offsets_opt.clear();
    }
    if (offsets_opt.empty()) {
        offsets_opt = "0";
    }
    module_range mod_nums;
    modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    auto value = get_value<xia::pixie::param::value_type>(value_opt);
    for (auto mod_num : mod_nums) {
        auto& mod = crate[mod_num];
        if (chans_opt.empty()) {
            const auto& desc =
                xia::pixie::param::lookup_module_descriptor(
                    param_opt, mod.module_var_descriptors);
            auto offsets = get_values<size_t>(offsets_opt, desc.size);
            if (param_opt == "all") {
                for (auto& var : mod.module_var_descriptors) {
                    for (auto offset : offsets) {
                        try {
                            mod.write_var(var.par, value, offset);
                        } catch (error& e) {
                            if (e.type != error::code::module_param_disabled &&
                                e.type != error::code::module_param_readonly) {
                                throw;
                            }
                        }
                    }
                }
            } else {
                for (auto offset : offsets) {
                    mod.write_var(param_opt, value, offset);
                }
            }
        } else {
            xia::pixie::channel::range channels;
            channels_option(channels, chans_opt, mod.num_channels);
            for (auto channel : channels) {
                const auto& desc =
                    xia::pixie::param::lookup_channel_descriptor(
                        param_opt, mod.channel_var_descriptors);
                auto offsets = get_values<size_t>(offsets_opt, desc.size);
                if (param_opt == "all") {
                    for (auto& var : mod.channel_var_descriptors) {
                        for (auto offset : offsets) {
                            try {
                                mod.write_var(
                                    var.par, value, channel, offset);
                            } catch (error& e) {
                                if (e.type != error::code::channel_param_disabled &&
                                    e.type != error::code::channel_param_readonly) {
                                    throw;
                                }
                            }
                        }
                    }
                } else {
                    for (auto offset : offsets) {
                        mod.write_var(
                            param_opt, value, channel, offset);
                    }
                }
            }
        }
    }
}

static void wait(command_context& context) {
    auto period_opt = context.cmd.get_arg();
    size_t multipler = 1;
    switch (period_opt.back()) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;
    case 's':
      multipler = 1000;
      break;
    case 'm':
      multipler = 60 * 1000;
      break;
    default:
      throw std::runtime_error("wait: invalid time units: " + period_opt.back());
      break;
    }
    auto msecs = get_value<size_t>(period_opt);
    msecs *= multipler;
    if (context.opts.verbose) {
        context.opts.out << "waiting " << msecs << " msecs" << std::endl;
    }
    xia::pixie::hw::wait(msecs * 1000);
}

int main(int argc, char* argv[]) {
    ominitool_register_commands(crate_commands);
    ominitool_register_commands(module_commands);
    ominitool_register_commands(util_commands);

    args_parser parser("Pixie-16 OmniTool ");

    parser.helpParams.addDefault = true;
    parser.helpParams.addChoices = true;

    args_group option_group(parser, "Options");
    args_help_flag help(option_group, "help", "Display this help menu", {'h', "help"});
    args_flag quiet_flag(
        option_group, "quiet_flag", "Quiet output", {'q', "quiet"},
        false);
    args_flag throw_unhandled_flag(
        option_group, "throw_unhandled_flag",
        "Throw an unhandled exception, it will detail the exception",
        {'t', "throw-unhandled"}, false);
    args_flag reg_trace(
        option_group, "reg_trace", "Registers debugging traces in the API.",
        {'R', "reg-trace"});
    args_flag simulate(
        option_group, "simulate", "Simulate the crate and modules",
        {'S', "simulate"}, false);
    args_size_flag num_modules_flag(
        option_group, "num_modules_flag",
        "Number of modules to report", {'n', "num-modules"}, 0);
    args_string_flag module_defs(
        option_group, "module_file_flag",
        "Crate simulation module definition file to load. "
        "The file contains the module to simulate.",
        {'M', "modules"});
    args_string_flag fw_host_path_flag(
        option_group, "fw_host_path_flag",
        "Path to the host installed firmware.",
        {'H', "hostpath"});
    args_strings_flag fw_file_flag(
        option_group, "fw_file_flag",
        "Firmware file(s) to load. Can be repeated. "
        "Takes the form rev:mod-rev-num:type:name"
        "Ex. r33339:15:sys:syspixie16_revfgeneral_adc250mhz_r33339.bin",
        {'F', "firmware"});
    args_strings_flag crate_file_flag(
        option_group, "crate_file_flag",
        "Crate firmware file to load. "
        "The file contain the list of firmware files.",
        {'C', "crate"});
    args_string_flag log_file_flag(
        option_group, "log_file_flag",
        "Operational log file. Use `stdout` for the console.", {'l', "log"});
    args_string_flag log_level_flag(
        option_group, "log_level_flag", "Set the logging level (off, error, warning, info, debug)",
        {'L', "log-level"});
    args_string_flag out_file_flag(
        option_group, "out_file_flag",
        "Output file. Default is the console.", {'o', "output"});
    args_strings_flag slot_map_flag(
        option_group, "slot_map_flag",
        "A list of slots used to define the slot to index mapping. "
        " Use this option to limit slots available to use.",
        {'s', "slot_map"});
    args_strings_flag cmd_file_flag(
        option_group, "cmd_file_flag",
        "File of commands to execute.", {'c', "cmd"});
    args_flag no_execute(
        option_group, "no_execute", "Parse the command but do not execute.",
        {'X', "no-execute"});

    args_group command_group(parser, "Commands");
    args_positional_list args_cmds(
        command_group, "commands",
        "Commands to be performed in order. Prefix with '--'. "
        "The command 'help' lists the available commands.");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& help) {
        std::cout << help.what() << std::endl;
        std::cout << parser;
        help_output(std::cout);
        return EXIT_SUCCESS;
    } catch (args::Error& e) {
        std::cout << e.what() << std::endl;
        std::cout << parser;
        help_output(std::cout);
        return EXIT_FAILURE;
    }

    try {
        xia::util::timepoint run;
        run.start();

        std::string log;
        if (log_file_flag) {
            log = args::get(log_file_flag);
        } else {
            log = "pixie16-omnitool.log";
        }

        auto log_level = xia::log::debug;
        if (log_level_flag) {
            std::string level = args::get(log_level_flag);
            if (level == "off") {
                log_level = xia::log::off;
            } else if  (level == "error") {
                log_level = xia::log::error;
            } else if  (level == "warning") {
                log_level = xia::log::warning;
            } else if  (level == "info") {
                log_level = xia::log::info;
            } else if  (level == "debug") {
                log_level = xia::log::debug;
            } else {
                throw std::runtime_error("invalid log level: " + level);
            }
        }
        xia::logging::start("log", log, false);
        xia::logging::set_level(log_level);

        std::ostream* out = &std::cout;
        std::ofstream output;
        if (out_file_flag) {
            std::string outfile = args::get(out_file_flag);
            output.open(outfile);
            if (!output) {
                throw std::runtime_error("cannot open: " + outfile);
            }
            out = &output;
        }

        process_options process_opts(*out);
        process_opts.verbose = !args::get(quiet_flag);
        process_opts.reg_trace = args::get(reg_trace);
        process_opts.num_modules = args::get(num_modules_flag);

        /*
         * The physical crates. Let the options select one.
         */
        xia::pixie::crate::crate crate_hw;
        xia::pixie::sim::crate crate_sim;
        xia::pixie::crate::crate* crate_selection = &crate_hw;

        if (simulate) {
            if (!module_defs) {
                throw std::runtime_error("simulation requires a module definition");
            }
            xia_log(xia::log::info) << "simulation: " << args::get(module_defs);
            xia::pixie::sim::load_module_defs(args::get(module_defs));
            crate_selection = &crate_sim;
        }

        /*
         * The logical module crate
         */
        xia::pixie::crate::module_crate crate(*crate_selection);

        if (fw_file_flag) {
            for (const auto& fw : args::get(fw_file_flag)) {
                process_opts.firmware_files.push_back(fw);
            }
        }

        if (crate_file_flag) {
            for (const auto& fw : args::get(crate_file_flag)) {
                process_opts.firmware_crate_files.push_back(fw);
            }
        }

        if (slot_map_flag) {
            for (const auto& slot : args::get(slot_map_flag)) {
                slot_range ss = get_values<int>(slot, xia::pixie::hw::max_slots);
                process_opts.slots.insert(
                    std::end(process_opts.slots), std::begin(ss), std::end(ss));
            }
        }

        if (fw_host_path_flag) {
            process_opts.firmware_host_path = args::get(fw_host_path_flag);
        } else {
            process_opts.firmware_host_path = xia::pixie::firmware::system_firmware_path;
        }

        command::arguments cmds;
        for (auto& cmd : args_cmds) {
            cmds.push_back(cmd);
        }
        if (cmd_file_flag) {
            for (const auto& name : args::get(cmd_file_flag)) {
                load_commands(name, cmds);
            }
        }

        /*
         * Execute the commands as a batch.
         */
        command_batch batch;
        batch.path = {"/crate", "/module", "/util"};
        batch.set_operation("init", [](command_context& context) {
            initialize(context);
        });
        batch.set_operation("probe", [](command_context& context) {
            firmware_load(context);
            probe(context);
        });
        batch.parse(cmds);

        bool execute = !args::get(no_execute);

        if (execute) {
            batch.execute(crate, process_opts);
        } else {
            batch.report(*out);
        }

        run.end();
        if (process_opts.verbose) {
            *out << "run time=" << run << std::endl;
        }
    } catch (xia::pixie::error::error& e) {
        xia_log(xia::log::error) << e;
        std::cerr << e << std::endl;
        return e.return_code();
    } catch (std::exception& e) {
        xia_log(xia::log::error) << e.what();
        std::cerr << "error: " << e.what() << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    } catch (...) {
        if (args::get(throw_unhandled_flag)) {
            throw;
        }
        xia_log(xia::log::error) << "unknown error: unhandled exception";
        std::cerr << "error: unknown error: unhandled exception" << std::endl;
        return xia::pixie::error::api_result_unknown_error();
    }

    return EXIT_SUCCESS;
}
