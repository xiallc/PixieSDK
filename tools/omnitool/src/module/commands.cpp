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

/**
 * @brief Module Commands
 */

#include <omnitool-module.hpp>

namespace xia {
namespace omnitool {
namespace module {
omnitool_command_handler_decl(adc_acq);
static const xia::omnitool::command::definition adc_acq_cmd = {
    "Module", "/module/adc-acq",
    omnitool_command_handers(adc_acq),
    {"init", "probe"},
    0, 1, 0,
    {},
    "[modules(s)]",
    "Acquire a module's ADC trace"
};

omnitool_command_handler_decl(adc_save);
static const command::definition adc_save_cmd = {
    "Module", "/module/adc-save",
    omnitool_command_handers(adc_save),
    {"init", "probe"},
    0, 3, 0,
    {},
    "[modules(s) [channel(s) [length]]]",
    "Save a module's ADC trace to a file"
};

omnitool_command_handler_decl(adj_off);
static const command::definition adj_off_cmd = {
    "Module", "/module/adj-off",
    omnitool_command_handers(adj_off),
    {"init", "probe"},
    0, 1, 0,
    {},
    "[modules(s)]",
    "Adjust the module's offsets"
};

omnitool_command_handler_decl(bl_acq);
static const command::definition bl_acq_cmd = {
    "Module", "/module/bl-acq",
    omnitool_command_handers(bl_acq),
    {"init", "probe"},
    0, 1, 0,
    {},
    "[module(s)]",
    "Acquire module baselines"
};

omnitool_command_handler_decl(bl_save);
static const command::definition bl_save_cmd = {
    "Module", "/module/bl-save",
    omnitool_command_handers(bl_save),
    {"init", "probe"},
    0, 2, 0,
    {},
    "[module(s) [channel(s)]]",
    "Save the module's baselines"
};

omnitool_command_handler_decl(copy);
static const command::definition copy_cmd = {
    "Module", "/module/copy",
    omnitool_command_handers(copy),
    {"init", "probe"},
    3, 5, 0,
    {},
    "filter-mask src-module dest-module [src-channel] [dest-channel]",
    "Copy parameters from one module to another"
};

omnitool_command_handler_decl(db);
static const command::definition db_cmd = {
    "Module", "/module/db",
    omnitool_command_handers(db),
    {"init", "probe"},
    1, 3, 0,
    {},
    "module(s) [channel(s)] [disable-swap]",
    "Daughter board control"
};

omnitool_command_handler_decl(hist_resume);
static const command::definition hist_resume_cmd = {
    "Module", "/module/hist-resume",
    omnitool_command_handers(hist_resume),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Resume module histograms"
};

omnitool_command_handler_decl(hist_save);
static const command::definition hist_save_cmd = {
    "Module", "/module/hist-save",
    omnitool_command_handers(hist_save),
    {"init", "probe"},
    1, 2, 0,
    {omnitool_command_opt_arg_decl("b")},
    "[-b bins] module(s) [channel(s)]",
    "Save a module's histogram to a file"
};

omnitool_command_handler_decl(hist_start);
static const command::definition hist_start_cmd = {
    "Module", "/module/hist-start",
    omnitool_command_handers(hist_start),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Start module histograms"
};

omnitool_command_handler_decl(list_mode);
static const command::definition list_mode_cmd = {
    "Module", "/module/list-mode",
    omnitool_command_handers(list_mode),
    {"init", "probe"},
    3, 3, 0,
    {},
    "module(s) secs file",
    "Run list mode saving the data to a file"
};

omnitool_command_handler_decl(list_resume);
static const command::definition list_resume_cmd = {
    "Module", "/module/list-resume",
    omnitool_command_handers(list_resume),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Resume module list mode",
};

omnitool_command_handler_decl(list_save);
static const command::definition list_save_cmd = {
    "Module", "/module/list-save",
    omnitool_command_handers(list_save),
    {"init", "probe"},
    3, 3, 0,
    {},
    "module(s) secs file",
    "Save a module's list-mode data to a file"
};

omnitool_command_handler_decl(list_start);
static const command::definition list_start_cmd = {
    "Module", "/module/list-start",
    omnitool_command_handers(list_start),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Start module list mode"
};

omnitool_command_handler_decl(lset_import);
static const command::definition lset_import_cmd = {
    "Module", "/module/lset-import",
    omnitool_command_handers(lset_import),
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) file [flush/sync]",
    "Import a legacy settings file to a module"
};

omnitool_command_handler_decl(lset_load);
static const command::definition lset_load_cmd = {
    "Module", "/module/lset-load",
    omnitool_command_handers(lset_load),
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) file [flush/sync]",
    "Load a legacy settings file to a modul's DSP memory"
};

omnitool_command_handler_decl(lset_report);
static const command::definition lset_report_cmd = {
    "Module", "/module/lset-report",
    omnitool_command_handers(lset_report),
    {"init", "probe"},
    2, 2, 0,
    {},
    "module(s) file",
    "Output a legacy settings fie in a readable format"
};

omnitool_command_handler_decl(mod_offline);
static const command::definition mod_offline_cmd = {
    "Module", "/module/mod-offline",
    omnitool_command_handers(mod_offline),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Set a module offline"
};

omnitool_command_handler_decl(mod_online);
static const command::definition mod_online_cmd = {
    "Module", "/module/mod-online",
    omnitool_command_handers(mod_online),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Set a module online"
};

omnitool_command_handler_decl(mset_import);
static const command::definition mset_import_cmd = {
    "Module", "/module/mset-import",
    omnitool_command_handers(mset_import),
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) file [flush/sync]",
    "Import a settings file to a module"
};

omnitool_command_handler_decl(override_fw);
static const command::definition override_fw_cmd = {
    "Module", "/module/firmware/override",
    omnitool_command_handers(override_fw),
    {"init", "probe"},
    2, 2, 0,
    {},
    "module fwfile",
    "Override a firmware file for a module"
};

omnitool_command_handler_decl(par_read);
static const command::definition par_read_cmd = {
    "Module", "/module/par-read",
    omnitool_command_handers(par_read),
    {"init", "probe"},
    2, 3, 0,
    {},
    "module(s) [channel(s)] param",
    "Read module/channel parameter"
};

omnitool_command_handler_decl(par_write);
static const command::definition par_write_cmd = {
    "Module", "/module/par-write",
    omnitool_command_handers(par_write),
    {"init", "probe"},
    3, 4, 0,
    {},
    "module(s) [channel(s)] param value",
    "Write module/channel parameter"
};

omnitool_command_handler_decl(reg_read);
static const command::definition reg_read_cmd = {
    "Module", "/module/reg-read",
    omnitool_command_handers(reg_read),
    {"init"},
    2, 2, 0,
    {omnitool_command_opt_decl("s"), omnitool_command_opt_decl("x")},
    "[-s] [-x] module/slot address/name/memory:name",
    "Read from a register in a module or slot (-s) memory address"
};

omnitool_command_handler_decl(reg_write);
static const command::definition reg_write_cmd = {
    "Module", "/module/reg-write",
    omnitool_command_handers(reg_write),
    {"init"},
    3, 3, 0,
    {omnitool_command_opt_decl("s")},
    "[-s] module/slot address/name/memory:name [value]",
    "Write to a register in a module or slot (-s) memory address"
};

omnitool_command_handler_decl(run_active);
static const command::definition run_active_cmd = {
    "Module", "/module/run-active",
    omnitool_command_handers(run_active),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "Does the module have an active run?"
};

omnitool_command_handler_decl(run_end);
static const command::definition run_end_cmd = {
    "Module", "/module/run-end",
    omnitool_command_handers(run_end),
    {"init", "probe"},
    1, 1, 0,
    {},
    "module(s)",
    "End module run(s)"
};

omnitool_command_handler_decl(set_dacs);
static const command::definition set_dacs_cmd = {
    "Module", "/module/set-dacs",
    omnitool_command_handers(set_dacs),
    {"init", "probe"},
    1, 1, 0,
    {},
    "modules(s)",
    "Set the module's DAC"
};

omnitool_command_handler_decl(stats);
static const command::definition stats_cmd = {
    "Module", "/module/stats",
    omnitool_command_handers(stats),
    {"init", "probe"},
    1, 2, 0,
    {omnitool_command_opt_arg_decl("s")},
    "[-s stat (pe/ocr/rt/lt)] module(s) [channel(s)]",
    "module channel stats"
};

omnitool_command_handler_decl(stats_rpt);
static const command::definition stats_rpt_cmd = {
    "Module", "/module/stats-rpt",
    omnitool_command_handers(stats_rpt),
    {"init", "probe"},
    2, 2, 0,
    {},
    "module(s) filename",
    "module channel stats"
};

omnitool_command_handler_decl(test);
static const command::definition test_cmd = {
    "Module", "/module/test",
    omnitool_command_handers(test),
    {"init", "probe"},
    1, 1, 0,
    {omnitool_command_opt_arg_decl("m")},
    "[-m mode (off/lmfifo)] module(s)",
    "Test control, default mode is 'off'"
};

omnitool_command_handler_decl(var_read);
static const command::definition var_read_cmd = {
    "Module", "/module/var-read",
    omnitool_command_handers(var_read),
    {"init", "probe"},
    2, 4, 0,
    {},
    "module(s) [channel(s)] param [offset(s)]",
    "Read module/channel variable. A channel references a channel variable."
};

omnitool_command_handler_decl(var_write);
static const command::definition var_write_cmd = {
    "Module", "/module/var-write",
    omnitool_command_handers(var_write),
    {"init", "probe"},
    3, 5, 0,
    {},
    "module(s) [channel(s)] param [offset(s)] value",
    "Write module/channel variable. A channel references a channel variable."
};

static const command::definitions commands = {
    adc_acq_cmd,
    adc_save_cmd,
    adj_off_cmd,
    bl_acq_cmd,
    bl_save_cmd,
    copy_cmd,
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
    mset_import_cmd,
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

void register_commands() {
    command::register_commands(commands);
}
} // namespace module
} // namespace omnitool
} // namespace xia
