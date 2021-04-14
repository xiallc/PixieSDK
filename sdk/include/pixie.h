#ifndef PIXIE_H
#define PIXIE_H

/*----------------------------------------------------------------------
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
*----------------------------------------------------------------------*/

#include <xia_windows_compat.hpp>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup PUBLIC_API Public API
 * Functions for use in end-user software.
 */

PIXIE_EXPORT double PIXIE_API IEEEFloating2Decimal(unsigned int IEEEFloatingNumber);

PIXIE_EXPORT unsigned int PIXIE_API Decimal2IEEEFloating(double DecimalNumber);

PIXIE_EXPORT int PIXIE_API PixieAcquireADCTrace(unsigned short ModNum);  // module number

PIXIE_EXPORT int PIXIE_API PixieAcquireBaselines(unsigned short ModNum);  // module number

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

PIXIE_EXPORT int PIXIE_API PixieCheckExternalFIFOStatus(unsigned int* nFIFOWords, unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API PixieCheckRunStatus(unsigned short ModNum);

PIXIE_EXPORT double PIXIE_API PixieComputeInputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                    unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API PixieComputeLiveTime(unsigned int* Statistics, unsigned short ModNum,
                              unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API PixieComputeOutputCountRate(unsigned int* Statistics, unsigned short ModNum,
                                     unsigned short ChanNum);

PIXIE_EXPORT double PIXIE_API PixieComputeProcessedEvents(unsigned int* Statistics, unsigned short ModNum);

PIXIE_EXPORT double PIXIE_API PixieComputeRealTime(unsigned int* Statistics, unsigned short ModNum);

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

PIXIE_EXPORT int PIXIE_API PixieReadDataFromExternalFIFO(unsigned int* ExtFIFO_Data, unsigned int nFIFOWords,
                                    unsigned short ModNum);

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

PIXIE_EXPORT int PIXIE_API PixieReadSglChanADCTrace(unsigned short* Trace_Buffer, unsigned int Trace_Length,
                               unsigned short ModNum, unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API PixieReadSglChanBaselines(double* Baselines, double* TimeStamps, unsigned short NumBases,
                                unsigned short ModNum, unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API PixieReadSglChanPar(const char* ChanParName, double* ChanParData, unsigned short ModNum,
                          unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API PixieReadSglModPar(const char* ModParName, unsigned int* ModParData, unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API PixieReadStatisticsFromModule(unsigned int* Statistics, unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API PixieSaveDSPParametersToFile(const char* FileName);

PIXIE_EXPORT int PIXIE_API PixieSaveHistogramToFile(const char* FileName, unsigned short ModNum);

PIXIE_EXPORT int PIXIE_API PixieStartHistogramRun(unsigned short ModNum, unsigned short mode);

PIXIE_EXPORT int PIXIE_API PixieStartListModeRun(unsigned short ModNum, unsigned short RunType, unsigned short mode);

PIXIE_EXPORT int PIXIE_API PixieWriteSglChanPar(const char* ChanParName, double ChanParData, unsigned short ModNum,
                           unsigned short ChanNum);

PIXIE_EXPORT int PIXIE_API PixieWriteSglModPar(const char* ModParName, unsigned int ModParData, unsigned short ModNum);

#ifdef __cplusplus
}
#endif

#endif /* PIXIE_H */
