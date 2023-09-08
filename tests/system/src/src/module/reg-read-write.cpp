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
 * @brief Module Regster Read command
 */

#include <pixie/utils/io.hpp>

#include <pixie/pixie16/crate.hpp>
#include <pixie/pixie16/fpga_comms.hpp>
#include <pixie/pixie16/fpga_fippi.hpp>
#include <pixie/pixie16/memory.hpp>
#include <pixie/pixie16/module.hpp>

#include <omnitool/commands.hpp>

#include <nolhmann/json.hpp>

namespace xia {
namespace omnitool {
namespace module {
/*
 * JSON support
 */
using json = nlohmann::json;

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

static void get_memory_reg(
    const std::string& mem_reg, std::string& mem, std::string& reg,
    const std::string& label) {
    reg = mem_reg;
    util::string::tolower(reg);
    auto has_mem_field = mem_reg.find(':') != std::string::npos;
    if (has_mem_field) {
        util::string::strings mr;
        util::string::split(mr, mem_reg, ':');
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

static pixie::hw::address get_address(
    const std::string& mem, const std::string& reg, const std::string& label) {
    pixie::hw::address address = 0;
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

void reg_read(command::context& context) {
    const std::string label = "reg-read";
    auto& crate = context.crate;
    auto slot_opt = context.cmd.get_option("-s");
    auto hex_opt = context.cmd.get_option("-x");
    auto mod_slot_opt = context.cmd.get_arg();
    auto reg_opt = context.cmd.get_arg();
    size_t mod_slot = util::io::get_value<size_t>(mod_slot_opt);
    std::string mem;
    std::string reg;
    get_memory_reg(reg_opt, mem, reg, label);
    pixie::hw::address address = get_address(mem, reg, label);
    if (slot_opt == "true") {
        auto mod = crate.find(mod_slot);
        mod_slot = mod->number;
    }
    auto& mod = crate[mod_slot];
    pixie::hw::word value;
    if (mem == "sys") {
        value = mod.read_word(address);
    } else {
        pixie::hw::fpga::comms comms(mod);
        if (!comms.done()) {
            throw std::runtime_error("reg-read: comms/sys FPGA not loaded");
        }
        if (mem == "fippi") {
            auto fippi = pixie::hw::memory::fippi(mod);
            value = fippi.read(address);
        } else if (mem == "dsp") {
            auto dsp = pixie::hw::memory::dsp(mod);
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

void reg_write(command::context& context) {
    const std::string label = "reg-write";
    auto slot_opt = context.cmd.get_option("-s");
    auto& crate = context.crate;
    auto mod_slot_opt = context.cmd.get_arg();
    auto reg_opt = context.cmd.get_arg();
    auto value_opt = context.cmd.get_arg();
    size_t mod_slot = util::io::get_value<size_t>(mod_slot_opt);
    std::string mem;
    std::string reg;
    get_memory_reg(reg_opt, mem, reg, label);
    pixie::hw::address address = get_address(mem, reg, label);
    if (slot_opt == "true") {
        auto mod = crate.find(mod_slot);
        mod_slot = mod->number;
    }
    auto& mod = crate[mod_slot];
    pixie::hw::word value =
        util::io::get_value<pixie::hw::word>(value_opt);
    if (mem == "sys") {
        mod.write_word(address, value);
    } else {
        pixie::hw::fpga::comms comms(mod);
        if (!comms.done()) {
            throw std::runtime_error("reg-write: comms/sys FPGA not loaded");
        }
        if (mem == "fippi") {
            auto fippi = pixie::hw::memory::fippi(mod);
            fippi.write(address, value);
        } else if (mem == "dsp") {
            auto dsp = pixie::hw::memory::dsp(mod);
            dsp.write(address, value);
        }
    }
}
} // namespace module
} // namespace omnitool
} // namespace xia
