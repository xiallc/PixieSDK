VERSION 5.00
Begin VB.Form dlgFPGAFirmwareFiles 
   Caption         =   "Select FPGA Firmware Files"
   ClientHeight    =   11490
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7965
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   11490
   ScaleWidth      =   7965
   Begin VB.Frame RevF16b100m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-16bit-100MSPS FPGA Firmware Files"
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
      TabIndex        =   36
      Top             =   2880
      Width           =   7695
      Begin VB.TextBox SPFPGAfile_RevF16b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   38
         Top             =   720
         Width           =   5895
      End
      Begin VB.TextBox CommFPGAfile_RevF16b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   37
         Top             =   360
         Width           =   5895
      End
      Begin VB.Image OpenSPFPGAfile_RevF16b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":0000
         Top             =   720
         Width           =   240
      End
      Begin VB.Image OpenComFPGAfile_RevF16b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":0532
         Top             =   360
         Width           =   240
      End
      Begin VB.Label CommFPGAlabel_RevF16b100m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   40
         Top             =   360
         Width           =   1335
      End
      Begin VB.Label SPFPGAlabel_RevF16b100m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   39
         Top             =   720
         Width           =   1095
      End
   End
   Begin VB.Frame RevF16b250m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-16bit-250MSPS FPGA Firmware Files"
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
      TabIndex        =   31
      Top             =   6840
      Width           =   7695
      Begin VB.TextBox SPFPGAfile_RevF16b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   33
         Top             =   720
         Width           =   5895
      End
      Begin VB.TextBox CommFPGAfile_RevF16b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   32
         Top             =   360
         Width           =   5895
      End
      Begin VB.Image OpenSPFPGAfile_RevF16b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":0A64
         Top             =   720
         Width           =   240
      End
      Begin VB.Image OpenComFPGAfile_RevF16b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":0F96
         Top             =   360
         Width           =   240
      End
      Begin VB.Label CommFPGAlabel_RevF16b250m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   35
         Top             =   360
         Width           =   1335
      End
      Begin VB.Label SPFPGAlabel_RevF16b250m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   34
         Top             =   720
         Width           =   1095
      End
   End
   Begin VB.Frame RevF14b500m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-14bit-500MSPS FPGA Firmware Files"
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
      TabIndex        =   26
      Top             =   9480
      Width           =   7695
      Begin VB.TextBox SPFPGAfile_RevF14b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   28
         Top             =   720
         Width           =   5895
      End
      Begin VB.TextBox CommFPGAfile_RevF14b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   27
         Top             =   360
         Width           =   5895
      End
      Begin VB.Image OpenSPFPGAfile_RevF14b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":14C8
         Top             =   720
         Width           =   240
      End
      Begin VB.Image OpenComFPGAfile_RevF14b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":19FA
         Top             =   360
         Width           =   240
      End
      Begin VB.Label CommFPGAlabel_RevF14b500m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   30
         Top             =   360
         Width           =   1335
      End
      Begin VB.Label SPFPGAlabel_RevF14b500m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   29
         Top             =   720
         Width           =   1095
      End
   End
   Begin VB.Frame RevF12b500m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-12bit-500MSPS FPGA Firmware Files"
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
      TabIndex        =   21
      Top             =   8160
      Width           =   7695
      Begin VB.TextBox CommFPGAfile_RevF12b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   23
         Top             =   360
         Width           =   5895
      End
      Begin VB.TextBox SPFPGAfile_RevF12b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   22
         Top             =   720
         Width           =   5895
      End
      Begin VB.Label SPFPGAlabel_RevF12b500m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   25
         Top             =   720
         Width           =   1095
      End
      Begin VB.Label CommFPGAlabel_RevF12b500m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   24
         Top             =   360
         Width           =   1335
      End
      Begin VB.Image OpenComFPGAfile_RevF12b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":1F2C
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenSPFPGAfile_RevF12b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":245E
         Top             =   720
         Width           =   240
      End
   End
   Begin VB.Frame RevF14b250m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-14bit-250MSPS FPGA Firmware Files"
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
      TabIndex        =   16
      Top             =   5520
      Width           =   7695
      Begin VB.TextBox CommFPGAfile_RevF14b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   18
         Top             =   360
         Width           =   5895
      End
      Begin VB.TextBox SPFPGAfile_RevF14b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   17
         Top             =   720
         Width           =   5895
      End
      Begin VB.Label SPFPGAlabel_RevF14b250m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   20
         Top             =   720
         Width           =   1095
      End
      Begin VB.Label CommFPGAlabel_RevF14b250m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   19
         Top             =   360
         Width           =   1335
      End
      Begin VB.Image OpenComFPGAfile_RevF14b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":2990
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenSPFPGAfile_RevF14b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":2EC2
         Top             =   720
         Width           =   240
      End
   End
   Begin VB.Frame RevF12b250m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-12bit-250MSPS FPGA Firmware Files"
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
      TabIndex        =   11
      Top             =   4200
      Width           =   7695
      Begin VB.TextBox SPFPGAfile_RevF12b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   13
         Top             =   720
         Width           =   5895
      End
      Begin VB.TextBox CommFPGAfile_RevF12b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   12
         Top             =   360
         Width           =   5895
      End
      Begin VB.Image OpenSPFPGAfile_RevF12b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":33F4
         Top             =   720
         Width           =   240
      End
      Begin VB.Image OpenComFPGAfile_RevF12b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":3926
         Top             =   360
         Width           =   240
      End
      Begin VB.Label CommFPGAlabel_RevF12b250m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   15
         Top             =   360
         Width           =   1335
      End
      Begin VB.Label SPFPGAlabel_RevF12b250m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   14
         Top             =   720
         Width           =   1095
      End
   End
   Begin VB.Frame RevF14b100m_FPGAFirmwareFiles 
      Caption         =   "Rev-F-14bit-100MSPS FPGA Firmware Files"
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
      TabIndex        =   5
      Top             =   1560
      Width           =   7695
      Begin VB.TextBox CommFPGAfile_RevF14b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   7
         Top             =   360
         Width           =   5895
      End
      Begin VB.TextBox SPFPGAfile_RevF14b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   6
         Top             =   720
         Width           =   5895
      End
      Begin VB.Label SPFPGAlabel_RevF14b100m 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   10
         Top             =   720
         Width           =   1095
      End
      Begin VB.Label CommFPGAlabel_RevF14b100m 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   8
         Top             =   360
         Width           =   1335
      End
      Begin VB.Image OpenComFPGAfile_RevF14b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":3E58
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenSPFPGAfile_RevF14b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":438A
         Top             =   720
         Width           =   240
      End
   End
   Begin VB.CommandButton ReturnfromdlgFPGAFirmwareFiles 
      Caption         =   "Close"
      Height          =   495
      Left            =   3360
      TabIndex        =   4
      Top             =   10850
      Width           =   1335
   End
   Begin VB.Frame RevBCD_FPGAFirmwareFiles 
      Caption         =   "Rev-B/C/D FPGA Firmware Files"
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
      Top             =   240
      Width           =   7695
      Begin VB.TextBox SPFPGAfile_RevBCD 
         Height          =   285
         Left            =   1320
         TabIndex        =   3
         Top             =   720
         Width           =   5895
      End
      Begin VB.TextBox CommFPGAfile_RevBCD 
         Height          =   285
         Left            =   1320
         TabIndex        =   2
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label SPFPGAlabel_RevBCD 
         Caption         =   "SP. FPGA"
         Height          =   375
         Left            =   240
         TabIndex        =   9
         Top             =   720
         Width           =   1095
      End
      Begin VB.Image OpenSPFPGAfile_RevBCD 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":48BC
         Top             =   720
         Width           =   240
      End
      Begin VB.Image OpenComFPGAfile_RevBCD 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgFPGAFirmwareFiles.frx":4DEE
         Top             =   360
         Width           =   240
      End
      Begin VB.Label CommFPGAlabel_RevBCD 
         Caption         =   "Comm. FPGA"
         Height          =   255
         Left            =   240
         TabIndex        =   1
         Top             =   360
         Width           =   1335
      End
   End
End
Attribute VB_Name = "dlgFPGAFirmwareFiles"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
  
Private Sub Form_Load()
    CommFPGAfile_RevBCD.Text = Pixie16_Com_FPGA_File_RevBCD
    SPFPGAfile_RevBCD.Text = Pixie16_SP_FPGA_File_RevBCD
    
    CommFPGAfile_RevF14b100m.Text = Pixie16_Com_FPGA_File_RevF14b100m
    SPFPGAfile_RevF14b100m.Text = Pixie16_SP_FPGA_File_RevF14b100m
    
    CommFPGAfile_RevF16b100m.Text = Pixie16_Com_FPGA_File_RevF16b100m
    SPFPGAfile_RevF16b100m.Text = Pixie16_SP_FPGA_File_RevF16b100m
    
    CommFPGAfile_RevF12b250m.Text = Pixie16_Com_FPGA_File_RevF12b250m
    SPFPGAfile_RevF12b250m.Text = Pixie16_SP_FPGA_File_RevF12b250m
    
    CommFPGAfile_RevF14b250m.Text = Pixie16_Com_FPGA_File_RevF14b250m
    SPFPGAfile_RevF14b250m.Text = Pixie16_SP_FPGA_File_RevF14b250m
    
    CommFPGAfile_RevF16b250m.Text = Pixie16_Com_FPGA_File_RevF16b250m
    SPFPGAfile_RevF16b250m.Text = Pixie16_SP_FPGA_File_RevF16b250m
    
    CommFPGAfile_RevF12b500m.Text = Pixie16_Com_FPGA_File_RevF12b500m
    SPFPGAfile_RevF12b500m.Text = Pixie16_SP_FPGA_File_RevF12b500m

    CommFPGAfile_RevF14b500m.Text = Pixie16_Com_FPGA_File_RevF14b500m
    SPFPGAfile_RevF14b500m.Text = Pixie16_SP_FPGA_File_RevF14b500m
End Sub

Private Sub OpenBrowser(ByVal folder As Integer)
    Dim sPath As String
    
    Select Case folder
        Case 0
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevBCD)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevBCD = sPath
            Pixie16_Com_FPGA_File_RevBCD = CommFPGAfile_RevBCD.Text
        Case 1
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevBCD)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevBCD = sPath
            Pixie16_SP_FPGA_File_RevBCD = SPFPGAfile_RevBCD.Text
        Case 2
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF14b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF14b100m = sPath
            Pixie16_Com_FPGA_File_RevF14b100m = CommFPGAfile_RevF14b100m.Text
        Case 3
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF14b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF14b100m = sPath
            Pixie16_SP_FPGA_File_RevF14b100m = SPFPGAfile_RevF14b100m.Text
        Case 4
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF16b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF16b100m = sPath
            Pixie16_Com_FPGA_File_RevF16b100m = CommFPGAfile_RevF16b100m.Text
        Case 5
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF16b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF16b100m = sPath
            Pixie16_SP_FPGA_File_RevF16b100m = SPFPGAfile_RevF16b100m.Text
        Case 6
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF12b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF12b250m = sPath
            Pixie16_Com_FPGA_File_RevF12b250m = CommFPGAfile_RevF12b250m.Text
        Case 7
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF12b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF12b250m = sPath
            Pixie16_SP_FPGA_File_RevF12b250m = SPFPGAfile_RevF12b250m.Text
        Case 8
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF14b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF14b250m = sPath
            Pixie16_Com_FPGA_File_RevF14b250m = CommFPGAfile_RevF14b250m.Text
        Case 9
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF14b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF14b250m = sPath
            Pixie16_SP_FPGA_File_RevF14b250m = SPFPGAfile_RevF14b250m.Text
        Case 10
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF16b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF16b250m = sPath
            Pixie16_Com_FPGA_File_RevF16b250m = CommFPGAfile_RevF16b250m.Text
        Case 11
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF16b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF16b250m = sPath
            Pixie16_SP_FPGA_File_RevF16b250m = SPFPGAfile_RevF16b250m.Text
        Case 12
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF12b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF12b500m = sPath
            Pixie16_Com_FPGA_File_RevF12b500m = CommFPGAfile_RevF12b500m.Text
        Case 13
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF12b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF12b500m = sPath
            Pixie16_SP_FPGA_File_RevF12b500m = SPFPGAfile_RevF12b500m.Text
        Case 14
            sPath = BrowseForFile(Me, "Select the communication FPGA file", Pixie16_Com_FPGA_File_RevF14b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            CommFPGAfile_RevF14b500m = sPath
            Pixie16_Com_FPGA_File_RevF14b500m = CommFPGAfile_RevF14b500m.Text
        Case 15
            sPath = BrowseForFile(Me, "Select the signal processing FPGA file", Pixie16_SP_FPGA_File_RevF14b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            SPFPGAfile_RevF14b500m = sPath
            Pixie16_SP_FPGA_File_RevF14b500m = SPFPGAfile_RevF14b500m.Text
    End Select
End Sub

Private Sub OpenComFPGAfile_RevBCD_Click()
    OpenBrowser (0)
End Sub

Private Sub OpenSPFPGAfile_RevBCD_Click()
    OpenBrowser (1)
End Sub

Private Sub OpenComFPGAfile_RevF14b100m_Click()
    OpenBrowser (2)
End Sub

Private Sub OpenSPFPGAfile_RevF14b100m_Click()
    OpenBrowser (3)
End Sub

Private Sub OpenComFPGAfile_RevF16b100m_Click()
    OpenBrowser (4)
End Sub

Private Sub OpenSPFPGAfile_RevF16b100m_Click()
    OpenBrowser (5)
End Sub

Private Sub OpenComFPGAfile_RevF12b250m_Click()
    OpenBrowser (6)
End Sub

Private Sub OpenSPFPGAfile_RevF12b250m_Click()
    OpenBrowser (7)
End Sub

Private Sub OpenComFPGAfile_RevF14b250m_Click()
    OpenBrowser (8)
End Sub

Private Sub OpenSPFPGAfile_RevF14b250m_Click()
    OpenBrowser (9)
End Sub

Private Sub OpenComFPGAfile_RevF16b250m_Click()
    OpenBrowser (10)
End Sub

Private Sub OpenSPFPGAfile_RevF16b250m_Click()
    OpenBrowser (11)
End Sub

Private Sub OpenComFPGAfile_RevF12b500m_Click()
    OpenBrowser (12)
End Sub

Private Sub OpenSPFPGAfile_RevF12b500m_Click()
    OpenBrowser (13)
End Sub

Private Sub OpenComFPGAfile_RevF14b500m_Click()
    OpenBrowser (14)
End Sub

Private Sub OpenSPFPGAfile_RevF14b500m_Click()
    OpenBrowser (15)
End Sub


Private Sub CommFPGAfile_RevBCD_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevBCD = CommFPGAfile_RevBCD.Text
    End If
End Sub

Private Sub CommFPGAfile_RevBCD_LostFocus()
    Pixie16_Com_FPGA_File_RevBCD = CommFPGAfile_RevBCD.Text
End Sub

Private Sub SPFPGAfile_RevBCD_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevBCD = SPFPGAfile_RevBCD.Text
    End If

End Sub

Private Sub SPFPGAfile_RevBCD_LostFocus()
    Pixie16_SP_FPGA_File_RevBCD = SPFPGAfile_RevBCD.Text
End Sub


Private Sub CommFPGAfile_RevF14b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF14b100m = CommFPGAfile_RevF14b100m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF14b100m_LostFocus()
    Pixie16_Com_FPGA_File_RevF14b100m = CommFPGAfile_RevF14b100m.Text
End Sub

Private Sub SPFPGAfile_RevF14b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF14b100m = SPFPGAfile_RevF14b100m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF14b100m_LostFocus()
    Pixie16_SP_FPGA_File_RevF14b100m = SPFPGAfile_RevF14b100m.Text
End Sub


Private Sub CommFPGAfile_RevF16b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF16b100m = CommFPGAfile_RevF16b100m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF16b100m_LostFocus()
    Pixie16_Com_FPGA_File_RevF16b100m = CommFPGAfile_RevF16b100m.Text
End Sub

Private Sub SPFPGAfile_RevF16b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF16b100m = SPFPGAfile_RevF16b100m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF16b100m_LostFocus()
    Pixie16_SP_FPGA_File_RevF16b100m = SPFPGAfile_RevF16b100m.Text
End Sub


Private Sub CommFPGAfile_RevF12b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF12b250m = CommFPGAfile_RevF12b250m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF12b250m_LostFocus()
    Pixie16_Com_FPGA_File_RevF12b250m = CommFPGAfile_RevF12b250m.Text
End Sub

Private Sub SPFPGAfile_RevF12b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF12b250m = SPFPGAfile_RevF12b250m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF12b250m_LostFocus()
    Pixie16_SP_FPGA_File_RevF12b250m = SPFPGAfile_RevF12b250m.Text
End Sub


Private Sub CommFPGAfile_RevF14b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF14b250m = CommFPGAfile_RevF14b250m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF14b250m_LostFocus()
    Pixie16_Com_FPGA_File_RevF14b250m = CommFPGAfile_RevF14b250m.Text
End Sub

Private Sub SPFPGAfile_RevF14b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF14b250m = SPFPGAfile_RevF14b250m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF14b250m_LostFocus()
    Pixie16_SP_FPGA_File_RevF14b250m = SPFPGAfile_RevF14b250m.Text
End Sub


Private Sub CommFPGAfile_RevF16b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF16b250m = CommFPGAfile_RevF16b250m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF16b250m_LostFocus()
    Pixie16_Com_FPGA_File_RevF16b250m = CommFPGAfile_RevF16b250m.Text
End Sub

Private Sub SPFPGAfile_RevF16b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF16b250m = SPFPGAfile_RevF16b250m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF16b250m_LostFocus()
    Pixie16_SP_FPGA_File_RevF16b250m = SPFPGAfile_RevF16b250m.Text
End Sub


Private Sub CommFPGAfile_RevF12b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF12b500m = CommFPGAfile_RevF12b500m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF12b500m_LostFocus()
    Pixie16_Com_FPGA_File_RevF12b500m = CommFPGAfile_RevF12b500m.Text
End Sub

Private Sub SPFPGAfile_RevF12b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF12b500m = SPFPGAfile_RevF12b500m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF12b500m_LostFocus()
    Pixie16_SP_FPGA_File_RevF12b500m = SPFPGAfile_RevF12b500m.Text
End Sub


Private Sub CommFPGAfile_RevF14b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_Com_FPGA_File_RevF14b500m = CommFPGAfile_RevF14b500m.Text
    End If
End Sub

Private Sub CommFPGAfile_RevF14b500m_LostFocus()
    Pixie16_Com_FPGA_File_RevF14b500m = CommFPGAfile_RevF14b500m.Text
End Sub

Private Sub SPFPGAfile_RevF14b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_SP_FPGA_File_RevF14b500m = SPFPGAfile_RevF14b500m.Text
    End If

End Sub

Private Sub SPFPGAfile_RevF14b500m_LostFocus()
    Pixie16_SP_FPGA_File_RevF14b500m = SPFPGAfile_RevF14b500m.Text
End Sub


Private Sub ReturnfromdlgFPGAFirmwareFiles_Click()
    Unload Me
End Sub
