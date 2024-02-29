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

/** @file module-threads.hpp
 * @brief Onmitool module thread workers
 */

#ifndef ONMITOOL_THREAD_HPP
#define ONMITOOL_THREAD_HPP

#include <pixie/error.hpp>
#include <pixie/utils/io.hpp>
#include <pixie/utils/thread.hpp>
#include <pixie/utils/time.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/crate-view.hpp>
#include <pixie/pixie16/hw.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace thread {
/*
 * Module worker base
 */
struct module_worker {
    using slot_type = pixie::hw::slot_type;

    int number;
    slot_type slot;
    int serial_num;
    int revision;
    int pci_bus;
    int pci_slot;

    std::atomic_bool running;
    bool has_error;
    util::time::timepoint period;
    size_t total;
    size_t last_total;

    module_worker();

    virtual void worker(
        command::context& context,
        pixie::module::module& module) = 0;
};

/**
 * If enabled show the performance of the module as it is running.
 */
template<typename W>
void module_performance(
    command::context& context, std::vector<W>& workers, bool& show_performance,
    util::time::timepoint& duration, util::time::timepoint& interval,
    size_t show_secs, size_t running) {
    if (show_performance && interval.secs() > show_secs) {
        auto secs = interval.secs();
        interval.restart();
        context.opts.out << "running: " << running << std::endl;
        size_t all_total = 0;
        for (auto& w : workers) {
            if (w.period.secs() > 0) {
                auto total = w.total;
                auto bytes = (total - w.last_total) * sizeof(pixie::hw::word);
                auto rate = double(bytes) / secs;
                all_total += total;
                bytes = total * sizeof(pixie::hw::word);
                char active = w.running.load() ? '>' : ' ';
                w.last_total = total;
                std::ostringstream oss;
                oss << ' ' << active << std::setw(2) << w.slot
                    << ": total: " << std::setw(8)
                    << util::io::humanize(bytes) << " rate: " << std::setw(8)
                    << util::io::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                    << " slot=" << w.pci_slot;
                context.opts.out << oss.str() << std::endl;
                xia_log(log::info) << oss.str();
            } else {
                context.opts.out << ' ' << std::setw(2) << w.slot
                                 << ": not running" << std::endl;
            }
        }
        all_total *= sizeof(pixie::hw::word);
        std::ostringstream oss;
        oss << " all: total: " << std::setw(8) << util::io::humanize(all_total)
            << " rate: " << std::setw(8)
            << util::io::humanize(double(all_total) / duration.secs()) << " bytes/sec";
        context.opts.out << oss.str() << std::endl;
        xia_log(log::info) << oss.str();
    }
}

/**
 * Print the performance statistics after the workers have finished.
 */
template<typename W>
void performance_stats(
    omnitool::command::context& context, std::vector<W>& workers, bool show_workers = false) {
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
                xia_log(log::info) << he_oss.str();
            }
            std::stringstream dr_oss;
            auto bytes = w.total * sizeof(pixie::hw::word);
            auto rate = double(bytes) / w.period.secs();
            dr_oss << "module: num:" << std::setw(2) << w.number << " slot:" << std::setw(2)
                   << w.slot << ": data received: "
                   << std::setw(8) << util::io::humanize(bytes)
                   << " bytes (" << std::setw(9) << bytes << "), rate: " << std::setw(8)
                   << util::io::humanize(rate) << " bytes/sec pci: bus=" << w.pci_bus
                   << " slot=" << w.pci_slot;
            context.opts.out << dr_oss.str() << std::endl;
            xia_log(log::info) << dr_oss.str();
        }
    }
    total *= sizeof(pixie::hw::word);
    std::stringstream oss;
    oss << "data received: " << util::io::humanize(total) << " bytes (" << total
        << "), rate: " << util::io::humanize(double(total) / secs, " bytes/sec");
    context.opts.out << oss.str() << std::endl;
    xia_log(log::info) << oss.str();
}

/**
 * Run module workers in a thread each.
 */
template<typename W>
void module_threads(
    command::context& context, command::module_range& mod_nums, std::vector<W>& workers,
    std::string error_message, bool show_performance = true) {
    if (workers.size() != mod_nums.size()) {
        throw std::runtime_error("workers and modules count mismatch");
    }
    auto& crate = context.crate;
    namespace thread = util::thread;
    thread::workers threads(mod_nums.size());
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        threads[m].body =
            [&context, &module = crate[mod_nums[m]], &worker = workers[m]]() {
            worker.worker(context, module);
        };
        threads[m].start();
    }
    util::time::timepoint duration(true);
    util::time::timepoint interval(true);
    size_t show_secs = 5;
    thread::waiter_func waiter =
        [&context, &workers, &show_performance, &duration, &interval, &show_secs](
            thread::workers::size_type running) {
            pixie::hw::wait(20 * 1000);
            module_performance(
                context, workers, show_performance, duration, interval,
                show_secs, running);
        };
    thread::finished_func thread_finished =
        [&context, &workers](thread::workers::size_type t) {
            workers[t].period.stop();
        };
    thread::error_func thread_error =
        [&context](thread::workers::size_type t, thread::error::code e) {
            context.opts.out << "module " << t << ": error: "
                             << pixie::error::api_result_text(e)
                             << std::endl;
        };
    thread::wait_until_finished(
        threads, waiter, thread_finished, thread_error, error_message);
}

/**
 * Set the module details in the worker.
 */
template<typename W>
void set_num_slot(pixie::module::module& module, W& worker) {
    worker.number = module.number;
    worker.slot = module.slot;
    worker.serial_num = module.serial_num;
    worker.revision = module.revision;
    worker.pci_bus = module.pci_bus();
    worker.pci_slot = module.pci_slot();
}

/**
 * Set the module details in the workers for a crate.
 */
template<typename W>
void set_num_slot(
    command::omnitool_crate& crate, std::vector<size_t>& mod_nums,
    std::vector<W>& workers) {
    for (size_t m = 0; m < mod_nums.size(); ++m) {
        auto& module = crate[mod_nums[m]];
        auto& worker = workers[m];
        xia::omnitool::thread::set_num_slot(module, worker);
    }
}
} // namespace thread
} // namespace omnitool
} // namespace xia

#endif  // ONMITOOL_THREAD_HPP
