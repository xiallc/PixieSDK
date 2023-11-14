
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

#include <cstring>
#include <fstream>
#include <mutex>

#include <string.h>

#include <pixie/utils/io.hpp>
#include <pixie/utils/path.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>

#include <omnitool-crate.hpp>

#include "crossline.h"

namespace xia {
namespace omnitool {
namespace command {
namespace completions {

static void module_modules_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    bool s, completion& completions) {

    pixie::channel::range channels;
    pixie::channel::range shared_channels;

    auto& crate = context.crate;
    module_range mod_nums;

    try {
        modules_option(mod_nums, "all", crate.num_modules);
    } catch (...) {
        return;
    }

    if (mod_nums.empty()) {
        return;
    }

    if (completions.argc() == (module_arg_idx + 1) && completions.incomplete) {
        auto& arg = completions.argv(module_arg_idx);
        if (completions.partial_match("all", arg) && s) {
            completions.add({completion_entry::node::argument, "all", group, "", "all"});
            return;
        }
        for (auto input_char : arg) {
            if (isalpha(input_char)) {
                return;
            }
        }
        if (arg[0] == '-') {
            return;
        }
    } else if (completions.argc() == module_arg_idx && !completions.incomplete && s) {
        completions.add({completion_entry::node::argument, "all", group, "", "all"});
    }

    /*
     * If the argument prior to modules is complete or the modules argument is
     * incomplete add modules to completions.
     */
    if ((completions.argc() == module_arg_idx && !completions.incomplete) ||
        (completions.argc() == module_arg_idx + 1 && completions.incomplete)) {
        for (auto mod_num : mod_nums) {
            completions.add({completion_entry::node::argument, std::to_string(mod_num), group,
                             "", std::to_string(mod_num)});
        }
    }
}

void modules_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    completion& completions) {
    return module_modules_completions(context, group, module_arg_idx, true, completions);
}

void module_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    completion& completions) {
    return module_modules_completions(context, group, module_arg_idx, false, completions);
}

void channels_completions(
    context& context, const std::string& group, size_t module_arg_idx,
    size_t channel_arg_idx, completion& completions) {

    pixie::channel::range channels;
    pixie::channel::range shared_channels;

    auto& crate = context.crate;
    module_range mod_nums;

    try {
        if (completions.argc() > module_arg_idx) {
            modules_option(mod_nums, completions.argv(module_arg_idx), crate.num_modules);
        } else {
            return;
        }
    } catch (...) {
        return;
    }

    if (completions.argc() <= module_arg_idx || mod_nums.empty()) {
        return;
    }

    if (completions.argc() == (channel_arg_idx + 1) && completions.incomplete) {
        auto& arg = completions.argv(channel_arg_idx);
        if (completions.partial_match("all", arg)) {
            completions.add({completion_entry::node::argument, "all", group, "", "all"});
            return;
        }
        for (auto input_char : arg) {
            if (isalpha(input_char)) {
                return;
            }
        }
    } else if (completions.argc() == channel_arg_idx && !completions.incomplete) {
        completions.add({completion_entry::node::argument, "all", group, "", "all"});
    }

    channels_option(shared_channels, "all", crate[mod_nums[0]].num_channels);

    /*
     * Finds the set of all common channels for input modules.
     */
    for (auto mod_num : mod_nums) {
        channels_option(channels, "all", crate[mod_num].num_channels);
        size_t channel = 0;
        while (channel < shared_channels.size()) {
            if (!std::count(channels.begin(), channels.end(), channel)) {
                shared_channels.erase(shared_channels.begin() + channel);
            } else {
                channel++;
            }
        }
        channels.clear();
    }

    /*
     * If the argument prior to channels is complete or the channels argument is
     * incomplete add modules to completions.
     */
    if ((completions.argc() == channel_arg_idx && !completions.incomplete) ||
        (completions.argc() == channel_arg_idx + 1 && completions.incomplete)) {
        for (auto channel : shared_channels) {
            completions.add({completion_entry::node::argument, std::to_string(channel), group,
                             "", std::to_string(channel)});
        }
    }
}

bool valid_channels_check(
    context& context, size_t module_arg_idx,
    size_t channel_arg_idx, completion& completions) {
    pixie::channel::range channels;
    pixie::channel::range shared_channels;

    auto& crate = context.crate;
    module_range mod_nums;

    try {
        if (completions.argc() > module_arg_idx) {
            modules_option(mod_nums, completions.argv(module_arg_idx), crate.num_modules);
            channels_option(shared_channels, completions.argv(channel_arg_idx),
                crate[mod_nums[0]].num_channels);
        } else {
            return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}

void argument_completion(
    completion_entry comp, size_t min_arg_idx, size_t max_arg_idx,
    completion& completions) {
    min_arg_idx++;
    max_arg_idx++;

    /* Check if the argument has already been set */
    for (size_t i = min_arg_idx - 1; i < completions.argc(); i++) {
        if (completions.argv(i) == comp.name) {
            return;
        }
    }

    /* Add completions for argument */
    if (completions.argc() >= min_arg_idx && completions.argc() <= max_arg_idx &&
        completions.incomplete) {
        auto arg = completions.argv(completions.argc() - 1);
        if (completions.partial_match(comp.name, arg)) {
            completions.add(comp);
        }
    } else if (completions.argc() >= (min_arg_idx - 1) && completions.argc() < max_arg_idx &&
               !completions.incomplete) {
        completions.add(comp);
    }
}

void help_argument_completion(
    completion_entries& entries, size_t arg_idx, completion& completions) {
    arg_idx++;

    if (((completions.argc()) == arg_idx && completions.incomplete) ||
        ((completions.argc()) == (arg_idx - 1) && !completions.incomplete)) {
        for (auto entry : entries) {
            completions.add(entry);
        }
    }
}

bool flag_completion(
    flag_handler handle_flags, std::string cmd, completion& completions) {

    /* Check we are still in the flag section of the command */
    if ((get_pos_arg_offset(cmd, completions) < completions.argc()
          && !completions.incomplete) ||
        (get_pos_arg_offset(cmd, completions) < completions.argc() - 1
         && completions.incomplete)) {
        return false;
    }

    /* Find the command */
    auto the_cmd = find_command(cmd);
    if (the_cmd == no_command()) {
        return false;
    }
    auto cmd_obj = command(*the_cmd);

    /* Create a list of flags for the command */
    completion_entries entries;
    for (auto option : cmd_obj.def.opts) {
        entries.push_back({completion_entry::node::argument, std::get<0>(option),
            cmd, "", std::get<0>(option)});
    }

    /*
     * Find the current flag we are completing and if it has an
     * argument pass it off to the handle_flags lambda function.
     */
    auto completing_flag = [&completions](auto& entry) {
        if ((!completions.incomplete &&
             entry.name == completions.argv(completions.argc() - 1)) ||
            (completions.incomplete &&
             entry.name == completions.argv(completions.argc() - 2))) {
            return true;
        } else {
            return false;
        }
    };
    auto entry_it = std::find_if(begin(entries), end(entries), completing_flag);
    if (entry_it != end(entries)) {
        auto flag_has_arg = [&entry_it](auto& option) {
            if ((*entry_it).name == std::get<0>(option) && std::get<1>(option) == "true") {
                return true;
            } else {
                return false;
            }
        };
        auto flag_arg_it =
            std::find_if(begin(cmd_obj.def.opts), end(cmd_obj.def.opts), flag_has_arg);
        if (flag_arg_it != end(cmd_obj.def.opts)) {
            if (handle_flags) {
                handle_flags(std::get<0>(*flag_arg_it));
            }
            return true;
        }
    }

    /* Add the flags not already set to the completions */
    for (auto entry : entries) {
        auto is_match = [&entry](auto& arg) {
            if (entry.name == arg) {
                return true;
            } else {
                return false;
            }
        };
        auto it = std::find_if(begin(completions.args), end(completions.args), is_match);

        if (it == end(completions.args) && (!completions.incomplete ||
            completions.partial_match(entry.name, completions.argv(completions.argc() - 1)))) {
            completions.add(entry);
        }
    }

    return false;
}

size_t get_pos_arg_offset(
    std::string cmd, completion& completions) {

    auto the_cmd = find_command(cmd);
    if (the_cmd == no_command()) {
        return 0;
    }
    auto cmd_obj = command(*the_cmd);

    /*
     * Find the latest flag in the arguments (accounting for flag
     * arguments).
     */
    size_t off = 1;
    for (size_t i = 1; i < completions.argc(); i++) {
        auto is_flag = [&i, &completions](auto& option) {
            if (completions.args[i] == std::get<0>(option)) {
                return true;
            } else {
                return false;
            }
        };
        auto flag_it = std::find_if(begin(cmd_obj.def.opts), end(cmd_obj.def.opts), is_flag);
        if (flag_it != end(cmd_obj.def.opts)) {
            if (std::get<1>(*flag_it) == "true") {
                off = i + 2;
            } else {
                off = i + 1;
            }
        }
    }
    return off;
}

void multiargument_completion(
    completion_entries& entries, size_t min_arg_idx,
    size_t max_arg_idx, completion& completions) {
    bool found = false;

    for (size_t i = min_arg_idx; i <= max_arg_idx; i++) {

        if (i >= completions.argc()) {
            continue;
        }
        auto prev_set = [&i, &completions](auto& entry) {
            if (entry.name == completions.argv(i)) {
                return true;
            } else {
                return false;
            }
        };
        auto found_it = std::find_if(begin(entries), end(entries), prev_set);
        if (found_it != end(entries)) {
            found = true;
        }
    }

    if (!found) {
        for (auto& entry : entries) {
            argument_completion(entry, min_arg_idx, max_arg_idx,
                completions);
        }
    }
}
}  // namespace completions
}  // namespace commands
}  // namespace omnitool
}  // namespace xia
