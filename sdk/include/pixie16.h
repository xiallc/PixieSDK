#ifndef PIXIE_H
#define PIXIE_H

/**----------------------------------------------------------------------
* Copyright (c) 2005 - 2021, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------**/
/// @file pixie.h
/// @brief C wrappers for the C++ API that expose the same signature as the legacy code
/// @author C. Johns
/// @date March 29, 2021

#include <xia_windows_compat.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup PUBLIC_API Public API
 * Functions for use in end-user software.
 */

/**
 * @defgroup CONTROL_TASK_RUN Control Task functions
 * @ingroup PUBLIC_API
 * A group of functions used to execute specific tasks on the module.
 */

/**
 * @defgroup RUN_STATISTICS Run Statistics
 * @ingroup PUBLIC_API
 * A group of functions used calculate run statistics from the return of
 * Pixie16ReadStatisticsFromModule.
 * @see Pixie16ReadStatisticsFromModule
 */

/**
* @defgroup CONFIGURATION Configuration Management
* @ingroup PUBLIC_API
* A group of functions used to setup and configure the modules
*/

/**
 * @defgroup DATA_PROCESSING Data Processing
 * @ingroup PUBLIC_API
 * A group of functions used to process data received from the Pixie modules.
 */

/**
* @defgroup LIST_MODE_DATA_PROCESSING List-Mode Data Processing
* @ingroup PUBLIC_API
* A group of functions used to process List-Mode data from the modules
*/

PIXIE_EXPORT double PIXIE_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

PIXIE_EXPORT unsigned int PIXIE_API Decimal2IEEEFloating(double DecimalNumber);

/**
 * @ingroup PUBLIC_API
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to acquire ADC traces from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its ADC traces acquired. But if ModNum is
 * equal to the total number of modules in the system, then all modules in the system will have
 * their ADC traces acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with ADC
 * trace data. A user’s application software should then call the function
 * Pixie16ReadSglChanADCTrace to read the ADC trace data out to the host computer,
 * channel by channel.
 *
 * @see Pixie16ReadSglChanADCTrace
 *
 * @param[in] ModNum: The module number that we'll be working with
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start run
 * @retval -3 - Acquiring ADC traces timed out
 */
PIXIE_EXPORT int PIXIE_API PixieAcquireADCTrace(unsigned short ModNum);  // module number

/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Acquire baselines
 *
 * Use this function to acquire baselines from Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its baselines acquired. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their baselines acquired.
 *
 * After the successful return of this function, the DSP’s internal memory will be filled with
 * baselines data. Users should then call the function Pixie16ReadSglChanBaselines to read the
 * baselines data out to the host computer, channel by channel.
 *
 * @see Pixie16ReadSglChanBaselines
 *
 * @param[in] ModNum: The module number to get baselines from
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the GET_BASELINES run
 * @retval -3 - GET_BASELINES run timed out
 */
PIXIE_EXPORT int PIXIE_API PixieAcquireBaselines(unsigned short ModNum);  // module number

/**
 * @ingroup CONTROL_TASK_RUN
 * @brief Adjust DC-Offsets
 *
 * Use this function to adjust the DC-offsets of Pixie-16 modules. Specify the module using ModNum
 * which starts counting at 0. If ModNum is set to be less than the total number of modules in the
 * system, only the module specified by ModNum will have its DC-offsets adjusted. But if ModNum is
 * set to be equal to the total number of modules in the system, then all modules in the system
 * will have their DC-offsets adjusted.
 *
 * After the DC-offset levels have been adjusted, the baseline level of the digitized input
 * signals will be determined by the DSP parameter BaselinePercent. For instance, if
 * BaselinePercent is set to 10(%), the baseline level of the input signals will be ~ 409 on the
 * 12-bit ADC scale (minimum: 0; maximum: 4095).
 *
 * The main purpose of this function is to ensure the input signals fall within the voltage range
 * of the ADCs so that all input signals can be digitized by the ADCs properly.
 *
 * @param[in] ModNum: The module number to adjust baselines for
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to start the ADJUST_OFFSETS run
 * @retval -3 - ADJUST_OFFSETS run timed out after 10 seconds of attempts
 */
PIXIE_EXPORT int PIXIE_API PixieAdjustOffsets(unsigned short ModNum);

/**
 * @ingroup PUBLIC_API
 *
 * @brief Boot one or more modules in the system.
 *
 * Use this function to boot Pixie modules so that they can be set up for data taking. The
 * function downloads to the Pixie modules the communication (or system) FPGA configurations,
 * signal processing FPGA configurations, trigger FPGA configurations (Revision A modules only),
 * executable code for the digital signal processor (DSP), and DSP parameters.
 *
 * The FPGA configurations consist of a fixed number of words depending on the hardware mounted on
 * the modules; the DSP codes have a length which depends on the actual compiled code; and the set
 * of DSP parameters always consists of 1280 32-bit words for each module. The host software has to
 * make the names of those boot data files on the hard disk available to the boot function.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will be booted. But
 * if ModNum is equal to the total number of modules in the system, e.g. there are 5 modules in
 * the chassis and ModNum = 5, then all modules in the system will be booted.
 *
 * #### Boot Pattern Bits
 * The boot pattern is a bit mask (shown below) indicating which on-board chip will be booted.
 * Under normal circumstances, all on-board chips should be booted, i.e. the boot pattern would
 * be 0x7F. For Rev-B, C, D, F modules, bit 1, i.e., “Boot trigger FPGA”, will be ignored even if
 * that bit is set to 1.
 * | Bit | Description | Applicable Hardware |
 * |---|---|---|
 * | 0 | Boot communication FPGA  | All Pixie Revisions  |
 * | 1 | Boot trigger FPGA  | Revision A - Ignored  |
 * | 2 | Boot signal processing FPGA  | All Pixie Revisions  |
 * | 3 | Boot digital signal processor (DSP)  | All Pixie Revisions  |
 * | 4 | Download DSP I/O parameters  | All Pixie Revisions  |
 * | 5 | Program on-board FPGAs  | All Pixie Revisions  |
 * | 6 | Set on-board DACs  | All Pixie Revisions  |
 *
 * @param[in] ComFPGAConfigFile: name of communications FPGA configuration file
 * @param[in] SPFPGAConfigFile: name of signal processing FPGA configuration file
 * @param[in] TrigFPGAConfigFile: name of trigger FPGA configuration file (!!! IGNORED)
 * @param[in] DSPCodeFile: name of executable code file for digital signal processor (DSP)
 * @param[in] DSPParFile: name of DSP parameter file
 * @param[in] DSPVarFile: name of DSP variable names file
 * @param[in] ModNum: The module number. If set to Number_Modules we'll boot all modules.
 * @param[in] BootPattern: boot pattern bit mask
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid module number
 * @retval -2 - Size of ComFPGAConfigFile is invalid
 * @retval -3 - Could not read the ComFPGAConfig file
 * @retval -4 - Failed to allocate memory to store ComFPGAConfig
 * @retval -5 - Failed to open ComFPGAConfigFile
 * @retval -10 - Size of SPFPGAConfigFile is invalid
 * @retval -11 - Failed to read SPFPGAConfigFile
 * @retval -12 - Failed to allocate memory to store SPFPGAConfigFile
 * @retval -13 - Failed to open SPFPGAConfigFile
 * @retval -14 - Failed to read DSPCodeFile
 * @retval -15 - Failed to allocate memory to store DSP executable code
 * @retval -16 - Failed to open DSPCodeFile
 * @retval -17 - Size of DSPParFile is invalid
 * @retval -18 - Failed to open DSPParFile
 * @retval -19 - Can't initialize DSP variable indices
 * @retval -20 - Can't copy DSP variable indices
 * @retval -21 - Failed to program Fippi in a module
 * @retval -22 - Failed to set DACs in a module
 * @retval -23 - Failed to start RESET_ADC run in a module
 * @retval -24 - RESET_ADC run timed out in a module
 * @retval -25 - Failed to boot the Communication FPGA
 * @retval -26 - Failed to boot signal processing FPGA
 * @retval -27 - Failed to boot DSP
 * @retval -28 - Failed to read DSPParFile
 */
PIXIE_EXPORT int PIXIE_API PixieBootModule(const char* ComFPGAConfigFile,
                                           const char* SPFPGAConfigFile,
                                           const char* TrigFPGAConfigFile, const char* DSPCodeFile,
                                           const char* DSPParFile, const char* DSPVarFile,
                                           unsigned short ModNum, unsigned short BootPattern);

/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Read the number of 32-bit words that the external FIFO currently has.
 *
 * Use this function to check the status of the external FIFO of a Pixie-16 module while a
 * list-mode data acquisition run is in progress. The function returns the number of words
 * (32-bit) that the external FIFO currently has. If the number of words is greater than a
 * user-set threshold, function Pixie16ReadDataFromExternalFIFO can then be used to read the data
 * from the external FIFO. The threshold can be set by the user to either minimize reading
 * overhead or to read data out of the FIFO as quickly as possible. The Pixie-16 API
 * (pixie16app_defs.h) has defined a threshold with value of 1024 for external FIFO read out
 * (EXTFIFO_READ_THRESH).
 *
 * @see Pixie16ReadDataFromExternalFIFO
 *
 * @param[out] nFIFOWords: The number of 32-bit unsigned integer words contained in the module's
 *     External FIFO
 * @param[in] ModNum: The module number to read from. Numbering starts counting at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 */
PIXIE_EXPORT int PIXIE_API PixieCheckExternalFIFOStatus(unsigned int* nFIFOWords, unsigned short ModNum);

/**
 * @ingroup PUBLIC_API
 * @brief Poll the run status of a Pixie module.
 *
 * Use this function to check the run status of a Pixie-16 module while a list-mode data
 * acquisition run is in progress. If the run is still in progress continue polling.
 *
 * If the return code of this function indicates the run has finished, there might still be some
 * data in the external FIFO (Rev-B, C, D, F modules) that need to be read out to the host
 * computer. In addition, final run statistics and histogram data are available for reading out too.
 *
 * In MCA histogram run mode, this function can also be called to check if the run is still in
 * progress even though it is normally self-terminating.
 *
 * @param[in] ModNum: The module number to interrogate and counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - No run is in progress
 * @retval  1 - Run is still in progress
 * @retval -1 - Invalid Pixie module number
 */
PIXIE_EXPORT int PIXIE_API PixieCheckRunStatus(unsigned short ModNum);

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute input count rate of a channel
 *
 * Use this function to calculate the input count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function, another
 * function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @note The units on the return value are triggers per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. The number of triggers divided by the live time in seconds
 *     otherwise.
 */
PIXIE_EXPORT double PIXIE_API PixieComputeInputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                    unsigned short ChanNum);

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute live time that a channel accumulated in a run
 *
 * Use this function to calculate the live time that one channel of a Pixie-16 module has spent on
 * data acquisition. This function does not communicate with Pixie-16 modules. Before calling this
 * function, another function, Pixie16ReadStatisticsFromModule, should be called to read
 * statistics data from the module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number, which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number, which starts counting at 0.
 * @return The live time of the module in seconds.
 */
PIXIE_EXPORT double PIXIE_API PixieComputeLiveTime(unsigned int* Statistics, unsigned short ModNum,
                              unsigned short ChanNum);

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute output count rate of a channel
 *
 * Use this function to calculate the output count rate on one channel of a Pixie-16 module. This
 * function does not communicate with Pixie-16 modules. Before calling this function, another
 * function, Pixie16ReadStatisticsFromModule, should be called to read statistics data from the
 * module.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @note The return value's units are channel events per second.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number which starts counting at 0.
 * @param[in] ChanNum: ChanNum is the channel number which starts counting at 0.
 * @return 0 if the live time was 0. Otherwise, the number of channel events divided by the
 *     live time in seconds.
 */
PIXIE_EXPORT double PIXIE_API PixieComputeOutputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                     unsigned short ChanNum);

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute number of events processed by a module
 *
 * @note This function is only used by Rev-A modules.
 *
 * Use this function to calculate the number of events that have been processed by a Pixie-16
 * module during a data acquisition run. This function does not communicate with Pixie-16 modules.
 * Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be
 * called to read statistics data from the module first.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: ModNum is the module number, which starts counting at 0.
 * @return The number of events processed by the module.
 */
PIXIE_EXPORT double PIXIE_API PixieComputeProcessedEvents(unsigned int* Statistics, unsigned short ModNum);

/**
 * @ingroup RUN_STATISTICS
 * @brief Compute real time that a module accumulated in a run
 *
 * Use this function to calculate the real time that a Pixie-16 module has spent on data
 * acquisition.This function does not communicate with Pixie-16 modules.
 * Before calling this function, another function, Pixie16ReadStatisticsFromModule, should be
 * called to read statistics data from the module first.
 *
 * The *Statistics array is filled with data from a Pixie-16 module after calling function
 * Pixie16ReadStatisticsFromModule.
 *
 * @see Pixie16ReadStatisticsFromModule
 *
 * @param[in] Statistics: A pointer to the statistics array whose size is exactly 448 unsigned
 *    integer words (32-bit).
 * @param[in] ModNum: The module number, which starts counting at 0.
 * @return The number of seconds that the module spent on data acquisition.
 */
PIXIE_EXPORT double PIXIE_API PixieComputeRealTime(unsigned int* Statistics, unsigned short ModNum);

/**
 * @ingroup PUBLIC_API
 * @brief Stop the run in a Pixie module.
 *
 * Use this function to end a histogram run, or to force the end of a list-mode run. In a
 * multi-module system, if all modules are running synchronously, only one module needs to be
 * addressed this way. It will immediately stop the run in all other module in the system.
 *
 * When stopping the run in the Director module (module #0) - a SYNC interrupt should be generated
 * to stop run in all modules simultaneously, if DSP parameter SynchWait is set to 1.
 * If SynchWait is set to 0, then CSR bit 0 is cleared to stop the run.
 *
 * @param[in] ModNum: The module number to interrogate and counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - No run is in progress
 * @retval -1 - Invalid Pixie module number
 */
PIXIE_EXPORT int PIXIE_API PixieEndRun(unsigned short ModNum);

/**
 * @ingroup PUBLIC_API
 *
 * @brief Close the modules and release resources used by PCI devices before
 * exiting the application.
 *
 * @param[in] ModNum: The module number that we'll be closing.
 * @returns A status code indicating the result of the operation
 * @retval:  0 - Success
 * @retval: -1 - Invalid Pixie module number
 * @retval: -2 - Failed to close Pixie module
 */
PIXIE_EXPORT int PIXIE_API PixieExitSystem(unsigned short ModNum);

/**
 * @ingroup PUBLIC_API
 *
 * @brief Initializes the system by mapping PXI slots to modules
 *
 * Use this function to configure the Pixie modules in the PXI chassis.
 *
 * NumModules is the total number of Pixie-16 modules installed in the system. PXISlotMap is the
 * pointer to an array that must have at least as many entries as there are Pixie-16 modules in the
 * chassis.
 *
 * PXISlotMap serves as a simple mapping of the logical module number and the physical slot number
 * that the modules reside in. The logical module number runs from 0. For instance, in a system
 * with 5 Pixie modules, these 5 modules may occupy slots 3 through 7. The user must fill
 * PXISlotMap as follows: PXISlotMap = {3, 4, 5, 6, 7 ...} since module number 0 resides in slot
 * number 3, etc. To find out in which slot a module is located, any piece of subsequent code can
 * use the expression PXISlotMap[ModNum], where ModNum is the logic module number.
 * OfflineMode is used to indicate to the API whether the system is running in OFFLINE mode (1) or
 * ONLINE mode (0). OFFLINE mode is useful for situations where no Pixie-16 modules are present but
 * users can still test their calls to the API functions in their application software.
 * This function must be called as the first step in the boot process. It makes the modules known
 * to the system and “opens” each module for communication.
 *
 * @param[in] NumModules: The total number of Pixie modules in the system provided by the user. If
 *     0 all slots are check for modules automatically.
 * @param[in] PXISlotMap: An array containing the PXI slot number for each module. The array index
 *     indicates which module number the slot number takes. The first slot number is Module 0.
 * @param[in] OfflineMode: Used to tell the API that there are no modules connected to the system.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid total number of Pixie modules
 * @retval -2 - Null pointer *PXISlotMap.
 * @retval -3 - Failed to initialize system
 * @retval -4 - Failed to read the module's information
 */
PIXIE_EXPORT int PIXIE_API PixieInitSystem(unsigned short NumModules, unsigned short* PXISlotMap,
                                           unsigned short OfflineMode);

/**
 * @ingroup LIST_MODE_DATA_PROCESSING
 * @brief Read data from the external FIFO.
 *
 * This function reads list mode data from the external FIFO of a Pixie-16 module. The data are
 * 32-bit unsigned integers. Normally, function Pixie16CheckExternalFIFOStatus is called first to
 * see how many words the external FIFO currently has, and then this function is called to read
 * the data from the FIFO.
 *
 * @see Pixie16CheckExternalFIFOStatus
 *
 * @param[out] ExtFIFO_Data: A pointer to a memory block containing enough space for `nFIFOWords`
 *    worth of 32-bit unsigned integers.
 * @param[in] nFIFOWords: The number of words that we're going to be reading.
 * @param[in] ModNum: The module number that we'll read from. Numbering starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Successful
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to read data from external FIFO
 */
PIXIE_EXPORT int PIXIE_API PixieReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data, unsigned int nFIFOWords,
                                    unsigned short ModNum);

/**
 * @ingroup DATA_PROCESSING
 * @brief Retrieve a channel's histogram data from a Pixie module.
 *
 * Use this function to read out the histogram data from a Pixie-16 module’s histogram memory.
 * Before calling this function, the host code should allocate appropriate amount of memory to
 * store the histogram data. The default histogram length is 32768. Histogram data are 32-bit
 * unsigned integers.
 *
 * @param[out] Histogram: The histogram data that we read from the module.
 * @param[in] NumWords: The number of words we'll read out of the histogram memory.
 * @param[in] ModNum: The module number that we want the histogram from
 * @param[in] ChanNum: The channel number that we'd like to read from
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Failed to get the histogram data. If this happens reboot the module.
 */
PIXIE_EXPORT int PIXIE_API PixieReadHistogramFromModule(unsigned int* Histogram, unsigned int NumWords,
                                   unsigned short ModNum, unsigned short ChanNum);

/**
 * @ingroup PUBLIC_API
 * @brief Read information stored on each module, including its revision, serial number, and ADC.
 *
 * Use this function to read information stored on each module, including its revision, serial
 * number, ADC bits and sampling rate. This should be done after initializing the PCI communication.
 * Information from the module can be used to select the appropriate firmware, DSP, and
 * configuration parameters files before booting the module.
 *
 * @param[in] ModNum: The module number (counts from 0) that we'll read information
 * @param[out] ModRev: The revision read from the on-board EEPROM
 * @param[out] ModSerNum: The serial number read from the on-board EEPROM
 * @param[out] ModADCBits: The ADC bit resolution read from the on-board EEPROM
 * @param[out] ModADCMSPS: The ADC sampling frequency read from the on-board EEPROM
 * @returns A status code indicating the result of the operation
 * @retval:  0 - Success
 * @retval: -1 - Invalid Pixie module number
 * @retval: -2 - Failed to read the serial number from I2C serial EEPROM
 * @retval: -3 - Failed to read the ADC information from I2C serial EEPROM
 */
PIXIE_EXPORT int PIXIE_API PixieReadModuleInfo(unsigned short ModNum, unsigned short* ModRev,
                                               unsigned int* ModSerNum, unsigned short* ModADCBits,
                                               unsigned short* ModADCMSPS);

/**
 * @ingroup PUBLIC_API
 * @brief Acquire ADC traces from a Pixie module.
 *
 * Use this function to read ADC trace data from a Pixie-16 module. Before calling this function,
 * another function Pixie16AcquireADCTrace should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the trace data. The
 * ADC trace data length for each channel is 8192. Since the trace data are 16-bit unsigned
 * integers (for hardware variants with less than 16-bit ADCs only the lower 12-bit or 14-bit
 * contain real data), two consecutive 16-bit words are packed into one 32-bit word in the DSP
 * internal memory. So for each channel, 4096 32-bit words are read out first from the DSP, and
 * then each 32-bit word is unpacked to form two 16-bit words.
 *
 * Specify the module using ModNum and the channel on the module using ChanNum. Note that both the
 * modules and channels are counted starting at 0.
 *
 * @see Pixie16AcquireADCTrace
 *
 * @param[out] Trace_Buffer: Pointer to the data buffer that will hold the data.
 * @param[in] Trace_Length: Length of the traces for the module.
 * @param[in] ModNum: The module that we want to read
 * @param[in] ChanNum: The channel that we want to read
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid trace length
 * @retval -4 - Failed to allocate memory to store ADC traces
 * @retval -5 - Failed to read ADC traces
 */
PIXIE_EXPORT int PIXIE_API PixieReadSglChanADCTrace(unsigned short* Trace_Buffer, unsigned int Trace_Length,
                               unsigned short ModNum, unsigned short ChanNum);

/**
 * @ingroup CONFIGURATION
 * @brief Read baselines from a channel in a module
 *
 * Use this function to read baseline data from a Pixie-16 module. Before calling this function,
 * another function Pixie16AcquireBaselines should be called to fill the DSP internal memory first.
 * Also, the host code should allocate appropriate amount of memory to store the baseline data.
 * The number of baselines for each channel is 3640. In the DSP internal memory, each baseline is
 * a 32-bit IEEE floating point number. After being read out to the host, this function will
 * convert each baseline data to a decimal number. In addition to baseline values, timestamps
 * corresponding to each baseline are also returned after this function call.
 *
 * @see Pixie16AcquireBaselines
 *
 * @param[out] Baselines: Buffer to hold the baseline values
 * @param[in] TimeStamps: The timestamp that the baselines were obtained at
 * @param[in] NumBases: The number of baselines values read
 * @param[in] ModNum: The module number to read the baselines from, counting from 0.
 * @param[in] ChanNum: The channel number to read the baselines from, counting from 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Requested number of baselines exceeded the limit
 * @retval -3 - Failed to allocate memory to store baselines
 * @retval -4 - Failed to read baselines
 */
PIXIE_EXPORT int PIXIE_API PixieReadSglChanBaselines(double* Baselines, double* TimeStamps, unsigned short NumBases,
                                unsigned short ModNum, unsigned short ChanNum);

/**
 * @ingroup CONFIGURATION
 * @brief Read a channel parameter from a module.
 *
 * #### List of Channel Parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | TRIGGER_RISETIME | &mu;s | user set | FASTLENGTH |
 * | TRIGGER_FLATTOP | &mu;s | user set | FASTGAP |
 * | TRIGGER_THRESHOLD | ADC units | user set | FASTTHRESH |
 * | ENERGY_RISETIME | &mu;s | user set | SLOWLENGTH |
 * | ENERGY_FLATTOP | &mu;s | user set | SLOWGAP |
 * | TAU | &mu;s | user set | PREAMPTAU |
 * | TRACE_LENGTH | &mu;s | user set | TRACELENGTH |
 * | TRACE_DELAY | &mu;s | user set | TRIGGERDELAY, PAFLENGTH |
 * | VOFFSET | V | user set | OFFSETDAC |
 * | XDT | &mu;s | user set | XWAIT |
 * | BASELINE_PERCENT | % | user set | BASELINEPERCENT |
 * | EMIN | None | user set | ENERGYLOW |
 * | BINFACTOR | None | user set | LOG2EBIN |
 * | BASELINE_AVERAGE | None | user set | LOG2BWEIGHT |
 * | CHANNEL_CSRA | bit pattern | user set | CHANCSRA |
 * | CHANNEL_CSRB | bit pattern | user set | CHANCSRB |
 * | BLCUT | None | user set/auto API | BLCUT |
 * | INTEGRATOR | None | user set | INTEGRATOR |
 * | FASTTRIGBACKLEN | &mu;s | user set | FASTTRIGBACKLEN |
 * | CFDDelay | &mu;s | user set | CFDDELAY |
 * | CFDScale | None | user set | CFDSCALE |
 * | CFDThresh | ADC units | user set | CFDTHRESH |
 * | QDCLen0 | &mu;s | user set | QDCLEN0 |
 * | QDCLen1 | &mu;s | user set | QDCLEN1 |
 * | QDCLen2 | &mu;s | user set | QDCLEN2 |
 * | QDCLen3 | &mu;s | user set | QDCLEN3 |
 * | QDCLen4 | &mu;s | user set | QDCLEN4 |
 * | QDCLen5 | &mu;s | user set | QDCLEN5 |
 * | QDCLen6 | &mu;s | user set | QDCLEN6 |
 * | QDCLen7 | &mu;s | user set | QDCLEN7 |
 * | ExtTrigStretch | &mu;s | user set | EXTTRIGSTRETCH |
 * | VetoStretch | &mu;s | user set | VETOSTRETCH |
 * | MultiplicityMaskL | bit pattern | user set | MULTIPLICITYMASKL |
 * | MultiplicityMaskH | bit pattern | user set | MULTIPLICITYMASKH |
 * | ExternDelayLen | &mu;s | user set | EXTERNDELAYLEN |
 * | FtrigoutDelay | &mu;s | user set | FTRIGOUTDELAY |
 * | ChanTrigStretch | &mu;s | user set | CHANTRIGSTRETCH |
 *
 * @param[in] ChanParName: The name of the channel parameter that we'll read
 * @param[out] ChanParData: Contains the value we read from the channel.
 * @param[in] ModNum: The module number we'll read from. Counting starts at 0.
 * @param[in] ChanNum: The channel number we'll read from. Counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid channel parameter name
 */
PIXIE_EXPORT int PIXIE_API PixieReadSglChanPar(const char* ChanParName, double* ChanParData, unsigned short ModNum,
                          unsigned short ChanNum);

/**
 * @ingroup CONFIGURATION
 * @brief Read a module parameter from a Pixie module
 *
 * #### List of module parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | MODULE_CSRA | bit pattern | user set | MODCSRA |
 * | MODULE_CSRB | bit pattern | user set | MODCSRB |
 * | MODULE_FORMAT | None | auto DSP | MODFORMAT |
 * | MAX_EVENTS | None | user set/auto DSP | MAXEVENTS |
 * | SYNCH_WAIT | logic (0, 1) | user set | SYNCHWAIT |
 * | IN_SYNCH | logic (0, 1) | user/DSP set | INSYNCH |
 * | SLOW_FILTER_RANGE | None | user set | SLOWFILTERRANGE |
 * | FAST_FILTER_RANGE | None | user set | FASTFILTERRANGE |
 * | FastTrigBackplaneEna | bit pattern | user set | FASTTRIGBACKPLANEENA |
 * | CrateID | None | user set | CRATEID |
 * | SlotID | None | user set | SLOTID |
 * | ModID | None | user set | MODID |
 * | TrigConfig0 | None | user set | TRIGCONFIG[0] |
 * | TrigConfig1 | None | user set | TRIGCONFIG[1] |
 * | TrigConfig2 | None | user set | TRIGCONFIG[2] |
 * | TrigConfig3 | None | user set | TRIGCONFIG[3] |
 * | HOST_RT_PRESET | seconds | user set | HOSTRUNTIMEPRESET |
 *
 * @param[in] ModParName: The name of the module parameter that we'll read
 * @param[out] ModParData: Contains the value of the requested parameters.
 * @param[in] ModNum: The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid module parameter name
 */
PIXIE_EXPORT int PIXIE_API PixieReadSglModPar(const char* ModParName, unsigned int* ModParData, unsigned short ModNum);

/**
 * @ingroup DATA_PROCESSING
 * @brief Retrieve statistics data from a module.
 *
 * Use this function to read out statistics data from a Pixie-16 module. Before calling this
 * function, the host code should allocate appropriate amount of memory to store the statistics
 * data. The number of statistics data for each module is fixed at 448. Statistics data are 32-bit
 * unsigned integers.
 *
 * @param[out] Statistics: Pointer to a data block with size 448 32-bit unsigned integers to hold
 *    the data read from the module.
 * @param[in] ModNum: The module number that we want statistics data from.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get statistics data. If this happens, then reboot the module.
 */
PIXIE_EXPORT int PIXIE_API PixieReadStatisticsFromModule(unsigned int* Statistics, unsigned short ModNum);

/**
 * @ingroup CONFIGURATION
 * @brief Save DSP parameters to a settings file
 *
 * Use this function to save DSP parameters to a settings file. It will first read the values of
 * DSP parameters on each Pixie-16 module and then write them to the settings file. Each module has
 * exactly 1280 DSP parameter values (32-bit unsigned integers), and depending on the value of
 * PRESET_MAX_MODULES (defined in pixie16app_defs.h), the settings file should have exactly
 * `(1280 * PRESET_MAX_MODULES * 4)` bytes when stored on the computer hard drive.
 *
 * @param[in] FileName: Absolute path to where we will write the DSP parameter file.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Failed to read DSP parameter values from the Pixie-16 modules
 * @retval -2 - Failed to open the DSP parameters file
 * @retval -3 - Failed to write DSP parameter values
 */
PIXIE_EXPORT int PIXIE_API PixieSaveDSPParametersToFile(const char* FileName);

/**
 * @ingroup DATA_PROCESSING
 * @brief Retrieve histogram data from a Pixie module and then save the data to a file.
 *
 * Use this function to read histogram data from a Pixie-16 module and save the histogram data to
 * a file with file name specified by the user. First this function saves the histogram data to a
 * binary file, and it then saves the histogram data to an ASCII file with run statistics data
 * appended to the end of the ASCII file. **Existing files will be overwritten.**
 *
 * @param[in] FileName: The file name for the file containing the histogram data
 * @param[in] ModNum: The module number we'll read histogram data from. Counting from 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Failed to get histogram data from module
 * @retval -3 - Failed to allocate memory to store histogram data for binary file
 * @retval -4 - Failed to open histogram data file
 * @retval -5 - Failed to open mca ascii output file
 * @retval -6 - Failed to allocate memory to store histogram data for ascii text file
 * @retval -7 - Failed to read histogram data from file
 * @retval -8 - Failed to read run statistics data from module
 */
PIXIE_EXPORT int PIXIE_API PixieSaveHistogramToFile(const char* FileName, unsigned short ModNum);

/**
 * @ingroup PUBLIC_API
 * @brief Start or resume a histogram run.
 *
 * Use this function to begin a data acquisition run that accumulates energy histograms, one for
 * each channel. It launches a data acquisition run in which only energy information is preserved
 * and histogrammed locally to each channel.
 *
 * Call this function for each Pixie-16 module in the system to initialize the run in each module.
 * Actual data acquisition will start synchronously in all modules when the last module finished
 * the initialization (requires the synchronization parameter to be set). Histogram runs can be
 * self-terminating when the elapsed run time exceeds the preset run time, or the user can
 * prematurely terminate the run by calling Pixie16EndRun. On completion, final histogram and
 * statistics data will be available.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its
 * histogram run started. But if ModNum is set to be equal to the total number of modules in the
 * system, then all modules in the system will have their runs started together.
 *
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 * @param[in] mode: How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Use mode=RESUME_RUN (=0) to resume an earlier run.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid mode
 * @retval -3 - Failed to start histogram run
 */
PIXIE_EXPORT int PIXIE_API PixieStartHistogramRun(unsigned short ModNum, unsigned short mode);

/**
 * @ingroup PUBLIC_API
 * @brief Start or resume a list-mode run.
 *
 * Use this function to start a list-mode data acquisition run in Pixie-16 modules. list-mode runs
 * are used to collect data on an event-by-event basis, gathering energies, timestamps, pulse shape
 * analysis values, and waveforms for each event. Runs will continue until the user terminates the
 * run by calling function Pixie16EndRun. To start the data acquisition this function has to be
 * called for every Pixie-16 module in the system. If all modules are to run synchronously, the
 * last module addressed will release all others and the acquisition starts then. The first module
 * to end the run will immediately stop the run in all other modules if run synchronization has
 * been set up among these modules.
 *
 * There is only one list-mode run type supported, that is, 0x100. However, different output data
 * options can be chosen by enabling or disabling different CHANCSRA bits.
 *
 * ModNum is the module number which starts counting at 0. If ModNum is set to be less than the
 * total number of modules in the system, only the module specified by ModNum will have its list
 * mode run started. But if ModNum is set to equal to the total number of modules in the system,
 * then all modules in the system will have their runs started together.
 *
 * @param[in] ModNum: The module number that we'd like to work against. Starts counting at 0.
 * @param[in] RunType: The type of run that we're going to be executing. There's only one 0x100.
 * @param[in] mode: How we'll handle existing data when starting up the run. Use mode=NEW_RUN (=1)
 *     to erase histograms and statistics information before launching the new
 *     run. Note that this will cause a startup delay of up to 1 millisecond.
 *     Use mode=RESUME_RUN (=0) to resume an earlier run. This mode has a startup delay of only a
 *     few microseconds.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid mode
 * @retval -3 - Failed to start list-mode run
 * @retval -4 - Invalid run type
 */
PIXIE_EXPORT int PIXIE_API PixieStartListModeRun(unsigned short ModNum, unsigned short RunType, unsigned short mode);

/**
 * @ingroup CONFIGURATION
 * @brief Write a channel parameter to a Pixie module.
 *
 * #### List of Channel Parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | TRIGGER_RISETIME | &mu;s | user set | FASTLENGTH |
 * | TRIGGER_FLATTOP | &mu;s | user set | FASTGAP |
 * | TRIGGER_THRESHOLD | ADC units | user set | FASTTHRESH |
 * | ENERGY_RISETIME | &mu;s | user set | SLOWLENGTH |
 * | ENERGY_FLATTOP | &mu;s | user set | SLOWGAP |
 * | TAU | &mu;s | user set | PREAMPTAU |
 * | TRACE_LENGTH | &mu;s | user set | TRACELENGTH |
 * | TRACE_DELAY | &mu;s | user set | TRIGGERDELAY, PAFLENGTH |
 * | VOFFSET | V | user set | OFFSETDAC |
 * | XDT | &mu;s | user set | XWAIT |
 * | BASELINE_PERCENT | % | user set | BASELINEPERCENT |
 * | EMIN | None | user set | ENERGYLOW |
 * | BINFACTOR | None | user set | LOG2EBIN |
 * | BASELINE_AVERAGE | None | user set | LOG2BWEIGHT |
 * | CHANNEL_CSRA | bit pattern | user set | CHANCSRA |
 * | CHANNEL_CSRB | bit pattern | user set | CHANCSRB |
 * | BLCUT | None | user set/auto API | BLCUT |
 * | INTEGRATOR | None | user set | INTEGRATOR |
 * | FASTTRIGBACKLEN | &mu;s | user set | FASTTRIGBACKLEN |
 * | CFDDelay | &mu;s | user set | CFDDELAY |
 * | CFDScale | None | user set | CFDSCALE |
 * | CFDThresh | ADC units | user set | CFDTHRESH |
 * | QDCLen0 | &mu;s | user set | QDCLEN0 |
 * | QDCLen1 | &mu;s | user set | QDCLEN1 |
 * | QDCLen2 | &mu;s | user set | QDCLEN2 |
 * | QDCLen3 | &mu;s | user set | QDCLEN3 |
 * | QDCLen4 | &mu;s | user set | QDCLEN4 |
 * | QDCLen5 | &mu;s | user set | QDCLEN5 |
 * | QDCLen6 | &mu;s | user set | QDCLEN6 |
 * | QDCLen7 | &mu;s | user set | QDCLEN7 |
 * | ExtTrigStretch | &mu;s | user set | EXTTRIGSTRETCH |
 * | VetoStretch | &mu;s | user set | VETOSTRETCH |
 * | MultiplicityMaskL | bit pattern | user set | MULTIPLICITYMASKL |
 * | MultiplicityMaskH | bit pattern | user set | MULTIPLICITYMASKH |
 * | ExternDelayLen | &mu;s | user set | EXTERNDELAYLEN |
 * | FtrigoutDelay | &mu;s | user set | FTRIGOUTDELAY |
 * | ChanTrigStretch | &mu;s | user set | CHANTRIGSTRETCH |
 *
 * @param[in] ChanParName: The name of the channel parameter that we'll write
 * @param[in] ChanParData: The value that we'll write to the provided channel
 * @param[in] ModNum: The module number we'll write to. Counting starts at 0.
 * @param[in] ChanNum: The channel number we'll write to. Counting starts at 0.
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid Pixie channel number
 * @retval -3 - Invalid channel parameter name
 * @retval -4 - Programing Fippi failed downloading channel parameter
 * @retval -5 - BLcutFinder failed downloading channel parameter
 * @retval -6 - SetDACs failed downloading channel parameter
 */
PIXIE_EXPORT int PIXIE_API PixieWriteSglChanPar(const char* ChanParName, double ChanParData, unsigned short ModNum,
                           unsigned short ChanNum);

/**
 * @ingroup CONFIGURATION
 * @brief Write a module parameter to a Pixie module
 *
 * #### List of module parameters
 * | Name | Unit | Type | DSP Name |
 * |-|-|-|-|
 * | MODULE_CSRA | bit pattern | user set | MODCSRA |
 * | MODULE_CSRB | bit pattern | user set | MODCSRB |
 * | MODULE_FORMAT | None | auto DSP | MODFORMAT |
 * | MAX_EVENTS | None | user set/auto DSP | MAXEVENTS |
 * | SYNCH_WAIT | logic (0, 1) | user set | SYNCHWAIT |
 * | IN_SYNCH | logic (0, 1) | user/DSP set | INSYNCH |
 * | SLOW_FILTER_RANGE | None | user set | SLOWFILTERRANGE |
 * | FAST_FILTER_RANGE | None | user set | FASTFILTERRANGE |
 * | FastTrigBackplaneEna | bit pattern | user set | FASTTRIGBACKPLANEENA |
 * | CrateID | None | user set | CRATEID |
 * | SlotID | None | user set | SLOTID |
 * | ModID | None | user set | MODID |
 * | TrigConfig0 | None | user set | TRIGCONFIG[0] |
 * | TrigConfig1 | None | user set | TRIGCONFIG[1] |
 * | TrigConfig2 | None | user set | TRIGCONFIG[2] |
 * | TrigConfig3 | None | user set | TRIGCONFIG[3] |
 * | HOST_RT_PRESET | seconds | user set | HOSTRUNTIMEPRESET |
 *
 * @param[in] ModParName: The name of the module parameter that we'll write
 * @param[in] ModParData: The value of the module parameter that we'll write
 * @param[in] ModNum: The module number we'll write to. Numbering starts counting at 0.
 *
 * @returns A status code indicating the result of the operation
 * @retval  0 - Success
 * @retval -1 - Invalid Pixie module number
 * @retval -2 - Invalid module parameter name
 * @retval -3 - Failed to program Fippi after downloading module parameter
 * @retval -4 - Failed to find BLcut after downloading module parameter
 */
PIXIE_EXPORT int PIXIE_API PixieWriteSglModPar(const char* ModParName, unsigned int ModParData, unsigned short ModNum);

#ifdef __cplusplus
}
#endif

#endif /* PIXIE_H */
