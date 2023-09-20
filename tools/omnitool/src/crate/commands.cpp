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
 * @brief Crate Commands
 */

#include <omnitool-crate.hpp>

namespace xia {
namespace omnitool {
namespace crate {
omnitool_command_handler_decl(boot);
static const xia::omnitool::command::definition boot_cmd = {
    "Crate", "/crate/boot",
    omnitool_command_handers(boot),
    {"init", "probe"},
    0, 4, 0,
    {},
    "[modules(s) [comms] [fippi] [dsp]]",
    "Boots the module(s)"
};

omnitool_command_handler_decl(crate_report);
static const xia::omnitool::command::definition crate_cmd = {
    "Crate", "/crate/crate",
    omnitool_command_handers(crate_report),
    {"init", "probe"},
    0, 0, 0,
    {},
    "",
    "Report the crate"
};

omnitool_command_handler_decl(export_);
static const xia::omnitool::command::definition export_cmd = {
    "Crate", "/crate/export",
    omnitool_command_handers(export_),
    {"init", "probe"},
    1, 1, 0,
    {},
    "file",
    "Export a configuration to a JSON file"
};

omnitool_command_handler_decl(fw_report);
static const xia::omnitool::command::definition fw_report_cmd = {
    "Crate", "/crate/firmware/report",
    omnitool_command_handers(fw_report),
    {"init"},
    0, 1, 0,
    {},
    "[file]",
    "Report on the system's firmware"
};

omnitool_command_handler_decl(import);
static const xia::omnitool::command::definition import_cmd = {
    "Crate", "/crate/import",
    omnitool_command_handers(import),
    {"init", "probe"},
    1, 1, 0,
    {},
    "file",
    "Import a JSON configuration file"
};

omnitool_command_handler_decl(report);
static const xia::omnitool::command::definition report_cmd = {
    "Crate", "/crate/report",
    omnitool_command_handers(report),
    {"init", "probe"},
    1, 3, 0,
    {omnitool_command_opt_decl("j")},
    "[-j] file [slot(s)]",
    "Report the crate's configuration"
};

static const command::definitions commands = {
    boot_cmd,
    crate_cmd,
    export_cmd,
    fw_report_cmd,
    import_cmd,
    report_cmd
};

void register_commands() {
    command::register_commands(commands);
}
} // namespace module
} // namespace omnitool
} // namespace xia
