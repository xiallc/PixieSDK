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

/** @file stats.cpp
 * @brief Implements functions and data structures related to stats reporting
 */

#include <nolhmann/json.hpp>

#include <pixie/stats.hpp>

#include <pixie/reports/reports.hpp>
#include <pixie/utils/io.hpp>

namespace xia {
namespace reports {

void stats_report(pixie::module::module& module_, std::ostream& out) {
    pixie::stats::stats stats(module_);
    module_.read_stats(stats);
    pixie::module::module::fifo_stats snapshot;
    snapshot = module_.run_stats;

    nlohmann::json json_out = {{"module_id", module_.slot},
                               {"stats_time", std::chrono::duration_cast<std::chrono::milliseconds>(
                                                  std::chrono::system_clock::now().time_since_epoch())
                                                  .count()},
                               {"real_time", stats.mod.real_time()},
                               {"list-mode",
                                {{"dma_in", snapshot.get_dma_in_bytes()},
                                 {"dropped", snapshot.dropped.load()},
                                 {"hw_overflows", snapshot.hw_overflows.load()},
                                 {"in", snapshot.get_in_bytes()},
                                 {"out", snapshot.get_out_bytes()},
                                 {"min_bandwidth", snapshot.min_bandwidth.load()},
                                 {"max_bandwidth", snapshot.max_bandwidth.load()},
                                 {"bandwidth", snapshot.bandwidth.load()}}},
                               {"hardware", nlohmann::json::array()}};

    for (auto& ch : stats.chans) {
        double output_efficiency = 0;
        if (ch.input_counts() != 0) {
            output_efficiency = ch.output_counts() / ch.input_counts();
        }
        json_out["hardware"].emplace_back(
            nlohmann::json({{"channel_number", ch.config.index},
                            {"live_time", ch.live_time()},
                            {"dead_time", 1. - ch.live_time() / stats.mod.real_time()},
                            {"input_counts", ch.input_counts()},
                            {"input_count_rate", ch.input_count_rate()},
                            {"output_counts", ch.output_counts()},
                            {"output_count_rate", ch.output_count_rate()},
                            {"output_efficiency", output_efficiency}}));
    }
    out << json_out;
}

}
}