Attribute VB_Name = "Pixie16_Utilities"
'Declarations:

'Used to disable the Close Form button and menu option
Private Declare Function GetSystemMenu Lib "user32" (ByVal hWnd As Long, _
                         ByVal bRevert As Long) As Long
Private Declare Function RemoveMenu Lib "user32" (ByVal hMenu As Long, _
                         ByVal nPosition As Long, ByVal wFlags As Long) As Long
'--------------------------------------


'###########################################
'   FromHex()
'       Convert a hex string to a decimal number.
'       This function only accept hex number in
'       the format of "FFFFFFFF"
'###########################################

Public Function FromHex(HexString As String) As Double
    Dim theTemp As Double
    Dim testStr1 As String
    Dim testStr2 As String
    Dim testStr3 As String
    Dim strlen As Integer
    On Error GoTo eh
    
    If Trim$(HexString) = "" Then
        FromHex = 0
        Exit Function
    End If
    
    ' Special case: HexString is from 0x8000 to 0xFFFF
    strlen = Len(HexString)
    If strlen = 4 Then
        HexString = "0000" & HexString
    End If
        
    theTemp = Val("&H" & HexString)

    '-------------------------------------------------------
    ' Val function has problem converting the following Hex
    ' numbers: 0x00008000 - 0x0000FFFF, 0x80000000 - 0xFFFFFFFF
    '-------------------------------------------------------
    testStr1 = Left(HexString, 1)   ' first character of the string
    testStr2 = Left(HexString, 4)   ' first 4 characters of the string
    testStr3 = Mid(HexString, 5, 1) ' the 5th character of the string
   
    ' check if the hex number is greater than 0x7FFFFFFF
    If Val("&H" & testStr1) > 7 Then
        theTemp = 4294967295# + theTemp + 1
    End If
    
    ' check if the hex number is greater than 0x00008000 but less than 0x00010000
    If Val("&H" & testStr3) > 7 And Val("&H" & testStr2) = 0 Then
        theTemp = 65536 + theTemp
    End If
        
    FromHex = theTemp
    Exit Function
eh:
    FromHex = 0
End Function


'###########################################
'   ToHex()
'       Convert a decimal number to a hex string
'###########################################

Public Function ToHex(Num As Double) As String
    Dim theTemp As String
    Dim firstChar As String
    Dim theVal As Long
    On Error GoTo eh
    
    ' Check if Num is greater than 0x7FFFFFFF
    If (Num > 2147483647) Then
        theVal = Num - 2147483648#
        theTemp = Hex$(theVal)
        Select Case Len(theTemp)
            Case 1
                theTemp = "8000000" & theTemp
            Case 2
                theTemp = "800000" & theTemp
            Case 3
                theTemp = "80000" & theTemp
            Case 4
                theTemp = "8000" & theTemp
            Case 5
                theTemp = "800" & theTemp
            Case 6
                theTemp = "80" & theTemp
            Case 7
                theTemp = "8" & theTemp
            Case 8
                firstChar = Left(theTemp, 1)
                If (firstChar = "1") Then
                   theTemp = "9" & Right(theTemp, 7)
                ElseIf (firstChar = "2") Then
                   theTemp = "A" & Right(theTemp, 7)
                 ElseIf (firstChar = "3") Then
                   theTemp = "B" & Right(theTemp, 7)
                ElseIf (firstChar = "4") Then
                   theTemp = "C" & Right(theTemp, 7)
                ElseIf (firstChar = "5") Then
                   theTemp = "D" & Right(theTemp, 7)
                ElseIf (firstChar = "6") Then
                   theTemp = "E" & Right(theTemp, 7)
                ElseIf (firstChar = "7") Then
                   theTemp = "F" & Right(theTemp, 7)
               End If
        End Select
    Else
        theTemp = Hex$(Num)
        Select Case Len(theTemp)
            Case 1
                theTemp = "0000000" & theTemp
            Case 2
                theTemp = "000000" & theTemp
            Case 3
                theTemp = "00000" & theTemp
            Case 4
                theTemp = "0000" & theTemp
            Case 5
                theTemp = "000" & theTemp
            Case 6
                theTemp = "00" & theTemp
            Case 7
                theTemp = "0" & theTemp
        End Select
    End If
    
    ToHex = theTemp
    
eh:
End Function


'###########################################
'   IsLoaded()
'       Return True if a form is loaded
'###########################################

Public Function IsLoaded(ByVal form_name As String) As Boolean
Dim frm As Form

    IsLoaded = False
    For Each frm In Forms
        If frm.Name = form_name Then
            IsLoaded = True
            Exit For
        End If
    Next frm
End Function


'###########################################
'   Dec2Bin()
'       Convert a byte to bin
'###########################################

Public Function Dec2Bin(ByVal intDec As Byte) As String
   Dim strHex          As String
   Dim strTempReturn   As String
   Dim strTempNibble   As String * 4
   Dim i               As Integer

   'Initialise the return variable

   strTempReturn = String(8, "*")

   'Get the hexidecimal value of the binary number

   strHex = Hex(intDec)

   'Test if strTempHex is 1 character long

   If (Len(strHex) = 1) Then
      strHex = "0" & strHex
   End If

   'Convert the hexidecimal number to binary

   For i = 1 To 2
      Select Case (Mid$(strHex, i, 1))
      Case "0"
         strTempNibble = "0000"
      Case "1"
         strTempNibble = "0001"
      Case "2"
         strTempNibble = "0010"
      Case "3"
         strTempNibble = "0011"
      Case "4"
         strTempNibble = "0100"
      Case "5"
         strTempNibble = "0101"
      Case "6"
         strTempNibble = "0110"
      Case "7"
         strTempNibble = "0111"
      Case "8"
         strTempNibble = "1000"
      Case "9"
         strTempNibble = "1001"
      Case "A"
         strTempNibble = "1010"
      Case "B"
         strTempNibble = "1011"
      Case "C"
         strTempNibble = "1100"
      Case "D"
         strTempNibble = "1101"
      Case "E"
         strTempNibble = "1110"
      Case "F"
         strTempNibble = "1111"
      End Select

      If i = 1 Then
         Mid(strTempReturn, 1, 4) = strTempNibble
      Else
         Mid(strTempReturn, 5, 4) = strTempNibble
      End If
   Next i

   'Return the value in binary

   Dec2Bin = strTempReturn
End Function


'###########################################
'   Bin2Long()
'       Convert binary number (32 characters) to long
'###########################################

Public Function Bin2Long(ByVal BinaryStr As String) As Long
    
    Dim X As Integer
    Dim ConvertLong As Long
    Dim ChrInStr As String
    
    ConvertedLong = 0
    For X = 1 To Len(BinaryStr)
        ChrInStr = Mid(BinaryStr, Len(BinaryStr) - (X - 1), 1)
        If ChrInStr = "1" Then
            ConvertedLong = ConvertedLong + 2 ^ (X - 1)
        End If
    Next X

    Bin2Long = ConvertedLong

End Function


'###########################################
'   RemoveCloseMenuItem()
'       Remove the Close menu item and disable
'       the Close button from a window
'###########################################

Public Sub RemoveCloseMenuItem(ByVal hWnd As Long)
    Const SC_CLOSE = &HF060
    Const MF_BYCOMMAND = 0
    Dim hMenu As Long
    ' get the system menu's handle
    hMenu = GetSystemMenu(hWnd, 0)
    ' remove the Close item
    RemoveMenu hMenu, SC_CLOSE, MF_BYCOMMAND
End Sub


'###########################################
'   RetrieveChanParFromModule()
'       Read channel parameters from a module
'###########################################

Public Function RetrieveChanParFromModule(ByVal ModNum As Integer) As Integer
    Dim X As Integer
    Dim Y As Integer
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim localChanCSRAlng As Long
    Dim retval As Long
    Dim dT As Double
    
    On Error GoTo ErrMSGOnConversion
    
    For X = 0 To NUM_CHANNELS - 1
    
        retval = Pixie16ReadSglChanPar("TRIGGER_RISETIME", TriggerFilterRiseTime(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read TRIGGER_RISETIME. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -1
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("TRIGGER_FLATTOP", TriggerFilterFlatTop(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read TRIGGER_FLATTOP. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -2
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("TRIGGER_THRESHOLD", TriggerThreshold(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read TRIGGER_THRESHOLD. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -3
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("ENERGY_RISETIME", EnergyFilterRiseTime(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read ENERGY_RISETIME. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -4
            Exit Function
        End If
   
        retval = Pixie16ReadSglChanPar("ENERGY_FLATTOP", EnergyFilterFlatTop(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read ENERGY_FLATTOP. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -5
            Exit Function
        End If
   
        retval = Pixie16ReadSglChanPar("TAU", PreampTau(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read TAU. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -6
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("TRACE_LENGTH", TraceLength(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read TRACE_LENGTH. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -7
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("TRACE_DELAY", TraceDelay(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read TRACE_DELAY. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -8
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("VOFFSET", DCoffset(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read VOFFSET. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -9
            Exit Function
       End If
    
        retval = Pixie16ReadSglChanPar("BASELINE_PERCENT", BaselinePercent(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read BASELINE_PERCENT. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -11
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("BLCUT", BLcut(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read BLCUT. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -11
            Exit Function
        End If
    
        retval = Pixie16ReadSglChanPar("BASELINE_AVERAGE", BaselineAverage(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read BASELINE_AVERAGE. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -11
            Exit Function
        End If
       
        retval = Pixie16ReadSglChanPar("EMIN", EnergyLow(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read EMIN. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -12
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("BINFACTOR", EnergyBinFactor(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read BINFACTOR. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -13
            Exit Function
        End If
       
        retval = Pixie16ReadSglChanPar("CHANNEL_CSRA", ChanCSRA(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read CHANNEL_CSRA. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -14
            Exit Function
        End If
       
        retval = Pixie16ReadSglChanPar("CHANNEL_CSRB", ChanCSRB(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read CHANNEL_CSRB. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -15
            Exit Function
        End If
       
        retval = Pixie16ReadSglChanPar("CFDDelay", CFDDelay(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read CFDDelay. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -16
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("CFDScale", CFDScale(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read CFDScale. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -17
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("CFDThresh", CFDThresh(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read CFDThresh. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -17
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("ExtTrigStretch", ExtTrigStretch(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read ExtTrigStretch. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -18
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("ChanTrigStretch", ChanTrigStretch(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read ChanTrigStretch. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -18
            Exit Function
        End If
       
       retval = Pixie16ReadSglChanPar("VetoStretch", VetoStretch(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read VetoStretch. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -19
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("FASTTRIGBACKLEN", FastTrigBackPlaneLen(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read FASTTRIGBACKLEN. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -20
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("ExternDelayLen", ExtDelayLength(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read ExternDelayLen. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -21
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("FtrigoutDelay", FastTrigBackplaneDelay(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read FtrigoutDelay. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -22
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("MultiplicityMaskL", MultiplicityMaskL(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read MultiplicityMaskL. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -23
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("MultiplicityMaskH", MultiplicityMaskH(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read MultiplicityMaskH. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -24
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen0", QDCLen0(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen0. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -25
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen1", QDCLen1(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen1. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -26
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen2", QDCLen2(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen2. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -27
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen3", QDCLen3(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen3. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -28
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen4", QDCLen4(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen4. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -29
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen5", QDCLen5(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen5. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -30
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen6", QDCLen6(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen6. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -31
            Exit Function
        End If
        
        retval = Pixie16ReadSglChanPar("QDCLen7", QDCLen7(X), ModNum, X)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read QDCLen7. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -32
            Exit Function
        End If
        
        localChanCSRAlng = CLng(ChanCSRA(X))
        If APP32_TstBit(CCSRA_POLARITY, localChanCSRAlng) = 1 Then
            Polarity(X) = 1
        Else
            Polarity(X) = 0
        End If
        If APP32_TstBit(CCSRA_ENARELAY, localChanCSRAlng) = 1 Then
            Vgain(X) = 1
        Else
            Vgain(X) = 0
        End If
        
    Next X
    
    ' Get XDT
    For X = 0 To 3
        retval = Pixie16ReadSglChanPar("XDT", Xdt(X), ModNum, X * 4)
        If retval < 0 Then
            ErrTxt = "(RetrieveChanParFromModule) failed to read XDT. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            RetrieveChanParFromModule = -10
            Exit Function
        End If
        
        ' Check if the returned Xdt value is 0 - which is possible
        ' if a corrupted settings file is being used
        If Xdt(X) = 0 Or Xdt(X) < 0 Then
            ' Set Xdt to the minimum
            dT = XDT_SCALE_FACTOR(ModNum) / DSP_CLOCK_MHZ
            
            ' Write to module
            For Y = 0 To 3
                retval = Pixie16WriteSglChanPar("XDT", dT, ModNum, X * 4 + Y)
                If retval < 0 Then
                    StrTxt = "Pixie16WriteSglChanPar failed to set XDT in module # " & Str$(ModNum)
                    Msg = MsgBox(StrTxt, 0, "Error")
                    Exit Function
                End If
            Next Y
            ' Read back from module
            retval = Pixie16ReadSglChanPar("XDT", Xdt(X), ModNum, X * 4)
            If retval < 0 Then
                StrTxt = "Pixie16ReadSglChanPar failed to set XDT in module # " & Str$(ModNum)
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Function
            End If
        End If
    Next X
       
    RetrieveChanParFromModule = 0   '   Success
    Exit Function
    
ErrMSGOnConversion:

    ErrTxt = "(RetrieveChanParFromModule) failed to convert ChanCSRA"
    Msg = MsgBox(ErrTxt, 0, "Error")
    RetrieveChanParFromModule = -17
  
End Function


'###########################################
'   UpdateChanParCtrlsOnDAQForm()
'       Update channel parameter controls on the DAQ form
'###########################################

Public Function UpdateChanParCtrlsOnDAQForm() As Integer
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim localChanCSRAlng As Long

    On Error GoTo ErrMSGOnConversion

    '*******************************************
    '   Filter tab - Energy Filter
    '*******************************************
    
    With DAQParametersSetup.EnergyFilterGrid
        .Redraw = False
        
        ' Energy filter rise time in col 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(EnergyFilterRiseTime(X - 1), "0.000")
        Next X
        
        ' Energy filter flat top in col 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(EnergyFilterFlatTop(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With
      
    '*******************************************
    '   Filter tab - Trigger Filter
    '*******************************************

    With DAQParametersSetup.TriggerFilterGrid
        .Redraw = False
        
        ' Trigger filter rise time in col 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(TriggerFilterRiseTime(X - 1), "0.000")
        Next X
        
        ' Trigger filter flat top in col 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(TriggerFilterFlatTop(X - 1), "0.000")
        Next X
        
        ' Trigger threshold in col 3
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 3) = Format(TriggerThreshold(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With
    
    '*******************************************
    '   Analog Signal Conditioning tab
    '*******************************************
    
    With DAQParametersSetup.AnalogGrid
        .Redraw = False
 
        ' Gain in col 1
        For X = 1 To NUM_CHANNELS
            If Vgain(X - 1) = 1 Then
                If ModRev(DAQParametersSetup.DAQParSetupModNumEdit.Value) = 15 Then
                    .TextMatrix(X, 1) = Format(HIGH_GAIN_REVF, "0.0")
                Else
                    .TextMatrix(X, 1) = Format(HIGH_GAIN_REVBCD, "0.0")
                End If
            Else
                If ModRev(DAQParametersSetup.DAQParSetupModNumEdit.Value) = 15 Then
                    .TextMatrix(X, 1) = Format(LOW_GAIN_REVF, "0.000")
                Else
                    .TextMatrix(X, 1) = Format(LOW_GAIN_REVBCD, "0.0")
                End If
            End If
        Next X
  
        ' DC offset in col 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(DCoffset(X - 1), "0.000")
        Next X
        
        ' Polarity in col 3
        For X = 1 To NUM_CHANNELS
            If Polarity(X - 1) = 1 Then
                .TextMatrix(X, 3) = "(+) Positive"
            Else
                .TextMatrix(X, 3) = "(-) Negative"
            End If
        Next X
      
        .Redraw = True
    End With
      
    '*******************************************
    '   Histogram Control tab
    '*******************************************
    
    With DAQParametersSetup.HistogramGrid
        .Redraw = False
        
       ' Minimum energy bin value in col 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = EnergyLow(X - 1)
        Next X
        
        ' Energy histogram binning factor in col 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = EnergyBinFactor(X - 1)
        Next X
   
        .Redraw = True
    End With
      
    '*******************************************
    '   Decay Time tab
    '*******************************************
    
    With DAQParametersSetup.DecayTimeGrid
        .Redraw = False
        
        ' Preamp Tau value in col 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(PreampTau(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With

    '*******************************************
    '
    '   Pulse Shape Analysis tab
    '
    '*******************************************
    
    With DAQParametersSetup.PSAGrid
        .Redraw = False
        
        ' Trace length in col 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(TraceLength(X - 1), "0.000")
        Next X
        
        ' Trace delay in col 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(TraceDelay(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With

    '*******************************************
    '   Baseline tab
    '*******************************************
    
    With DAQParametersSetup.BaselineGrid
        .Redraw = False
        
        ' Baseline cut in col 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = BLcut(X - 1)
        Next X
        
        ' Baseline percent in col 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = BaselinePercent(X - 1)
        Next X
        
        ' Baseline average in col 3
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 3) = BaselineAverage(X - 1)
        Next X
        
        .Redraw = True
    End With
      
    
    '*******************************************
    '   Control registers tab
    '*******************************************
    
    localChanCSRAlng = CLng(ChanCSRA(DAQParametersSetup.CHANCSRAChanNumEdit.Value))
    
    ' Good channel
    If APP32_TstBit(CCSRA_GOOD, localChanCSRAlng) = 1 Then
        DAQParametersSetup.GoodCheck.Value = 1
    Else
        DAQParametersSetup.GoodCheck.Value = 0
    End If
    DAQParametersSetup.GoodCheck.Refresh

    ' Histogram energy
    If APP32_TstBit(CCSRA_HISTOE, localChanCSRAlng) = 1 Then
        DAQParametersSetup.HistogrammingCheck.Value = 1
    Else
        DAQParametersSetup.HistogrammingCheck.Value = 0
    End If
    DAQParametersSetup.HistogrammingCheck.Refresh

    ' Capture trace
    If APP32_TstBit(CCSRA_TRACEENA, localChanCSRAlng) = 1 Then
        DAQParametersSetup.TraceCaptureCheck.Value = 1
    Else
        DAQParametersSetup.TraceCaptureCheck.Value = 0
    End If
    DAQParametersSetup.TraceCaptureCheck.Refresh

    ' Capture QDC sums
    If APP32_TstBit(CCSRA_QDCENA, localChanCSRAlng) = 1 Then
        DAQParametersSetup.QDCCaptureCheck.Value = 1
    Else
        DAQParametersSetup.QDCCaptureCheck.Value = 0
    End If
    DAQParametersSetup.QDCCaptureCheck.Refresh

    ' Enable CFD trigger
    If APP32_TstBit(CCSRA_CFDMODE, localChanCSRAlng) = 1 Then
        DAQParametersSetup.CFDTRigCheck.Value = 1
    Else
        DAQParametersSetup.CFDTRigCheck.Value = 0
    End If
    DAQParametersSetup.CFDTRigCheck.Refresh

    ' Require global external trigger for validation
    If APP32_TstBit(CCSRA_GLOBTRIG, localChanCSRAlng) = 1 Then
        DAQParametersSetup.GlobalExternalTrigCheck.Value = 1
    Else
        DAQParametersSetup.GlobalExternalTrigCheck.Value = 0
    End If
    DAQParametersSetup.GlobalExternalTrigCheck.Refresh

    ' Capture raw energy sums and baseline
    If APP32_TstBit(CCSRA_ESUMSENA, localChanCSRAlng) = 1 Then
        DAQParametersSetup.CaptureESumsCheck.Value = 1
    Else
        DAQParametersSetup.CaptureESumsCheck.Value = 0
    End If
    DAQParametersSetup.CaptureESumsCheck.Refresh

    ' Require channel external trigger for validation
    If APP32_TstBit(CCSRA_CHANTRIG, localChanCSRAlng) = 1 Then
        DAQParametersSetup.ChanTrigCheck.Value = 1
    Else
        DAQParametersSetup.ChanTrigCheck.Value = 0
    End If
    DAQParametersSetup.ChanTrigCheck.Refresh

    ' Normal pileup rejection
    If APP32_TstBit(CCSRA_PILEUPCTRL, localChanCSRAlng) = 1 Then
        DAQParametersSetup.NormalPileupControlCheck.Value = 1
    Else
        DAQParametersSetup.NormalPileupControlCheck.Value = 0
    End If
    DAQParametersSetup.NormalPileupControlCheck.Refresh

    ' Inverse pileup rejection
    If APP32_TstBit(CCSRA_INVERSEPILEUP, localChanCSRAlng) = 1 Then
        DAQParametersSetup.InversePileupControlCheck.Value = 1
    Else
        DAQParametersSetup.InversePileupControlCheck.Value = 0
    End If
    DAQParametersSetup.InversePileupControlCheck.Refresh

    If IsLoaded("AdvancedChanCSRABitsForm") Then

        ' Fast trigger selection
        If APP32_TstBit(CCSRA_FTRIGSEL, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanFTRIGSELCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanFTRIGSELCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanFTRIGSELCheck.Refresh
    
        ' Module validation trigger selection
        If APP32_TstBit(CCSRA_EXTTRIGSEL, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ModuleETRIGSELCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ModuleETRIGSELCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ModuleETRIGSELCheck.Refresh
    
        ' Channel validation trigger selection
        If APP32_TstBit(CCSRA_CHANTRIGSEL, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanETRIGSELCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanETRIGSELCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanETRIGSELCheck.Refresh
    
        ' Data acq synchronization
        If APP32_TstBit(CCSRA_SYNCDATAACQ, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanSyncDataAcq.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanSyncDataAcq.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanSyncDataAcq.Refresh
    
        ' Enable channel veto
        If APP32_TstBit(CCSRA_VETOENA, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanEnableVetoCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanEnableVetoCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanEnableVetoCheck.Refresh
    
        ' Energy cut feature
        If APP32_TstBit(CCSRA_ENAENERGYCUT, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanEnableEnergyCutCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanEnableEnergyCutCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanEnableEnergyCutCheck.Refresh

        ' Group trigger selection
        If APP32_TstBit(CCSRA_GROUPTRIGSEL, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanGroupTrigSelCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanGroupTrigSelCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanGroupTrigSelCheck.Refresh

        ' Channel veto selection
        If APP32_TstBit(CCSRA_CHANVETOSEL, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ChanVetoSelCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ChanVetoSelCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ChanVetoSelCheck.Refresh

        ' Module veto selection
        If APP32_TstBit(CCSRA_MODVETOSEL, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.ModVetoSelCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.ModVetoSelCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.ModVetoSelCheck.Refresh
        
        ' Recording external clock timestamps in event header
        If APP32_TstBit(CCSRA_EXTTSENA, localChanCSRAlng) = 1 Then
            AdvancedChanCSRABitsForm.EnaRecordExtTSCheck.Value = 1
        Else
            AdvancedChanCSRABitsForm.EnaRecordExtTSCheck.Value = 0
        End If
        AdvancedChanCSRABitsForm.EnaRecordExtTSCheck.Refresh
    End If

    '###########################################
    '
    '   CFD trigger tab
    '
    '###########################################
    
    With DAQParametersSetup.CFDTriggerGrid
        .Redraw = False
       
        ' CFD Delay
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(CFDDelay(X - 1), "0.000")
        Next X
        
        ' CFD scaling factor
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = CFDScale(X - 1)
        Next X
        
        ' CFD threshold
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 3) = CFDThresh(X - 1)
        Next X
        
        .Redraw = True
    End With

    '###########################################
    '
    '   Trigger Stretch Lengths tab
    '
    '###########################################
    
    With DAQParametersSetup.TriggerLengthsGrid
        .Redraw = False
        
        ' External Trigger Stretch
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(ExtTrigStretch(X - 1), "0.000")
        Next X
        
        ' Channel Trigger Stretch
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(ChanTrigStretch(X - 1), "0.000")
        Next X
        
        ' Veto Stretch
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 3) = Format(VetoStretch(X - 1), "0.000")
        Next X
        
        ' Length of the Fast Trigger Transmitted on the BackPlane
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 4) = Format(FastTrigBackPlaneLen(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With

    '###########################################
    '
    '   FIFO Delays tab
    '
    '###########################################
    
    With DAQParametersSetup.FIFODelaysGrid
        .Redraw = False
        
        ' External Delay Length
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(ExtDelayLength(X - 1), "0.000")
        Next X
        
        ' Delay of Fast Trigger transmitted on the Backplane
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(FastTrigBackplaneDelay(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With

    '###########################################
    '
    '   Multiplicity tab
    '
    '###########################################
    
    With DAQParametersSetup.MultiplicityGrid
        .Redraw = False
        
        ' Multiplicity Mask Low
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = ToHex(MultiplicityMaskL(X - 1))
        Next X
        
        ' Multiplicity Mask High
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = ToHex(MultiplicityMaskH(X - 1))
        Next X
        
        .Redraw = True
    End With

    With DAQParametersSetup.TrigConfigGrid
        .Redraw = False
        bDoNotEditTC = True
        
        .Cols = 5
        .Rows = Number_Modules_Total + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 800
        
        If Number_Modules_Total <= 8 Then
            .ColWidth(1) = 1040
            .ColWidth(2) = 1040
            .ColWidth(3) = 1040
            .ColWidth(4) = 1045
        Else
            .ColWidth(1) = 975
            .ColWidth(2) = 975
            .ColWidth(3) = 975
            .ColWidth(4) = 980
        End If
        
        .RowHeight(0) = 500
        For X = 1 To Number_Modules_Total
            .RowHeight(X) = 350
        Next X
       
        .TextMatrix(0, 0) = "Module #"
        .TextMatrix(0, 1) = "TrigConfig0"
        .TextMatrix(0, 2) = "TrigConfig1"
        .TextMatrix(0, 3) = "TrigConfig2"
        .TextMatrix(0, 4) = "TrigConfig3"
       
        For X = 0 To 4
            For Y = 0 To Number_Modules_Total
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Module #
        For X = 1 To Number_Modules_Total
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        ' Trigger configuration register #0
        For X = 1 To Number_Modules_Total
            If TrigConfig0(X - 1) < 0 Then
                .TextMatrix(X, 1) = ToHex(CDbl(TrigConfig0(X - 1)) + 4294967296#)
            Else
                .TextMatrix(X, 1) = ToHex(CDbl(TrigConfig0(X - 1)))
            End If
        Next X
        
        ' Trigger configuration register #1
        For X = 1 To Number_Modules_Total
            If TrigConfig1(X - 1) < 0 Then
                .TextMatrix(X, 2) = ToHex(CDbl(TrigConfig1(X - 1)) + 4294967296#)
            Else
                .TextMatrix(X, 2) = ToHex(CDbl(TrigConfig1(X - 1)))
            End If
        Next X
        
        ' Trigger configuration register #2
        For X = 1 To Number_Modules_Total
            If TrigConfig2(X - 1) < 0 Then
                .TextMatrix(X, 3) = ToHex(CDbl(TrigConfig2(X - 1)) + 4294967296#)
            Else
                .TextMatrix(X, 3) = ToHex(CDbl(TrigConfig2(X - 1)))
            End If
        Next X
        
        ' Trigger configuration register #3
        For X = 1 To Number_Modules_Total
            If TrigConfig3(X - 1) < 0 Then
                .TextMatrix(X, 4) = ToHex(CDbl(TrigConfig3(X - 1)) + 4294967296#)
            Else
                .TextMatrix(X, 4) = ToHex(CDbl(TrigConfig3(X - 1)))
            End If
        Next X
        
        bDoNotEditTC = False
        .Redraw = True
    End With

    With DAQParametersSetup.FTNNEnaGrid
        .Redraw = False
        bDoNotEditFTNN = True
        
        .Cols = 2
        .Rows = Number_Modules_Total + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 800
        .ColWidth(1) = 4150
        
        .RowHeight(0) = 500
        For X = 1 To Number_Modules_Total
            .RowHeight(X) = 350
        Next X
       
        .TextMatrix(0, 0) = "Module #"
        .TextMatrix(0, 1) = "Fast trigger nearest neighbor enable"
       
        For X = 0 To 1
            For Y = 0 To Number_Modules_Total
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Module #
        For X = 1 To Number_Modules_Total
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        ' Fast trigger nearest neighbor enable
        For X = 1 To Number_Modules_Total
            If FTNNEna(X - 1) < 0 Then
                .TextMatrix(X, 1) = ToHex(CDbl(FTNNEna(X - 1)) + 4294967296#)
            Else
                .TextMatrix(X, 1) = ToHex(CDbl(FTNNEna(X - 1)))
            End If
        Next X

        bDoNotEditFTNN = False
        .Redraw = True
    End With

    '###########################################
    '
    '   QDC tab
    '
    '###########################################
    
    With DAQParametersSetup.QDCGrid
        .Redraw = False
        
        ' QDC Length 0
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 1) = Format(QDCLen0(X - 1), "0.000")
        Next X
        
        ' QDC Length 1
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 2) = Format(QDCLen1(X - 1), "0.000")
        Next X
        
        ' QDC Length 2
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 3) = Format(QDCLen2(X - 1), "0.000")
        Next X
        
        ' QDC Length 3
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 4) = Format(QDCLen3(X - 1), "0.000")
        Next X
        
        ' QDC Length 4
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 5) = Format(QDCLen4(X - 1), "0.000")
        Next X
        
        ' QDC Length 5
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 6) = Format(QDCLen5(X - 1), "0.000")
        Next X
        
        ' QDC Length 6
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 7) = Format(QDCLen6(X - 1), "0.000")
        Next X
        
        ' QDC Length 7
        For X = 1 To NUM_CHANNELS
            .TextMatrix(X, 8) = Format(QDCLen7(X - 1), "0.000")
        Next X
        
        .Redraw = True
    End With

    UpdateChanParCtrlsOnDAQForm = 0    '   Success
    Exit Function
    
ErrMSGOnConversion:

    ErrTxt = "(UpdateChanParCtrlsOnDAQForm) failed to convert ChanCSRA"
    Msg = MsgBox(ErrTxt, 0, "Error")
    UpdateChanParCtrlsOnDAQForm = -1
    
End Function


'###########################################
'   RetrieveModParFromModule()
'       Read module parameters from a module
'###########################################

Public Function RetrieveModParFromModule(ByVal ModNum As Integer) As Integer
    Dim X As Integer
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim retval As Long
    
    retval = Pixie16ReadSglModPar("MODULE_CSRA", ModCSRA(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read MODULE_CSRA. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -1
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("MODULE_CSRB", ModCSRB(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read MODULE_CSRB. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -2
        Exit Function
    End If
  
    retval = Pixie16ReadSglModPar("MAX_EVENTS", MaxEvents(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read MAX_EVENTS. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -3
        Exit Function
    End If

    retval = Pixie16ReadSglModPar("SYNCH_WAIT", SynchWait(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read SYNCH_WAIT. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -4
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("IN_SYNCH", InSynch(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read IN_SYNCH. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -5
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("HOST_RT_PRESET", HostRunTimePreset(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read HOST_RT_PRESET. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -5
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", SlowFilterRange(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read SLOW_FILTER_RANGE. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -6
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("FAST_FILTER_RANGE", FastFilterRange(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read FAST_FILTER_RANGE. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -7
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("TrigConfig0", TrigConfig0(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read TrigConfig0. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -7
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("TrigConfig1", TrigConfig1(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read TrigConfig1. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -7
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("TrigConfig2", TrigConfig2(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read TrigConfig2. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -7
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("TrigConfig3", TrigConfig3(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read TrigConfig3. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -7
        Exit Function
    End If
    
    retval = Pixie16ReadSglModPar("FastTrigBackplaneEna", FTNNEna(ModNum), ModNum)
    If retval < 0 Then
        ErrTxt = "(RetrieveModParFromModule) failed to read FastTrigBackplaneEna. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        RetrieveModParFromModule = -7
        Exit Function
    End If
    
    ' Update list mode trace interval
    LMTraceDT(ModNum) = 2 ^ FastFilterRange(ModNum) / SYSTEM_CLOCK_MHZ(ModNum)
    
    RetrieveModParFromModule = 0   '   Success
    
End Function


'###########################################
'   UpdateModParCtrlsOnMDIForm()
'       Update module parameters on the MDI form
'###########################################

Public Function UpdateModParCtrlsOnMDIForm(ByVal ModNum As Integer) As Integer
    
    If SynchWait(ModNum) = 1 Then
        Pixie16MDIForm.SynchStartStopCheckBox.Value = 1
    Else
        Pixie16MDIForm.SynchStartStopCheckBox.Value = 0
    End If

    If InSynch(ModNum) = 0 Then
        Pixie16MDIForm.SynchClocksCheckBox.Value = 1
    Else
        Pixie16MDIForm.SynchClocksCheckBox.Value = 0
    End If

    Pixie16MDIForm.PresetRunTimeEdit.Value = IEEEFloating2Decimal(HostRunTimePreset(ModNum))

    UpdateModParCtrlsOnMDIForm = 0    '   Success
    
End Function


'###########################################
'   UpdateModParCtrlsOnDAQForm()
'       Update module parameters on the DAQ parameters
'       setup form
'###########################################

Public Function UpdateModParCtrlsOnDAQForm(ByVal ModNum As Integer) As Integer
    Dim localModCSRB As Long
    
    DAQParametersSetup.SlowFilterRangeEdit.Value = SlowFilterRange(ModNum)

    '###########################################
    '
    '   Control registers tab
    '
    '###########################################
    
    localModCSRB = ModCSRB(DAQParametersSetup.DAQParSetupModNumEdit.Value)
    
    ' Enable pullups for backplane bus lines
    If APP32_TstBit(MODCSRB_PULLUP, localModCSRB) = 1 Then
        DAQParametersSetup.ModulePullUpsCheck.Value = 1
    Else
        DAQParametersSetup.ModulePullUpsCheck.Value = 0
    End If
    DAQParametersSetup.ModulePullUpsCheck.Refresh

    ' Accept external trigger and run inhibit signals
    If APP32_TstBit(MODCSRB_DIRMOD, localModCSRB) = 1 Then
        DAQParametersSetup.DirectorModuleCheck.Value = 1
    Else
        DAQParametersSetup.DirectorModuleCheck.Value = 0
    End If
    DAQParametersSetup.DirectorModuleCheck.Refresh

    ' Crate master module (multiple crates only)
    If APP32_TstBit(MODCSRB_CHASSISMASTER, localModCSRB) = 1 Then
        DAQParametersSetup.CrateMasterCheck.Value = 1
    Else
        DAQParametersSetup.CrateMasterCheck.Value = 0
    End If
    DAQParametersSetup.CrateMasterCheck.Refresh

    ' Enable run inhibit signal input
    If APP32_TstBit(MODCSRB_INHIBITENA, localModCSRB) = 1 Then
        DAQParametersSetup.EnableRunInhibitCheck.Value = 1
    Else
        DAQParametersSetup.EnableRunInhibitCheck.Value = 0
    End If
    DAQParametersSetup.EnableRunInhibitCheck.Refresh

    ' Multiple crates
    If APP32_TstBit(MODCSRB_MULTCRATES, localModCSRB) = 1 Then
        DAQParametersSetup.MultipleCratesCheck.Value = 1
    Else
        DAQParametersSetup.MultipleCratesCheck.Value = 0
    End If
    DAQParametersSetup.MultipleCratesCheck.Refresh

    If IsLoaded("AdvancedModCSRBBitsForm") Then
    
        ' Global fast trigger selection
        If APP32_TstBit(MODCSRB_GFTSEL, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleGFTSELCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleGFTSELCheck.Value = 0
        End If
        AdvancedModCSRBBitsForm.ModuleGFTSELCheck.Refresh
        
        ' External validation trigger selection
        If APP32_TstBit(MODCSRB_ETSEL, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleETSELCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleETSELCheck.Value = 0
        End If
        AdvancedModCSRBBitsForm.ModuleETSELCheck.Refresh
        
        ' Sort events
        If APP32_TstBit(MODCSRB_SORTEVENTS, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleSortEventsCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleSortEventsCheck.Value = 0
        End If
        AdvancedModCSRBBitsForm.ModuleSortEventsCheck.Refresh
        
        ' Send fast triggers to backplane
        If APP32_TstBit(MODCSRB_BKPLFASTTRIG, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleSendFTBackplaneCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleSendFTBackplaneCheck.Value = 0
        End If
        AdvancedModCSRBBitsForm.ModuleSendFTBackplaneCheck.Refresh
    
    End If
    
    UpdateModParCtrlsOnDAQForm = 0    '   Success
    
End Function


'###########################################
'   fitGauss()
'       Gauss fit of a spectrum
'###########################################

Public Function fitGauss(spectrum() As Double, ByVal lower As Long, _
                        ByVal upper As Long, ByRef pos As Double, ByRef fwhm As Double)
    ' long spectrum[];                Input: spectrum to fit                  */
    ' int *lower;                     Input: starting bin to use in fit       */
    ' int *upper;                     Input: ending bin to use in fit         */
    ' float *pos;                     Output: fit result - peak position      */
    ' float *fwhm;                    Output: fit result - full width half max*/
    Dim s0 As Long
    Dim i As Long
    Dim s1 As Double
    Dim sx As Double
    Dim sy As Double
    Dim sxx As Double
    Dim sxy As Double
    Dim det As Double
    Dim a As Double
    Dim b As Double
    Dim fact As Double
    Dim X As Double
    Dim Y As Double
    Dim wt As Double
    Dim cut1 As Long
    Dim cut2 As Long
    
    pos = 0#
    fwhm = 0#
    
    cut1 = 10     'minimum number of events in bin
    cut2 = 3      'minimum number of bins used in fit
    
    s0 = 0
    s1 = sx = sy = sxx = sxy = 0#
    If (lower = upper) Then
        fitGauss = 1
        Exit Function
    End If
    If (lower = LBound(spectrum)) Then lower = LBound(spectrum) + 1
    If (upper = UBound(spectrum)) Then upper = UBound(spectrum) - 1
    For i = lower To upper
        If (Not ((spectrum(i - 1) < cut1) Or (spectrum(i + 1) < cut1))) Then
            s0 = s0 + 1
            X = CDbl(i) + 0.5
            Y = Log((spectrum(i - 1) + 0#) / spectrum(i + 1))
            wt = 1# / (1# / spectrum(i + 1) + 1# / spectrum(i - 1))
            s1 = s1 + wt
            sx = sx + wt * X
            sy = sy + wt * Y
            sxx = sxx + wt * X * X
            sxy = sxy + wt * X * Y
        End If
    Next i
    If (s0 < cut2) Then
        fitGauss = 2
        Exit Function
    End If
    det = s1 * sxx - sx * sx
    If (det = 0#) Then
        fitGauss = 3
        Exit Function
    End If
    fact = 1# / det
    a = fact * (sy * sxx - sx * sxy)
    b = fact * (s1 * sxy - sx * sy)
    If (b <= 0#) Then
        fitGauss = 4
        Exit Function
    End If
    fwhm = CDbl(2.355 * Sqr(2# / b))
    pos = CDbl(-a / b)
    fitGauss = 0
End Function


'###########################################
'   CenterChild()
'       Display the child form in the center
'###########################################

Public Function CenterChild(Parent As Form, Child As Form)
    Dim iTop As Integer
    Dim iLeft As Integer
    If Parent.WindowState <> 0 Then Exit Function
    iTop = ((Parent.Height - Child.Height) \ 2)
    iLeft = ((Parent.Width - Child.Width) \ 2)
    Child.Move iLeft, iTop
End Function


'###########################################
'   UpdateADCSampleIntervals()
'       Update ADC sample intervals on the
'       AcquireADCTracesForm
'###########################################

Public Function UpdateADCSampleIntervals(ModNum As Integer)
    Dim ComboListIndex As Integer
  
    ComboListIndex = Round(Log(Xdt(0) * DSP_CLOCK_MHZ / XDT_SCALE_FACTOR(ModNum)) / Log(2#))
    AcquireADCTracesForm.SampleIntervalComboChs0to3.ListIndex = ComboListIndex
    
    ComboListIndex = Round(Log(Xdt(1) * DSP_CLOCK_MHZ / XDT_SCALE_FACTOR(ModNum)) / Log(2#))
    AcquireADCTracesForm.SampleIntervalComboChs4to7.ListIndex = ComboListIndex
    
    ComboListIndex = Round(Log(Xdt(2) * DSP_CLOCK_MHZ / XDT_SCALE_FACTOR(ModNum)) / Log(2#))
    AcquireADCTracesForm.SampleIntervalComboChs8to11.ListIndex = ComboListIndex

    ComboListIndex = Round(Log(Xdt(3) * DSP_CLOCK_MHZ / XDT_SCALE_FACTOR(ModNum)) / Log(2#))
    AcquireADCTracesForm.SampleIntervalComboChs12to15.ListIndex = ComboListIndex

End Function


'###########################################
'   UpdatePXISlotsGrid()
'       Update PXI Slots Grid
'###########################################

Public Function UpdatePXISlotsGrid()
    Dim X As Integer
    
    With Pixie16MDIForm.SetupPXISlotsGrid
        .Redraw = False
        
        For X = 1 To .Rows - 1
            If ModRev(X - 1) = 11 Then
                .TextMatrix(X, 1) = "B"
            ElseIf ModRev(X - 1) = 12 Then
                .TextMatrix(X, 1) = "C"
            ElseIf ModRev(X - 1) = 13 Then
                .TextMatrix(X, 1) = "D"
            ElseIf ModRev(X - 1) = 15 Then
                .TextMatrix(X, 1) = "F"
            Else
                .TextMatrix(X, 1) = "X"
            End If
                        
            .TextMatrix(X, 2) = ModSerNum(X - 1)
            .TextMatrix(X, 3) = ModADCBits(X - 1)
            .TextMatrix(X, 4) = ModADCMSPS(X - 1)
        Next X

        .Redraw = True
    End With

End Function


'###########################################
'   Set_SYSTEM_CLOCK_MHZ()
'       Recalculate SYSTEM_CLOCK_MHZ
'###########################################

Public Function Set_SYSTEM_CLOCK_MHZ()
    Dim ModNum As Integer
    
    For ModNum = 0 To Number_Modules_Total - 1
        Select Case ModRev(ModNum)
            Case 11, 12, 13
                SYSTEM_CLOCK_MHZ(ModNum) = 100
            Case 15
                If ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 100 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 100
                ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 100 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 100
                ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 250 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 250
                ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 250 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 250
                ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 250 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 250
                ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 500 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 500
                ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 500 Then
                    SYSTEM_CLOCK_MHZ(ModNum) = 500
                End If
        End Select
    Next ModNum
    
End Function


'###########################################
'   Set_XDT_SCALE_FACTOR()
'       Recalculate XDT_SCALE_FACTOR
'###########################################

Public Function Set_XDT_SCALE_FACTOR()
    Dim ModNum As Integer
    
    For ModNum = 0 To Number_Modules_Total - 1
        Select Case ModRev(ModNum)
            Case 11, 12, 13
                XDT_SCALE_FACTOR(ModNum) = 6
            Case 15
                If ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 100 Then
                    XDT_SCALE_FACTOR(ModNum) = 6
                ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 100 Then
                    XDT_SCALE_FACTOR(ModNum) = 6
                ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 250 Then
                    XDT_SCALE_FACTOR(ModNum) = 8
                ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 250 Then
                    XDT_SCALE_FACTOR(ModNum) = 8
                ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 250 Then
                    XDT_SCALE_FACTOR(ModNum) = 8
                ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 500 Then
                    XDT_SCALE_FACTOR(ModNum) = 6
                ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 500 Then
                    XDT_SCALE_FACTOR(ModNum) = 6
                End If
        End Select
    Next ModNum
    
End Function


'###########################################
'   Set_ADC_Sampling_Intervals()
'       Set ADC sampling intervals
'###########################################

Public Function Set_ADC_Sampling_Intervals(ModNum As Integer)
    Dim i As Integer
        
    If ModRev(ModNum) = 11 Or ModRev(ModNum) = 12 Or ModRev(ModNum) = 13 Then
        
        ' Check whether we need to remove old items
        If FirstTimeSettingADCSamplingInterval = False Then
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs0to3.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs0to3.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs4to7.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs4to7.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs8to11.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs8to11.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs12to15.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs12to15.RemoveItem 0
            Next i
        End If
                
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "15.36 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "15.36 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "15.36 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "15.36 탎"

    ElseIf ModRev(ModNum) = 15 And ModADCMSPS(ModNum) = 250 Then
    
        ' Check whether we need to remove old items
        If FirstTimeSettingADCSamplingInterval = False Then
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs0to3.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs0to3.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs4to7.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs4to7.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs8to11.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs8to11.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs12to15.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs12to15.RemoveItem 0
            Next i
        End If
    
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "80 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "160 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "320 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "640 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "1.28 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "2.56 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "5.12 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "10.24 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "20.48 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "80 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "160 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "320 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "640 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "1.28 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "2.56 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "5.12 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "10.24 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "20.48 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "80 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "160 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "320 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "640 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "1.28 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "2.56 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "5.12 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "10.24 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "20.48 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "80 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "160 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "320 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "640 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "1.28 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "2.56 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "5.12 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "10.24 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "20.48 탎"

    Else

        ' Check whether we need to remove old items
        If FirstTimeSettingADCSamplingInterval = False Then
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs0to3.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs0to3.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs4to7.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs4to7.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs8to11.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs8to11.RemoveItem 0
            Next i
            For i = 1 To AcquireADCTracesForm.SampleIntervalComboChs12to15.ListCount
                AcquireADCTracesForm.SampleIntervalComboChs12to15.RemoveItem 0
            Next i
        End If

        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs0to3.AddItem "15.36 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs4to7.AddItem "15.36 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs8to11.AddItem "15.36 탎"
        
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "60 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "120 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "240 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "480 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "960 ns"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "1.92 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "3.84 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "7.68 탎"
        AcquireADCTracesForm.SampleIntervalComboChs12to15.AddItem "15.36 탎"
    End If

End Function


