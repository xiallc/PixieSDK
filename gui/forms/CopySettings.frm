VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Begin VB.Form CopySettings 
   Caption         =   "Copy Settings Between Modules/Channels"
   ClientHeight    =   6915
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13470
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   6915
   ScaleWidth      =   13470
   Begin VB.CommandButton ClearAllCommand 
      Caption         =   "Clear all"
      Height          =   375
      Left            =   1920
      TabIndex        =   22
      Top             =   6200
      Width           =   1095
   End
   Begin VB.CommandButton SelectAllCommand 
      Caption         =   "Select all"
      Height          =   375
      Left            =   360
      TabIndex        =   21
      Top             =   6200
      Width           =   1095
   End
   Begin VB.Frame CS_DestinationFrame 
      Caption         =   "Destination Modules and Channels"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   4095
      Left            =   3480
      TabIndex        =   12
      Top             =   1080
      Width           =   9855
      Begin MSFlexGridLib.MSFlexGrid CS_DestinationGrid 
         Height          =   3615
         Left            =   120
         TabIndex        =   13
         Top             =   360
         Width           =   9585
         _ExtentX        =   16907
         _ExtentY        =   6376
         _Version        =   393216
      End
   End
   Begin VB.CommandButton CloseCommand 
      Caption         =   "Close"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   7440
      TabIndex        =   11
      Top             =   5760
      Width           =   1575
   End
   Begin VB.CommandButton CopyCommand 
      Caption         =   "Copy"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   4680
      TabIndex        =   10
      Top             =   5760
      Width           =   1455
   End
   Begin VB.Frame CS_CopyItemsFrame 
      Caption         =   "Items to Copy"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   4935
      Left            =   240
      TabIndex        =   2
      Top             =   960
      Width           =   3135
      Begin VB.CheckBox QDCCheck 
         Caption         =   "QDC"
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
         TabIndex        =   20
         Top             =   4440
         Width           =   2655
      End
      Begin VB.CheckBox MultiplicityCheck 
         Caption         =   "Multiplicity"
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
         TabIndex        =   19
         Top             =   4080
         Width           =   2535
      End
      Begin VB.CheckBox FIFODelaysCheck 
         Caption         =   "FIFO delays"
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
         TabIndex        =   18
         Top             =   3720
         Width           =   2775
      End
      Begin VB.CheckBox TrigStretchCheck 
         Caption         =   "Trigger stretch lengths"
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
         TabIndex        =   17
         Top             =   3360
         Width           =   2655
      End
      Begin VB.CheckBox CFDTriggerCheck 
         Caption         =   "CFD trigger"
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
         TabIndex        =   16
         Top             =   3000
         Width           =   2055
      End
      Begin VB.CheckBox PSACheck 
         Caption         =   "Pulse shape analysis"
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
         TabIndex        =   9
         Top             =   1920
         Width           =   2295
      End
      Begin VB.CheckBox ASCCheck 
         Caption         =   "Analog signal conditioning"
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
         TabIndex        =   4
         Top             =   840
         Width           =   2655
      End
      Begin VB.CheckBox DecayTimeCheck 
         Caption         =   "Decay time"
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
         TabIndex        =   8
         Top             =   1560
         Width           =   1455
      End
      Begin VB.CheckBox ChanCSRACheck 
         Caption         =   "Channel CSRA register"
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
         TabIndex        =   7
         Top             =   2640
         Width           =   2655
      End
      Begin VB.CheckBox HistogramCheck 
         Caption         =   "Histogram control"
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
         TabIndex        =   6
         Top             =   1200
         Width           =   1935
      End
      Begin VB.CheckBox BaselineCheck 
         Caption         =   "Baseline control"
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
         TabIndex        =   5
         Top             =   2280
         Width           =   1815
      End
      Begin VB.CheckBox FilterCheck 
         Caption         =   "Filter"
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
         TabIndex        =   3
         Top             =   480
         Width           =   855
      End
   End
   Begin CWUIControlsLib.CWNumEdit CS_SourceChannelEdit 
      Height          =   495
      Left            =   4320
      TabIndex        =   14
      Top             =   240
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
      scale_1         =   3
      ClassName_3     =   "CCWScale"
      opts_3          =   65536
      dMax_3          =   10
      discInterval_3  =   1
      ValueVarType_1  =   5
      IncValueVarType_1=   5
      IncValue_Val_1  =   1
      AccelIncVarType_1=   5
      AccelInc_Val_1  =   5
      RangeMinVarType_1=   5
      RangeMaxVarType_1=   5
      RangeMax_Val_1  =   15
      Bindings_1      =   4
      ClassName_4     =   "CCWBindingHolderArray"
      Editor_4        =   5
      ClassName_5     =   "CCWBindingHolderArrayEditor"
      Owner_5         =   1
   End
   Begin CWUIControlsLib.CWNumEdit CS_SourceModuleEdit 
      Height          =   495
      Left            =   1680
      TabIndex        =   15
      Top             =   240
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
      AccelInc_Val_1  =   5
      RangeMinVarType_1=   5
      RangeMaxVarType_1=   5
      RangeMax_Val_1  =   6
      Bindings_1      =   4
      ClassName_4     =   "CCWBindingHolderArray"
      Editor_4        =   5
      ClassName_5     =   "CCWBindingHolderArrayEditor"
      Owner_5         =   1
   End
   Begin VB.Label CS_SourceChannelLabel 
      Caption         =   "Source Channel"
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
      Left            =   2760
      TabIndex        =   1
      Top             =   360
      Width           =   1455
   End
   Begin VB.Label CS_SourceModuleLabel 
      Caption         =   "Source Module"
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
      TabIndex        =   0
      Top             =   360
      Width           =   1455
   End
End
Attribute VB_Name = "CopySettings"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub CloseCommand_Click()
    Unload Me
End Sub

Private Sub Form_Load()

    Dim X As Integer
    Dim Y As Integer
    
    '###########################################
    '   Initialize source module limit
    '###########################################
    
    CS_SourceModuleEdit.Maximum = Number_Modules_Total - 1
    
    '###########################################
    '   Initialize copy items
    '###########################################
    
    ' All checkboxes are checked
    
    FilterCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(0, CS_ItemsMask)
    ASCCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(1, CS_ItemsMask)
    HistogramCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(2, CS_ItemsMask)
    DecayTimeCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(3, CS_ItemsMask)
    PSACheck.Value = 1
    CS_ItemsMask = APP32_SetBit(4, CS_ItemsMask)
    BaselineCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(5, CS_ItemsMask)
    ChanCSRACheck.Value = 1
    CS_ItemsMask = APP32_SetBit(7, CS_ItemsMask)
    CFDTriggerCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(8, CS_ItemsMask)
    TrigStretchCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(9, CS_ItemsMask)
    FIFODelaysCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(10, CS_ItemsMask)
    MultiplicityCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(11, CS_ItemsMask)
    QDCCheck.Value = 1
    CS_ItemsMask = APP32_SetBit(12, CS_ItemsMask)
 
    '###########################################
    '   Initialize destination grid
    '###########################################
    
    With CS_DestinationGrid
        .Redraw = False
        
        .Cols = NUM_CHANNELS + 1
        .Rows = Number_Modules_Total + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
         .ColWidth(0) = 600
         For X = 1 To .Cols - 1
            .ColWidth(X) = 500
            .TextMatrix(0, X) = "Ch" + Str(X - 1)
         Next X
        
        .RowHeight(0) = 550
         For X = 1 To .Rows - 1
            .RowHeight(X) = 400
            .TextMatrix(X, 0) = X - 1
            .Row = X
            .Col = 0
            .CellBackColor = vbYellow
         Next X
     
        .TextMatrix(0, 0) = "Module"
        
        For X = 0 To .Cols - 1
            For Y = 0 To .Rows - 1
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' don't load the icons yet.
        For X = 1 To .Cols - 1
            For Y = 1 To .Rows - 1
                .Row = Y
                .Col = X
                Set .CellPicture = LoadPicture
                CS_DestinationMask((Y - 1) * NUM_CHANNELS + (X - 1)) = 0
            Next Y
        Next X
    
        .Redraw = True
    End With

End Sub


Private Sub CopyCommand_Click()
    Dim SourceModule, SourceChannel, ModuleNumber As Integer
    Dim retval As Integer
    Dim StrTxt As String
    Dim Msg As Integer
    Dim count As Integer
    
    CopyCommand.Enabled = False
    
    SourceModule = CS_SourceModuleEdit.Value
    SourceChannel = CS_SourceChannelEdit.Value
        
    retval = Pixie16CopyDSPParameters(CS_ItemsMask, SourceModule, SourceChannel, CS_DestinationMask(0))
    If retval < 0 Then
        StrTxt = "Pixie16CopyDSPParameters returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        CopyCommand.Enabled = True
        Exit Sub
    End If
    
    ' Retrieve module parameters
    For count = 0 To Number_Modules_Total - 1
        retval = RetrieveModParFromModule(count)
        If retval < 0 Then
            StrTxt = "RetrieveModParFromModule " & "(Module # " & Str$(count) & ") returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            CopyCommand.Enabled = True
            Exit Sub
        End If
    Next count
    
    ' Update IN_SYNCH and SYNCH_WAIT - here we use module #0
    ' since all other modules should be the same
    retval = UpdateModParCtrlsOnMDIForm(0)
    
    If IsLoaded("DAQParametersSetup") Then
    
        ' Update all relevant parameters
        ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
        
        retval = RetrieveChanParFromModule(ModuleNumber)
        If retval < 0 Then
            StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            CopyCommand.Enabled = True
            Exit Sub
        End If
    
        ' Update module parameters displays for the currently selected module
        retval = UpdateModParCtrlsOnDAQForm(ModuleNumber)
        
        ' Update channel parameters displays
        retval = UpdateChanParCtrlsOnDAQForm()
    End If
    
    CopyCommand.Enabled = True
End Sub


'###############################################################
'
'   Subroutine that is used to set destination modules and channels
'
'###############################################################

Private Sub CS_DestinationGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k As Long
    
    With CS_DestinationGrid
    
        If X < .ColWidth(0) And Y > .RowHeight(0) Then
        
            For k = 1 To NUM_CHANNELS
                .Col = k
                Set .CellPicture = LoadPicture(StartUpDir + "\checkmark.bmp")
                .CellPictureAlignment = flexAlignCenterCenter
                CS_DestinationMask((.Row - 1) * NUM_CHANNELS + (.Col - 1)) = 1
            Next k
            
        ElseIf .Row > 0 And .Col > 0 Then
            
            If CS_DestinationMask((.Row - 1) * NUM_CHANNELS + (.Col - 1)) = 0 Then
                Set .CellPicture = LoadPicture(StartUpDir + "\checkmark.bmp")
                .CellPictureAlignment = flexAlignCenterCenter
                CS_DestinationMask((.Row - 1) * NUM_CHANNELS + (.Col - 1)) = 1
            Else
                Set .CellPicture = LoadPicture
                CS_DestinationMask((.Row - 1) * NUM_CHANNELS + (.Col - 1)) = 0
            End If
            
        End If
    
    End With
End Sub

'###############################################################
'
'   Subroutines that are used to set copy items mask
'
'###############################################################

Private Sub FilterCheck_Click()
    If FilterCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(0, CS_ItemsMask)
    ElseIf FilterCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(0, CS_ItemsMask)
    End If
End Sub

Private Sub ASCCheck_Click()
    If ASCCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(1, CS_ItemsMask)
    ElseIf ASCCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(1, CS_ItemsMask)
    End If
End Sub

Private Sub HistogramCheck_Click()
    If HistogramCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(2, CS_ItemsMask)
    ElseIf HistogramCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(2, CS_ItemsMask)
    End If
End Sub

Private Sub DecayTimeCheck_Click()
    If DecayTimeCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(3, CS_ItemsMask)
    ElseIf DecayTimeCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(3, CS_ItemsMask)
    End If
End Sub

Private Sub PSACheck_Click()
    If PSACheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(4, CS_ItemsMask)
    ElseIf PSACheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(4, CS_ItemsMask)
    End If
End Sub

Private Sub BaselineCheck_Click()
    If BaselineCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(5, CS_ItemsMask)
    ElseIf BaselineCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(5, CS_ItemsMask)
    End If
End Sub

Private Sub ChanCSRACheck_Click()
    If ChanCSRACheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(7, CS_ItemsMask)
    ElseIf ChanCSRACheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(7, CS_ItemsMask)
    End If
End Sub

Private Sub CFDTriggerCheck_Click()
    If CFDTriggerCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(8, CS_ItemsMask)
    ElseIf CFDTriggerCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(8, CS_ItemsMask)
    End If
End Sub

Private Sub TrigStretchCheck_Click()
    If TrigStretchCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(9, CS_ItemsMask)
    ElseIf TrigStretchCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(9, CS_ItemsMask)
    End If
End Sub

Private Sub FIFODelaysCheck_Click()
    If FIFODelaysCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(10, CS_ItemsMask)
    ElseIf FIFODelaysCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(10, CS_ItemsMask)
    End If
End Sub

Private Sub MultiplicityCheck_Click()
    If MultiplicityCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(11, CS_ItemsMask)
    ElseIf MultiplicityCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(11, CS_ItemsMask)
    End If
End Sub

Private Sub QDCCheck_Click()
    If QDCCheck.Value = 0 Then         ' not checked
        CS_ItemsMask = APP32_ClrBit(12, CS_ItemsMask)
    ElseIf QDCCheck.Value = 1 Then     ' checked
        CS_ItemsMask = APP32_SetBit(12, CS_ItemsMask)
    End If
End Sub

Private Sub SelectAllCommand_Click()
    FilterCheck.Value = 1
    ASCCheck.Value = 1
    HistogramCheck.Value = 1
    DecayTimeCheck.Value = 1
    PSACheck.Value = 1
    BaselineCheck.Value = 1
    ChanCSRACheck.Value = 1
    CFDTriggerCheck.Value = 1
    TrigStretchCheck.Value = 1
    FIFODelaysCheck.Value = 1
    MultiplicityCheck.Value = 1
    QDCCheck.Value = 1
    
    FilterCheck.Refresh
    ASCCheck.Refresh
    HistogramCheck.Refresh
    DecayTimeCheck.Refresh
    PSACheck.Refresh
    BaselineCheck.Refresh
    ChanCSRACheck.Refresh
    CFDTriggerCheck.Refresh
    TrigStretchCheck.Refresh
    FIFODelaysCheck.Refresh
    MultiplicityCheck.Refresh
    QDCCheck.Refresh
End Sub

Private Sub ClearAllCommand_Click()
    FilterCheck.Value = 0
    ASCCheck.Value = 0
    HistogramCheck.Value = 0
    DecayTimeCheck.Value = 0
    PSACheck.Value = 0
    BaselineCheck.Value = 0
    ChanCSRACheck.Value = 0
    CFDTriggerCheck.Value = 0
    TrigStretchCheck.Value = 0
    FIFODelaysCheck.Value = 0
    MultiplicityCheck.Value = 0
    QDCCheck.Value = 0
    
    FilterCheck.Refresh
    ASCCheck.Refresh
    HistogramCheck.Refresh
    DecayTimeCheck.Refresh
    PSACheck.Refresh
    BaselineCheck.Refresh
    ChanCSRACheck.Refresh
    CFDTriggerCheck.Refresh
    TrigStretchCheck.Refresh
    FIFODelaysCheck.Refresh
    MultiplicityCheck.Refresh
    QDCCheck.Refresh
End Sub

