VERSION 5.00
Begin VB.Form AdvancedChanCSRABitsForm 
   Caption         =   "Advanced ChanCSRA Bits Setup"
   ClientHeight    =   6975
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9930
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   6975
   ScaleWidth      =   9930
   Begin VB.CommandButton CloseAdvancedModCSRBSetupCommand 
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
      Height          =   615
      Left            =   3960
      TabIndex        =   1
      Top             =   6120
      Width           =   1935
   End
   Begin VB.Frame EditChanCSRAFrame 
      Caption         =   "Channel Control Register A (Advanced)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   5535
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   9615
      Begin VB.CheckBox EnaRecordExtTSCheck 
         Caption         =   "[21] Enable (checked) or disable (unchecked) recording of external clock timestamps in event header"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   360
         TabIndex        =   11
         Top             =   4800
         Width           =   8775
      End
      Begin VB.CheckBox ModVetoSelCheck 
         Caption         =   "[20] Module veto selection (module validation trigger (checked) or front panel module veto (unchecked))"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   360
         TabIndex        =   10
         Top             =   4200
         Width           =   8775
      End
      Begin VB.CheckBox ChanVetoSelCheck 
         Caption         =   "[19] Channel veto selection (channel validation trigger (checked) or front panel channel veto (unchecked))"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   360
         TabIndex        =   9
         Top             =   3600
         Width           =   8775
      End
      Begin VB.CheckBox ChanGroupTrigSelCheck 
         Caption         =   "[18] Group trigger selection (external group trigger (checked) or local fast trigger (unchecked))"
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
         Left            =   360
         TabIndex        =   8
         Top             =   3200
         Width           =   9015
      End
      Begin VB.CheckBox ModuleETRIGSELCheck 
         Caption         =   "[1] Module validation trigger selection (module veto input (checked) or module validation trigger (unchecked))"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Left            =   360
         TabIndex        =   3
         Top             =   720
         Width           =   9015
      End
      Begin VB.CheckBox ChanFTRIGSELCheck 
         Caption         =   "[0] Fast trigger selection (external fast trigger (checked) or selected group trigger (unchecked))"
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
         Left            =   360
         TabIndex        =   2
         Top             =   360
         Width           =   8895
      End
      Begin VB.CheckBox ChanETRIGSELCheck 
         Caption         =   "[3] Channel validation trigger selection (channel veto input (checked) or channel validation trigger (unchecked))"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   360
         TabIndex        =   4
         Top             =   1400
         Width           =   9015
      End
      Begin VB.CheckBox ChanEnableVetoCheck 
         Caption         =   "[6] Enable channel trigger veto"
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
         Left            =   360
         TabIndex        =   6
         Top             =   2400
         Width           =   4455
      End
      Begin VB.CheckBox ChanEnableEnergyCutCheck 
         Caption         =   "[17] Enable ""no traces for large pulses"" feature"
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
         Left            =   360
         TabIndex        =   7
         Top             =   2800
         Width           =   5295
      End
      Begin VB.CheckBox ChanSyncDataAcq 
         Caption         =   "[4] Block data acquisition if trace or header buffers are full"
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
         Left            =   360
         TabIndex        =   5
         Top             =   2000
         Width           =   9015
      End
   End
End
Attribute VB_Name = "AdvancedChanCSRABitsForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub ChanEnableEnergyCutCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanEnableEnergyCutCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_ENAENERGYCUT, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_ENAENERGYCUT, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ChanEnableVetoCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanEnableVetoCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_VETOENA, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_VETOENA, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ChanETRIGSELCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanETRIGSELCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_CHANTRIGSEL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_CHANTRIGSEL, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ChanFTRIGSELCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanFTRIGSELCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_FTRIGSEL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_FTRIGSEL, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ChanGroupTrigSelCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanGroupTrigSelCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_GROUPTRIGSEL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_GROUPTRIGSEL, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ChanSyncDataAcq_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanSyncDataAcq.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_SYNCDATAACQ, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_SYNCDATAACQ, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ChanVetoSelCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanVetoSelCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_CHANVETOSEL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_CHANVETOSEL, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ModVetoSelCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ModVetoSelCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_MODVETOSEL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_MODVETOSEL, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub EnaRecordExtTSCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If EnaRecordExtTSCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_EXTTSENA, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_EXTTSENA, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub CloseAdvancedModCSRBSetupCommand_Click()
    Unload Me
End Sub

Private Sub Form_Load()
    Dim localChanCSRAlng As Long
    Dim ChannelNumber As Integer

    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
       
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))

    ' Fast trigger selection
    If APP32_TstBit(CCSRA_FTRIGSEL, localChanCSRAlng) = 1 Then
        ChanFTRIGSELCheck.Value = 1
    Else
        ChanFTRIGSELCheck.Value = 0
    End If
    ChanFTRIGSELCheck.Refresh

    ' Module validation trigger selection
    If APP32_TstBit(CCSRA_EXTTRIGSEL, localChanCSRAlng) = 1 Then
        ModuleETRIGSELCheck.Value = 1
    Else
        ModuleETRIGSELCheck.Value = 0
    End If
    ModuleETRIGSELCheck.Refresh

    ' Channel validation trigger selection
    If APP32_TstBit(CCSRA_CHANTRIGSEL, localChanCSRAlng) = 1 Then
        ChanETRIGSELCheck.Value = 1
    Else
        ChanETRIGSELCheck.Value = 0
    End If
    ChanETRIGSELCheck.Refresh

    ' Data acq synchronization
    If APP32_TstBit(CCSRA_SYNCDATAACQ, localChanCSRAlng) = 1 Then
        ChanSyncDataAcq.Value = 1
    Else
        ChanSyncDataAcq.Value = 0
    End If
    ChanSyncDataAcq.Refresh

    ' Enable channel veto
    If APP32_TstBit(CCSRA_VETOENA, localChanCSRAlng) = 1 Then
        ChanEnableVetoCheck.Value = 1
    Else
        ChanEnableVetoCheck.Value = 0
    End If
    ChanEnableVetoCheck.Refresh

    ' Energy cut feature
    If APP32_TstBit(CCSRA_ENAENERGYCUT, localChanCSRAlng) = 1 Then
        ChanEnableEnergyCutCheck.Value = 1
    Else
        ChanEnableEnergyCutCheck.Value = 0
    End If
    ChanEnableEnergyCutCheck.Refresh

    ' Group trigger selection
    If APP32_TstBit(CCSRA_GROUPTRIGSEL, localChanCSRAlng) = 1 Then
        ChanGroupTrigSelCheck.Value = 1
    Else
        ChanGroupTrigSelCheck.Value = 0
    End If
    ChanGroupTrigSelCheck.Refresh

    ' Channel veto selection
    If APP32_TstBit(CCSRA_CHANVETOSEL, localChanCSRAlng) = 1 Then
        ChanVetoSelCheck.Value = 1
    Else
        ChanVetoSelCheck.Value = 0
    End If
    ChanVetoSelCheck.Refresh

    ' Module veto selection
    If APP32_TstBit(CCSRA_MODVETOSEL, localChanCSRAlng) = 1 Then
        ModVetoSelCheck.Value = 1
    Else
        ModVetoSelCheck.Value = 0
    End If
    ModVetoSelCheck.Refresh

    ' Recording external clock timestamps in event header
    If APP32_TstBit(CCSRA_EXTTSENA, localChanCSRAlng) = 1 Then
        EnaRecordExtTSCheck.Value = 1
    Else
        EnaRecordExtTSCheck.Value = 0
    End If
    EnaRecordExtTSCheck.Refresh
End Sub

Private Sub ModuleETRIGSELCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    ChannelNumber = DAQParametersSetup.CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ModuleETRIGSELCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_EXTTRIGSEL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_EXTTRIGSEL, localChanCSRAlng)
    End If
    
    ChanCSRA(ChannelNumber) = CDbl(localChanCSRAlng)
    
    ' Write ChanCSRA value to module
    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(ModuleNumber) & " channel # " & Str$(ChannelNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

