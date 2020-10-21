VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form SaveSettingsForm 
   Caption         =   "Save Settings"
   ClientHeight    =   3195
   ClientLeft      =   5835
   ClientTop       =   3975
   ClientWidth     =   6840
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3195
   ScaleWidth      =   6840
   Begin VB.CommandButton CancelButton 
      Caption         =   "Cancel"
      Height          =   495
      Left            =   4080
      TabIndex        =   7
      Top             =   2280
      Width           =   1095
   End
   Begin MSComDlg.CommonDialog dlgDSPSettingsFile 
      Left            =   600
      Top             =   2160
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CheckBox DSPSettingsCheckBox 
      Caption         =   "Check2"
      Height          =   255
      Left            =   210
      TabIndex        =   6
      Top             =   1485
      Width           =   255
   End
   Begin VB.CheckBox VBGUIEnvCheckBox 
      Caption         =   "Check1"
      Enabled         =   0   'False
      Height          =   255
      Left            =   210
      TabIndex        =   5
      Top             =   525
      Value           =   2  'Grayed
      Width           =   255
   End
   Begin VB.TextBox DSPSettingsFileText 
      Height          =   285
      Left            =   480
      TabIndex        =   3
      Top             =   1440
      Width           =   5775
   End
   Begin VB.CommandButton SaveButton 
      Caption         =   "Save"
      Height          =   495
      Left            =   1560
      TabIndex        =   2
      Top             =   2280
      Width           =   1095
   End
   Begin VB.TextBox VBGUIEnvSettingsFileText 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   480
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   480
      Width           =   5775
   End
   Begin VB.Image OpenDSPSettingsFile 
      Height          =   225
      Left            =   6360
      Picture         =   "SaveSettingsForm.frx":0000
      Top             =   1440
      Width           =   240
   End
   Begin VB.Label DSPSettingsLabel 
      Caption         =   "DSP Settings"
      Height          =   255
      Left            =   480
      TabIndex        =   4
      Top             =   1200
      Width           =   1215
   End
   Begin VB.Label VBGUISettingsLabel 
      Caption         =   "VB_GUI Environment Settings"
      Height          =   255
      Left            =   480
      TabIndex        =   1
      Top             =   240
      Width           =   2175
   End
End
Attribute VB_Name = "SaveSettingsForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub CancelButton_Click()
    Unload Me
End Sub

Private Sub DSPSettingsCheckBox_Click()
    If SystemBooted = False Then
        DSPSettingsCheckBox.Value = 0
        DSPSettingsFileText.BackColor = &H8000000F
        DSPSettingsFileText.Text = vbNullString
        Exit Sub
    End If
    If DSPSettingsCheckBox.Value = 1 Then
        DSPSettingsFileText.BackColor = &H80000005
        DSPSettingsFileText.Text = Pixie16_DSP_Par_File
    Else
        DSPSettingsFileText.BackColor = &H8000000F
        DSPSettingsFileText.Text = vbNullString
    End If
End Sub

Private Sub DSPSettingsFileText_KeyPress(KeyAscii As Integer)
    If KeyAscii = 13 Then
        Pixie16_DSP_Par_File = DSPSettingsFileText
    End If
End Sub

Private Sub DSPSettingsFileText_LostFocus()
    Pixie16_DSP_Par_File = DSPSettingsFileText
End Sub

Private Sub Form_Load()
    SaveSettingsForm.Height = 3600
    SaveSettingsForm.Width = 6960
    VBGUIEnvSettingsFileText.Text = StartUpDir + "\" + PIXIE16_VB_SETTINGS_FILE
    VBGUIEnvSettingsFileText.BackColor = &H8000000F
    DSPSettingsCheckBox.Value = 0
End Sub

Private Sub OpenDSPSettingsFile_Click()
    Dim filename As String
    
    If SystemBooted = False Then
        Exit Sub
    End If
    If DSPSettingsCheckBox.Value = 0 Then
        Exit Sub
    End If
    
    dlgDSPSettingsFile.filename = vbNullString
    
    dlgDSPSettingsFile.Filter = "Binary Files (*.set)|*.set|All|*.*"
    dlgDSPSettingsFile.ShowSave
      
    If dlgDSPSettingsFile.filename = vbNullString Then
       Exit Sub
    End If
    
    Pixie16_DSP_Par_File = dlgDSPSettingsFile.filename
    DSPSettingsFileText.Text = Pixie16_DSP_Par_File
End Sub

Private Sub SaveButton_Click()
    Dim retval As Long
    Dim Msg As Integer
    Dim StrTxt As String

    Call Pixie16MDIForm.SaveVBSettingsToFile
    
    If SystemBooted = True Then
        ' Save DSP parameters to file
        retval = Pixie16SaveDSPParametersToFile(Pixie16_DSP_Par_File)
        If retval < 0 Then
            StrTxt = "Pixie16SaveDSPParametersToFile failed. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
    End If
    
    Unload Me
End Sub

