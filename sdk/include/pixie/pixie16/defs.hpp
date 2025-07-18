/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2021 XIA LLC, All rights reserved.
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

/** @file defs.hpp
 * @brief Defines hardware constants used by the SDK.
 */

#ifndef PIXIE_HW_DEFS_H
#define PIXIE_HW_DEFS_H

#include <cstdint>

#include <pixie/pixie16/hw.hpp>

namespace xia {
namespace pixie {
namespace hw {
/**
 * @brief Defines device level memory addresses used to access data
 *          directly on the hardware.
 */
struct device {
    /*
     * IO FPGA, also called COMM or BOOT CPLD
     */
    static const address CFG_DATACS = 0x00;
    static const address CFG_CTRLCS = 0x04;
    static const address CFG_RDCS = 0x08;
    static const address CFG_RDVER = 0x0c;
    static const address I2CM24C64 = 0x10;
    static const address PCF8574 = 0x10;
    static const address CFG_DCMRST = 0x14;
    static const address CFG_DAC = 0x18;
    /*
     * System FPGA
     */
    static const address CSR = 0x48;
    static const address REQUEST_HBR = 0x80;
    static const address HBR_DONE = 0x84;
    static const address WRT_EXT_MEM = 0xc0;
    static const address EXT_MEM_TEST = 0xc4;
    static const address WRT_DSP_MMA = 0xc8;
    static const address SET_EXT_FIFO = 0xcc;
    static const address WRT_DSP_DMAC11 = 0xd0;
    static const address WRT_DSP_II11 = 0xd4;
    static const address WRT_CLR_EXTMEM = 0xd8;
    static const address WRT_DSP_C11 = 0xdc;
    static const address WRT_DSP_IM11 = 0xe0;
    static const address WRT_DSP_EC11 = 0xe4;
    static const address RD_WRT_FIFO_WML = 0xe8;
    static const address SET_EXMEM_FIFO = 0xec;
    static const address SET_INT_FIFO = 0xf0;
    static const address WRT_RESET_RFCNT = 0xf4;
};

/**
 * @brief Defines FIPPI  memory addresses used to access data
 *          directly on the hardware.
 *
 * Addresses are per FIPPI device. Use @ref fippi_addr to get host bus address.
 */
struct fippi {
    /*
     * Device selection
     */
    static constexpr address select_bit = 12;
    static constexpr address channel_select_bit = 6;
    static constexpr address select_mask = 0xf;
    /*
     * DSP visible registers
     */
    static constexpr address CSRIN = 0x00800400;
    static constexpr address COINCPATTERN = 0x00800402;
    static constexpr address ADCCTRL = 0x00800404;
    static constexpr address HOSTCLR = 0x00800408;
    static constexpr address ADCSPI = 0x0080040a;
    static constexpr address BITSLIP = 0x0080040c;
    static constexpr address HDR_IDS = 0x00800412;
    static constexpr address ADCFIFOCTRL = 0x00800418;
    static constexpr address GROUPMODE_ABCD = 0x0080041a;
    static constexpr address GROUPMODE_EFGH = 0x0080041c;
    static constexpr address GROUPMODE_FIP = 0x0080041e;
    static constexpr address VALID_WINDOW = 0x00800420;
    static constexpr address ADCFRAME = 0x00800428;
    /*
     * Input/Output registers are 64bit wide.
     *
     * The host uses the LSB of the address to access each half of the
     * 64 register.
     */
    static constexpr address P16_00_L = 0x00800001;  /* RW */
    static constexpr address P16_00_H = 0x00800000;  /* RW */
    static constexpr address P16_01_L = 0x00800003;  /* RW */
    static constexpr address P16_01_H = 0x00800002;  /* RW */
    static constexpr address P16_02_L = 0x00800005;  /*  W */
    static constexpr address P16_02_H = 0x00800004;  /*  W */
    static constexpr address P16_03_L = 0x00800007;  /*  W */
    static constexpr address P16_05_L = 0x0080000b;  /*  W */
    static constexpr address P16_05_H = 0x0080000a;  /*  W */
    static constexpr address P16_06_L = 0x0080000d;  /*  W */
    static constexpr address P16_06_H = 0x0080000c;  /*  W */
    static constexpr address P16_07_L = 0x0080000f;  /*  W */
    static constexpr address P16_07_H = 0x0080000e;  /*  W */
    static constexpr address P16_08_L = 0x00800011;  /* R  */
    static constexpr address P16_08_H = 0x00800010;  /* R  */
    static constexpr address P16_09_L = 0x00800013;  /* R  */
    static constexpr address P16_09_H = 0x00800013;  /* R  */
    static constexpr address P16_10_L = 0x00800015;  /* R  */
    static constexpr address P16_10_H = 0x00800014;  /* R  */
    static constexpr address P16_11_L = 0x00800017;  /* R  */
    static constexpr address P16_11_H = 0x00800016;  /* R  */
    static constexpr address P16_12_L = 0x00800019;  /* R  */
    static constexpr address P16_12_H = 0x00800018;  /* R  */
    static constexpr address P16_13_L = 0x0080001b;  /*  W */
    static constexpr address P16_17_U = 0x00800022;  /*  W */
    static constexpr address P16_23_L = 0x0080002f;  /*  W */
    static constexpr address P16_23_H = 0x0080002e;  /*  W */
    static constexpr address P16_24_L = 0x00800031;  /*  W */
    static constexpr address P16_24_H = 0x00800030;  /*  W */
    static constexpr address P16_25_L = 0x00800033;  /* R  */
    static constexpr address P16_25_H = 0x00800032;  /* R  */
    static constexpr address P16_26_L = 0x00800035;  /* R  */
    static constexpr address P16_26_H = 0x00800034;  /* R  */
    static constexpr address P16_27_L = 0x00800037;  /* R  */
    static constexpr address P16_27_H = 0x00800036;  /* R  */
    static constexpr address P16_28_L = 0x00800039;  /* R  */
    static constexpr address P16_28_H = 0x00800038;  /* R  */
    static constexpr address P16_29_L = 0x0080003b;  /* R  */
    static constexpr address P16_29_H = 0x0080003a;  /* R  */
    /*
     * Logical mappings of the I/O registers
     */
    static constexpr address FIPPI_DAC = P16_03_L;
};
/**
 * @brief Return the host bus address for a FIPPI register given
 *          the FIPPI device to address and the register.
 */
static inline address fippi_addr(int device, address reg) {
    return reg | ((device & fippi::select_mask) << fippi::select_bit);
}
/**
 * @brief Return the host bus address for a FIPPI register given
 *          the FIPPI device to address, the channel and the register.
 */
static inline address fippi_addr(int device, int channel, address reg) {
    return reg |
        ((device & fippi::select_mask) << fippi::select_bit) |
        ((channel & fippi::select_mask) << fippi::channel_select_bit);
}
/**
 * @brief Defines various bit numbers used to decode/encode data from the hw.
 */
struct bit {
    /*
     * Host PCI IO CPLD CTRL register bit definitions
     */
    /**
     *  Select System FPGA
     */
    static constexpr size_t CPLDCSR_SEL_SYS_FPGA = 0;
    /**
     *  PROGB System FPGA
     */
    static constexpr size_t CPLDCSR_PROGB_SYS_FPGA = 1;
    /**
     *  Select FIPPI 0 FPGA
     */
    static constexpr size_t CPLDCSR_SEL_FIPPI_0_FPGA = 4;
    /**
     *  PROGB FIPPI 0 FPGA
     */
    static constexpr size_t CPLDCSR_PROGB_FIPPI_0_FPGA = 5;
    /**
     *  Select FIPPI 1 FPGA
     */
    static constexpr size_t CPLDCSR_SEL_FIPPI_1_FPGA = 6;
    /**
     *  PROGB FIPPI 1 FPGA
     */
    static constexpr size_t CPLDCSR_PROGB_FIPPI_1_FPGA = 7;
    /**
     *  Select FIPPI 2 FPGA
     */
    static constexpr size_t CPLDCSR_SEL_FIPPI_2_FPGA = 8;
    /**
     *  PROGB FIPPI 2 FPGA
     */
    static constexpr size_t CPLDCSR_PROGB_FIPPI_2_FPGA = 9;
    /**
     *  Select FIPPI 3 FPGA
     */
    static constexpr size_t CPLDCSR_SEL_FIPPI_3_FPGA = 10;
    /**
     *  PROGB FIPPI 3 FPGA
     */
    static constexpr size_t CPLDCSR_PROGB_FIPPI_3_FPGA = 11;
    /**
     *  Control connections of PXI nearest neighbor lines (J2) onto the
     *  backplane for Rev-B/C/D modules
     */
    static const size_t CPLDCSR_BPCONNECT = 12;
    /**
     * Control backplane pullups: 1: pulled up, 0: not pulled up
     */
    static const size_t CPLDCSR_PULLUP = 13;
    /*
     * Host Bus Request bit definitions. Used on rev H and later and don't on
     * earlier versions.
     */
    /*
     * HBR pin
     */
    static const size_t HDR_HBR = 0;
    /*
     * DSP CS pin, active low for the DSP to have the bus
     */
    static const size_t HBR_DSP_nCS = 1;
    /*
     * Auto-increment after read
     */
    static const size_t HBR_AUTO_INC = 2;
    /*
     * Read another word from new address after rread
     */
    static const size_t HBR_READ_AGAIN_READ = 3;
    /*
     * Host PCI CSR bit definitions
     */
    /**
     * Enable run
     */
    static const size_t RUNENA = 0;
    /**
     * Enable DSP code download
     */
    static const size_t DSPDOWNLOAD = 1;
    /**
     * Indicate PCI I/O is active
     */
    static const size_t PCIACTIVE = 2;
    /**
     * Control pull-up for the SYNC lines
     */
    static const size_t PULLUP_CTRL = 3;
    /**
     * Reset DSP
     */
    static const size_t DSPRESET = 4;
    /**
     * External FIFO watermark level indicator
     */
    static const size_t EXTFIFO_WML = 6;
    /**
     * Run active indicator
     */
    static const size_t RUNACTIVE = 13;
    /**
     * Clearing external memory active indicator
     */
    static const size_t CLREXTMEM_ACTIVE = 15;

    /**
     * Control pullups for PXI trigger lines on the backplane through CPLD
     */
    static const size_t MODCSRB_CPLDPULLUP = 0;
    /**
     * Control the director mode for multi-crate sync runs
     */
    static const size_t MODCSRB_DIRMOD = 4;
    /**
     * Set this module as the Director module (1) or non-Director module (0)
     */
    static const size_t MODCSRB_CHASSISMASTER = 6;

    /*
     * CHANCSRA bits definitions
     */
    /**
     * fast trigger selection - 1: select external fast trigger; 0: select
     * group trigger
     */
    static const size_t CCSRA_FTRIGSEL = 0;
    /**
     * module validation signal selection - 1: select module gate signal; 0:
     * select global validation signal (RevD & RevF only)
     */
    static const size_t CCSRA_EXTTRIGSEL = 1;
    /**
     * good-channel bit - 1: channel data will be read out; 0: channel data
     * will not be read out
     */
    static const size_t CCSRA_GOOD = 2;
    /**
     * channel validation signal selection - 1: select channel gate signal; 0:
     * select channel validation signal (RevD & RevF only)
     */
    static const size_t CCSRA_CHANTRIGSEL = 3;
    /**
     * block data acquisition if trace or header DPMs are full - 1: enable; 0:
     * disable
     */
    static const size_t CCSRA_SYNCDATAACQ = 4;
    /**
     * input signal polarity control
     */
    static const size_t CCSRA_POLARITY = 5;
    /**
     * veto channel trigger - 1: enable; 0: disable
     */
    static const size_t CCSRA_VETOENA = 6;
    /**
     * histogram energy in the on-board MCA
     */
    static const size_t CCSRA_HISTOE = 7;
    /**
     * trace capture and associated header data - 1: enable; 0: disable
     */
    static const size_t CCSRA_TRACEENA = 8;
    /**
     * QDC summing and associated header data - 1: enable; 0: dsiable
     */
    static const size_t CCSRA_QDCENA = 9;
    /**
     * CFD for real time, trace capture and QDC capture - 1: enable; 0: disable
     */
    static const size_t CCSRA_CFDMODE = 10;
    /**
     * global trigger for validation - 1: enable; 0: disable
     */
    static const size_t CCSRA_GLOBTRIG = 11;
    /**
     * raw energy sums and baseline in event header - 1: enable; 0: disable
     */
    static const size_t CCSRA_ESUMSENA = 12;
    /**
     * channel trigger for validation - 1: enable; 0: disable
     */
    static const size_t CCSRA_CHANTRIG = 13;
    /**
     * Control input relay: 1: connect, 0: disconnect
     */
    static const size_t CCSRA_ENARELAY = 14;
};
/**
 * @brief Defines various bit masks used to manage the hw.
 */
struct mask {
    /**
     *  PROGB ALL FPGAS
     */
    static constexpr size_t CPLDCSR_PROGB_ALL_FPGAS =
        1 << bit::CPLDCSR_PROGB_SYS_FPGA |
        1 << bit::CPLDCSR_PROGB_FIPPI_0_FPGA | 1 << bit::CPLDCSR_PROGB_FIPPI_1_FPGA |
        1 << bit::CPLDCSR_PROGB_FIPPI_2_FPGA | 1 << bit::CPLDCSR_PROGB_FIPPI_3_FPGA;
};

/**
 * @brief Data structure defining I2C device addresses
 *
 * @note these value must be even
 */
struct i2c_device {
    static constexpr size_t mb_eeprom = 0xa0;
    static constexpr size_t mb_config_pio = 0x42;
    static constexpr size_t mb_gain_pio_0 = 0x44;
    static constexpr size_t mb_gain_pio_1 = 0x48;
};

/**
 * @brief Data structure defining limits for variables and parameters.
 */
struct limit {
    static const size_t CFDDELAY_MAX = 63;
    static const size_t CFDDELAY_MIN = 1;
    static const size_t CFDSCALE_MAX = 7;
    static const size_t CFDTHRESH_MAX = 65535;
    static const size_t CFDTHRESH_MIN = 1;
    static const size_t CHANTRIGSTRETCH_MAX = 4095;
    static const size_t CHANTRIGSTRETCH_MIN = 1;
    static const size_t DAC_VOLTAGE_RANGE = 3;
    static const size_t DSP_CLOCK_MHZ = 100;
    static const size_t EXTDELAYLEN_MAX_REVBCD = 255;
    static const size_t EXTDELAYLEN_MAX_REVF = 511;
    static const size_t EXTDELAYLEN_MIN = 0;
    static const size_t EXTTRIGSTRETCH_MAX = 4095;
    static const size_t EXTTRIGSTRETCH_MIN = 1;
    static const size_t FASTFILTERRANGE_MAX = 0;
    static const size_t FASTFILTERRANGE_MIN = 0;
    static const size_t FASTFILTER_MAX_LEN = 127;
    static const size_t FASTLENGTH_MIN_LEN = 2;
    static const size_t FASTTRIGBACKLEN_MAX = 4095;
    static const size_t FASTTRIGBACKDELAY_MIN = 0;
    static const size_t FASTTRIGBACKLEN_MIN_100MHZFIPCLK = 1;
    static const size_t FASTTRIGBACKLEN_MIN_125MHZFIPCLK = 2;
    static const size_t FAST_THRESHOLD_MAX = 65535;
    static const size_t QDCLEN_MAX = 32767;
    static const size_t QDCLEN_MIN = 1;
    static const size_t RESET_DELAY_MAX = 255;
    static const size_t SLOWFILTERRANGE_MAX = 6;
    static const size_t SLOWFILTERRANGE_MIN = 1;
    static const size_t SLOWFILTER_MAX_LEN = 127;
    static const size_t SLOWGAP_MIN_LEN = 3;
    static const size_t SLOWLENGTH_MIN_LEN = 2;
    static const size_t TRACEDELAY_MAX = 1023;
    static const size_t TRACELENGTH_MAX_REVH = 4000;
    static const size_t TRACE_STEP_SIZE_REVH = 32;
    static const size_t VETOSTRETCH_MAX = 4095;
    static const size_t VETOSTRETCH_MIN = 1;
};
}  // namespace hw
}  // namespace pixie
}  // namespace xia

#endif  // PIXIE_HW_DEFS_H
