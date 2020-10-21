Attribute VB_Name = "Pixie16_Declarations"
Option Explicit

Public Declare Function Pixie16InitSystem Lib "PixieAppDLL.dll" Alias _
        "_Pixie16InitSystem@12" (ByVal NumModules As Integer, ByRef _
        PXISlotMap As Integer, ByVal OfflineMode As Integer) As Long
        
Public Declare Function Pixie16ExitSystem Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ExitSystem@4" (ByVal ModNum As Integer) As Long
        
Public Declare Function Pixie16BootModule Lib "PixieAppDLL.dll" Alias _
        "_Pixie16BootModule@32" (ByVal ComFPGAConfigFile As String, ByVal SPFPGAConfigFile _
        As String, ByVal TrigFPGAConfigFile As String, ByVal DSPcodefile As String, _
        ByVal DSPParfile As String, ByVal DSPvarfile As String, _
        ByVal ModNum As Integer, ByVal BootPattern As Integer) As Long

Public Declare Function Pixie16AcquireADCTrace Lib "PixieAppDLL.dll" Alias _
        "_Pixie16AcquireADCTrace@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ReadSglChanADCTrace Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadSglChanADCTrace@16" (ByRef Trace_Buffer As Integer, ByVal _
        Trace_Length As Long, ByVal ModNum As Integer, ByVal ChanNum As Integer) _
        As Long

Public Declare Function Pixie16IMbufferIO Lib "PixieAppDLL.dll" Alias _
        "_Pixie16IMbufferIO@20" (ByRef Buffer As Long, ByVal _
        NumWords As Long, ByVal Address As Long, ByVal Direction As Integer, _
        ByVal ModNum As Integer) As Long

Public Declare Function Pixie16EMbufferIO Lib "PixieAppDLL.dll" Alias _
        "_Pixie16EMbufferIO@20" (ByRef Buffer As Long, ByVal _
        NumWords As Long, ByVal Address As Long, ByVal Direction As Integer, _
        ByVal ModNum As Integer) As Long

Public Declare Function Pixie16StartListModeRun Lib "PixieAppDLL.dll" Alias _
        "_Pixie16StartListModeRun@12" (ByVal ModNum As Integer, _
        ByVal RunType As Integer, ByVal mode As Integer) As Long

Public Declare Function Pixie16StartHistogramRun Lib "PixieAppDLL.dll" Alias _
        "_Pixie16StartHistogramRun@8" (ByVal ModNum As Integer, _
        ByVal mode As Integer) As Long

Public Declare Function Pixie16CheckRunStatus Lib "PixieAppDLL.dll" Alias _
        "_Pixie16CheckRunStatus@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16EndRun Lib "PixieAppDLL.dll" Alias _
        "_Pixie16EndRun@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ComputeInputCountRate Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeInputCountRate@12" (ByRef Statistics As Long, ByVal _
        ModNum As Integer, ByVal ChanNum As Integer) As Double

Public Declare Function Pixie16ComputeOutputCountRate Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeOutputCountRate@12" (ByRef Statistics As Long, ByVal _
        ModNum As Integer, ByVal ChanNum As Integer) As Double

Public Declare Function Pixie16ComputeLiveTime Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeLiveTime@12" (ByRef Statistics As Long, ByVal _
        ModNum As Integer, ByVal ChanNum As Integer) As Double

Public Declare Function Pixie16ComputeProcessedEvents Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeProcessedEvents@8" (ByRef Statistics As Long, ByVal _
        ModNum As Integer) As Double

Public Declare Function Pixie16ComputeRealTime Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeRealTime@8" (ByRef Statistics As Long, ByVal _
        ModNum As Integer) As Double

Public Declare Function Pixie16complexFFT Lib "PixieAppDLL.dll" Alias _
        "_Pixie16complexFFT@8" (ByRef data As Double, ByVal length As Long) _
        As Long

Public Declare Function APP16_TstBit Lib "PixieAppDLL.dll" Alias _
        "_APP16_TstBit@8" (ByVal bit As Integer, ByVal Value As Integer) As Integer
              
Public Declare Function APP16_SetBit Lib "PixieAppDLL.dll" Alias _
        "_APP16_SetBit@8" (ByVal bit As Integer, ByVal Value As Integer) As Integer
        
Public Declare Function APP16_ClrBit Lib "PixieAppDLL.dll" Alias _
        "_APP16_ClrBit@8" (ByVal bit As Integer, ByVal Value As Integer) As Integer
        
Public Declare Function APP32_SetBit Lib "PixieAppDLL.dll" Alias _
        "_APP32_SetBit@8" (ByVal bit As Integer, ByVal Value As Long) As Long
        
Public Declare Function APP32_ClrBit Lib "PixieAppDLL.dll" Alias _
        "_APP32_ClrBit@8" (ByVal bit As Integer, ByVal Value As Long) As Long
        
Public Declare Function APP32_TstBit Lib "PixieAppDLL.dll" Alias _
        "_APP32_TstBit@8" (ByVal bit As Integer, ByVal Value As Long) As Long

Public Declare Function Pixie16SetDACs Lib "PixieAppDLL.dll" Alias _
        "_Pixie16SetDACs@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ProgramFippi Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ProgramFippi@4" (ByVal ModNum As Integer) As Long
        
Public Declare Function Pixie16AdjustOffsets Lib "PixieAppDLL.dll" Alias _
        "_Pixie16AdjustOffsets@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16AcquireBaselines Lib "PixieAppDLL.dll" Alias _
        "_Pixie16AcquireBaselines@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ReadSglChanBaselines Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadSglChanBaselines@20" (ByRef Baselines As Double, ByRef BaseTimeStamps As Double, _
        ByVal NumBases As Integer, ByVal ModNum As Integer, ByVal ChanNum As Integer) As Long

Public Declare Function Pixie16RampOffsetDACs Lib "PixieAppDLL.dll" Alias _
        "_Pixie16RampOffsetDACs@4" (ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ControlTaskRun Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ControlTaskRun@12" (ByVal ModNum As Integer, ByVal ControlTask As Integer, _
        ByVal MaxPoll As Long) As Long

Public Declare Function Pixie16BLcutFinder Lib "PixieAppDLL.dll" Alias _
        "_Pixie16BLcutFinder@12" (ByVal ModNum As Integer, ByVal ChanNum As Integer, _
        ByRef BLcut As Long) As Long

Public Declare Function Pixie16TauFinder Lib "PixieAppDLL.dll" Alias _
        "_Pixie16TauFinder@8" (ByVal ModNum As Integer, ByRef Tau As Double) As Long

Public Declare Function Pixie16WriteSglModPar Lib "PixieAppDLL.dll" Alias _
        "_Pixie16WriteSglModPar@12" (ByVal ModParName As String, _
        ByVal ModParData As Long, ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ReadSglModPar Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadSglModPar@12" (ByVal ModParName As String, _
        ByRef ModParData As Long, ByVal ModNum As Integer) As Long

Public Declare Function Pixie16WriteSglChanPar Lib "PixieAppDLL.dll" Alias _
        "_Pixie16WriteSglChanPar@20" (ByVal ChanParName As String, _
        ByVal ChanParData As Double, ByVal ModNum As Integer, _
        ByVal ChanNum As Integer) As Long

Public Declare Function Pixie16ReadSglChanPar Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadSglChanPar@16" (ByVal ChanParName As String, _
        ByRef ChanParData As Double, ByVal ModNum As Integer, _
        ByVal ChanNum As Integer) As Long

Public Declare Function Pixie16ReadHistogramFromModule Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadHistogramFromModule@16" (ByRef Histogram As Long, ByVal _
        NumWords As Long, ByVal ModNum As Integer, ByVal ChanNum As Integer) _
        As Long

Public Declare Function Pixie16ReadStatisticsFromModule Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadStatisticsFromModule@8" (ByRef Statistics As Long, _
        ByVal ModNum As Integer) As Long

Public Declare Function Pixie16SaveHistogramToFile Lib "PixieAppDLL.dll" Alias _
        "_Pixie16SaveHistogramToFile@8" (ByVal FileName As String, ByVal ModNum _
        As Integer) As Long

Public Declare Function Pixie16GetModuleEvents Lib "PixieAppDLL.dll" Alias _
        "_Pixie16GetModuleEvents@8" (ByVal FileName As String, _
        ByRef ModuleEvents As Long) As Long

Public Declare Function Pixie16GetEventsInfo Lib "PixieAppDLL.dll" Alias _
        "_Pixie16GetEventsInfo@12" (ByVal FileName As String, _
        ByRef EventInformation As Long, ByVal ModNum As Integer) As Long

Public Declare Function Pixie16ReadListModeTrace Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadListModeTrace@16" (ByVal FileName As String, ByRef Trace_Data _
        As Integer, ByVal NumWords As Integer, ByVal FileLocation As Long) As Long

Public Declare Function Pixie16ReadHistogramFromFile Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadHistogramFromFile@20" (ByVal FileName As String, ByRef Histogram As Long, ByVal _
        NumWords As Long, ByVal CardNo As Integer, ByVal ChannelNo As Integer) _
        As Long

Public Declare Function Pixie16SaveDSPParametersToFile Lib "PixieAppDLL.dll" Alias _
        "_Pixie16SaveDSPParametersToFile@4" (ByVal FileName As String) As Long

Public Declare Function Pixie16LoadDSPParametersFromFile Lib "PixieAppDLL.dll" Alias _
        "_Pixie16LoadDSPParametersFromFile@4" (ByVal FileName As String) As Long

Public Declare Function Pixie16CopyDSPParameters Lib "PixieAppDLL.dll" Alias _
        "_Pixie16CopyDSPParameters@16" (ByVal BitMask As Integer, ByVal SourceModule As Integer, _
        ByVal SourceChannel As Integer, ByRef DestinationMask As Integer) As Long

Public Declare Function Pixie16ReadMSGFile Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadMSGFile@4" (ByVal ReturnMsgStr As String) As Long

Public Declare Function Decimal2IEEEFloating Lib "PixieAppDLL.dll" Alias _
        "_Decimal2IEEEFloating@8" (ByVal DecimalNumber As Double) As Long

Public Declare Function IEEEFloating2Decimal Lib "PixieAppDLL.dll" Alias _
        "_IEEEFloating2Decimal@4" (ByVal IEEEFloatingNumber As Long) As Double

Public Declare Sub Sleep Lib "kernel32" _
                (ByVal dwMilliseconds As Long)

Public Declare Function Pixie16SaveExternalFIFODataToFile Lib "PixieAppDLL.dll" Alias _
        "_Pixie16SaveExternalFIFODataToFile@16" (ByVal FileName As String, ByRef nFIFOWords As Long, _
        ByVal ModNum As Integer, ByVal EndOfRunRead As Integer) As Long

Public Declare Function Pixie16ReadModuleInfo Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ReadModuleInfo@20" (ByVal ModNum As Integer, ByRef ModRev As Integer, _
        ByRef ModSerNum As Long, ByRef ModADCBits As Integer, ByRef ModADCMSPS As Integer) As Long

Public Declare Function Pixie16ComputeFastFiltersOffline Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeFastFiltersOffline@32" (ByVal FileName As String, ByVal ModuleNumber As Integer, _
        ByVal ChannelNumber As Integer, ByVal FileLocation As Long, ByVal RcdTraceLength As Integer, _
        ByRef RcdTrace As Integer, ByRef fastfilter As Double, ByRef cfd As Double) As Long
        
Public Declare Function Pixie16ComputeSlowFiltersOffline Lib "PixieAppDLL.dll" Alias _
        "_Pixie16ComputeSlowFiltersOffline@28" (ByVal FileName As String, ByVal ModuleNumber As Integer, _
        ByVal ChannelNumber As Integer, ByVal FileLocation As Long, ByVal RcdTraceLength As Integer, _
        ByRef RcdTrace As Integer, ByRef slowfilter As Double) As Long

Public Declare Function Pixie16SetOfflineVariant Lib "PixieAppDLL.dll" Alias _
        "_Pixie16SetOfflineVariant@8" (ByVal ModuleNumber As Integer, _
        ByVal ModuleOfflineVariant As Integer) As Long


