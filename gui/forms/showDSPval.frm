VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Begin VB.Form showDSPval 
   Caption         =   "Show DSP Parameter Values"
   ClientHeight    =   9615
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   12075
   Icon            =   "showDSPval.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   9615
   ScaleWidth      =   12075
   Begin VB.CommandButton ConvertDataType 
      Caption         =   "Hexadecimal"
      Height          =   375
      Left            =   5280
      TabIndex        =   11
      Top             =   9000
      Width           =   1215
   End
   Begin VB.CommandButton ApplyDSPParameters 
      Caption         =   "Apply"
      Height          =   615
      Left            =   8400
      TabIndex        =   10
      Top             =   8760
      Width           =   1455
   End
   Begin VB.TextBox txtEdit2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000000&
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   4080
      TabIndex        =   9
      Top             =   8400
      Width           =   1455
   End
   Begin VB.TextBox txtEdit1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000000&
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   120
      TabIndex        =   8
      Top             =   8400
      Width           =   1455
   End
   Begin VB.CommandButton CloseShowDSPVal 
      Caption         =   "Close"
      Height          =   615
      Left            =   10080
      TabIndex        =   7
      Top             =   8760
      Width           =   1455
   End
   Begin VB.CommandButton RefreshDSPVal 
      Caption         =   "Refresh"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   6720
      TabIndex        =   6
      Top             =   8760
      Width           =   1455
   End
   Begin MSFlexGridLib.MSFlexGrid MSFlexGrid3 
      Height          =   8175
      Left            =   8200
      TabIndex        =   2
      Top             =   480
      Width           =   3735
      _ExtentX        =   6588
      _ExtentY        =   14420
      _Version        =   393216
      BackColorFixed  =   -2147483643
   End
   Begin MSFlexGridLib.MSFlexGrid MSFlexGrid2 
      Height          =   8175
      Left            =   4230
      TabIndex        =   1
      Top             =   480
      Width           =   3900
      _ExtentX        =   6879
      _ExtentY        =   14420
      _Version        =   393216
      BackColorFixed  =   -2147483643
   End
   Begin MSFlexGridLib.MSFlexGrid MSFlexGrid1 
      Height          =   8175
      Left            =   200
      TabIndex        =   0
      Top             =   480
      Width           =   3950
      _ExtentX        =   6985
      _ExtentY        =   14420
      _Version        =   393216
      BackColorFixed  =   -2147483643
   End
   Begin CWUIControlsLib.CWNumEdit ShowDSPValModNumEdit 
      Height          =   495
      Left            =   1800
      TabIndex        =   13
      Top             =   9000
      Width           =   735
      _Version        =   393218
      _ExtentX        =   1296
      _ExtentY        =   873
      _StockProps     =   4
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Reset_0         =   0   'False
      CompatibleVers_0=   393218
      NumEdit_0       =   1
      ClassName_1     =   "CCWNumEdit"
      opts_1          =   458814
      ButtonPosition_1=   1
      format_1        =   2
      ClassName_2     =   "CCWFormat"
      Format_2        =   "."
      scale_1         =   3
      ClassName_3     =   "CCWScale"
      opts_3          =   65536
      dMax_3          =   10
      discInterval_3  =   1
      ValueVarType_1  =   5
      IncValueVarType_1=   5
      IncValue_Val_1  =   1
      AccelIncVarType_1=   5
      AccelInc_Val_1  =   1
      RangeMinVarType_1=   5
      RangeMaxVarType_1=   5
      RangeMax_Val_1  =   6
      Bindings_1      =   4
      ClassName_4     =   "CCWBindingHolderArray"
      Editor_4        =   5
      ClassName_5     =   "CCWBindingHolderArrayEditor"
      Owner_5         =   1
   End
   Begin VB.Label ShowDSPValModNumLabel 
      Caption         =   "Module Number"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   240
      TabIndex        =   12
      Top             =   9120
      Width           =   1575
   End
   Begin VB.Label Results 
      Alignment       =   2  'Center
      Caption         =   "Results"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   9160
      TabIndex        =   5
      Top             =   120
      Width           =   1935
   End
   Begin VB.Label ChannelParameters 
      Alignment       =   2  'Center
      Caption         =   "Channel Parameters"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   4950
      TabIndex        =   4
      Top             =   120
      Width           =   1935
   End
   Begin VB.Label ModuleParameters 
      Alignment       =   2  'Center
      Caption         =   "Module Parameters"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   1100
      TabIndex        =   3
      Top             =   120
      Width           =   1935
   End
End
Attribute VB_Name = "showDSPval"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim bDoNotEdit1   As Boolean
Dim bDoNotEdit2   As Boolean

Dim bOnFixedPart1 As Boolean
Dim bOnFixedPart2 As Boolean


Private Sub CloseShowDSPVal_Click()
    showDSPval.Hide
End Sub

Private Sub ConvertDataType_Click()
    Dim X As Integer
    Dim startaddr As Long
    
    ' Calculate the starting address of the DSP IOparams
    startaddr = DSP_IOPARAMS_ADDR - DSP_IMBUFFER_ADDR
    
    ' Update data type indicator
    If DSPValIsHexadecimal = True Then
        DSPValIsHexadecimal = False
        ConvertDataType.Caption = "Decimal"
    Else
        DSPValIsHexadecimal = True
        ConvertDataType.Caption = "Hexadecimal"
    End If
    
    ' Update display for module parameters
    With MSFlexGrid1
        .Redraw = False
        
        For X = 1 To NUM_MODULE_PAR
            If DSPMemBlock0(startaddr + X - 1) < 0 Then
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + X - 1)) + 4294967296#)
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + X - 1)) + 4294967296#
                End If
            Else
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + X - 1)))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + X - 1))
                End If
            End If
        Next X
        
        .Redraw = True
    End With

    ' Update display for channel parameters
    With MSFlexGrid2
        .Redraw = False
        
        For X = 1 To NUM_CHANNEL_PAR
            If DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) < 0 Then
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) + 4294967296#))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) + 4294967296#)
                End If
            Else
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1)))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1))
                End If
            End If
        Next X
        
        .Redraw = True
    End With

    ' Update display for result parameters
    With MSFlexGrid3
        .Redraw = False
        
        For X = 1 To NUM_RESULTS_PAR
            If DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1) < 0 Then
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1) + 4294967296#))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1) + 4294967296#)
                End If
            Else
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1)))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1))
                End If
            End If
        Next X
        
        .Redraw = True
    End With

End Sub

Private Sub Form_Load()
   
    ' Update maximum number of modules
    ShowDSPValModNumEdit.Maximum = Number_Modules_Total - 1
    
    ' Load DSP variables and the grids - use default module 0
    Call Load_DSPvals(0)
    
    ' Set appropriate caption
    If DSPValIsHexadecimal = True Then
        ConvertDataType.Caption = "Hexadecimal"
    Else
        ConvertDataType.Caption = "Decimal"
    End If

End Sub


Private Sub Form_Click()
    Call pSetCellValue
End Sub

Private Sub pSetCellValue()
    '
    ' NOTE:
    '       This code should be called anytime
    '       the grid loses focus and the grid's
    '       contents may change.  Otherwise, the
    '       cell's new value may be lost and the
    '       textbox may not line up correctly.
    '
    If txtEdit1.Visible Then
        MSFlexGrid1.Text = txtEdit1.Text
        txtEdit1.Visible = False
    End If
    
    If txtEdit2.Visible Then
        MSFlexGrid2.Text = txtEdit2.Text
        txtEdit2.Visible = False
    End If
    
End Sub


Private Sub MSFlexGrid1_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPart1 Then Exit Sub
    Call pEditGrid1(32)
End Sub

Private Sub pEditGrid1(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEdit1
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = MSFlexGrid1
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57, 65 To 70
                '
                ' Replace the current text but only
                ' if the user entered a number.
                '
                .Text = Chr(KeyAscii)
                .SelStart = 1
            Case Else
                '
                ' If an alpha character was entered,
                ' use a zero instead.
                '
                .Text = "0"
        End Select
    End With
    '
    ' Show the textbox at the right place.
    '
    With MSFlexGrid1
        If .CellWidth < 0 Then Exit Sub
        txtEdit1.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEdit.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEdit1.Visible = True
    txtEdit1.SetFocus
End Sub


Private Sub MSFlexGrid1_GotFocus()
    
    If bDoNotEdit1 Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
End Sub


Private Sub MSFlexGrid1_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditGrid1(KeyAscii)
End Sub


Private Sub MSFlexGrid1_LeaveCell()
    
    If bDoNotEdit1 Then Exit Sub
    Call MSFlexGrid1_GotFocus
End Sub


Private Sub MSFlexGrid1_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With MSFlexGrid1
        For k = 0 To .Cols - 1
            If .ColIsVisible(k) Then
                lWidth = lWidth + .ColWidth(k)
            End If
        Next k
        For k = 0 To .Rows - 1
            If .RowIsVisible(k) Then
                lHeight = lHeight + .RowHeight(k)
            End If
        Next
        '
        ' See if we are on the fixed part of the grid.
        '
        bOnFixedPart1 = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub


Private Sub MSFlexGrid1_Scroll()
    Call MSFlexGrid1_GotFocus
End Sub


Private Function fLabel(lRow As Long, lCol As Long) As Long
    fLabel = lCol + MSFlexGrid1.Cols * lRow
End Function


Private Sub RefreshDSPVal_Click()
    Dim X As Long
    Dim result As Long
    Dim Msg As Integer
    Dim StrTxt As String
    Dim startaddr As Long
    Dim index As Long
    Dim ModuleNumber As Integer
    
    ModuleNumber = CInt(ShowDSPValModNumEdit.Text)
    
    ' Calculate the starting address of the DSP IOparams
    startaddr = DSP_IOPARAMS_ADDR - DSP_IMBUFFER_ADDR
    
    ' Check if we are using online mode
    If IsOfflineOperation = 0 Then
    
        ' Read module parameters
        result = Pixie16IMbufferIO(DSPMemBlock0(startaddr), NUM_MODULE_PAR, DSP_IOPARAMS_ADDR, MOD_READ, ModuleNumber)
        If result < 0 Then
            StrTxt = "RefreshDSPVal: Pixie16IMbufferIO for module # " & ModuleNumber & " failed, retval = " & result
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If

    Else
        
        ' In offline mode we clear DSPMemBlock0
        For index = startaddr To startaddr + NUM_MODULE_PAR - 1
            DSPMemBlock0(index) = 0
        Next index
    
    End If
    
    ' Update display for module parameters
    With MSFlexGrid1
        .Redraw = False
        
        For X = 1 To NUM_MODULE_PAR
            If DSPMemBlock0(startaddr + X - 1) < 0 Then
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + X - 1)) + 4294967296#)
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + X - 1)) + 4294967296#
                End If
            Else
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + X - 1)))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + X - 1))
                End If
            End If
        Next X
        
        .Redraw = True
    End With

    ' Check if we are using online mode
    If IsOfflineOperation = 0 Then
    
        ' Read channel parameters
        result = Pixie16IMbufferIO(DSPMemBlock0(startaddr + NUM_MODULE_PAR), NUM_CHANNEL_PAR, DSP_IOPARAMS_ADDR + NUM_MODULE_PAR, MOD_READ, ModuleNumber)
        If result < 0 Then
            StrTxt = "RefreshDSPVal: Pixie16IMbufferIO for module # " & ModuleNumber & " failed, retval = " & result
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If

    Else
        
        ' In offline mode we clear DSPMemBlock0
        For index = startaddr + NUM_MODULE_PAR To startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR - 1
            DSPMemBlock0(index) = 0
        Next index
    
    End If
    
    ' Update display for channel parameters
    With MSFlexGrid2
        .Redraw = False
        
        For X = 1 To NUM_CHANNEL_PAR
            If DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) < 0 Then
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) + 4294967296#))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) + 4294967296#)
                End If
            Else
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1)))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1))
                End If
            End If
        Next X
        
        .Redraw = True
    End With

    ' Check if we are using online mode
    If IsOfflineOperation = 0 Then
    
        ' Read result parameters
        result = Pixie16IMbufferIO(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR), NUM_RESULTS_PAR, DSP_IOPARAMS_ADDR + NUM_MODULE_PAR + NUM_CHANNEL_PAR, MOD_READ, ModuleNumber)
        If result < 0 Then
            StrTxt = "RefreshDSPVal: Pixie16IMbufferIO for module # " & ModuleNumber & " failed, retval = " & result
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If

    Else
        
        ' In offline mode we clear DSPMemBlock0
        For index = startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR To startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + NUM_RESULTS_PAR - 1
            DSPMemBlock0(index) = 0
        Next index
    
    End If
    
    ' Update display for result parameters
    With MSFlexGrid3
        .Redraw = False
        
        For X = 1 To NUM_RESULTS_PAR
            If DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1) < 0 Then
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1) + 4294967296#))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1) + 4294967296#)
                End If
            Else
                If DSPValIsHexadecimal = True Then
                    .TextMatrix(X, 2) = ToHex(CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1)))
                Else
                    .TextMatrix(X, 2) = CDbl(DSPMemBlock0(startaddr + NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1))
                End If
            End If
        Next X
        
        .Redraw = True
    End With

End Sub


Private Sub ApplyDSPParameters_Click()

    Dim X As Long
    Dim result As Long
    Dim Msg As Integer
    Dim StrTxt As String
    Dim startaddr As Long
    Dim ConvertedNum As Double
    Dim ModuleNumber As Integer
    Dim retval As Long
    
    ' Calculate the starting address of the DSP IOparams
    startaddr = DSP_IOPARAMS_ADDR - DSP_IMBUFFER_ADDR

    ' Retrieve module parameters
    With MSFlexGrid1
    
        For X = 1 To NUM_MODULE_PAR
            If DSPValIsHexadecimal = True Then
                ConvertedNum = FromHex(.TextMatrix(X, 2))
            Else
                ConvertedNum = CDbl(.TextMatrix(X, 2))
            End If
            
            If ConvertedNum > 2147483647 Then
                DSPMemBlock0(startaddr + X - 1) = ConvertedNum - 4294967296#
            Else
                DSPMemBlock0(startaddr + X - 1) = ConvertedNum
            End If
        Next
        
    End With
  
    ' Retrieve channel parameters
    With MSFlexGrid2
    
        For X = 1 To NUM_CHANNEL_PAR
            If DSPValIsHexadecimal = True Then
                ConvertedNum = FromHex(.TextMatrix(X, 2))
            Else
                ConvertedNum = CDbl(.TextMatrix(X, 2))
            End If
            
            If ConvertedNum > 2147483647 Then
                DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) = ConvertedNum - 4294967296#
            Else
                DSPMemBlock0(startaddr + NUM_MODULE_PAR + X - 1) = ConvertedNum
            End If
        Next
    
    End With
    
    ' Check if this is an online operation
    If IsOfflineOperation = 0 Then
        
        ModuleNumber = ShowDSPValModNumEdit.Value
        
        ' Download to the DSP
        result = Pixie16IMbufferIO(DSPMemBlock0(startaddr), N_DSP_IO_PAR, DSP_IOPARAMS_ADDR, MOD_WRITE, ModuleNumber)
        If result < 0 Then
            StrTxt = "ApplyDSPParameters: Pixie16IMbufferIO for module # " & ModuleNumber & " failed, retval = " & result
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
    
        ' Set DACs
        result = Pixie16ControlTaskRun(ModuleNumber, 0, 10000)
        If (result < 0) Then
            Msg = MsgBox("SetDACs started unsuccessfully", 0, "Error")
        End If
        
        ' Program Fippi
        result = Pixie16ControlTaskRun(ModuleNumber, 5, 10000)
        If (result < 0) Then
            Msg = MsgBox("ProgramFiPPI started unsuccessfully", 0, "Error")
        End If

        ' Update all relevant parameters
        retval = RetrieveModParFromModule(ModuleNumber)
        If retval < 0 Then
            StrTxt = "RetrieveModParFromModule " & "(Module # " & Str$(ModuleNumber) & ") returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        retval = RetrieveChanParFromModule(ModuleNumber)
        If retval < 0 Then
            StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
    
        If IsLoaded("DAQParametersSetup") Then
            If ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value Then
                ' Update module parameters displays for the currently selected module
                retval = UpdateModParCtrlsOnDAQForm(ModuleNumber)
                
                ' Update channel parameters displays
                retval = UpdateChanParCtrlsOnDAQForm()
            End If
        End If
        
        If IsLoaded("AcquireADCTracesForm") = True Then
            If ModuleNumber = AcquireADCTracesForm.ADCTracesModNumEdit.Value Then
                ' Set available ADC sample intervals based on module variant
                Call Set_ADC_Sampling_Intervals(ModuleNumber)
                
                ' Update ADC sample intervals
                Call UpdateADCSampleIntervals(ModuleNumber)
            End If
        End If
    End If
    
End Sub


Private Sub txtEdit1_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With MSFlexGrid1
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEdit1.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEdit1 = True
                    .Row = .Row - 1
                    bDoNotEdit1 = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEdit1 = True
                    .Row = .Row + 1
                    bDoNotEdit1 = False
                End If
        End Select
    End With
End Sub


Private Sub txtEdit1_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers plus 'A' to 'F'.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case 65 To 70
        Case Else
            KeyAscii = 0
    End Select
End Sub


Private Sub MSFlexGrid2_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPart2 Then Exit Sub
    Call pEditGrid2(32)
End Sub

Private Sub pEditGrid2(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEdit2
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = MSFlexGrid2
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57, 65 To 70
                '
                ' Replace the current text but only
                ' if the user entered a number or 'A' to 'F'.
                '
                .Text = Chr(KeyAscii)
                .SelStart = 1
            Case Else
                '
                ' If an alpha character was entered,
                ' use a zero instead.
                '
                .Text = "0"
        End Select
    End With
    '
    ' Show the textbox at the right place.
    '
    With MSFlexGrid2
        If .CellWidth < 0 Then Exit Sub
        txtEdit2.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEdit.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEdit2.Visible = True
    txtEdit2.SetFocus
End Sub


Private Sub MSFlexGrid2_GotFocus()
    
    If bDoNotEdit2 Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
End Sub


Private Sub MSFlexGrid2_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditGrid2(KeyAscii)
End Sub


Private Sub MSFlexGrid2_LeaveCell()
    
    If bDoNotEdit2 Then Exit Sub
    Call MSFlexGrid2_GotFocus
End Sub


Private Sub MSFlexGrid2_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With MSFlexGrid2
        For k = 0 To .Cols - 1
            If .ColIsVisible(k) Then
                lWidth = lWidth + .ColWidth(k)
            End If
        Next k
        For k = 0 To .Rows - 1
            If .RowIsVisible(k) Then
                lHeight = lHeight + .RowHeight(k)
            End If
        Next
        '
        ' See if we are on the fixed part of the grid.
        '
        bOnFixedPart2 = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub


Private Sub MSFlexGrid2_Scroll()
    Call MSFlexGrid2_GotFocus
End Sub


Private Function fLabe2(lRow As Long, lCol As Long) As Long
    fLabe2 = lCol + MSFlexGrid2.Cols * lRow
End Function


Private Sub txtEdit2_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With MSFlexGrid2
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEdit2.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEdit2 = True
                    .Row = .Row - 1
                    bDoNotEdit2 = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEdit2 = True
                    .Row = .Row + 1
                    bDoNotEdit2 = False
                End If
        End Select
    End With
End Sub


Private Sub txtEdit2_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers plus 'A' to 'F'.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case 65 To 70
        Case Else
            KeyAscii = 0
    End Select
End Sub


Private Sub Load_DSPvals(ModNum As Integer)
    Dim X As Integer
    Dim Y As Integer
    Dim LineStr1 As String
    Dim LineStr2 As String
    Dim startingaddr As Long
    Dim currentaddr As Long
    Dim nextaddr As Long
    Dim addrdiff As Long
    Dim Msg As Integer
    Dim ErrTxt As String
    Dim fso
    Dim Pixie16_DSP_Var_File As String
    
    ' Create a file system object
    Set fso = CreateObject("Scripting.FileSystemObject")
    
    ' Select Pixie16_DSP_Var_File
    Select Case ModRev(ModNum)
        Case 11, 12, 13
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevBCD
        Case 15
            If ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 100 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b100m
            ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 100 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF16b100m
            ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 250 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF12b250m
            ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 250 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b250m
            ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 250 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF16b250m
            ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 500 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF12b500m
            ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 500 Then
                Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b500m
            End If
    End Select
    
    ' Make sure DSP code variable/list files exist in the Offline mode
    ' Default module type in Offline mode is 12-bit, 250 MSPS
    If IsOfflineOperation = 1 Then
        If (fso.FileExists(Pixie16_DSP_Var_File_RevF12b250m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF12b250m
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevBCD)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevBCD
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevF14b100m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b100m
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevF16b100m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF16b100m
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevF14b250m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b250m
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevF16b250m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF16b250m
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevF12b500m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF12b500m
        ElseIf (fso.FileExists(Pixie16_DSP_Var_File_RevF14b500m)) Then
            Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b500m
        End If
    End If
        
    ' Check if Pixie16_DSP_Var_File exists
    If (fso.FileExists(Pixie16_DSP_Var_File)) Then
        Open Pixie16_DSP_Var_File For Input As #1
    Else    ' file doesn't exist
        ErrTxt = "Pixie16 DSP Var File " & Pixie16_DSP_Var_File & " doesn't exist. Please check boot file paths."
        Msg = MsgBox(ErrTxt, 0, "Error")
        Exit Sub
    End If
      
    With MSFlexGrid1
        .Redraw = False
        
        .Cols = 3
        .Rows = NUM_MODULE_PAR + 1
        
        .FixedCols = 2
        
        .ColWidth(0) = 600
        .ColWidth(1) = 1705
        .ColWidth(2) = 1310
              
        .TextMatrix(0, 0) = "Index"
        .TextMatrix(0, 1) = "DSP PAR"
        .TextMatrix(0, 2) = "DSP VAL"

        For X = 0 To 2
            For Y = 0 To NUM_MODULE_PAR
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignRightCenter
            Next Y
        Next X
        
        ' Read one line to obtain the starting address
        If Not EOF(1) Then
            Line Input #1, LineStr1
        Else
            ErrTxt = "DSP var file " & Pixie16_DSP_Var_File & " doesn't contain valid module parameters"
            Msg = MsgBox(ErrTxt, 0, "Error")
            ' Close the Pixie16_DSP_Var_File
            Close #1
            Exit Sub
        End If

        ' Extract the starting address
        startingaddr = Val("&H" & Mid(LineStr1, 6, 5))
        currentaddr = startingaddr
        
        ' Read the DSP variable names from the Pixie16_DSP_Var_File
        Do While 1
                
            ' Read the next line
            If Not EOF(1) Then
                Line Input #1, LineStr2
            Else
                LineStr2 = ""   ' to indicate the end of file
            End If
            
            If LineStr2 <> "" Then
                nextaddr = Val("&H" & Mid(LineStr2, 6, 5))
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = nextaddr - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = "[" & Format(X, "00") & "]"
                    Next X
                Else
                    .TextMatrix(currentaddr - startingaddr + 1, 1) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
            Else
                ' we already reach the end of file
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = startingaddr + NUM_MODULE_PAR - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = "[" & Format(X, "00") & "]"
                    Next X
                    
                Else
                    .TextMatrix(currentaddr - startingaddr + 1, 1) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
                LineStr1 = LineStr2
                Exit Do

            End If
            
            ' Update current address and linestr
            currentaddr = nextaddr
            LineStr1 = LineStr2
            
            ' Check if we are done with module parameters
            If currentaddr >= (startingaddr + NUM_MODULE_PAR) Then
                Exit Do
            End If
            
        Loop
        
        For X = 1 To NUM_MODULE_PAR
            .Row = X
            .Col = 0
            .TextMatrix(X, 0) = X - 1
        Next X
        
        ' Move to the first cell in the 'DSPVal' column
        .Row = 1
        .Col = 2
        
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEdit1.Font
        .Name = MSFlexGrid1.Font.Name
        .Size = MSFlexGrid1.Font.Size
        .Weight = MSFlexGrid1.Font.Weight
    End With
    txtEdit1.BackColor = vb3DLight
   
    txtEdit1 = ""
    bDoNotEdit1 = False
    
    With MSFlexGrid2
        .Redraw = False
        
        .Cols = 3
        .Rows = NUM_CHANNEL_PAR + 1
      
        .FixedCols = 2
        
        .ColWidth(0) = 600
        .ColWidth(1) = 1645
        .ColWidth(2) = 1310
       
        .TextMatrix(0, 0) = "Index"
        .TextMatrix(0, 1) = "DSP PAR"
        .TextMatrix(0, 2) = "DSP VAL"
        
        For X = 0 To 2
            For Y = 0 To NUM_CHANNEL_PAR
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignRightCenter
            Next Y
        Next X

        ' Continue to read the Pixie16_DSP_Var_File
        ' LineStr1 already loaded by the last read in MSFlexGrid1

        If LineStr1 = "" Then
            ErrTxt = "DSP var file " & Pixie16_DSP_Var_File & " doesn't contain valid channel parameters"
            Msg = MsgBox(ErrTxt, 0, "Error")
            ' Close the Pixie16_DSP_Var_File
            Close #1
            Exit Sub
        End If
        
        ' Extract the starting address
        startingaddr = Val("&H" & Mid(LineStr1, 6, 5))
        currentaddr = startingaddr
        
        ' Read the DSP variable names from the Pixie16_DSP_Var_File
        Do While 1
                
            ' Read the next line
            If Not EOF(1) Then
                Line Input #1, LineStr2
            Else
                LineStr2 = ""   ' to indicate the end of file
            End If
            
            If LineStr2 <> "" Then
                nextaddr = Val("&H" & Mid(LineStr2, 6, 5))
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = nextaddr - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = "[" & Format(X, "00") & "]"
                    Next X
                Else
                    .TextMatrix(currentaddr - startingaddr + 1, 1) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
                
            Else
                ' we already reach the end of file
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = startingaddr + NUM_CHANNEL_PAR - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = "[" & Format(X, "00") & "]"
                    Next X
                Else
                    .TextMatrix(currentaddr - startingaddr + 1, 1) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
                LineStr1 = LineStr2
                Exit Do
              
            End If
            
            ' Update current address and linestr
            currentaddr = nextaddr
            LineStr1 = LineStr2
            
            ' Check if we are done with channel parameters
            If currentaddr >= (startingaddr + NUM_CHANNEL_PAR) Then
                Exit Do
            End If
            
        Loop

        For X = 1 To NUM_CHANNEL_PAR
            .Row = X
            .Col = 0
            .TextMatrix(X, 0) = NUM_MODULE_PAR + X - 1
        Next X
        
        ' Move to the first cell in the 'DSPVal' column
        .Row = 1
        .Col = 2
        
        .Redraw = True
    End With
    
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEdit2.Font
        .Name = MSFlexGrid2.Font.Name
        .Size = MSFlexGrid2.Font.Size
        .Weight = MSFlexGrid2.Font.Weight
    End With
    txtEdit2.BackColor = vb3DLight
   
    txtEdit2 = ""
    bDoNotEdit2 = False
    
    With MSFlexGrid3
        .Redraw = False
        
        .Cols = 3
        .Rows = NUM_RESULTS_PAR + 1
        
        .FixedCols = 2
        
        .ColWidth(0) = 600
        .ColWidth(1) = 1400
        .ColWidth(2) = 1400
       
        .TextMatrix(0, 0) = "Index"
        .TextMatrix(0, 1) = "DSP PAR"
        .TextMatrix(0, 2) = "DSP VAL"

        For X = 0 To 2
            For Y = 0 To NUM_RESULTS_PAR
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignRightCenter
            Next Y
        Next X

        ' Continue to read the Pixie16_DSP_Var_File
        ' LineStr1 already loaded by the last read in MSFlexGrid2
        
        If LineStr1 = "" Then
            ErrTxt = "DSP var file " & Pixie16_DSP_Var_File & " doesn't contain valid result parameters"
            Msg = MsgBox(ErrTxt, 0, "Error")
            ' Close the Pixie16_DSP_Var_File
            Close #1
            Exit Sub
        End If
        
        ' Extract the starting address
        startingaddr = Val("&H" & Mid(LineStr1, 6, 5))
        currentaddr = startingaddr
        
        ' Read the DSP variable names from the Pixie16_DSP_Var_File
        Do While 1
                
            ' Read the next line
            If Not EOF(1) Then
                Line Input #1, LineStr2
            Else
                LineStr2 = ""   ' to indicate the end of file
            End If
            
            If LineStr2 <> "" Then
                nextaddr = Val("&H" & Mid(LineStr2, 6, 5))
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = nextaddr - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = "[" & Format(X, "00") & "]"
                    Next X
                Else
                    .TextMatrix(currentaddr - startingaddr + 1, 1) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
                ' Update current address and linestr
                currentaddr = nextaddr
                LineStr1 = LineStr2
                
            Else
                ' we already reach the end of file
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = startingaddr + NUM_RESULTS_PAR - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .TextMatrix(currentaddr - startingaddr + 1 + X, 1) = "[" & Format(X, "00") & "]"
                    Next X
                Else
                    .TextMatrix(currentaddr - startingaddr + 1, 1) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                 
                Exit Do
                
            End If
            
        Loop
        
        For X = 1 To NUM_RESULTS_PAR
            .Row = X
            .Col = 0
            .CellAlignment = flexAlignRightCenter
            .TextMatrix(X, 0) = NUM_MODULE_PAR + NUM_CHANNEL_PAR + X - 1
        Next X
        
        .Redraw = True
    End With
    
    ' Close the Pixie16_DSP_Var_File
    Close #1

End Sub

