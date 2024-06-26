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
 * @brief Crate Import command
 */

#include <pixie/utils/time.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool-commands.hpp>

namespace xia {
namespace omnitool {
namespace crate {
void import(command::context& context) {
    auto& crate = context.crate;
    auto path_opt = context.cmd.get_arg();
    util::time::timepoint tp;
    pixie::module::number_slots modules;
    crate->check_active_run();
    tp.start();
    crate->import_config(path_opt, modules);
    crate->initialize_afe();
    tp.end();
    context.opts.out << "Modules imported: " << modules.size()
                     << " time=" << tp << std::endl;
}

void import_comp(command::context& , command::completion& ) {
}
} // namespace crate
} // namespace omnitool
} // namespace xia
