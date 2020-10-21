VERSION 5.00
Begin VB.Form AdvancedModCSRBBitsForm 
   Caption         =   "Advanced ModCSRB Bits Setup"
   ClientHeight    =   5235
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9930
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   5235
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
      Top             =   4320
      Width           =   1935
   End
   Begin VB.Frame EditModCSRBAdvancedFrame 
      Caption         =   "Module Control Register B (Advanced)"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   3525
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   9615
      Begin VB.CheckBox ModuleETSELCheck 
         Caption         =   $"AdvancedModCSRBBitsForm.frx":0000
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
         TabIndex        =   3
         Top             =   1200
         Width           =   8955
      End
      Begin VB.CheckBox ModuleSendFTBackplaneCheck 
         Caption         =   "[13] Enable sending fast triggers to backplane (only one module can enable this option in each PCI bus segment of a chassis)"
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
         TabIndex        =   5
         Top             =   2600
         Width           =   8895
      End
      Begin VB.CheckBox ModuleGFTSELCheck 
         Caption         =   $"AdvancedModCSRBBitsForm.frx":00B2
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
         TabIndex        =   2
         Top             =   480
         Width           =   8955
      End
      Begin VB.CheckBox ModuleSortEventsCheck 
         Caption         =   "[12] Sort events based on timestamps (all channels should have same settings to use this feature)"
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
         TabIndex        =   4
         Top             =   1950
         Width           =   9000
      End
   End
End
Attribute VB_Name = "AdvancedModCSRBBitsForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub CloseAdvancedModCSRBSetupCommand_Click()
    Unload Me
End Sub

Private Sub Form_Load()
    Dim localModCSRB As Long
        
    localModCSRB = ModCSRB(DAQParametersSetup.DAQParSetupModNumEdit.Value)

    ' Global fast trigger selection
    If APP32_TstBit(MODCSRB_GFTSEL, localModCSRB) = 1 Then
        ModuleGFTSELCheck.Value = 1
    Else
        ModuleGFTSELCheck.Value = 0
    End If
    ModuleGFTSELCheck.Refresh
    
    ' External validation trigger selection
    If APP32_TstBit(MODCSRB_ETSEL, localModCSRB) = 1 Then
        ModuleETSELCheck.Value = 1
    Else
        ModuleETSELCheck.Value = 0
    End If
    ModuleETSELCheck.Refresh
    
    ' Sort events
    If APP32_TstBit(MODCSRB_SORTEVENTS, localModCSRB) = 1 Then
        ModuleSortEventsCheck.Value = 1
    Else
        ModuleSortEventsCheck.Value = 0
    End If
    ModuleSortEventsCheck.Refresh
    
    ' Send fast triggers to backplane
    If APP32_TstBit(MODCSRB_BKPLFASTTRIG, localModCSRB) = 1 Then
        ModuleSendFTBackplaneCheck.Value = 1
    Else
        ModuleSendFTBackplaneCheck.Value = 0
    End If
    ModuleSendFTBackplaneCheck.Refresh

End Sub

Private Sub ModuleETSELCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If ModuleETSELCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_ETSEL, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_ETSEL, localModCSRB)
    End If
    
    ModCSRB(ModuleNumber) = localModCSRB
    
    ' Write ModCSRB value to module
    retval = Pixie16WriteSglModPar("MODULE_CSRB", ModCSRB(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglModPar failed to set MODULE_CSRB in module # " & Str$(ModuleNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ModuleGFTSELCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If ModuleGFTSELCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_GFTSEL, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_GFTSEL, localModCSRB)
    End If
    
    ModCSRB(ModuleNumber) = localModCSRB
    
    ' Write ModCSRB value to module
    retval = Pixie16WriteSglModPar("MODULE_CSRB", ModCSRB(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglModPar failed to set MODULE_CSRB in module # " & Str$(ModuleNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ModuleSendFTBackplaneCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If ModuleSendFTBackplaneCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_BKPLFASTTRIG, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_BKPLFASTTRIG, localModCSRB)
    End If
    
    ModCSRB(ModuleNumber) = localModCSRB
    
    ' Write ModCSRB value to module
    retval = Pixie16WriteSglModPar("MODULE_CSRB", ModCSRB(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglModPar failed to set MODULE_CSRB in module # " & Str$(ModuleNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub

Private Sub ModuleSortEventsCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If ModuleSortEventsCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_SORTEVENTS, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_SORTEVENTS, localModCSRB)
    End If
    
    ModCSRB(ModuleNumber) = localModCSRB
    
    ' Write ModCSRB value to module
    retval = Pixie16WriteSglModPar("MODULE_CSRB", ModCSRB(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglModPar failed to set MODULE_CSRB in module # " & Str$(ModuleNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub
