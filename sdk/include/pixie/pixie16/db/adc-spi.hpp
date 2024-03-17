/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2024 XIA LLC, All rights reserved.
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

/** @file adc-spi.hpp
 * @brief Defines ADC SPI driver for the FIPPI
 */

#ifndef PIXIE_DB_ADC_SPI_H
#define PIXIE_DB_ADC_SPI_H

#include <pixie/pixie16/db/db.hpp>

namespace xia {
namespace pixie {
namespace fixture {
namespace db {
/**
 * @brief ADC SPI Driver
 * There is an instance per daughter board to acesses a single ADC SPI
 * interface. The driver is concurrent.
 */
struct adc_spi_driver {
    using lock_type = std::mutex;
    using lock_guard = std::lock_guard<lock_type>;
    db& board;
    lock_type lock;
    adc_spi_driver(db& board);
    ~adc_spi_driver();
    void write_reg(channel& chan, param::value_type reg, param::value_type value);
    param::value_type read_reg(channel& chan, param::value_type reg);
};
}  // namespace db
}  // namespace fixture
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_DB_ADC_SPI_H
