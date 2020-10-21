VERSION 5.00
Begin VB.Form dlgDSPCodeFiles 
   Caption         =   "Select DSP Code Files"
   ClientHeight    =   11490
   ClientLeft      =   165
   ClientTop       =   -1545
   ClientWidth     =   7965
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   11490
   ScaleWidth      =   7965
   Begin VB.CommandButton ReturnfromdlgDSPCodeFiles 
      Caption         =   "Close"
      Height          =   495
      Left            =   3120
      TabIndex        =   5
      Top             =   10900
      Width           =   1695
   End
   Begin VB.Frame RevF16b100m_DSPCodeFiles 
      Caption         =   "Rev-F-16bit-100MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   50
      Top             =   2880
      Width           =   7695
      Begin VB.TextBox DSPlistfile_RevF16b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   53
         Top             =   900
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF16b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   52
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPcodefile_RevF16b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   51
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label DSPlistfilelabel_RevF16b100m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   56
         Top             =   880
         Width           =   855
      End
      Begin VB.Label DSPvarfilelabel_RevF16b100m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   55
         Top             =   620
         Width           =   855
      End
      Begin VB.Image OpenDSPcodefile_RevF16b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":0000
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenDSPvarfile_RevF16b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":0532
         Top             =   620
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF16b100m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   54
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPlistfile_RevF16b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":0A64
         Top             =   880
         Width           =   240
      End
   End
   Begin VB.Frame RevF16b250m_DSPCodeFiles 
      Caption         =   "Rev-F-16bit-250MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   43
      Top             =   6840
      Width           =   7695
      Begin VB.TextBox DSPcodefile_RevF16b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   46
         Top             =   360
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF16b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   45
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPlistfile_RevF16b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   44
         Top             =   900
         Width           =   5895
      End
      Begin VB.Image OpenDSPlistfile_RevF16b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":0F96
         Top             =   880
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF16b250m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   49
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPvarfile_RevF16b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":14C8
         Top             =   620
         Width           =   240
      End
      Begin VB.Image OpenDSPcodefile_RevF16b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":19FA
         Top             =   360
         Width           =   240
      End
      Begin VB.Label DSPvarfilelabel_RevF16b250m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   48
         Top             =   620
         Width           =   855
      End
      Begin VB.Label DSPlistfilelabel_RevF16b250m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   47
         Top             =   880
         Width           =   855
      End
   End
   Begin VB.Frame RevF14b500m_DSPCodeFiles 
      Caption         =   "Rev-F-14bit-500MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   36
      Top             =   9480
      Width           =   7695
      Begin VB.TextBox DSPlistfile_RevF14b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   39
         Top             =   900
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF14b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   38
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPcodefile_RevF14b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   37
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label DSPlistfilelabel_RevF14b500m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   42
         Top             =   880
         Width           =   855
      End
      Begin VB.Label DSPvarfilelabel_RevF14b500m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   41
         Top             =   620
         Width           =   855
      End
      Begin VB.Image OpenDSPcodefile_RevF14b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":1F2C
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenDSPvarfile_RevF14b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":245E
         Top             =   620
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF14b500m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   40
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPlistfile_RevF14b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":2990
         Top             =   880
         Width           =   240
      End
   End
   Begin VB.Frame RevF12b500m_DSPCodeFiles 
      Caption         =   "Rev-F-12bit-500MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   29
      Top             =   8160
      Width           =   7695
      Begin VB.TextBox DSPlistfile_RevF12b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   32
         Top             =   900
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF12b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   31
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPcodefile_RevF12b500m 
         Height          =   285
         Left            =   1320
         TabIndex        =   30
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label DSPlistfilelabel_RevF12b500m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   35
         Top             =   880
         Width           =   855
      End
      Begin VB.Label DSPvarfilelabel_RevF12b500m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   34
         Top             =   620
         Width           =   855
      End
      Begin VB.Image OpenDSPcodefile_RevF12b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":2EC2
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenDSPvarfile_RevF12b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":33F4
         Top             =   620
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF12b500m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   33
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPlistfile_RevF12b500m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":3926
         Top             =   880
         Width           =   240
      End
   End
   Begin VB.Frame RevF14b250m_DSPCodeFiles 
      Caption         =   "Rev-F-14bit-250MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   22
      Top             =   5520
      Width           =   7695
      Begin VB.TextBox DSPlistfile_RevF14b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   25
         Top             =   900
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF14b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   24
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPcodefile_RevF14b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   23
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label DSPlistfilelabel_RevF14b250m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   28
         Top             =   880
         Width           =   855
      End
      Begin VB.Label DSPvarfilelabel_RevF14b250m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   27
         Top             =   620
         Width           =   855
      End
      Begin VB.Image OpenDSPcodefile_RevF14b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":3E58
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenDSPvarfile_RevF14b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":438A
         Top             =   620
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF14b250m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   26
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPlistfile_RevF14b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":48BC
         Top             =   880
         Width           =   240
      End
   End
   Begin VB.Frame RevF12b250m_DSPCodeFiles 
      Caption         =   "Rev-F-12bit-250MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   15
      Top             =   4200
      Width           =   7695
      Begin VB.TextBox DSPlistfile_RevF12b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   18
         Top             =   900
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF12b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   17
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPcodefile_RevF12b250m 
         Height          =   285
         Left            =   1320
         TabIndex        =   16
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label DSPlistfilelabel_RevF12b250m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   21
         Top             =   880
         Width           =   855
      End
      Begin VB.Label DSPvarfilelabel_RevF12b250m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   20
         Top             =   620
         Width           =   855
      End
      Begin VB.Image OpenDSPcodefile_RevF12b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":4DEE
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenDSPvarfile_RevF12b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":5320
         Top             =   620
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF12b250m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   19
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPlistfile_RevF12b250m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":5852
         Top             =   880
         Width           =   240
      End
   End
   Begin VB.Frame RevF14b100m_DSPCodeFiles 
      Caption         =   "Rev-F-14bit-100MSPS DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   8
      Top             =   1560
      Width           =   7695
      Begin VB.TextBox DSPcodefile_RevF14b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   11
         Top             =   360
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevF14b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   10
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPlistfile_RevF14b100m 
         Height          =   285
         Left            =   1320
         TabIndex        =   9
         Top             =   900
         Width           =   5895
      End
      Begin VB.Image OpenDSPlistfile_RevF14b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":5D84
         Top             =   880
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevF14b100m 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   14
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPvarfile_RevF14b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":62B6
         Top             =   620
         Width           =   240
      End
      Begin VB.Image OpenDSPcodefile_RevF14b100m 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":67E8
         Top             =   360
         Width           =   240
      End
      Begin VB.Label DSPvarfilelabel_RevF14b100m 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   13
         Top             =   620
         Width           =   855
      End
      Begin VB.Label DSPlistfilelabel_RevF14b100m 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   12
         Top             =   880
         Width           =   855
      End
   End
   Begin VB.Frame RevBCD_DSPCodeFiles 
      Caption         =   "Rev-B/C/D DSP Code Files"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1250
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   7695
      Begin VB.TextBox DSPlistfile_RevBCD 
         Height          =   285
         Left            =   1320
         TabIndex        =   3
         Top             =   900
         Width           =   5895
      End
      Begin VB.TextBox DSPvarfile_RevBCD 
         Height          =   285
         Left            =   1320
         TabIndex        =   2
         Top             =   630
         Width           =   5895
      End
      Begin VB.TextBox DSPcodefile_RevBCD 
         Height          =   285
         Left            =   1320
         TabIndex        =   1
         Top             =   360
         Width           =   5895
      End
      Begin VB.Label DSPlistfilelabel_RevBCD 
         Caption         =   "DSP list file"
         Height          =   255
         Left            =   240
         TabIndex        =   7
         Top             =   880
         Width           =   855
      End
      Begin VB.Label DSPvarfilelabel_RevBCD 
         Caption         =   "DSP var file"
         Height          =   255
         Left            =   240
         TabIndex        =   6
         Top             =   620
         Width           =   855
      End
      Begin VB.Image OpenDSPcodefile_RevBCD 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":6D1A
         Top             =   360
         Width           =   240
      End
      Begin VB.Image OpenDSPvarfile_RevBCD 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":724C
         Top             =   620
         Width           =   240
      End
      Begin VB.Label DSPCodelabel_RevBCD 
         Caption         =   "DSP code"
         Height          =   255
         Left            =   240
         TabIndex        =   4
         Top             =   360
         Width           =   855
      End
      Begin VB.Image OpenDSPlistfile_RevBCD 
         Height          =   225
         Left            =   7320
         Picture         =   "dlgDSPCodeFiles.frx":777E
         Top             =   880
         Width           =   240
      End
   End
End
Attribute VB_Name = "dlgDSPCodeFiles"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
  
Private Sub Form_Load()
    DSPcodefile_RevBCD.Text = Pixie16_DSP_Code_File_RevBCD
    DSPvarfile_RevBCD.Text = Pixie16_DSP_Var_File_RevBCD
    DSPlistfile_RevBCD.Text = Pixie16_DSP_List_File_RevBCD

    DSPcodefile_RevF14b100m.Text = Pixie16_DSP_Code_File_RevF14b100m
    DSPvarfile_RevF14b100m.Text = Pixie16_DSP_Var_File_RevF14b100m
    DSPlistfile_RevF14b100m.Text = Pixie16_DSP_List_File_RevF14b100m

    DSPcodefile_RevF16b100m.Text = Pixie16_DSP_Code_File_RevF16b100m
    DSPvarfile_RevF16b100m.Text = Pixie16_DSP_Var_File_RevF16b100m
    DSPlistfile_RevF16b100m.Text = Pixie16_DSP_List_File_RevF16b100m
    
    DSPcodefile_RevF12b250m.Text = Pixie16_DSP_Code_File_RevF12b250m
    DSPvarfile_RevF12b250m.Text = Pixie16_DSP_Var_File_RevF12b250m
    DSPlistfile_RevF12b250m.Text = Pixie16_DSP_List_File_RevF12b250m

    DSPcodefile_RevF14b250m.Text = Pixie16_DSP_Code_File_RevF14b250m
    DSPvarfile_RevF14b250m.Text = Pixie16_DSP_Var_File_RevF14b250m
    DSPlistfile_RevF14b250m.Text = Pixie16_DSP_List_File_RevF14b250m

    DSPcodefile_RevF16b250m.Text = Pixie16_DSP_Code_File_RevF16b250m
    DSPvarfile_RevF16b250m.Text = Pixie16_DSP_Var_File_RevF16b250m
    DSPlistfile_RevF16b250m.Text = Pixie16_DSP_List_File_RevF16b250m
    
    DSPcodefile_RevF12b500m.Text = Pixie16_DSP_Code_File_RevF12b500m
    DSPvarfile_RevF12b500m.Text = Pixie16_DSP_Var_File_RevF12b500m
    DSPlistfile_RevF12b500m.Text = Pixie16_DSP_List_File_RevF12b500m

    DSPcodefile_RevF14b500m.Text = Pixie16_DSP_Code_File_RevF14b500m
    DSPvarfile_RevF14b500m.Text = Pixie16_DSP_Var_File_RevF14b500m
    DSPlistfile_RevF14b500m.Text = Pixie16_DSP_List_File_RevF14b500m
End Sub

Private Sub OpenBrowser(ByVal folder As Integer)
    Dim sPath As String
    
    Select Case folder
        Case 0
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevBCD)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevBCD = sPath
            Pixie16_DSP_Code_File_RevBCD = DSPcodefile_RevBCD.Text
        Case 1
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevBCD)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevBCD = sPath
            Pixie16_DSP_Var_File_RevBCD = DSPvarfile_RevBCD.Text
        Case 2
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevBCD)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevBCD = sPath
            Pixie16_DSP_List_File_RevBCD = DSPlistfile_RevBCD.Text
        Case 3
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF14b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF14b100m = sPath
            Pixie16_DSP_Code_File_RevF14b100m = DSPcodefile_RevF14b100m.Text
        Case 4
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF14b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF14b100m = sPath
            Pixie16_DSP_Var_File_RevF14b100m = DSPvarfile_RevF14b100m.Text
        Case 5
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF14b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF14b100m = sPath
            Pixie16_DSP_List_File_RevF14b100m = DSPlistfile_RevF14b100m.Text
        Case 6
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF16b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF16b100m = sPath
            Pixie16_DSP_Code_File_RevF16b100m = DSPcodefile_RevF16b100m.Text
        Case 7
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF16b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF16b100m = sPath
            Pixie16_DSP_Var_File_RevF16b100m = DSPvarfile_RevF16b100m.Text
        Case 8
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF16b100m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF16b100m = sPath
            Pixie16_DSP_List_File_RevF16b100m = DSPlistfile_RevF16b100m.Text
        Case 9
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF12b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF12b250m = sPath
            Pixie16_DSP_Code_File_RevF12b250m = DSPcodefile_RevF12b250m.Text
        Case 10
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF12b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF12b250m = sPath
            Pixie16_DSP_Var_File_RevF12b250m = DSPvarfile_RevF12b250m.Text
        Case 11
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF12b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF12b250m = sPath
            Pixie16_DSP_List_File_RevF12b250m = DSPlistfile_RevF12b250m.Text
        Case 12
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF14b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF14b250m = sPath
            Pixie16_DSP_Code_File_RevF14b250m = DSPcodefile_RevF14b250m.Text
        Case 13
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF14b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF14b250m = sPath
            Pixie16_DSP_Var_File_RevF14b250m = DSPvarfile_RevF14b250m.Text
        Case 14
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF14b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF14b250m = sPath
            Pixie16_DSP_List_File_RevF14b250m = DSPlistfile_RevF14b250m.Text
        Case 15
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF16b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF16b250m = sPath
            Pixie16_DSP_Code_File_RevF16b250m = DSPcodefile_RevF16b250m.Text
        Case 16
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF16b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF16b250m = sPath
            Pixie16_DSP_Var_File_RevF16b250m = DSPvarfile_RevF16b250m.Text
        Case 17
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF16b250m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF16b250m = sPath
            Pixie16_DSP_List_File_RevF16b250m = DSPlistfile_RevF16b250m.Text
        Case 18
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF12b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF12b500m = sPath
            Pixie16_DSP_Code_File_RevF12b500m = DSPcodefile_RevF12b500m.Text
        Case 19
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF12b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF12b500m = sPath
            Pixie16_DSP_Var_File_RevF12b500m = DSPvarfile_RevF12b500m.Text
        Case 20
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF12b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF12b500m = sPath
            Pixie16_DSP_List_File_RevF12b500m = DSPlistfile_RevF12b500m.Text
        Case 21
            sPath = BrowseForFile(Me, "Select the DSP code file", Pixie16_DSP_Code_File_RevF14b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPcodefile_RevF14b500m = sPath
            Pixie16_DSP_Code_File_RevF14b500m = DSPcodefile_RevF14b500m.Text
        Case 22
            sPath = BrowseForFile(Me, "Select the DSP var file", Pixie16_DSP_Var_File_RevF14b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPvarfile_RevF14b500m = sPath
            Pixie16_DSP_Var_File_RevF14b500m = DSPvarfile_RevF14b500m.Text
        Case 23
            sPath = BrowseForFile(Me, "Select the DSP list file", Pixie16_DSP_List_File_RevF14b500m)
            If Len(sPath) = 0 Then Exit Sub  'user selected cancel
            DSPlistfile_RevF14b500m = sPath
            Pixie16_DSP_List_File_RevF14b500m = DSPlistfile_RevF14b500m.Text
    End Select
End Sub

Private Sub OpenDSPcodefile_RevBCD_Click()
    OpenBrowser (0)
End Sub

Private Sub OpenDSPvarfile_RevBCD_Click()
    OpenBrowser (1)
End Sub

Private Sub OpenDSPlistfile_RevBCD_Click()
    OpenBrowser (2)
End Sub

Private Sub OpenDSPcodefile_RevF14b100m_Click()
    OpenBrowser (3)
End Sub

Private Sub OpenDSPvarfile_RevF14b100m_Click()
    OpenBrowser (4)
End Sub

Private Sub OpenDSPlistfile_RevF14b100m_Click()
    OpenBrowser (5)
End Sub

Private Sub OpenDSPcodefile_RevF16b100m_Click()
    OpenBrowser (6)
End Sub

Private Sub OpenDSPvarfile_RevF16b100m_Click()
    OpenBrowser (7)
End Sub

Private Sub OpenDSPlistfile_RevF16b100m_Click()
    OpenBrowser (8)
End Sub

Private Sub OpenDSPcodefile_RevF12b250m_Click()
    OpenBrowser (9)
End Sub

Private Sub OpenDSPvarfile_RevF12b250m_Click()
    OpenBrowser (10)
End Sub

Private Sub OpenDSPlistfile_RevF12b250m_Click()
    OpenBrowser (11)
End Sub

Private Sub OpenDSPcodefile_RevF14b250m_Click()
    OpenBrowser (12)
End Sub

Private Sub OpenDSPvarfile_RevF14b250m_Click()
    OpenBrowser (13)
End Sub

Private Sub OpenDSPlistfile_RevF14b250m_Click()
    OpenBrowser (14)
End Sub

Private Sub OpenDSPcodefile_RevF16b250m_Click()
    OpenBrowser (15)
End Sub

Private Sub OpenDSPvarfile_RevF16b250m_Click()
    OpenBrowser (16)
End Sub

Private Sub OpenDSPlistfile_RevF16b250m_Click()
    OpenBrowser (17)
End Sub

Private Sub OpenDSPcodefile_RevF12b500m_Click()
    OpenBrowser (18)
End Sub

Private Sub OpenDSPvarfile_RevF12b500m_Click()
    OpenBrowser (19)
End Sub

Private Sub OpenDSPlistfile_RevF12b500m_Click()
    OpenBrowser (20)
End Sub

Private Sub OpenDSPcodefile_RevF14b500m_Click()
    OpenBrowser (21)
End Sub

Private Sub OpenDSPvarfile_RevF14b500m_Click()
    OpenBrowser (22)
End Sub

Private Sub OpenDSPlistfile_RevF14b500m_Click()
    OpenBrowser (23)
End Sub


Private Sub DSPcodefile_RevBCD_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevBCD = DSPcodefile_RevBCD.Text
    End If
End Sub

Private Sub DSPcodefile_RevBCD_LostFocus()
    Pixie16_DSP_Code_File_RevBCD = DSPcodefile_RevBCD.Text
End Sub

Private Sub DSPvarfile_RevBCD_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevBCD = DSPvarfile_RevBCD.Text
    End If
End Sub

Private Sub DSPvarfile_RevBCD_LostFocus()
    Pixie16_DSP_Var_File_RevBCD = DSPvarfile_RevBCD.Text
End Sub

Private Sub DSPlistfile_RevBCD_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevBCD = DSPlistfile_RevBCD.Text
    End If
End Sub

Private Sub DSPlistfile_RevBCD_LostFocus()
    Pixie16_DSP_List_File_RevBCD = DSPlistfile_RevBCD.Text
End Sub


Private Sub DSPcodefile_RevF14b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF14b100m = DSPcodefile_RevF14b100m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF14b100m_LostFocus()
    Pixie16_DSP_Code_File_RevF14b100m = DSPcodefile_RevF14b100m.Text
End Sub

Private Sub DSPvarfile_RevF14b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF14b100m = DSPvarfile_RevF14b100m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF14b100m_LostFocus()
    Pixie16_DSP_Var_File_RevF14b100m = DSPvarfile_RevF14b100m.Text
End Sub

Private Sub DSPlistfile_RevF14b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF14b100m = DSPlistfile_RevF14b100m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF14b100m_LostFocus()
    Pixie16_DSP_List_File_RevF14b100m = DSPlistfile_RevF14b100m.Text
End Sub


Private Sub DSPcodefile_RevF16b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF16b100m = DSPcodefile_RevF16b100m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF16b100m_LostFocus()
    Pixie16_DSP_Code_File_RevF16b100m = DSPcodefile_RevF16b100m.Text
End Sub

Private Sub DSPvarfile_RevF16b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF16b100m = DSPvarfile_RevF16b100m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF16b100m_LostFocus()
    Pixie16_DSP_Var_File_RevF16b100m = DSPvarfile_RevF16b100m.Text
End Sub

Private Sub DSPlistfile_RevF16b100m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF16b100m = DSPlistfile_RevF16b100m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF16b100m_LostFocus()
    Pixie16_DSP_List_File_RevF16b100m = DSPlistfile_RevF16b100m.Text
End Sub


Private Sub DSPcodefile_RevF12b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF12b250m = DSPcodefile_RevF12b250m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF12b250m_LostFocus()
    Pixie16_DSP_Code_File_RevF12b250m = DSPcodefile_RevF12b250m.Text
End Sub

Private Sub DSPvarfile_RevF12b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF12b250m = DSPvarfile_RevF12b250m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF12b250m_LostFocus()
    Pixie16_DSP_Var_File_RevF12b250m = DSPvarfile_RevF12b250m.Text
End Sub

Private Sub DSPlistfile_RevF12b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF12b250m = DSPlistfile_RevF12b250m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF12b250m_LostFocus()
    Pixie16_DSP_List_File_RevF12b250m = DSPlistfile_RevF12b250m.Text
End Sub


Private Sub DSPcodefile_RevF14b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF14b250m = DSPcodefile_RevF14b250m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF14b250m_LostFocus()
    Pixie16_DSP_Code_File_RevF14b250m = DSPcodefile_RevF14b250m.Text
End Sub

Private Sub DSPvarfile_RevF14b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF14b250m = DSPvarfile_RevF14b250m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF14b250m_LostFocus()
    Pixie16_DSP_Var_File_RevF14b250m = DSPvarfile_RevF14b250m.Text
End Sub

Private Sub DSPlistfile_RevF14b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF14b250m = DSPlistfile_RevF14b250m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF14b250m_LostFocus()
    Pixie16_DSP_List_File_RevF14b250m = DSPlistfile_RevF14b250m.Text
End Sub


Private Sub DSPcodefile_RevF16b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF16b250m = DSPcodefile_RevF16b250m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF16b250m_LostFocus()
    Pixie16_DSP_Code_File_RevF16b250m = DSPcodefile_RevF16b250m.Text
End Sub

Private Sub DSPvarfile_RevF16b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF16b250m = DSPvarfile_RevF16b250m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF16b250m_LostFocus()
    Pixie16_DSP_Var_File_RevF16b250m = DSPvarfile_RevF16b250m.Text
End Sub

Private Sub DSPlistfile_RevF16b250m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF16b250m = DSPlistfile_RevF16b250m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF16b250m_LostFocus()
    Pixie16_DSP_List_File_RevF16b250m = DSPlistfile_RevF16b250m.Text
End Sub


Private Sub DSPcodefile_RevF12b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF12b500m = DSPcodefile_RevF12b500m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF12b500m_LostFocus()
    Pixie16_DSP_Code_File_RevF12b500m = DSPcodefile_RevF12b500m.Text
End Sub

Private Sub DSPvarfile_RevF12b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF12b500m = DSPvarfile_RevF12b500m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF12b500m_LostFocus()
    Pixie16_DSP_Var_File_RevF12b500m = DSPvarfile_RevF12b500m.Text
End Sub

Private Sub DSPlistfile_RevF12b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF12b500m = DSPlistfile_RevF12b500m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF12b500m_LostFocus()
    Pixie16_DSP_List_File_RevF12b500m = DSPlistfile_RevF12b500m.Text
End Sub


Private Sub DSPcodefile_RevF14b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Code_File_RevF14b500m = DSPcodefile_RevF14b500m.Text
    End If
End Sub

Private Sub DSPcodefile_RevF14b500m_LostFocus()
    Pixie16_DSP_Code_File_RevF14b500m = DSPcodefile_RevF14b500m.Text
End Sub

Private Sub DSPvarfile_RevF14b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_Var_File_RevF14b500m = DSPvarfile_RevF14b500m.Text
    End If
End Sub

Private Sub DSPvarfile_RevF14b500m_LostFocus()
    Pixie16_DSP_Var_File_RevF14b500m = DSPvarfile_RevF14b500m.Text
End Sub

Private Sub DSPlistfile_RevF14b500m_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        Pixie16_DSP_List_File_RevF14b500m = DSPlistfile_RevF14b500m.Text
    End If
End Sub

Private Sub DSPlistfile_RevF14b500m_LostFocus()
    Pixie16_DSP_List_File_RevF14b500m = DSPlistfile_RevF14b500m.Text
End Sub


Private Sub ReturnfromdlgDSPCodeFiles_Click()
    Unload Me
End Sub
