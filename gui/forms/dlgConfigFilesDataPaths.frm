VERSION 5.00
Begin VB.Form dlgConfigFilesDataPaths 
   Caption         =   "Configuration Files & Output Data Paths"
   ClientHeight    =   6675
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7965
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   6675
   ScaleWidth      =   7965
   Begin VB.CommandButton Command_SetDSPCodeFiles 
      Caption         =   "Select DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   2280
      TabIndex        =   9
      Top             =   1680
      Width           =   3135
   End
   Begin VB.CommandButton Command_SetFPGAFirmwareFiles 
      Caption         =   "Select FPGA Firmware Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   2280
      TabIndex        =   8
      Top             =   480
      Width           =   3135
   End
   Begin VB.TextBox DSPParfile 
      Height          =   285
      Left            =   1560
      TabIndex        =   6
      Top             =   3120
      Width           =   5775
   End
   Begin VB.CommandButton ReturnfromdlgFilesPaths 
      Caption         =   "Close"
      Height          =   615
      Left            =   3360
      TabIndex        =   3
      Top             =   5640
      Width           =   1335
   End
   Begin VB.Frame OutputDataFiles 
      Caption         =   "Output Data Paths"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1215
      Left            =   120
      TabIndex        =   0
      Top             =   4080
      Width           =   7695
      Begin VB.TextBox HistogramDataFilePath 
         Height          =   285
         Left            =   1320
         TabIndex        =   5
         Top             =   720
         Width           =   5895
      End
      Begin VB.TextBox ListModeDataFilePath 
         Height          =   285
         Left            =   1320
         TabIndex        =   4
         Top             =   360
         Width           =   5895
      End
      Begin VB.Image OpenHistogramDataFilePath 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgConfigFilesDataPaths.frx":0000
         Top             =   720
         Width           =   240
      End
      Begin VB.Image OpenListModeDataFilePath 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgConfigFilesDataPaths.frx":0532
         Top             =   360
         Width           =   240
      End
      Begin VB.Label histogramdatafilepathlabel 
         Caption         =   "Histogram data"
         Height          =   375
         Left            =   240
         TabIndex        =   2
         Top             =   720
         Width           =   1095
      End
      Begin VB.Label listmodedatafilepathlabel 
         Caption         =   "List mode data"
         Height          =   255
         Left            =   240
         TabIndex        =   1
         Top             =   360
         Width           =   1335
      End
   End
   Begin VB.Image OpenDSPParFile 
      Height          =   225
      Left            =   7440
      Picture         =   "dlgConfigFilesDataPaths.frx":0A64
      Top             =   3120
      Width           =   240
   End
   Begin VB.Label dspparfilelabel 
      Alignment       =   2  'Center
      Caption         =   "DSP Parameters File"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   120
      TabIndex        =   7
      Top             =   2880
      Width           =   1335
   End
End
Attribute VB_Name = "dlgConfigFilesDataPaths"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
  
Private Sub Form_Load()
    DSPParfile.Text = Pixie16_DSP_Par_File
    ListModeDataFilePath.Text = Pixie16_ListModeDataFile_Path
    HistogramDataFilePath.Text = Pixie16_HistogramDataFile_Path
End Sub

Private Sub Command_SetDSPCodeFiles_Click()

    ' Check if the form is minimized or maximized
    If dlgDSPCodeFiles.WindowState <> vbNormal Then
        dlgDSPCodeFiles.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, dlgDSPCodeFiles
    dlgDSPCodeFiles.Height = 12000
    dlgDSPCodeFiles.Width = 8085
    
    dlgDSPCodeFiles.ZOrder 0

End Sub

Private Sub Command_SetFPGAFirmwareFiles_Click()

    ' Check if the form is minimized or maximized
    If dlgFPGAFirmwareFiles.WindowState <> vbNormal Then
        dlgFPGAFirmwareFiles.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, dlgFPGAFirmwareFiles
    dlgFPGAFirmwareFiles.Height = 12000
    dlgFPGAFirmwareFiles.Width = 8085
    
    dlgFPGAFirmwareFiles.ZOrder 0

End Sub

Private Sub OpenDSPParFile_Click()
    Dim sPath As String
    
    sPath = BrowseForFile(Me, "Select the DSP parameters file", Pixie16_DSP_Par_File)
    If Len(sPath) = 0 Then Exit Sub  'user selected cancel
    DSPParfile = sPath
    Pixie16_DSP_Par_File = DSPParfile.Text
End Sub

Private Sub DSPParfile_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Par_File = DSPParfile.Text
    End If
End Sub

Private Sub DSPParfile_LostFocus()
    Pixie16_DSP_Par_File = DSPParfile.Text
End Sub

Private Sub OpenListModeDataFilePath_Click()
    Dim sPath As String
    
    sPath = BrowseForFolder(Me, "Select the list mode data file path", Pixie16_ListModeDataFile_Path)
    If Len(sPath) = 0 Then Exit Sub  'user selected cancel
    ListModeDataFilePath = sPath + "\"
    Pixie16_ListModeDataFile_Path = ListModeDataFilePath.Text
End Sub

Private Sub ListModeDataFilePath_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_ListModeDataFile_Path = ListModeDataFilePath.Text
    End If
End Sub

Private Sub ListModeDataFilePath_LostFocus()
    Pixie16_ListModeDataFile_Path = ListModeDataFilePath.Text
End Sub

Private Sub OpenHistogramDataFilePath_Click()
    Dim sPath As String
    
    sPath = BrowseForFolder(Me, "Select the histogram data file path", Pixie16_HistogramDataFile_Path)
    If Len(sPath) = 0 Then Exit Sub  'user selected cancel
    HistogramDataFilePath = sPath + "\"
    Pixie16_HistogramDataFile_Path = HistogramDataFilePath.Text
End Sub

Private Sub HistogramDataFilePath_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_HistogramDataFile_Path = HistogramDataFilePath.Text
    End If
End Sub

Private Sub HistogramDataFilePath_LostFocus()
    Pixie16_HistogramDataFile_Path = HistogramDataFilePath.Text
End Sub

Private Sub ReturnfromdlgFilesPaths_Click()
    Unload Me
End Sub

