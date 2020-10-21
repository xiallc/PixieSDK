Attribute VB_Name = "Pixie16_Globals"
Option Explicit

'###########################################
'
'   Constant definitions
'
'###########################################

'===========================
'   Module characteristics
'===========================

Public Const MAX_NUM_PIXIE16_MODULES = 24 ' Maximum allowed number of Pixie-16 modules
Public Const NUM_CHANNELS = 16            ' don't change number of channels
Public Const HIGH_GAIN_REVBCD = 4#        ' The voltgae gain when the input relay is closed (Rev-B,C,D)
Public Const LOW_GAIN_REVBCD = 0.9        ' The voltage gain when the iput relay is opened (Rev-B,C,D)
Public Const HIGH_GAIN_REVF = 2.5         ' The voltgae gain when the input relay is closed (Rev-F)
Public Const LOW_GAIN_REVF = 0.625        ' The voltage gain when the iput relay is opened (Rev-F)
Public Const MAX_HISTOGRAM_LENGTH = 32768 ' Maximum length of MCA histogram

Public Const DSP_CLOCK_MHZ = 100          ' DSP clock in MHz

'===========================
'   CHANCSRA bits definition
'===========================

Public Const CCSRA_FTRIGSEL = 0        ' Fast trigger selection (local fast trigger vs. external fast trigger)
Public Const CCSRA_EXTTRIGSEL = 1      ' Module validation signal selection (module validation trigger vs. module GATE input)
Public Const CCSRA_GOOD = 2            ' Channel may be read, good-channel bit
Public Const CCSRA_CHANTRIGSEL = 3     ' Channel validation signal selection (channel validation trigger vs. channel GATE input)
Public Const CCSRA_SYNCDATAACQ = 4     ' Block data acquisition if trace or header buffers are full
Public Const CCSRA_POLARITY = 5        ' Channel triggers on rising edge if bit is set
Public Const CCSRA_VETOENA = 6         ' Enable channel trigger veto
Public Const CCSRA_HISTOE = 7          ' Channel histograms energy in the on-chip MCA
Public Const CCSRA_TRACEENA = 8        ' Capture trace and associated header data
Public Const CCSRA_QDCENA = 9          ' Capture QDC sums and associated header data
Public Const CCSRA_CFDMODE = 10        ' Use CFD for real time, trace capture and QDC capture
Public Const CCSRA_GLOBTRIG = 11       ' Require global trigger for validation
Public Const CCSRA_ESUMSENA = 12       ' Record raw energy sums and baseline in event header
Public Const CCSRA_CHANTRIG = 13       ' Require channel trigger for validation
Public Const CCSRA_ENARELAY = 14       ' Control input relay: 1: connect, 0: disconnect
Public Const CCSRA_PILEUPCTRL = 15     ' Control normal pileup rejection
Public Const CCSRA_INVERSEPILEUP = 16  ' Control inverse pileup rejection
'bits[16:15]
'00: record all events (trace, timestamps, etc., but no energy for piled-up events)
'01: only record single events (trace, energy, timestamps, etc.) (i.e., reject piled-up events)
'10: record trace, timestamps, etc., for piled-up events but do not record trace for single events
'11: only record trace, timestamps, etc., for piled-up events (i.e., reject single events)
Public Const CCSRA_ENAENERGYCUT = 17   ' Enable 'no traces for large pulses" feature
Public Const CCSRA_GROUPTRIGSEL = 18   ' Group trigger selection (external group trigger or local fast trigger)
Public Const CCSRA_CHANVETOSEL = 19    ' Channel veto selection (channel validation trigger or front panel channel veto)
Public Const CCSRA_MODVETOSEL = 20     ' Module veto selection (module validation trigger or front panel module veto)
Public Const CCSRA_EXTTSENA = 21       ' External clock timestamps in event header - 1: enable; 0: disable


'===========================
'   MODCSRB bits definition
'===========================

Public Const MODCSRB_PULLUP = 0          ' Control pullups for PXI trigger lines on the backplane through CPLD
Public Const MODCSRB_DIRMOD = 4          ' Sends triggers to trigger card & backplane
Public Const MODCSRB_CHASSISMASTER = 6   ' Control chassis master module: 1: chassis master module; 0: chassis non-master module
Public Const MODCSRB_GFTSEL = 7          ' Select global fast trigger source
Public Const MODCSRB_ETSEL = 8           ' Select external trigger source
Public Const MODCSRB_INHIBITENA = 10     ' Control external INHIBIT signal: 1: use INHIBIT; 0: don't use INHIBIT
Public Const MODCSRB_MULTCRATES = 11     ' Distribute clock and triggers in multiple crates: multiple crates (1) or only single crate (0)
Public Const MODCSRB_SORTEVENTS = 12     ' Sort events based on their timestamps
Public Const MODCSRB_BKPLFASTTRIG = 13   ' Enable connection of fast trigger to backplane

'===========================
'   DSP operation characteristics
'===========================

Public Const DSP_IMBUFFER_ADDR = &H40000  ' DSP internal data memory buffer start address
Public Const DSP_PMDATA_ADDR = &H49180    ' DSP internal program memory buffer start address
Public Const DSP_IOPARAMS_ADDR = &H4A000  ' DSP I/O parameters start address
Public Const ADC_TRACE_LENGTH = 8192      ' Default ADC trace length for each channel
Public Const BASELINES_LENGTH = 3640      ' Number of baseline values for each channel
Public Const N_DSP_IO_PAR = 832           ' Number of DSP I/O parameters, 32-bit integer
Public Const N_STATISTICS_PAR = 448       ' Number of run statistics data words
Public Const NUM_MODULE_PAR = 64          ' Number of DSP module parameters
Public Const NUM_CHANNEL_PAR = 768        ' Number of DSP channel parameters
Public Const NUM_RESULTS_PAR = 448        ' Number of DSP results parameters

'===========================
'   Run control parameters
'===========================

Public Const NEW_RUN = 1               ' new DAQ run
Public Const RESUME_RUN = 0            ' resume previous DAQ run
Public Const MOD_READ = 1              ' Read from module
Public Const MOD_WRITE = 0             ' Write to module
Public Const LMEVENTINFOLENGTH = 68    ' Event information length for list mode data
Public Const EXTFIFO_LENGTH = 131072   ' Length of the external FIFO

'===========================
'   Pixie-16 VB_GUI environmental settings file
'===========================

Public Const PIXIE16_VB_SETTINGS_FILE = "Pixie16_VB_Settings.txt"


'###########################################
'
'   Global parameters definitions
'
'###########################################

'===========================
'   Global parameters applicable to one module
'===========================

Public ModCSRA(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public ModCSRB(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public MaxEvents(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public InSynch(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public SynchWait(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public SlowFilterRange(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public FastFilterRange(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public HostRunTimePreset(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public TrigConfig0(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public TrigConfig1(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public TrigConfig2(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public TrigConfig3(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public FTNNEna(MAX_NUM_PIXIE16_MODULES - 1) As Long

Public ModRev(MAX_NUM_PIXIE16_MODULES - 1) As Integer
Public ModSerNum(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public ModADCBits(MAX_NUM_PIXIE16_MODULES - 1) As Integer
Public ModADCMSPS(MAX_NUM_PIXIE16_MODULES - 1) As Integer

Public SYSTEM_CLOCK_MHZ(MAX_NUM_PIXIE16_MODULES - 1) As Integer     ' System clock in MHz
Public XDT_SCALE_FACTOR(MAX_NUM_PIXIE16_MODULES - 1) As Integer     ' XDT scale factor in DSP clock cycles

'===========================
'   Global parameters applicable to one channel of one module
'===========================

Public EnergyFilterRiseTime(NUM_CHANNELS - 1) As Double
Public EnergyFilterFlatTop(NUM_CHANNELS - 1) As Double
Public TriggerFilterRiseTime(NUM_CHANNELS - 1) As Double
Public TriggerFilterFlatTop(NUM_CHANNELS - 1) As Double
Public TriggerThreshold(NUM_CHANNELS - 1) As Double
Public PreampTau(NUM_CHANNELS - 1) As Double
Public TraceLength(NUM_CHANNELS - 1) As Double
Public TraceDelay(NUM_CHANNELS - 1) As Double
Public DCoffset(NUM_CHANNELS - 1) As Double
Public BaselinePercent(NUM_CHANNELS - 1) As Double
Public EnergyLow(NUM_CHANNELS - 1) As Double
Public EnergyBinFactor(NUM_CHANNELS - 1) As Double
Public ChanCSRA(NUM_CHANNELS - 1) As Double
Public ChanCSRB(NUM_CHANNELS - 1) As Double
Public BLcut(NUM_CHANNELS - 1) As Double
Public BaselineAverage(NUM_CHANNELS - 1) As Double
Public Polarity(NUM_CHANNELS - 1) As Integer
Public Vgain(NUM_CHANNELS - 1) As Integer
Public CFDDelay(NUM_CHANNELS - 1) As Double
Public CFDScale(NUM_CHANNELS - 1) As Double
Public CFDThresh(NUM_CHANNELS - 1) As Double
Public ExtTrigStretch(NUM_CHANNELS - 1) As Double
Public ChanTrigStretch(NUM_CHANNELS - 1) As Double
Public VetoStretch(NUM_CHANNELS - 1) As Double
Public FastTrigBackPlaneLen(NUM_CHANNELS - 1) As Double
Public ExtDelayLength(NUM_CHANNELS - 1) As Double
Public FastTrigBackplaneDelay(NUM_CHANNELS - 1) As Double
Public MultiplicityMaskL(NUM_CHANNELS - 1) As Double
Public MultiplicityMaskH(NUM_CHANNELS - 1) As Double
Public QDCLen0(NUM_CHANNELS - 1) As Double
Public QDCLen1(NUM_CHANNELS - 1) As Double
Public QDCLen2(NUM_CHANNELS - 1) As Double
Public QDCLen3(NUM_CHANNELS - 1) As Double
Public QDCLen4(NUM_CHANNELS - 1) As Double
Public QDCLen5(NUM_CHANNELS - 1) As Double
Public QDCLen6(NUM_CHANNELS - 1) As Double
Public QDCLen7(NUM_CHANNELS - 1) As Double
Public GetTauValues(NUM_CHANNELS - 1) As Double

Public Xdt(3) As Double
Public XdtSglChan As Double

'===========================
'   Run control parameters
'===========================

Public StartUpDir As String
Public Number_Modules_Total As Integer
Public Number_Modules_Booted As Integer
Public IsOfflineOperation As Integer
Public SystemBooted As Boolean
Public RunType As Integer
Public PresetRunTime As Double
Public RunPollingInterval As Double
Public NumberOfFIFOFills As Long
Public OutputFileBaseName As String
Public RunNumber As Long
Public AutoIncRunNumber As Boolean
Public ResumeRun As Boolean
Public ComputeFilterResponses As Integer
Public PXISlotMap(MAX_NUM_PIXIE16_MODULES - 1) As Integer

Public SysTimerFreq As Currency
Public RunStartTime As Currency
Public CurrentRunTime As Currency
Public ElapsedRunTime As Double

Public CS_ItemsMask As Integer
Public CS_DestinationMask(MAX_NUM_PIXIE16_MODULES * NUM_CHANNELS - 1) As Integer

Public roi() As Long
Public LMTraceDT(MAX_NUM_PIXIE16_MODULES - 1) As Double

Public nFIFOWords(MAX_NUM_PIXIE16_MODULES - 1) As Double

Public FirstTimeSettingADCSamplingInterval As Boolean

'===========================
'   File names and paths
'===========================

Public Pixie16_Com_FPGA_File_RevBCD As String
Public Pixie16_Com_FPGA_File_RevF14b100m As String
Public Pixie16_Com_FPGA_File_RevF16b100m As String
Public Pixie16_Com_FPGA_File_RevF12b250m As String
Public Pixie16_Com_FPGA_File_RevF14b250m As String
Public Pixie16_Com_FPGA_File_RevF16b250m As String
Public Pixie16_Com_FPGA_File_RevF12b500m As String
Public Pixie16_Com_FPGA_File_RevF14b500m As String

Public Pixie16_SP_FPGA_File_RevBCD As String
Public Pixie16_SP_FPGA_File_RevF14b100m As String
Public Pixie16_SP_FPGA_File_RevF16b100m As String
Public Pixie16_SP_FPGA_File_RevF12b250m As String
Public Pixie16_SP_FPGA_File_RevF14b250m As String
Public Pixie16_SP_FPGA_File_RevF16b250m As String
Public Pixie16_SP_FPGA_File_RevF12b500m As String
Public Pixie16_SP_FPGA_File_RevF14b500m As String

Public Pixie16_DSP_Code_File_RevBCD As String
Public Pixie16_DSP_Var_File_RevBCD As String
Public Pixie16_DSP_List_File_RevBCD As String

Public Pixie16_DSP_Code_File_RevF14b100m As String
Public Pixie16_DSP_Var_File_RevF14b100m As String
Public Pixie16_DSP_List_File_RevF14b100m As String

Public Pixie16_DSP_Code_File_RevF16b100m As String
Public Pixie16_DSP_Var_File_RevF16b100m As String
Public Pixie16_DSP_List_File_RevF16b100m As String

Public Pixie16_DSP_Code_File_RevF12b250m As String
Public Pixie16_DSP_Var_File_RevF12b250m As String
Public Pixie16_DSP_List_File_RevF12b250m As String

Public Pixie16_DSP_Code_File_RevF14b250m As String
Public Pixie16_DSP_Var_File_RevF14b250m As String
Public Pixie16_DSP_List_File_RevF14b250m As String

Public Pixie16_DSP_Code_File_RevF16b250m As String
Public Pixie16_DSP_Var_File_RevF16b250m As String
Public Pixie16_DSP_List_File_RevF16b250m As String

Public Pixie16_DSP_Code_File_RevF12b500m As String
Public Pixie16_DSP_Var_File_RevF12b500m As String
Public Pixie16_DSP_List_File_RevF12b500m As String

Public Pixie16_DSP_Code_File_RevF14b500m As String
Public Pixie16_DSP_Var_File_RevF14b500m As String
Public Pixie16_DSP_List_File_RevF14b500m As String

Public Pixie16_DSP_Par_File As String
Public Pixie16_ListModeDataFile_Path As String
Public Pixie16_HistogramDataFile_Path As String
Public Pixie16_ListModeDataFile_Name(MAX_NUM_PIXIE16_MODULES - 1) As String
Public Pixie16_HistogramDataFile_Name(MAX_NUM_PIXIE16_MODULES - 1) As String
Public Pixie16_SettingsFile_Name As String

'===========================
'   ADC traces
'===========================

Public ADCTraceCh0(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh1(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh2(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh3(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh4(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh5(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh6(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh7(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh8(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh9(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh10(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh11(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh12(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh13(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh14(ADC_TRACE_LENGTH - 1) As Long
Public ADCTraceCh15(ADC_TRACE_LENGTH - 1) As Long

Public ADCTraceSglCh(ADC_TRACE_LENGTH - 1) As Long

Public ADCTraceRdCh(ADC_TRACE_LENGTH - 1) As Integer

'===========================
'   Baselines
'===========================

Public BaselinesCh0(BASELINES_LENGTH - 1) As Double
Public BaselinesCh1(BASELINES_LENGTH - 1) As Double
Public BaselinesCh2(BASELINES_LENGTH - 1) As Double
Public BaselinesCh3(BASELINES_LENGTH - 1) As Double
Public BaselinesCh4(BASELINES_LENGTH - 1) As Double
Public BaselinesCh5(BASELINES_LENGTH - 1) As Double
Public BaselinesCh6(BASELINES_LENGTH - 1) As Double
Public BaselinesCh7(BASELINES_LENGTH - 1) As Double
Public BaselinesCh8(BASELINES_LENGTH - 1) As Double
Public BaselinesCh9(BASELINES_LENGTH - 1) As Double
Public BaselinesCh10(BASELINES_LENGTH - 1) As Double
Public BaselinesCh11(BASELINES_LENGTH - 1) As Double
Public BaselinesCh12(BASELINES_LENGTH - 1) As Double
Public BaselinesCh13(BASELINES_LENGTH - 1) As Double
Public BaselinesCh14(BASELINES_LENGTH - 1) As Double
Public BaselinesCh15(BASELINES_LENGTH - 1) As Double

Public BaselinesSglCh(BASELINES_LENGTH - 1) As Double
Public BaseTimeStamps(BASELINES_LENGTH - 1) As Double

'===========================
'   MCA spectra
'===========================

Public MCASpecCh0(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh1(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh2(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh3(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh4(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh5(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh6(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh7(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh8(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh9(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh10(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh11(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh12(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh13(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh14(MAX_HISTOGRAM_LENGTH - 1) As Long
Public MCASpecCh15(MAX_HISTOGRAM_LENGTH - 1) As Long

Public MCASpecSglCh(MAX_HISTOGRAM_LENGTH - 1) As Long

'===========================
'   List mode traces
'===========================

Public ListModeTraceCh0() As Long
Public ListModeTraceCh1() As Long
Public ListModeTraceCh2() As Long
Public ListModeTraceCh3() As Long
Public ListModeTraceCh4() As Long
Public ListModeTraceCh5() As Long
Public ListModeTraceCh6() As Long
Public ListModeTraceCh7() As Long
Public ListModeTraceCh8() As Long
Public ListModeTraceCh9() As Long
Public ListModeTraceCh10() As Long
Public ListModeTraceCh11() As Long
Public ListModeTraceCh12() As Long
Public ListModeTraceCh13() As Long
Public ListModeTraceCh14() As Long
Public ListModeTraceCh15() As Long

Public ListModeTraceSglCh() As Long
Public ListModeTraceRdCh() As Integer

Public LMTraceFilterRSTrace() As Integer
Public LMTraceFastFilter() As Double
Public LMTraceCFD() As Double
Public LMTraceSlowFilter() As Double

Public ModuleListModeEvents(MAX_NUM_PIXIE16_MODULES - 1) As Long
Public ListModeEventInformation() As Long

'===========================
'   Run statistics data
'===========================

Public StatisticsData(N_STATISTICS_PAR - 1) As Long

'===========================
'   Debug - DSP parameter values
'===========================

Public DSPMemBlock0(65535) As Long
Public DSPMemBlock1(65535) As Long
Public DSPValIsHexadecimal As Boolean
Public DSPMemBlk0IsHexadecimal As Boolean
Public DSPMemBlk1IsHexadecimal As Boolean

'===========================
'   Controls for the text edit boxes on
'   the DAQParametersSetup form
'===========================

Public bDoNotEditEF    As Boolean
Public bDoNotEditTF    As Boolean
Public bDoNotEditASC   As Boolean
Public bDoNotEditHC    As Boolean
Public bDoNotEditTAU   As Boolean
Public bDoNotEditPSA   As Boolean
Public bDoNotEditBL    As Boolean
Public bDoNotEditDT    As Boolean
Public bDoNotEditCFD   As Boolean
Public bDoNotEditTL    As Boolean
Public bDoNotEditFIFO  As Boolean
Public bDoNotEditMP    As Boolean
Public bDoNotEditTC    As Boolean
Public bDoNotEditFTNN  As Boolean
Public bDoNotEditQDC   As Boolean

