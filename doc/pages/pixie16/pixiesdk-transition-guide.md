# Transitioning from the Legacy API to the PixieSDK API

## Table of Contents

* [Introduction](#introduction)
* [Header files](#header-files)
* [Globally defined constants](#globally-defined-constants)
    * [Table of Defined Constants](#table-of-defined-constants)
* [Error Codes](#error-codes)
* [Functions](#functions)
    * [Table of API Functions](#table-of-api-functions)

<hr>

<a id="introduction"></a>

## Introduction

The new API has a number of differences with the Legacy API. We have not strictly followed our API
Update Policy since the Legacy API is still available for use.

We'll detail the major differences here to provide users with an idea of what to look for.
Implementations are as varied as our users. We can't cover all the uses in this document.

We're happy to help with your transition if you [email our Support Team](mailto:support@xia.com).

<a id="library-changes"></a>

## Library changes

The Legacy API originally had two libraries that users needed to link against. One was for the
application API, and the other for internal functions used by that API. We've now simplified that
process. The new API library automatically combines the internal library (PixieSDK.a) within it.
This allows us to make sure that the boundary between internal and external usages stays clear.

We've adopted CMake as a build script generator. For customers using CMake to build their own
projects, we offer `FindPixieSDK.cmake` to ease the location and linkage of these libraries.

> Software should be linked against the new API library: `Pixie16Api.so`.

<a id="header-files"></a>

## Header files

The Legacy API had several header files that users may have included in their software. These
include `pixie16app_defs.h`, `pixie16app_export.h`, and `xia_common.h`.

We have removed `xia_common.h` from the library. This header included some helper functions used by
XIA in other projects. Many of these helper functions are now included in the core language
functionality.

> All Legacy header file usages should be replaced with a single include of `pixie16.h`. <br>
> All macros taken from `xia_common.h` should either be removed, updated with the language
> standards, or internalized to the user's code.

<a id="globally-defined-constants"></a>

## Globally defined constants

The Legacy API exposed a number of globally defined constants that need to be internal to the SDK.
This includes DSP sizes, maximum lengths for objects (trace, histogram), and control register bit
values. We have internalized many of these constants. We have kept a handful of the defines
available in `pixie.h` while we finalize their implementation.

Users often calculate the size of the Statistics block using the following calculation:

Stastistics Size = `N_DSP_PAR` - `DSP_IO_BORDER`.

We now have a helper function (`Pixie16GetStatisticsSize()`) that returns the size for the user.
This allows us to make modifications to the statistics without impacts to the user's code. We've
found that customers often use the constant `RANDOMINDICES_LENGTH` to initialize trace containers.
The correct constant to use is `MAX_ADC_TRACE_LEN`.

> All usages of constants defined in `pixie16app_defs.h` or `pixie16sys_defs.h` should be removed,
> replaced, or brought into the user's code.

<a id="table-of-defined-constants"></a>

### Table of Defined Constants

Please note that we will be deprecating these constants with the Legacy API.

|Name|Value|
|----|----|
|CCSRA_ENARELAY | 14 |
|CCSRA_GOOD | 2 |
|CCSRA_POLARITY | 5 |
|EXTERNAL_FIFO_LENGTH | 131072 |
|LIST_MODE_RUN | 0x100 |
|MAX_ADC_TRACE_LEN | 8192 |
|MAX_HISTOGRAM_LENGTH | 32768 |
|MAX_NUM_BASELINES | 3640 |
|NEW_RUN | 1 |
|NUMBER_OF_CHANNELS | 16 |
|RESUME_RUN | 0 |
|SYS_MAX_NUM_MODULES | 32 |

<a id="error-codes"></a>

## Error Codes

The Legacy API functions redefined the meaning of their exit codes. `Pixie16BootModule` and
`Pixie16InitSystem` both have a `-1` error code. The meaning of that error code is different. The
new API now has consistent error codes across all functions. This provides users with much more
actionable insights into what the error was.

> All error handling should be updated to include checks on the appropriate error codes.

<a id="functions"></a>

## Functions

We have kept the function calls between the Legacy API and the SDK identical. They have the same
signature, name, and return values. We have **removed** functions that were used exclusively for GUI
support, internal QA, or data analysis.

> Existing function calls should not need to be updated. Any calls to removed functions should be
> updated.

<a id="table-of-api-functions"></a>

### Table of API Functions

|Function|Legacy API| PixieSDK API | Notes |
|----|:----:|:----:|----|
|APP16_ClrBit|X|X|Will be deprecated in SDK.|
|APP16_SetBit|X|X|Will be deprecated in SDK.|
|APP16_TstBit|X|X|Will be deprecated in SDK.|
|APP32_ClrBit|X|X|Will be deprecated in SDK.|
|APP32_SetBit|X|X|Will be deprecated in SDK.|
|APP32_TstBit|X|X|Will be deprecated in SDK.|
|IEEEFloating2Decimal|X|X||
|Decimal2IEEEFloating|X|X||
| Pixie16AcquireADCTrace |X|X||
| Pixie16AcquireBaselines |X|X||
| Pixie16AdjustOffsets |X|X||
| Pixie16BLcutFinder |X|X||
| Pixie16BootModule |X|X||
| Pixie16CheckExternalFIFOStatus |X|X||
| Pixie16CheckRunStatus |X|X||
| Pixie16ComputeFastFiltersOffline |X|--|Used as part of GUI support|
| Pixie16ComputeInputCountRate |X|X||
| Pixie16ComputeLiveTime |X|X||
| Pixie16ComputeOutputCountRate |X|X||
| Pixie16ComputeProcessedEvents |X|X|Rev A modules only!|
| Pixie16ComputeRealTime |X|X||
| Pixie16ComputeSlowFiltersOffline |X|--|Used as part of GUI support|
| Pixie16ControlTaskRun |X|--|QA use only|
| Pixie16CopyDSPParameters |X|X||
| Pixie16EMbufferIO |X|--|QA use only|
| Pixie16EndRun |X|X||
| Pixie16ExitSystem |X|X||
| Pixie16GetEventsInfo |X|--|Used as part of GUI support|
| Pixie16GetModuleEvents |X|--|Used as part of GUI support|
| Pixie16IMbufferIO |X|--|QA use only|
| Pixie16InitSystem |X|X||
| Pixie16LoadDSPParametersFromFile |X|X||
| Pixie16ProgramFippi |X|--|Part of Pixie16BootModule|
| Pixie16RampOffsetDACs (deprecated) |X|--|Deprecated for several years|
| Pixie16ReadCSR |X|--|QA use only|
| Pixie16ReadDataFromExternalFIFO |X|X||
| Pixie16ReadHistogramFromFile |X|--|Used as part of GUI support|
| Pixie16ReadHistogramFromModule |X|X||
| Pixie16ReadListModeTrace |X|--|Used as part of GUI support|
| Pixie16ReadModuleInfo |X|X||
| Pixie16ReadSglChanADCTrace |X|X||
| Pixie16ReadSglChanBaselines |X|X||
| Pixie16ReadSglChanPar |X|X||
| Pixie16ReadSglModPar |X|X||
| Pixie16ReadStatisticsFromModule |X|X||
| Pixie16RegisterIO |X|--|QA use only|
| Pixie16SaveDSPParametersToFile |X|X||
| Pixie16SaveExternalFIFODataToFile |X|--|QA use only|
| Pixie16SaveHistogramToFile |X|--||
| Pixie16SetDACs |X|X||
| Pixie16StartHistogramRun |X|X||
| Pixie16StartListModeRun |X|X||
| Pixie16TauFinder |X|X||
| Pixie16WriteCSR |X|--|QA use only|
| Pixie16WriteSglChanPar |X|X||
| Pixie16WriteSglModPar |X|X||