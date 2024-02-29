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
 * @brief Module List Mode and List Save commands
 */

#include <cstring>
#include <fstream>

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/crate-view.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-module.hpp>
#include <omnitool-threads.hpp>

namespace xia {
namespace omnitool {
namespace module {
static void module_check(command::omnitool_crate& crate, std::vector<size_t> mod_nums) {
    for (auto mod_num : mod_nums) {
        command::module_range ms;
        command::modules_option(ms, "all", crate.get_modules());
        if (std::find(begin(ms), end(ms), mod_num) == end(ms)) {
            throw std::runtime_error(
                std::string("invalid module number: " + std::to_string(mod_num)));
        }
        if (!crate[mod_num].online()) {
            throw std::runtime_error(
                std::string("module offline: " + std::to_string(mod_num)));
        }
    }
}

struct list_save_worker : public thread::module_worker {
    std::string name;
    size_t seconds;
    bool run_task;

    list_save_worker();
    list_save_worker(const list_save_worker& orig);
    list_save_worker(list_save_worker&& orig);
    void worker(command::context& context, pixie::module::module& module);
};

list_save_worker::list_save_worker() : seconds(0) {}

list_save_worker::list_save_worker(const list_save_worker& orig) : module_worker(orig),
    name(orig.name), seconds(orig.seconds), run_task(orig.run_task) {}

list_save_worker::list_save_worker(list_save_worker&& orig) : module_worker(orig),
    name(orig.name), seconds(orig.seconds), run_task(orig.run_task) {}

void list_save_worker::worker(
    command::context& context, pixie::module::module& module) {
    name += '-' + std::to_string(module.slot) + ".lmd";
    std::ofstream out(name, std::ios::binary);
    if (!out) {
        throw std::runtime_error(
            std::string("list mode file open: ") + name + ": " +
            std::strerror(errno));
    }
    const auto mode = pixie::hw::run::run_mode::new_run;
    if (run_task) {
        module.start_listmode(mode);
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
                lm.size() * sizeof(pixie::hw::word));
        } else {
            pixie::hw::wait(poll_period_usecs);
        }
    }
    if (run_task) {
        module.run_end();
        lm.clear();
        if (module.read_list_mode(lm) > 0) {
            total += lm.size();
            out.write(
                reinterpret_cast<char*>(lm.data()),
                lm.size() * sizeof(pixie::hw::word));
        }
        context.opts.out << "list-mode: " << module.slot
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

static void list_mode_command(command::context& context, bool run_task) {
    auto& crate = context.crate;
    auto mod_nums_opt = context.cmd.get_arg();
    auto secs_opt = context.cmd.get_arg();
    auto name_opt = context.cmd.get_arg();
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.get_modules());
    auto secs = util::io::get_value<size_t>(secs_opt);
    auto name = name_opt;
    module_check(crate, mod_nums);
    if (run_task) {
        for (auto mod_num : mod_nums) {
            crate[mod_num].run_check();
        }
    }
    if (secs == 0) {
        throw std::runtime_error(std::string("list mode run/save period is 0"));
    }
    auto saves = std::vector<list_save_worker>(mod_nums.size());
    thread::set_num_slot(crate, mod_nums, saves);
    for (auto& s : saves) {
        s.name = name;
        s.seconds = secs;
        s.run_task = run_task;
    };
    auto thread = std::thread(
        [context, mod_nums, saves]() mutable {
        thread::module_threads(
            context, mod_nums, saves, "list mode command error; see log");
        thread::performance_stats(context, saves);
    });
    thread.detach();
}

void list_mode(command::context& context) {
    list_mode_command(context, true);
}

void list_mode_comp(
    command::context& context, command::completion& completions) {
    auto list_mode_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, list_mode_cmd.name, 1, completions);
}

void list_save(command::context& context) {
    list_mode_command(context, false);
}

void list_save_comp(
    command::context& context, command::completion& completions) {
    auto list_save_cmd = context.cmd.def;

    command::completions::modules_completions(
        context, list_save_cmd.name, 1, completions);
}
} // namespace module
} // namespace omnitool
} // namespace xia
