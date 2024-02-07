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
 * @brief Module Test command
 */

#include <fstream>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>
#include <omnitool-completions.hpp>
#include <omnitool-defs.hpp>
#include <omnitool-module.hpp>
#include <omnitool-threads.hpp>

namespace xia {
namespace omnitool {
namespace module {
struct test_fifo_worker : public thread::module_worker {
    size_t length;

    test_fifo_worker();
    void worker(command::context& context, pixie::module::module& module);
};

test_fifo_worker::test_fifo_worker() : length(0) {}

void test_fifo_worker::worker(
    command::context& , pixie::module::module& module) {
    try {
        std::ofstream out(
          std::string(omnitool::list_mode_prefix) + "-control-task-11.lmd",
            std::ios::out | std::ios::binary);
        module.start_test(pixie::module::module::test::lm_fifo);
        const size_t poll_period_usecs = 10 * 1000;
        period.start();
        while (total < length) {
            size_t data_available = module.read_list_mode_level();
            if (data_available > 0) {
                pixie::hw::words lm;
                module.read_list_mode(lm);
                total += lm.size();
                out.write(reinterpret_cast<char*>(lm.data()), lm.size() * sizeof(uint32_t));
            }
            if (data_available == 0) {
                pixie::hw::wait(poll_period_usecs);
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

void test(command::context& context) {
    auto& crate = context.crate;
    auto mode_opt = context.cmd.get_option("-m");
    auto mod_nums_opt = context.cmd.get_arg();
    auto mode = pixie::module::module::test::off;
    if (!mode_opt.empty()) {
        if (mode_opt == "lmfifo") {
            mode = pixie::module::module::test::lm_fifo;
        } else if (mode_opt != "off") {
            throw std::runtime_error(std::string("invalid test mode: " + mode_opt));
        }
    }
    command::module_range mod_nums;
    command::modules_option(mod_nums, mod_nums_opt, crate.num_modules);
    for (auto mod_num : mod_nums) {
        crate[mod_num].run_check();
    }
    size_t bytes = 500 * 1024 * 1000;
    auto tests = std::vector<test_fifo_worker>(mod_nums.size());
    thread::set_num_slot(crate, mod_nums, tests);
    for (auto& t : tests) {
        t.length = (bytes) / sizeof(xia::pixie::hw::word);
    }
    context.opts.out << "Test: " << mode_opt << " (" << int(mode)
                     << ") length=" << util::io::humanize(bytes)
                     << std::endl;
    thread::module_threads(
        context, mod_nums, tests, "fifo test error; see log");
    performance_stats(context, tests, true);
}

void test_comp(
    command::context& context, command::completion& completions) {
    auto test_cmd = context.cmd.def;

    command::completions::flag_handler flags_func =
    [&context, &completions, &test_cmd](auto flag) {
        if (flag == "-m") {
            command::completion_entries entries;
            entries.push_back({command::completion_entry::node::argument, "off",
                test_cmd.name, "", "off"});
            entries.push_back({command::completion_entry::node::argument, "lmfifo",
                test_cmd.name, "", "lmfifo"});

            if (!completions.incomplete) {
                for (auto entry : entries) {
                    completions.add(entry);
                }
            } else {
                auto arg = completions.argv(completions.argc() - 1);
                for (auto entry : entries) {
                    if (completions.partial_match(entry.name, arg)) {
                        completions.add(entry);
                    }
                }
            }
        }
    };

    auto not_completed = !command::completions::flag_completion(flags_func,
        test_cmd.name, completions);
    if (not_completed) {
        auto off = command::completions::get_pos_arg_offset(
            test_cmd.name, completions);
        if (off != 0) {
            command::completions::modules_completions(
                context, test_cmd.name, off, completions);
        }
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
