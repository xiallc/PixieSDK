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

/** @file pixie16app_defs.h
 * @brief Defines user-facing constants for use with the API.
 * @note This file will be deprecated July 31, 2023
 */

#ifndef XIA_PIXIE16APP_DEFS_H
#define XIA_PIXIE16APP_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN64) || defined(_WIN32)
#define PIXIE16APP_EXPORT __declspec(dllexport)
#define PIXIE16APP_API _stdcall
#else
#define PIXIE16APP_EXPORT
#define PIXIE16APP_API
#endif

/*-------------------------------------
    Define math constants
  -------------------------------------*/

#ifndef PI
#define PI 3.14159265358979
#endif

#ifndef PI2
#define PI2 6.28318530717959
#endif


/*------------------------------------- 
    Define boot patterns
  -------------------------------------*/
#define BOOTPATTERN_COMFPGA_BIT 0
#define BOOTPATTERN_SPFPGA_BIT 2
#define BOOTPATTERN_DSPCODE_BIT 3
#define BOOTPATTERN_DSPPAR_BIT 4
#define BOOTPATTERN_PROGFIPPI_BIT 5
#define BOOTPATTERN_SETDACS_BIT 6


/*-----------------------------------------------------------------
    size of system FPGA, trigger FPGA, Fippi, DSP parameters files
  -----------------------------------------------------------------*/

// Rev-B, C, D
#define N_COM_FPGA_CONF_REVBCD 162962  // size of communications FPGA configuration (32-bit word)
#define N_SP_FPGA_CONF_REVBCD 162962  // size of signal processing FPGA configuration (32-bit word)

// Rev-F
#define N_COM_FPGA_CONF_REVF 162962  // size of communications FPGA configuration (32-bit word)
#define N_SP_FPGA_CONF_REVF 371101  // size of signal processing FPGA configuration (32-bit word)


#define N_DSP_PAR 1280  // number of DSP parameters (32-bit word)
#define DSP_IO_BORDER 832  // number of DSP I/O variables


/*-----------------------------------------------------------------
    module specifications
  -----------------------------------------------------------------*/

#define PRESET_MAX_MODULES 24  // Preset maximum number of Pixie modules
#define NUMBER_OF_CHANNELS 16


#define DSP_CLOCK_MHZ 100  // DSP clock frequency in MHz
#define SYSTEM_CLOCK_MHZ 100  // System FPGA clock frequency in MHz (used for real time counting)

#define DAC_VOLTAGE_RANGE 3.0  // Pixie-16 DAC range is -1.5 V to +1.5 V

#define MAX_ADC_TRACE_LEN 8192  // Maximum ADC trace length for a channel

/*-----------------------------------------------------------------
    run type
  -----------------------------------------------------------------*/

#define NEW_RUN 1  // New data run
#define RESUME_RUN 0  // Resume run

#define LIST_MODE_RUN 0x100  // List mode run
#define HISTOGRAM_RUN 0x301  // Histogram run


/*-----------------------------------------------------------------
    I/O mode
  -----------------------------------------------------------------*/

#define MOD_READ 1  // Host read from modules
#define MOD_WRITE 0  // Host write to modules


/*-----------------------------------------------------------------
    Data memory, buffer, histogram, and list mode data structure
  -----------------------------------------------------------------*/

#define DSP_IMBUFFER_START_ADDR 0x40000  // 32-bit wide
#define DSP_IMBUFFER_END_ADDR 0x5FFFF  // 32-bit wide

#define DSP_EMBUFFER_START_ADDR 0x0  // 32-bit wide
#define DSP_EMBUFFER_END_ADDR 0x7FFFF  // 32-bit wide

#define DATA_MEMORY_ADDRESS 0x4A000  // DSP data memory address
#define HISTOGRAM_MEMORY_ADDRESS 0x0  // histogram memory buffer in external memory
#define MAX_HISTOGRAM_LENGTH 32768  // Maximum MCA histogram length
#define IO_BUFFER_ADDRESS 0x50000  // Address of I/O output buffer
#define IO_BUFFER_LENGTH 65536  // Length of I/O output buffer
#define EXTERNAL_FIFO_LENGTH 131072  // Length of external FIFO

#define EVENT_INFO_LENGTH 68  // Information length for each event
#define CHANNEL_INFO_LENGTH 4  // Information length for each channel
#define EVENT_INFO_HEADER_LENGTH 4  // Information length for each event header


/*------------------------------------- 
	Length limits for certain DSP parameters
 --------------------------------------*/

#define FASTFILTER_MAX_LEN 127
#define FAST_THRESHOLD_MAX 65535
#define MIN_FASTLENGTH_LEN 2

#define SLOWFILTER_MAX_LEN 127
#define MIN_SLOWLENGTH_LEN 2
#define MIN_SLOWGAP_LEN 3

#define FASTFILTERRANGE_MAX 0
#define FASTFILTERRANGE_MIN 0

#define SLOWFILTERRANGE_MAX 6
#define SLOWFILTERRANGE_MIN 1

#define FASTTRIGBACKLEN_MAX 4095
#define FASTTRIGBACKLEN_MIN_100MHZFIPCLK 1
#define FASTTRIGBACKLEN_MIN_125MHZFIPCLK 2

#define CFDDELAY_MAX 63
#define CFDDELAY_MIN 1

#define CFDSCALE_MAX 7

#define CFDTHRESH_MAX 65535
#define CFDTHRESH_MIN 1

#define EXTTRIGSTRETCH_MAX 4095
#define EXTTRIGSTRETCH_MIN 1

#define VETOSTRETCH_MAX 4095
#define VETOSTRETCH_MIN 1

#define EXTDELAYLEN_MAX_REVBCD 255
#define EXTDELAYLEN_MAX_REVF 511
#define EXTDELAYLEN_MIN 0

#define FASTTRIGBACKDELAY_MAX_REVBCD 255
#define FASTTRIGBACKDELAY_MAX_REVF 511
#define FASTTRIGBACKDELAY_MIN 0

#define QDCLEN_MAX 32767
#define QDCLEN_MIN 1

#define TRACELEN_MIN_500MHZADC 10
#define TRACELEN_MIN_250OR100MHZADC 4

#define TRACEDELAY_MAX 1023

#define CHANTRIGSTRETCH_MAX 4095
#define CHANTRIGSTRETCH_MIN 1

/*------------------------------------- 
	CHANCSRA bits definitions
 --------------------------------------*/

#define CCSRA_FTRIGSEL                                                                             \
    0  // fast trigger selection - 1: select external fast trigger; 0: select group trigger
#define CCSRA_EXTTRIGSEL                                                                           \
    1  // module validation signal selection - 1: select module gate signal; 0: select global validation signal (RevD & RevF only)
#define CCSRA_GOOD                                                                                 \
    2  // good-channel bit - 1: channel data will be read out; 0: channel data will not be read out
#define CCSRA_CHANTRIGSEL                                                                          \
    3  // channel validation signal selection - 1: select channel gate signal; 0: select channel validation signal (RevD & RevF only)
#define CCSRA_SYNCDATAACQ                                                                          \
    4  // block data acquisition if trace or header DPMs are full - 1: enable; 0: disable
#define CCSRA_POLARITY 5  // input signal polarity control
#define CCSRA_VETOENA 6  // veto channel trigger - 1: enable; 0: disable
#define CCSRA_HISTOE 7  // histogram energy in the on-board MCA
#define CCSRA_TRACEENA 8  // trace capture and associated header data - 1: enable; 0: disable
#define CCSRA_QDCENA 9  // QDC summing and associated header data - 1: enable; 0: dsiable
#define CCSRA_CFDMODE                                                                              \
    10  // CFD for real time, trace capture and QDC capture - 1: enable; 0: disable
#define CCSRA_GLOBTRIG 11  // global trigger for validation - 1: enable; 0: disable
#define CCSRA_ESUMSENA 12  // raw energy sums and baseline in event header - 1: enable; 0: disable
#define CCSRA_CHANTRIG 13  // channel trigger for validation - 1: enable; 0: disable
#define CCSRA_ENARELAY 14  // Control input relay: 1: connect, 0: disconnect

// Control pileup rejection using bit 15 and 16 of ChanCSRA:
// bits[16:15]
// 00: record all events (trace, timestamps, etc., but no energy for piled-up events)
// 01: only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)
// 10: record trace, timestamps, etc., for piled-up events but do not record trace for single events
// 11: only record trace, timestamps, etc., for piled-up events (i.e., reject single events)
#define CCSRA_PILEUPCTRL 15
#define CCSRA_INVERSEPILEUP 16

#define CCSRA_ENAENERGYCUT 17  // Enable "no trace for large pulses" feature - 1: enable; 0: disable
#define CCSRA_GROUPTRIGSEL                                                                         \
    18  // Group trigger selection - 1: external group trigger; 0: local fast trigger
#define CCSRA_CHANVETOSEL                                                                          \
    19  // Channel veto selection - 1: channel validation trigger; 0: front panel channel veto
#define CCSRA_MODVETOSEL                                                                           \
    20  // Module veto selection - 1: module validation trigger; 0: front panel module veto
#define CCSRA_EXTTSENA 21  // External timestamps in event header - 1: enable; 0: disable

/*-------------------------------------
	MODCSRB bits definitions
 --------------------------------------*/

#define MODCSRB_CPLDPULLUP 0  // Control pullups for PXI trigger lines on the backplane through CPLD
#define MODCSRB_DIRMOD 4  // Set this module as the Director module (1) or non-Director module (0)
#define MODCSRB_CHASSISMASTER                                                                      \
    6  // Control chassis master module: 1: chassis master module; 0: chassis non-master module
#define MODCSRB_GFTSEL 7  // Select global fast trigger source
#define MODCSRB_ETSEL 8  // Select external trigger source
#define MODCSRB_INHIBITENA                                                                         \
    10  // Control external INHIBIT signal: use INHIBIT (1) or don't use INHIBIT (0)
#define MODCSRB_MULTCRATES                                                                         \
    11  // Distribute clock and triggers in multiple crates: multiple crates (1) or only single crate (0)
#define MODCSRB_SORTEVENTS 12  // Sort (1) or don't sort events based on their timestamps
#define MODCSRB_BKPLFASTTRIG 13  // Enable connection of fast triggers to backplane

/*-------------------------------------
	CPLD CSR bits definitions
 --------------------------------------*/

#define CPLDCSR_BPCONNECT                                                                          \
    12  // Control connections of PXI nearest neighbor lines (J2) onto the backplane for Rev-B/C/D modules
#define CPLDCSR_PULLUP 13  // Control backplane pullups: 1: pulled up, 0: not pulled up

/*------------------------------------- 
	Control parameters
 --------------------------------------*/

#define MAX_PAR_NAME_LENGTH 65  // Maximum length of parameter names
#define RANDOMINDICES_LENGTH 8192  // number of random indices (currently only used for tau finder)
#define MAX_ERRMSG_LENGTH 1024  // Maximum length of error message

#define BASELINES_BLOCK_LEN                                                                        \
    18  // Length of each baselines length (default: 2 timestamp words + 16 baselines)
#define MAX_NUM_BASELINES                                                                          \
    3640  // Maximum number of baselines available after each baseline acquisition run
#define MAX_NUM_DCVALUES                                                                           \
    16384  // Maximum number of DC values available after each RampOffsetDACs run

#define EXTFIFO_READ_THRESH 1024  // Reading out threshold for external FIFO watermmark level

#define PCI_STOPRUN_REGADDR 0x44  // PCI register address in the System FPGA for stopping run

/*------------------------------------- 
	Frequently used Control Tasks
 --------------------------------------*/

#define SET_DACS 0  // Set DACs
#define ENABLE_INPUT 1  // Enable detect signal input
#define RAMP_OFFSETDACS 3  // Ramp Offset DACs
#define GET_TRACES 4  // Acquire ADC traces
#define PROGRAM_FIPPI 5  // Program FIPPIs
#define GET_BASELINES 6  // Get baselines
#define ADJUST_OFFSETS 7  // Adjust DC-offsets
#define TAU_FINDER 8  // Find decay time Tau

#define RESET_ADC 23  // Reset ADCs

#ifdef __cplusplus
}
#endif

#endif
