VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form DAQParametersSetup 
   Caption         =   "Set DAQ Parameters"
   ClientHeight    =   10560
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10185
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   10560
   ScaleWidth      =   10185
   Begin TabDlg.SSTab SSTab1 
      Height          =   8895
      Left            =   120
      TabIndex        =   3
      Top             =   240
      Width           =   9930
      _ExtentX        =   17515
      _ExtentY        =   15690
      _Version        =   393216
      Tabs            =   12
      TabsPerRow      =   4
      TabHeight       =   520
      TabCaption(0)   =   "Filter"
      TabPicture(0)   =   "DAQParametersSetup.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "TriggerFilterLabel"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "EnergyFilterLabel"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "SlowFilterRangeLabel"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "FastFilterRangeLabel"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "FastFilterRangeEdit"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "SlowFilterRangeEdit"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "TriggerFilterGrid"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "EnergyFilterGrid"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "txtEditTF"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "txtEditEF"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).ControlCount=   10
      TabCaption(1)   =   "Analog Signal Conditioning"
      TabPicture(1)   =   "DAQParametersSetup.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "txtEditASC"
      Tab(1).Control(1)=   "AnalogGrid"
      Tab(1).ControlCount=   2
      TabCaption(2)   =   "Histogram Control"
      TabPicture(2)   =   "DAQParametersSetup.frx":0038
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "HistogramGrid"
      Tab(2).Control(1)=   "txtEditHC"
      Tab(2).Control(2)=   "BaselineControlTextBox"
      Tab(2).ControlCount=   3
      TabCaption(3)   =   "Decay Time"
      TabPicture(3)   =   "DAQParametersSetup.frx":0054
      Tab(3).ControlEnabled=   0   'False
      Tab(3).Control(0)=   "FindTauFrame"
      Tab(3).Control(1)=   "txtEditTau"
      Tab(3).Control(2)=   "DecayTimeGrid"
      Tab(3).ControlCount=   3
      TabCaption(4)   =   "Pulse Shape Analysis"
      TabPicture(4)   =   "DAQParametersSetup.frx":0070
      Tab(4).ControlEnabled=   0   'False
      Tab(4).Control(0)=   "txtEditPSA"
      Tab(4).Control(1)=   "PSAGrid"
      Tab(4).ControlCount=   2
      TabCaption(5)   =   "Baseline Control"
      TabPicture(5)   =   "DAQParametersSetup.frx":008C
      Tab(5).ControlEnabled=   0   'False
      Tab(5).Control(0)=   "Text1"
      Tab(5).Control(1)=   "txtEditBaseline"
      Tab(5).Control(2)=   "BaselineGrid"
      Tab(5).ControlCount=   3
      TabCaption(6)   =   "Control Registers"
      TabPicture(6)   =   "DAQParametersSetup.frx":00A8
      Tab(6).ControlEnabled=   0   'False
      Tab(6).Control(0)=   "EditChanCSRAFrame"
      Tab(6).Control(1)=   "EditModCSRBFrame"
      Tab(6).ControlCount=   2
      TabCaption(7)   =   "CFD Trigger"
      TabPicture(7)   =   "DAQParametersSetup.frx":00C4
      Tab(7).ControlEnabled=   0   'False
      Tab(7).Control(0)=   "CFDTextBox"
      Tab(7).Control(1)=   "txtEditCFDTrigger"
      Tab(7).Control(2)=   "CFDTriggerGrid"
      Tab(7).ControlCount=   3
      TabCaption(8)   =   "Trigger Stretch Lengths"
      TabPicture(8)   =   "DAQParametersSetup.frx":00E0
      Tab(8).ControlEnabled=   0   'False
      Tab(8).Control(0)=   "txtEditTrigLen"
      Tab(8).Control(1)=   "TriggerLengthsGrid"
      Tab(8).ControlCount=   2
      TabCaption(9)   =   "FIFO Delays"
      TabPicture(9)   =   "DAQParametersSetup.frx":00FC
      Tab(9).ControlEnabled=   0   'False
      Tab(9).Control(0)=   "txtEditFIFODelays"
      Tab(9).Control(1)=   "FIFODelaysGrid"
      Tab(9).ControlCount=   2
      TabCaption(10)  =   "Multiplicity"
      TabPicture(10)  =   "DAQParametersSetup.frx":0118
      Tab(10).ControlEnabled=   0   'False
      Tab(10).Control(0)=   "txtEditFTNNEna"
      Tab(10).Control(1)=   "FTNNEnaGrid"
      Tab(10).Control(2)=   "txtEditTrigConfig"
      Tab(10).Control(3)=   "TrigConfigGrid"
      Tab(10).Control(4)=   "MultiplicityTextBox"
      Tab(10).Control(5)=   "txtEditMultiplicity"
      Tab(10).Control(6)=   "MultiplicityGrid"
      Tab(10).ControlCount=   7
      TabCaption(11)  =   "QDC"
      TabPicture(11)  =   "DAQParametersSetup.frx":0134
      Tab(11).ControlEnabled=   0   'False
      Tab(11).Control(0)=   "txtEditQDC"
      Tab(11).Control(1)=   "QDCGrid"
      Tab(11).ControlCount=   2
      Begin VB.TextBox Text1 
         BackColor       =   &H80000000&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   3255
         Left            =   -69000
         MultiLine       =   -1  'True
         TabIndex        =   62
         Text            =   "DAQParametersSetup.frx":0150
         Top             =   2400
         Width           =   3255
      End
      Begin VB.TextBox BaselineControlTextBox 
         BackColor       =   &H80000000&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2175
         Left            =   -69840
         MultiLine       =   -1  'True
         TabIndex        =   61
         Text            =   "DAQParametersSetup.frx":0245
         Top             =   2400
         Width           =   3255
      End
      Begin VB.TextBox txtEditFTNNEna 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   0
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   0
         EndProperty
         Height          =   495
         Left            =   -67250
         TabIndex        =   60
         Top             =   7560
         Visible         =   0   'False
         Width           =   1455
      End
      Begin MSFlexGridLib.MSFlexGrid FTNNEnaGrid 
         Height          =   3350
         Left            =   -70600
         TabIndex        =   59
         Top             =   4820
         Width           =   5050
         _ExtentX        =   8916
         _ExtentY        =   5900
         _Version        =   393216
      End
      Begin VB.TextBox txtEditTrigConfig 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   0
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   0
         EndProperty
         Height          =   495
         Left            =   -67250
         TabIndex        =   58
         Top             =   3960
         Visible         =   0   'False
         Width           =   1455
      End
      Begin MSFlexGridLib.MSFlexGrid TrigConfigGrid 
         Height          =   3365
         Left            =   -70600
         TabIndex        =   57
         Top             =   1320
         Width           =   5050
         _ExtentX        =   8916
         _ExtentY        =   5927
         _Version        =   393216
      End
      Begin VB.TextBox MultiplicityTextBox 
         Alignment       =   2  'Center
         BackColor       =   &H8000000F&
         BorderStyle     =   0  'None
         Height          =   315
         Left            =   -72480
         TabIndex        =   56
         Text            =   "(All entries in the above tables are hexadecimal numbers)"
         Top             =   8400
         Width           =   4815
      End
      Begin VB.TextBox CFDTextBox 
         BackColor       =   &H80000000&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2655
         Left            =   -69120
         MultiLine       =   -1  'True
         TabIndex        =   55
         Text            =   "DAQParametersSetup.frx":031D
         Top             =   2400
         Width           =   3255
      End
      Begin VB.TextBox txtEditQDC 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   495
         Left            =   -67080
         TabIndex        =   52
         Top             =   7200
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditMultiplicity 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   0
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   0
         EndProperty
         Height          =   495
         Left            =   -72360
         TabIndex        =   50
         Top             =   6840
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditFIFODelays 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   495
         Left            =   -71040
         TabIndex        =   48
         Top             =   7200
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditTrigLen 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   495
         Left            =   -67320
         TabIndex        =   46
         Top             =   7200
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditCFDTrigger 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   495
         Left            =   -71520
         TabIndex        =   44
         Top             =   7080
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.Frame EditChanCSRAFrame 
         Caption         =   "Channel Control Register A"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   4575
         Left            =   -74640
         TabIndex        =   33
         Top             =   4185
         Width           =   8895
         Begin VB.CommandButton ChanCSRAAdvancedBits_Button 
            Caption         =   "Advanced Settings"
            Height          =   375
            Left            =   600
            TabIndex        =   66
            Top             =   3960
            Width           =   1575
         End
         Begin VB.CheckBox InversePileupControlCheck 
            Caption         =   "[16] Inverse pileup rejection control"
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
            TabIndex        =   65
            Top             =   3510
            Width           =   3855
         End
         Begin VB.CheckBox CaptureESumsCheck 
            Caption         =   "[12] Capture raw energy sums and baseline"
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
            TabIndex        =   54
            Top             =   2460
            Width           =   5055
         End
         Begin VB.CheckBox QDCCaptureCheck 
            Caption         =   "[9] Capture QDC sums"
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
            TabIndex        =   53
            Top             =   1410
            Width           =   3615
         End
         Begin CWUIControlsLib.CWNumEdit CHANCSRAChanNumEdit 
            Height          =   495
            Left            =   7800
            TabIndex        =   41
            Top             =   1440
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
         Begin VB.CheckBox NormalPileupControlCheck 
            Caption         =   "[15] Normal pileup rejection control"
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
            TabIndex        =   40
            Top             =   3165
            Width           =   3615
         End
         Begin VB.CheckBox ChanTrigCheck 
            Caption         =   "[13] Require channel external trigger for validation"
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
            TabIndex        =   39
            Top             =   2805
            Width           =   4815
         End
         Begin VB.CheckBox GlobalExternalTrigCheck 
            Caption         =   "[11] Require global external trigger for validation"
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
            TabIndex        =   38
            Top             =   2115
            Width           =   4695
         End
         Begin VB.CheckBox CFDTRigCheck 
            Caption         =   "[10] Enable CFD trigger"
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
            TabIndex        =   37
            Top             =   1755
            Width           =   2415
         End
         Begin VB.CheckBox TraceCaptureCheck 
            Caption         =   "[8] Capture trace"
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
            TabIndex        =   36
            Top             =   1065
            Width           =   2295
         End
         Begin VB.CheckBox GoodCheck 
            Caption         =   "[2] Good channel"
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
            TabIndex        =   35
            Top             =   360
            Width           =   2055
         End
         Begin VB.CheckBox HistogrammingCheck 
            Caption         =   "[7] Histogram energies"
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
            TabIndex        =   34
            Top             =   705
            Width           =   2775
         End
         Begin VB.Label ChanCSRALabel 
            Caption         =   "Channel Number"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   12
               Charset         =   0
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   495
            Left            =   5880
            TabIndex        =   42
            Top             =   1560
            Width           =   1935
         End
      End
      Begin VB.Frame EditModCSRBFrame 
         Caption         =   "Module Control Register B"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2925
         Left            =   -74640
         TabIndex        =   27
         Top             =   1100
         Width           =   8895
         Begin VB.CommandButton ModCSRBAdvancedBits_Button 
            Caption         =   "Advanced Settings"
            Height          =   375
            Left            =   600
            TabIndex        =   69
            Top             =   2350
            Width           =   1575
         End
         Begin VB.CheckBox ModulePullUpsCheck 
            Caption         =   "[0] Enable pullups for backplane bus lines"
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
            TabIndex        =   32
            Top             =   360
            Width           =   4095
         End
         Begin VB.CheckBox DirectorModuleCheck 
            Caption         =   "[4] Send triggers to trigger card and local backplane"
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
            TabIndex        =   31
            Top             =   740
            Width           =   5000
         End
         Begin VB.CheckBox CrateMasterCheck 
            Caption         =   "[6] Chassis master module"
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
            TabIndex        =   30
            Top             =   1120
            Width           =   2760
         End
         Begin VB.CheckBox EnableRunInhibitCheck 
            Caption         =   "[10] Enable run inhibit signal input"
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
            TabIndex        =   29
            Top             =   1500
            Width           =   3350
         End
         Begin VB.CheckBox MultipleCratesCheck 
            Caption         =   "[11] Synchronize runs and distribute triggers among multiple chassis"
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
            TabIndex        =   28
            Top             =   1880
            Width           =   6375
         End
         Begin VB.Label Label2 
            Caption         =   "(Enable for only one module among all chassis)"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   -1  'True
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   3700
            TabIndex        =   68
            Top             =   1590
            Width           =   3495
         End
         Begin VB.Label ModCSRB_6_note 
            Caption         =   "(Enable for only one module per chassis)"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   -1  'True
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   3195
            TabIndex        =   67
            Top             =   1210
            Width           =   3255
         End
         Begin VB.Label ModCSRB_4_note 
            Caption         =   "(Enable for only one module among all chassis)"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   -1  'True
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   5350
            TabIndex        =   64
            Top             =   830
            Width           =   3300
         End
         Begin VB.Label ModCSRB_0_note 
            Caption         =   "(Enable for only one module per chassis)"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   400
               Underline       =   -1  'True
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   255
            Left            =   4500
            TabIndex        =   63
            Top             =   450
            Width           =   3015
         End
      End
      Begin VB.Frame FindTauFrame 
         Caption         =   "Auto Tau_Finder"
         Height          =   6855
         Left            =   -71040
         TabIndex        =   24
         Top             =   1140
         Width           =   5175
         Begin VB.TextBox AutoTauFinderText 
            Appearance      =   0  'Flat
            BackColor       =   &H8000000F&
            BorderStyle     =   0  'None
            Height          =   975
            Left            =   2760
            MultiLine       =   -1  'True
            TabIndex        =   71
            Text            =   "DAQParametersSetup.frx":0409
            Top             =   4560
            Width           =   2175
         End
         Begin VB.CommandButton AcceptTauValue 
            Caption         =   "Accept Found Decay Times"
            Height          =   855
            Left            =   2760
            TabIndex        =   26
            Top             =   3360
            Width           =   2175
         End
         Begin VB.CommandButton AutoFindTau 
            Caption         =   "Find Decay Times (Tau) for All 16 Channels"
            Height          =   855
            Left            =   2760
            TabIndex        =   25
            Top             =   2160
            Width           =   2175
         End
         Begin MSFlexGridLib.MSFlexGrid AutoTauFinderGrid 
            Height          =   6120
            Left            =   240
            TabIndex        =   70
            Top             =   480
            Width           =   2085
            _ExtentX        =   3678
            _ExtentY        =   10795
            _Version        =   393216
         End
      End
      Begin VB.TextBox txtEditBaseline 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   -72840
         TabIndex        =   23
         Top             =   7020
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditPSA 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   -72840
         TabIndex        =   21
         Top             =   7140
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditTau 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   -74160
         TabIndex        =   19
         Top             =   7140
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditHC 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   -72840
         TabIndex        =   17
         Top             =   7140
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditASC 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   -71160
         TabIndex        =   15
         Top             =   7020
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditEF 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   2160
         TabIndex        =   7
         Top             =   6900
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.TextBox txtEditTF 
         Appearance      =   0  'Flat
         BackColor       =   &H80000000&
         BorderStyle     =   0  'None
         Height          =   495
         Left            =   4440
         TabIndex        =   6
         Top             =   6900
         Visible         =   0   'False
         Width           =   1455
      End
      Begin MSFlexGridLib.MSFlexGrid EnergyFilterGrid 
         Height          =   6120
         Left            =   360
         TabIndex        =   4
         Top             =   1620
         Width           =   3435
         _ExtentX        =   6059
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid TriggerFilterGrid 
         Height          =   6120
         Left            =   4320
         TabIndex        =   5
         Top             =   1620
         Width           =   5090
         _ExtentX        =   8969
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin CWUIControlsLib.CWNumEdit SlowFilterRangeEdit 
         Height          =   495
         Left            =   2520
         TabIndex        =   10
         Top             =   8100
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
         Value_Val_1     =   2
         IncValueVarType_1=   5
         IncValue_Val_1  =   2
         AccelIncVarType_1=   5
         AccelInc_Val_1  =   2
         RangeMinVarType_1=   5
         RangeMin_Val_1  =   2
         RangeMaxVarType_1=   5
         RangeMax_Val_1  =   12
         Bindings_1      =   4
         ClassName_4     =   "CCWBindingHolderArray"
         Editor_4        =   5
         ClassName_5     =   "CCWBindingHolderArrayEditor"
         Owner_5         =   1
      End
      Begin CWUIControlsLib.CWNumEdit FastFilterRangeEdit 
         Height          =   495
         Left            =   6840
         TabIndex        =   11
         Top             =   8100
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
         RangeMax_Val_1  =   8
         Bindings_1      =   4
         ClassName_4     =   "CCWBindingHolderArray"
         Editor_4        =   5
         ClassName_5     =   "CCWBindingHolderArrayEditor"
         Owner_5         =   1
      End
      Begin MSFlexGridLib.MSFlexGrid AnalogGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   14
         Top             =   1620
         Width           =   5085
         _ExtentX        =   8969
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid HistogramGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   16
         Top             =   1620
         Width           =   3435
         _ExtentX        =   6059
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid DecayTimeGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   18
         Top             =   1620
         Width           =   2085
         _ExtentX        =   3678
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid PSAGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   20
         Top             =   1620
         Width           =   3435
         _ExtentX        =   6059
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid BaselineGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   22
         Top             =   1620
         Width           =   5190
         _ExtentX        =   9155
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid CFDTriggerGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   43
         Top             =   1620
         Width           =   4790
         _ExtentX        =   8440
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid TriggerLengthsGrid 
         Height          =   6120
         Left            =   -74760
         TabIndex        =   45
         Top             =   1800
         Width           =   9135
         _ExtentX        =   16113
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid FIFODelaysGrid 
         Height          =   6120
         Left            =   -74640
         TabIndex        =   47
         Top             =   1800
         Width           =   5340
         _ExtentX        =   9419
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid MultiplicityGrid 
         Height          =   6120
         Left            =   -74850
         TabIndex        =   49
         Top             =   1320
         Width           =   4130
         _ExtentX        =   7276
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin MSFlexGridLib.MSFlexGrid QDCGrid 
         Height          =   6120
         Left            =   -74940
         TabIndex        =   51
         Top             =   1800
         Width           =   9500
         _ExtentX        =   16748
         _ExtentY        =   10795
         _Version        =   393216
      End
      Begin VB.Label FastFilterRangeLabel 
         Caption         =   "Trigger Filter Range"
         Height          =   375
         Left            =   5280
         TabIndex        =   13
         Top             =   8220
         Width           =   1695
      End
      Begin VB.Label SlowFilterRangeLabel 
         Caption         =   "Energy Filter Range"
         Height          =   375
         Left            =   960
         TabIndex        =   12
         Top             =   8220
         Width           =   1695
      End
      Begin VB.Label EnergyFilterLabel 
         Caption         =   "Energy Filter"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   1200
         TabIndex        =   9
         Top             =   1260
         Width           =   1575
      End
      Begin VB.Label TriggerFilterLabel 
         Caption         =   "Trigger Filter"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   5880
         TabIndex        =   8
         Top             =   1260
         Width           =   1575
      End
   End
   Begin VB.CommandButton CloseDAQParSetupCommand 
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
      Left            =   7920
      TabIndex        =   2
      Top             =   9720
      Width           =   1935
   End
   Begin CWUIControlsLib.CWNumEdit DAQParSetupModNumEdit 
      Height          =   495
      Left            =   3120
      TabIndex        =   0
      Top             =   9600
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
   Begin VB.Label SetDAQParModNumLabel 
      Caption         =   "Module Number"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1200
      TabIndex        =   1
      Top             =   9675
      Width           =   1815
   End
End
Attribute VB_Name = "DAQParametersSetup"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim bOnFixedPartEF  As Boolean
Dim bOnFixedPartTF  As Boolean
Dim bOnFixedPartASC As Boolean
Dim bOnFixedPartHC  As Boolean
Dim bOnFixedPartTAU As Boolean
Dim bOnFixedPartPSA As Boolean
Dim bOnFixedPartBL  As Boolean
Dim bOnFixedPartCFD  As Boolean
Dim bOnFixedPartTL   As Boolean
Dim bOnFixedPartFIFO As Boolean
Dim bOnFixedPartMP   As Boolean
Dim bOnFixedPartTC   As Boolean
Dim bOnFixedPartFTNN As Boolean
Dim bOnFixedPartQDC  As Boolean

Private Sub ChanCSRAAdvancedBits_Button_Click()
    ' Check if the form is minimized or maximized
    If AdvancedChanCSRABitsForm.WindowState <> vbNormal Then
        AdvancedChanCSRABitsForm.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, AdvancedChanCSRABitsForm
    AdvancedChanCSRABitsForm.Height = 7485
    AdvancedChanCSRABitsForm.Width = 10050
    
    AdvancedChanCSRABitsForm.ZOrder 0

End Sub

Private Sub CloseDAQParSetupCommand_Click()
    Dim MsgTxt As String
    Dim Msg As Integer
    
    ' Do not close this form if its children "AdvancedChanCSRABitsForm" or "AdvancedModCSRBBitsForm" are still opened
    If IsLoaded("AdvancedChanCSRABitsForm") Or IsLoaded("AdvancedModCSRBBitsForm") Then
        MsgTxt = "Please close the Advanced ChanCSRA Bits Setup form and the Advanced ModCSRB Bits Setup form first"
        Msg = MsgBox(MsgTxt, 0, "Info")
        Exit Sub
    Else
        Unload Me
    End If
    
End Sub

Private Sub Form_Load()
    Dim X As Integer
    Dim Y As Integer
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim retval As Long
    Dim localModCSRB As Long
    Dim localChanCSRAlng As Long
    
    On Error GoTo ErrMSGOnConversion
       
    '*******************************************
    '   Disable close button
    '*******************************************
    
    RemoveCloseMenuItem (Me.hWnd)
       
    ' Update maximum number of modules
    DAQParSetupModNumEdit.Maximum = Number_Modules_Total - 1

    ' Update all relevant parameters
    ModuleNumber = DAQParSetupModNumEdit.Value
    
    retval = RetrieveChanParFromModule(ModuleNumber)
    If retval < 0 Then
        StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

    '###########################################
    '
    '   Filter tab - Energy Filter
    '
    '###########################################
    
    ' Enforce Energy Filter Range
    SlowFilterRangeEdit.Minimum = 1
    SlowFilterRangeEdit.Maximum = 6
    SlowFilterRangeEdit.IncDecValue = 1
    
    SlowFilterRangeEdit.Value = SlowFilterRange(ModuleNumber)
    
    FastFilterRangeEdit.Visible = False
    FastFilterRangeLabel.Visible = False
    
    With EnergyFilterGrid
        .Redraw = False
        bDoNotEditEF = True
                
        .Cols = 3
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1350
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Rise Time [탎]"
        .TextMatrix(0, 2) = "Flat Top [탎]"

        ' Align cells
        For X = 0 To 2
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Energy filter rise time
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(EnergyFilterRiseTime(X - 1), "0.000")
        Next X
        
        .Col = 2    ' Energy filter flat top
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(EnergyFilterFlatTop(X - 1), "0.000")
        Next X
        
        bDoNotEditEF = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditEF.Font
        .Name = EnergyFilterGrid.Font.Name
        .Size = EnergyFilterGrid.Font.Size
        .Weight = EnergyFilterGrid.Font.Weight
    End With
    txtEditEF.BackColor = vb3DLight
   
    txtEditEF = ""
    bDoNotEditEF = False


    '###########################################
    '
    '   Filter tab - Trigger Filter
    '
    '###########################################
    
    With TriggerFilterGrid
        .Redraw = False
        bDoNotEditTF = True
        
        .Cols = 4
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1350
        .ColWidth(3) = 1650
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Rise Time [탎]"
        .TextMatrix(0, 2) = "Flat Top [탎]"
        .TextMatrix(0, 3) = "Threshold [ADC units]"

        For X = 0 To 3
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Trigger filter rise time
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(TriggerFilterRiseTime(X - 1), "0.000")
        Next X
        
        .Col = 2    ' Trigger filter flat top
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(TriggerFilterFlatTop(X - 1), "0.000")
        Next X
        
        .Col = 3    ' Trigger threshold
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(TriggerThreshold(X - 1), "0.000")
        Next X
        
        bDoNotEditTF = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditTF.Font
        .Name = TriggerFilterGrid.Font.Name
        .Size = TriggerFilterGrid.Font.Size
        .Weight = TriggerFilterGrid.Font.Weight
    End With
    txtEditTF.BackColor = vb3DLight
   
    txtEditTF = ""
    bDoNotEditTF = False
    
    
    '###########################################
    '
    '   Analog Signal Conditioning tab
    '
    '###########################################
    
    With AnalogGrid
        .Redraw = False
        bDoNotEditASC = True
        
        .Cols = 4
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1350
        .ColWidth(3) = 1650
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Gain [V/V]"
        .TextMatrix(0, 2) = "Offset [V]"
        .TextMatrix(0, 3) = "Polarity"

        For X = 0 To 3
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
   
        'Load parameter values
  
        .Col = 1    ' Gain
        For X = 1 To NUM_CHANNELS
            .Row = X
            If Vgain(X - 1) = 1 Then
                If ModRev(DAQParSetupModNumEdit.Value) = 15 Then
                    .TextMatrix(X, 1) = Format(HIGH_GAIN_REVF, "0.0")
                Else
                    .TextMatrix(X, 1) = Format(HIGH_GAIN_REVBCD, "0.0")
                End If
            Else
                If ModRev(DAQParSetupModNumEdit.Value) = 15 Then
                    .TextMatrix(X, 1) = Format(LOW_GAIN_REVF, "0.000")
                Else
                    .TextMatrix(X, 1) = Format(LOW_GAIN_REVBCD, "0.0")
                End If
            End If
        Next X
  
        .Col = 2    ' DC offset
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(DCoffset(X - 1), "0.000")
        Next X
        
        .Col = 3    ' Polarity
        For X = 1 To NUM_CHANNELS
            .Row = X
            If Polarity(X - 1) = 1 Then
                .TextMatrix(.Row, .Col) = "(+) Positive"
            Else
                .TextMatrix(.Row, .Col) = "(-) Negative"
            End If
        Next X
      
        bDoNotEditASC = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditASC.Font
        .Name = AnalogGrid.Font.Name
        .Size = AnalogGrid.Font.Size
        .Weight = AnalogGrid.Font.Weight
    End With
    txtEditASC.BackColor = vb3DLight
   
    txtEditASC = ""
    bDoNotEditASC = False


    '###########################################
    '
    '   Histogram Control tab
    '
    '###########################################
    
    With HistogramGrid
        .Redraw = False
        bDoNotEditHC = True
        
        .Cols = 3
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1350
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Emin"
        .TextMatrix(0, 2) = "Binning Factor"
        
        For X = 0 To 2
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Minimum energy bin value
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = EnergyLow(X - 1)
        Next X
        
        .Col = 2    ' Energy histogram binning factor
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = EnergyBinFactor(X - 1)
        Next X
        
        bDoNotEditHC = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditHC.Font
        .Name = HistogramGrid.Font.Name
        .Size = HistogramGrid.Font.Size
        .Weight = HistogramGrid.Font.Weight
    End With
    txtEditHC.BackColor = vb3DLight
   
    txtEditHC = ""
    bDoNotEditHC = False


    '###########################################
    '
    '   Decay Time tab
    '
    '###########################################

    With DecayTimeGrid
        .Redraw = False
        bDoNotEditTAU = True
        
        .Cols = 2
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Decay Time [탎]"

        For X = 0 To 1
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Preamp Tau value
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(PreampTau(X - 1), "0.000")
        Next X
        
        bDoNotEditTAU = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditTau.Font
        .Name = DecayTimeGrid.Font.Name
        .Size = DecayTimeGrid.Font.Size
        .Weight = DecayTimeGrid.Font.Weight
    End With
    txtEditTau.BackColor = vb3DLight
   
    txtEditTau = ""
    bDoNotEditTAU = False

    With AutoTauFinderGrid
        .Redraw = False

        .Cols = 2
        .Rows = NUM_CHANNELS + 1

        .ColWidth(0) = 635
        .ColWidth(1) = 1350

        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Tau [탎]"

        For X = 0 To 1
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X

        .Redraw = True
    End With

    ' Disable AcceptTauValue until AutoFindTau is called
    AcceptTauValue.Enabled = False
    

    '###########################################
    '
    '   Pulse Shape Analysis tab
    '
    '###########################################
    
    With PSAGrid
        .Redraw = False
        bDoNotEditPSA = True
        
        .Cols = 3
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1350
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Trace Length [탎]"
        .TextMatrix(0, 2) = "Trace Delay [탎]"
        
        For X = 0 To 2
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Trace length
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(TraceLength(X - 1), "0.000")
        Next X
        
        .Col = 2    ' Trace delay
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(TraceDelay(X - 1), "0.000")
        Next X
        
        bDoNotEditPSA = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditPSA.Font
        .Name = PSAGrid.Font.Name
        .Size = PSAGrid.Font.Size
        .Weight = PSAGrid.Font.Weight
    End With
    txtEditPSA.BackColor = vb3DLight
   
    txtEditPSA = ""
    bDoNotEditPSA = False


    '###########################################
    '
    '   Baseline tab
    '
    '###########################################
    
    With BaselineGrid
        .Redraw = False
        bDoNotEditBL = True
        
        .Cols = 4
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1600
        .ColWidth(3) = 1500
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Baseline Cut"
        .TextMatrix(0, 2) = "Baseline Percent [%]"
        .TextMatrix(0, 3) = "Baseline Average"
        
        For X = 0 To 3
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Baseline cut
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = BLcut(X - 1)
        Next X
        
        .Col = 2    ' Baseline percent
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = BaselinePercent(X - 1)
        Next X
        
        .Col = 3    ' Baseline average
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = BaselineAverage(X - 1)
        Next X
       
        bDoNotEditBL = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditBaseline.Font
        .Name = BaselineGrid.Font.Name
        .Size = BaselineGrid.Font.Size
        .Weight = BaselineGrid.Font.Weight
    End With
    txtEditBaseline.BackColor = vb3DLight
   
    txtEditBaseline = ""
    bDoNotEditBL = False


    '###########################################
    '
    '   Control registers tab
    '
    '###########################################
    
    localModCSRB = ModCSRB(DAQParSetupModNumEdit.Value)
    
    ' Enable pullups for backplane bus lines
    If APP32_TstBit(MODCSRB_PULLUP, localModCSRB) = 1 Then
        ModulePullUpsCheck.Value = 1
    Else
        ModulePullUpsCheck.Value = 0
    End If
    
    ' Accept external trigger and run inhibit signals
    If APP32_TstBit(MODCSRB_DIRMOD, localModCSRB) = 1 Then
        DirectorModuleCheck.Value = 1
    Else
        DirectorModuleCheck.Value = 0
    End If

    ' Crate master module (multiple crates only)
    If APP32_TstBit(MODCSRB_CHASSISMASTER, localModCSRB) = 1 Then
        CrateMasterCheck.Value = 1
    Else
        CrateMasterCheck.Value = 0
    End If

    ' Enable run inhibit signal input
    If APP32_TstBit(MODCSRB_INHIBITENA, localModCSRB) = 1 Then
        EnableRunInhibitCheck.Value = 1
    Else
        EnableRunInhibitCheck.Value = 0
    End If

    ' Multiple crates
    If APP32_TstBit(MODCSRB_MULTCRATES, localModCSRB) = 1 Then
        MultipleCratesCheck.Value = 1
    Else
        MultipleCratesCheck.Value = 0
    End If
    
    If IsLoaded("AdvancedModCSRBBitsForm") Then
    
        ' Global fast trigger selection
        If APP32_TstBit(MODCSRB_GFTSEL, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleGFTSELCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleGFTSELCheck.Value = 0
        End If
        
        ' External validation trigger selection
        If APP32_TstBit(MODCSRB_ETSEL, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleETSELCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleETSELCheck.Value = 0
        End If
        
        ' Sort events
        If APP32_TstBit(MODCSRB_SORTEVENTS, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleSortEventsCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleSortEventsCheck.Value = 0
        End If
        
        ' Send fast triggers to backplane
        If APP32_TstBit(MODCSRB_BKPLFASTTRIG, localModCSRB) = 1 Then
            AdvancedModCSRBBitsForm.ModuleSendFTBackplaneCheck.Value = 1
        Else
            AdvancedModCSRBBitsForm.ModuleSendFTBackplaneCheck.Value = 0
        End If
    
    End If

    localChanCSRAlng = CLng(ChanCSRA(DAQParametersSetup.CHANCSRAChanNumEdit.Value))
    
    ' Good channel
    If APP32_TstBit(CCSRA_GOOD, localChanCSRAlng) = 1 Then
        GoodCheck.Value = 1
    Else
        GoodCheck.Value = 0
    End If
    
    ' Histogram energy
    If APP32_TstBit(CCSRA_HISTOE, localChanCSRAlng) = 1 Then
        HistogrammingCheck.Value = 1
    Else
        HistogrammingCheck.Value = 0
    End If
    
    ' Capture trace
    If APP32_TstBit(CCSRA_TRACEENA, localChanCSRAlng) = 1 Then
        TraceCaptureCheck.Value = 1
    Else
        TraceCaptureCheck.Value = 0
    End If
    
    ' Capture QDC sums
    If APP32_TstBit(CCSRA_QDCENA, localChanCSRAlng) = 1 Then
        QDCCaptureCheck.Value = 1
    Else
        QDCCaptureCheck.Value = 0
    End If
    
    ' Enable CFD trigger
    If APP32_TstBit(CCSRA_CFDMODE, localChanCSRAlng) = 1 Then
        CFDTRigCheck.Value = 1
    Else
        CFDTRigCheck.Value = 0
    End If
    
    ' Require global external trigger for validation
    If APP32_TstBit(CCSRA_GLOBTRIG, localChanCSRAlng) = 1 Then
        GlobalExternalTrigCheck.Value = 1
    Else
        GlobalExternalTrigCheck.Value = 0
    End If
    
    ' Capture raw energy sums and baseline
    If APP32_TstBit(CCSRA_ESUMSENA, localChanCSRAlng) = 1 Then
        CaptureESumsCheck.Value = 1
    Else
        CaptureESumsCheck.Value = 0
    End If
    
    ' Require channel external trigger for validation
    If APP32_TstBit(CCSRA_CHANTRIG, localChanCSRAlng) = 1 Then
        ChanTrigCheck.Value = 1
    Else
        ChanTrigCheck.Value = 0
    End If
    
    ' Normal pileup rejection
    If APP32_TstBit(CCSRA_PILEUPCTRL, localChanCSRAlng) = 1 Then
        NormalPileupControlCheck.Value = 1
    Else
        NormalPileupControlCheck.Value = 0
    End If

    ' Inverse pileup rejection
    If APP32_TstBit(CCSRA_INVERSEPILEUP, localChanCSRAlng) = 1 Then
        InversePileupControlCheck.Value = 1
    Else
        InversePileupControlCheck.Value = 0
    End If
    InversePileupControlCheck.Refresh

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
    
    With CFDTriggerGrid
        .Redraw = False
        bDoNotEditCFD = True
        
        .Cols = 4
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1350
        .ColWidth(2) = 1350
        .ColWidth(3) = 1350
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "CFD Delay [탎]"
        .TextMatrix(0, 2) = "CFD Scale"
        .TextMatrix(0, 3) = "CFD Threshold"
        
        For X = 0 To 3
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' CFD Delay
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(CFDDelay(X - 1), "0.000")
        Next X

        .Col = 2    ' CFD scaling factor
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = CFDScale(X - 1)
        Next X
        
        .Col = 3    ' CFD threshold
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = CFDThresh(X - 1)
        Next X
        
        bDoNotEditCFD = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditCFDTrigger.Font
        .Name = CFDTriggerGrid.Font.Name
        .Size = CFDTriggerGrid.Font.Size
        .Weight = CFDTriggerGrid.Font.Weight
    End With
    txtEditCFDTrigger.BackColor = vb3DLight
   
    txtEditCFDTrigger = ""
    bDoNotEditCFD = False


    '###########################################
    '
    '   Trigger Stretch Lengths tab
    '
    '###########################################
    
    With TriggerLengthsGrid
        .Redraw = False
        bDoNotEditTL = True
        
        .Cols = 5
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 2100
        .ColWidth(2) = 2100
        .ColWidth(3) = 1400
        .ColWidth(4) = 2800
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "External Trigger Stretch [탎]"
        .TextMatrix(0, 2) = "Channel Trigger Stretch [탎]"
        .TextMatrix(0, 3) = "Veto Stretch [탎]"
        .TextMatrix(0, 4) = "Fast Trigger Backplane Length [탎]"
        
        For X = 0 To 4
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' External Trigger Stretch
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(ExtTrigStretch(X - 1), "0.000")
        Next X
        
        .Col = 2    ' Channel Trigger Stretch
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(ChanTrigStretch(X - 1), "0.000")
        Next X
        
        .Col = 3    ' Veto Stretch
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(VetoStretch(X - 1), "0.000")
        Next X
        
        .Col = 4    ' Length of the Fast Trigger Transmitted on the BackPlane
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(FastTrigBackPlaneLen(X - 1), "0.000")
        Next X
        
        bDoNotEditTL = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditTrigLen.Font
        .Name = TriggerLengthsGrid.Font.Name
        .Size = TriggerLengthsGrid.Font.Size
        .Weight = TriggerLengthsGrid.Font.Weight
    End With
    txtEditTrigLen.BackColor = vb3DLight
   
    txtEditTrigLen = ""
    bDoNotEditTL = False


    '###########################################
    '
    '   FIFO Delays tab
    '
    '###########################################
    
    With FIFODelaysGrid
        .Redraw = False
        bDoNotEditFIFO = True
        
        .Cols = 3
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 2000
        .ColWidth(2) = 2600
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "External Delay Length [탎]"
        .TextMatrix(0, 2) = "Fast Trigger Backplane Delay [탎]"
        
        For X = 0 To 2
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' External Delay Length
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(ExtDelayLength(X - 1), "0.000")
        Next X
        
        .Col = 2    ' Delay of Fast Trigger transmitted on the Backplane
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(FastTrigBackplaneDelay(X - 1), "0.000")
        Next X
        
        bDoNotEditFIFO = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditFIFODelays.Font
        .Name = FIFODelaysGrid.Font.Name
        .Size = FIFODelaysGrid.Font.Size
        .Weight = FIFODelaysGrid.Font.Weight
    End With
    txtEditFIFODelays.BackColor = vb3DLight
   
    txtEditFIFODelays = ""
    bDoNotEditFIFO = False


    '###########################################
    '
    '   Multiplicity tab
    '
    '###########################################
    
    With MultiplicityGrid
        .Redraw = False
        bDoNotEditMP = True
        
        .Cols = 3
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 665
        .ColWidth(1) = 1675
        .ColWidth(2) = 1680
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "Multiplicity Mask Low"
        .TextMatrix(0, 2) = "Multiplicity Mask High"
        
        For X = 0 To 2
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' Multiplicity Mask Low
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = ToHex(MultiplicityMaskL(X - 1))
        Next X
        
        .Col = 2    ' Multiplicity Mask High
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = ToHex(MultiplicityMaskH(X - 1))
        Next X
        
        bDoNotEditMP = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditMultiplicity.Font
        .Name = MultiplicityGrid.Font.Name
        .Size = MultiplicityGrid.Font.Size
        .Weight = MultiplicityGrid.Font.Weight
    End With
    txtEditMultiplicity.BackColor = vb3DLight
   
    txtEditMultiplicity = ""
    bDoNotEditMP = False

    With TrigConfigGrid
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
        
        .Col = 1    ' Trigger configuration register #0
        For X = 1 To Number_Modules_Total
            .Row = X
            If TrigConfig0(X - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig0(X - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig0(X - 1)))
            End If
        Next X
        
        .Col = 2    ' Trigger configuration register #1
        For X = 1 To Number_Modules_Total
            .Row = X
            If TrigConfig1(X - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig1(X - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig1(X - 1)))
            End If
        Next X
        
        .Col = 3    ' Trigger configuration register #2
        For X = 1 To Number_Modules_Total
            .Row = X
            If TrigConfig2(X - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig2(X - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig2(X - 1)))
            End If
        Next X
       
        .Col = 4    ' Trigger configuration register #3
        For X = 1 To Number_Modules_Total
            .Row = X
            If TrigConfig3(X - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig3(X - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig3(X - 1)))
            End If
        Next X
       
       bDoNotEditTC = False
        .Redraw = True
    End With
    
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditTrigConfig.Font
        .Name = TrigConfigGrid.Font.Name
        .Size = TrigConfigGrid.Font.Size
        .Weight = TrigConfigGrid.Font.Weight
    End With
    txtEditTrigConfig.BackColor = vb3DLight
   
    txtEditTrigConfig = ""
    bDoNotEditTC = False

    With FTNNEnaGrid
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
        
        .Col = 1    ' Fast trigger nearest neighbor enable
        For X = 1 To Number_Modules_Total
            .Row = X
            If FTNNEna(X - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(FTNNEna(X - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(FTNNEna(X - 1)))
            End If
        Next X

        bDoNotEditFTNN = False
        .Redraw = True
    End With
    
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditFTNNEna.Font
        .Name = FTNNEnaGrid.Font.Name
        .Size = FTNNEnaGrid.Font.Size
        .Weight = FTNNEnaGrid.Font.Weight
    End With
    txtEditFTNNEna.BackColor = vb3DLight
   
    txtEditFTNNEna = ""
    bDoNotEditFTNN = False


    '###########################################
    '
    '   QDC tab
    '
    '###########################################
    
    With QDCGrid
        .Redraw = False
        bDoNotEditQDC = True
        
        .Cols = 9
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 635
        .ColWidth(1) = 1095
        .ColWidth(2) = 1095
        .ColWidth(3) = 1095
        .ColWidth(4) = 1095
        .ColWidth(5) = 1095
        .ColWidth(6) = 1095
        .ColWidth(7) = 1095
        .ColWidth(8) = 1095
        
        .RowHeight(0) = 500
        For X = 1 To NUM_CHANNELS
            .RowHeight(X) = 350
        Next X
        
        .TextMatrix(0, 0) = "Chan #"
        .TextMatrix(0, 1) = "QDCLen0 [탎]"
        .TextMatrix(0, 2) = "QDCLen1 [탎]"
        .TextMatrix(0, 3) = "QDCLen2 [탎]"
        .TextMatrix(0, 4) = "QDCLen3 [탎]"
        .TextMatrix(0, 5) = "QDCLen4 [탎]"
        .TextMatrix(0, 6) = "QDCLen5 [탎]"
        .TextMatrix(0, 7) = "QDCLen6 [탎]"
        .TextMatrix(0, 8) = "QDCLen7 [탎]"
        
        For X = 0 To 8
            For Y = 0 To NUM_CHANNELS
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
    
        ' Initialize Chan #
        For X = 1 To NUM_CHANNELS
            .Row = X
            .Col = 0
            .TextMatrix(.Row, .Col) = X - 1
        Next X
        
        'Load parameter values
        
        .Col = 1    ' QDC Length 0
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen0(X - 1), "0.000")
        Next X
        
        .Col = 2    ' QDC Length 1
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen1(X - 1), "0.000")
        Next X
        
        .Col = 3    ' QDC Length 2
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen2(X - 1), "0.000")
        Next X
        
        .Col = 4    ' QDC Length 3
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen3(X - 1), "0.000")
        Next X
        
        .Col = 5    ' QDC Length 4
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen4(X - 1), "0.000")
        Next X
        
        .Col = 6    ' QDC Length 5
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen5(X - 1), "0.000")
        Next X
        
        .Col = 7    ' QDC Length 6
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen6(X - 1), "0.000")
        Next X
        
        .Col = 8    ' QDC Length 7
        For X = 1 To NUM_CHANNELS
            .Row = X
            .TextMatrix(.Row, .Col) = Format(QDCLen7(X - 1), "0.000")
        Next X
        
        bDoNotEditQDC = False
        .Redraw = True
    End With
      
    '
    ' Set the grid and textbox
    ' to the same font.
    '
    With txtEditQDC.Font
        .Name = QDCGrid.Font.Name
        .Size = QDCGrid.Font.Size
        .Weight = QDCGrid.Font.Weight
    End With
    txtEditQDC.BackColor = vb3DLight
   
    txtEditQDC = ""
    bDoNotEditQDC = False

    Exit Sub

ErrMSGOnConversion:

    ErrTxt = "While loading DAQParametersSetup form, failed to convert ChanCSRA"
    Msg = MsgBox(ErrTxt, 0, "Error")
 
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
    
    ' Energy Filter
    If txtEditEF.Visible Then
        EnergyFilterGrid.Text = txtEditEF.Text
        txtEditEF.Visible = False
    End If
    
    ' Trigger Filter
    If txtEditTF.Visible Then
        TriggerFilterGrid.Text = txtEditTF.Text
        txtEditTF.Visible = False
    End If
    
    ' Analog Signal Conditioning
    If txtEditASC.Visible Then
        AnalogGrid.Text = txtEditASC.Text
        txtEditASC.Visible = False
    End If
   
    ' Histogram Control
    If txtEditHC.Visible Then
        HistogramGrid.Text = txtEditHC.Text
        txtEditHC.Visible = False
    End If
    
    ' Decay Time
    If txtEditTau.Visible Then
        DecayTimeGrid.Text = txtEditTau.Text
        txtEditTau.Visible = False
    End If
    
    ' Pulse Shape Analysis
    If txtEditPSA.Visible Then
        PSAGrid.Text = txtEditPSA.Text
        txtEditPSA.Visible = False
    End If
   
    ' Baseline
    If txtEditBaseline.Visible Then
        BaselineGrid.Text = txtEditBaseline.Text
        txtEditBaseline.Visible = False
    End If
        
    ' CFD trigger
    If txtEditCFDTrigger.Visible Then
        CFDTriggerGrid.Text = txtEditCFDTrigger.Text
        txtEditCFDTrigger.Visible = False
    End If
        
    ' Trigger Lengths
    If txtEditTrigLen.Visible Then
        TriggerLengthsGrid.Text = txtEditTrigLen.Text
        txtEditTrigLen.Visible = False
    End If

    ' FIFO Delays
    If txtEditFIFODelays.Visible Then
        FIFODelaysGrid.Text = txtEditFIFODelays.Text
        txtEditFIFODelays.Visible = False
    End If

    ' Multiplicity
    If txtEditMultiplicity.Visible Then
        MultiplicityGrid.Text = txtEditMultiplicity.Text
        txtEditMultiplicity.Visible = False
    End If
    
    If txtEditTrigConfig.Visible Then
        TrigConfigGrid.Text = txtEditTrigConfig.Text
        txtEditTrigConfig.Visible = False
    End If
    
    If txtEditFTNNEna.Visible Then
        FTNNEnaGrid.Text = txtEditFTNNEna.Text
        txtEditFTNNEna.Visible = False
    End If
    
    ' QDC
    If txtEditQDC.Visible Then
        QDCGrid.Text = txtEditQDC.Text
        txtEditQDC.Visible = False
    End If

End Sub



'###############################################################
'
'   Filter tab - Energy Filter
'       Subroutines that are used to set energy filter parameters
'
'###############################################################

Private Sub EnergyFilterGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartEF Then Exit Sub
    Call pEditEnergyFilterGrid(32)
End Sub

Private Sub pEditEnergyFilterGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditEF
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = EnergyFilterGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With EnergyFilterGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditEF.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditEF.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditEF.Visible = True
    txtEditEF.SetFocus
End Sub

Private Sub EnergyFilterGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditEF Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With EnergyFilterGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Energy filter rise time
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> EnergyFilterRiseTime(.Row - 1) Then
                        EnergyFilterRiseTime(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("ENERGY_RISETIME", EnergyFilterRiseTime(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set ENERGY_RISETIME in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read parameters back from module (only those possibly changed)
                        
                        retval = Pixie16ReadSglChanPar("ENERGY_RISETIME", EnergyFilterRiseTime(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ENERGY_RISETIME from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 1) = Format(EnergyFilterRiseTime(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("ENERGY_FLATTOP", EnergyFilterFlatTop(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ENERGY_FLATTOP from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 2) = Format(EnergyFilterFlatTop(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BLCUT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        BaselineGrid.TextMatrix(.Row, 1) = BLcut(.Row - 1)
                        
                    End If
                ElseIf .Col = 2 Then    ' Energy filter flat top
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> EnergyFilterFlatTop(.Row - 1) Then
                        EnergyFilterFlatTop(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("ENERGY_FLATTOP", EnergyFilterFlatTop(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set ENERGY_FLATTOP in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read parameters back from module (only those possibly changed)
                        
                        retval = Pixie16ReadSglChanPar("ENERGY_FLATTOP", EnergyFilterFlatTop(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ENERGY_FLATTOP from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 2) = Format(EnergyFilterFlatTop(.Row - 1), "0.000")
                        
                       retval = Pixie16ReadSglChanPar("ENERGY_RISETIME", EnergyFilterRiseTime(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                       If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ENERGY_RISETIME from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 1) = Format(EnergyFilterRiseTime(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BLCUT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        BaselineGrid.TextMatrix(.Row, 1) = BLcut(.Row - 1)
                        
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With EnergyFilterGrid
        If .Col = 1 Then    ' Restore energy filter rise time
            .TextMatrix(.Row, .Col) = Format(EnergyFilterRiseTime(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore energy filter flat top
            .TextMatrix(.Row, .Col) = Format(EnergyFilterFlatTop(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub EnergyFilterGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditEnergyFilterGrid(KeyAscii)
End Sub

Private Sub EnergyFilterGrid_LeaveCell()
    
    If bDoNotEditEF Then Exit Sub
    Call EnergyFilterGrid_GotFocus
End Sub

Private Sub EnergyFilterGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With EnergyFilterGrid
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
        bOnFixedPartEF = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub ModCSRBAdvancedBits_Button_Click()
    ' Check if the form is minimized or maximized
    If AdvancedModCSRBBitsForm.WindowState <> vbNormal Then
        AdvancedModCSRBBitsForm.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, AdvancedModCSRBBitsForm
    AdvancedModCSRBBitsForm.Height = 5745
    AdvancedModCSRBBitsForm.Width = 10050
    
    AdvancedModCSRBBitsForm.ZOrder 0

End Sub

Private Sub txtEditEF_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With EnergyFilterGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditEF.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditEF = True
                    .Row = .Row - 1
                    bDoNotEditEF = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditEF = True
                    .Row = .Row + 1
                    bDoNotEditEF = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditEF_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   Filter tab - Trigger Filter
'      Subroutines that are used to set trigger filter parameters
'
'###############################################################

Private Sub TriggerFilterGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartTF Then Exit Sub
    Call pEditTriggerFilterGrid(32)
End Sub

Private Sub pEditTriggerFilterGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditTF
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = TriggerFilterGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With TriggerFilterGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditTF.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditTF.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditTF.Visible = True
    txtEditTF.SetFocus
End Sub

Private Sub TriggerFilterGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditTF Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With TriggerFilterGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Trigger filter rise time
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> TriggerFilterRiseTime(.Row - 1) Then
                        TriggerFilterRiseTime(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("TRIGGER_RISETIME", TriggerFilterRiseTime(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set TRIGGER_RISETIME in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read parameters back from module (only those possibly changed)
                        
                        retval = Pixie16ReadSglChanPar("TRIGGER_RISETIME", TriggerFilterRiseTime(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRIGGER_RISETIME from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 1) = Format(TriggerFilterRiseTime(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("TRIGGER_FLATTOP", TriggerFilterFlatTop(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRIGGER_FLATTOP from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 2) = Format(TriggerFilterFlatTop(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("TRIGGER_THRESHOLD", TriggerThreshold(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRIGGER_THRESHOLD from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, 3) = Format(TriggerThreshold(.Row - 1), "0.000")
                        
                    End If
                ElseIf .Col = 2 Then    ' Trigger filter flat top
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> TriggerFilterFlatTop(.Row - 1) Then
                        TriggerFilterFlatTop(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("TRIGGER_FLATTOP", TriggerFilterFlatTop(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set TRIGGER_FLATTOP in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        retval = Pixie16ReadSglChanPar("TRIGGER_FLATTOP", TriggerFilterFlatTop(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRIGGER_FLATTOP from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 2) = Format(TriggerFilterFlatTop(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("TRIGGER_RISETIME", TriggerFilterRiseTime(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRIGGER_RISETIME from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        .TextMatrix(.Row, 1) = Format(TriggerFilterRiseTime(.Row - 1), "0.000")
                        
                    End If
                ElseIf .Col = 3 Then    ' Trigger threshold
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> TriggerThreshold(.Row - 1) Then
                        TriggerThreshold(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("TRIGGER_THRESHOLD", TriggerThreshold(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set TRIGGER_THRESHOLD in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("TRIGGER_THRESHOLD", TriggerThreshold(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRIGGER_THRESHOLD from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(TriggerThreshold(.Row - 1), "0.000")
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With TriggerFilterGrid
        If .Col = 1 Then    ' Restore trigger filter rise time
            .TextMatrix(.Row, .Col) = Format(TriggerFilterRiseTime(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore trigger filter flat top
            .TextMatrix(.Row, .Col) = Format(TriggerFilterRiseTime(.Row - 1), "0.000")
        ElseIf .Col = 3 Then    ' Restore trigger threshold
            .TextMatrix(.Row, .Col) = Format(TriggerThreshold(.Row - 1), "0.000")
        End If
    End With

End Sub

Private Sub TriggerFilterGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditTriggerFilterGrid(KeyAscii)
End Sub

Private Sub TriggerFilterGrid_LeaveCell()
    
    If bDoNotEditTF Then Exit Sub
    Call TriggerFilterGrid_GotFocus
End Sub

Private Sub TriggerFilterGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With TriggerFilterGrid
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
        bOnFixedPartTF = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditTF_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With TriggerFilterGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditTF.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditTF = True
                    .Row = .Row - 1
                    bDoNotEditTF = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditTF = True
                    .Row = .Row + 1
                    bDoNotEditTF = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditTF_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   Analog Signal Conditioning tab
'      Subroutines that are used to set energy filter parameters
'
'###############################################################

Private Sub AnalogGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartASC Then Exit Sub
    If AnalogGrid.Col = 1 Then Exit Sub
    If AnalogGrid.Col = 3 Then Exit Sub
    Call pEditAnalogGrid(32)
End Sub

Private Sub pEditAnalogGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditASC
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = AnalogGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With AnalogGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditASC.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditASC.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditASC.Visible = True
    txtEditASC.SetFocus
End Sub

Private Sub AnalogGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditASC Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With AnalogGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 2 Then    ' DC-offset
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> DCoffset(.Row - 1) Then
                        DCoffset(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("VOFFSET", DCoffset(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set VOFFSET in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("VOFFSET", DCoffset(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read VOFFSET from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(DCoffset(.Row - 1), "0.000")
                    End If
                End If
            End If
            
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With AnalogGrid
        If .Col = 2 Then    ' Restore DC-offset
            .TextMatrix(.Row, .Col) = Format(DCoffset(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub AnalogGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    If AnalogGrid.Col = 1 Then Exit Sub
    If AnalogGrid.Col = 3 Then Exit Sub
    Call pEditAnalogGrid(KeyAscii)
End Sub

Private Sub AnalogGrid_LeaveCell()
    
    If bDoNotEditASC Then Exit Sub
    Call AnalogGrid_GotFocus
End Sub

Private Sub AnalogGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    Dim localChanCSRAlng As Long
    
    With AnalogGrid
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
        bOnFixedPartASC = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
                       
        If bOnFixedPartASC = False Then
                       
            ' See if we are on the gain column
            If .Col = 1 Then
                If ModRev(DAQParSetupModNumEdit.Value) = 15 Then
                    If CDbl(.TextMatrix(.Row, .Col)) = HIGH_GAIN_REVF Then
                        Vgain(.Row - 1) = 0
                        localChanCSRAlng = CLng(ChanCSRA(.Row - 1))
                        localChanCSRAlng = APP32_ClrBit(CCSRA_ENARELAY, localChanCSRAlng)
                        ChanCSRA(.Row - 1) = CDbl(localChanCSRAlng)
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        .TextMatrix(.Row, .Col) = Format(LOW_GAIN_REVF, "0.000")
                    Else
                        Vgain(.Row - 1) = 1
                        localChanCSRAlng = CLng(ChanCSRA(.Row - 1))
                        localChanCSRAlng = APP32_SetBit(CCSRA_ENARELAY, localChanCSRAlng)
                        ChanCSRA(.Row - 1) = CDbl(localChanCSRAlng)
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        .TextMatrix(.Row, .Col) = Format(HIGH_GAIN_REVF, "0.0")
                    End If
                Else
                    If CDbl(.TextMatrix(.Row, .Col)) = HIGH_GAIN_REVBCD Then
                        Vgain(.Row - 1) = 0
                        localChanCSRAlng = CLng(ChanCSRA(.Row - 1))
                        localChanCSRAlng = APP32_ClrBit(CCSRA_ENARELAY, localChanCSRAlng)
                        ChanCSRA(.Row - 1) = CDbl(localChanCSRAlng)
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        .TextMatrix(.Row, .Col) = Format(LOW_GAIN_REVBCD, "0.0")
                    Else
                        Vgain(.Row - 1) = 1
                        localChanCSRAlng = CLng(ChanCSRA(.Row - 1))
                        localChanCSRAlng = APP32_SetBit(CCSRA_ENARELAY, localChanCSRAlng)
                        ChanCSRA(.Row - 1) = CDbl(localChanCSRAlng)
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        .TextMatrix(.Row, .Col) = Format(HIGH_GAIN_REVBCD, "0.0")
                    End If
                End If
                
                retval = Pixie16ReadSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                If retval < 0 Then
                    StrTxt = "Pixie16ReadSglChanPar failed to read BLCUT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                    Msg = MsgBox(StrTxt, 0, "Error")
                    Exit Sub
                End If
                ' Update display
                BaselineGrid.TextMatrix(.Row, 1) = BLcut(.Row - 1)
                
            End If
            
            ' See if we are on the Polarity column
            If .Col = 3 Then
                If .TextMatrix(.Row, .Col) = "(+) Positive" Then
                    Polarity(.Row - 1) = 0
                    localChanCSRAlng = CLng(ChanCSRA(.Row - 1))
                    localChanCSRAlng = APP32_ClrBit(CCSRA_POLARITY, localChanCSRAlng)
                    ChanCSRA(.Row - 1) = CDbl(localChanCSRAlng)
                    
                    ' Write to module
                    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                    If retval < 0 Then
                        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                        Msg = MsgBox(StrTxt, 0, "Error")
                        Exit Sub
                    End If
                    
                    .TextMatrix(.Row, .Col) = "(-) Negative"
                Else
                    Polarity(.Row - 1) = 1
                    localChanCSRAlng = CLng(ChanCSRA(.Row - 1))
                    localChanCSRAlng = APP32_SetBit(CCSRA_POLARITY, localChanCSRAlng)
                    ChanCSRA(.Row - 1) = CDbl(localChanCSRAlng)
                    
                    ' Write to module
                    retval = Pixie16WriteSglChanPar("CHANNEL_CSRA", ChanCSRA(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                    If retval < 0 Then
                        StrTxt = "Pixie16WriteSglChanPar failed to set CHANNEL_CSRA in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                        Msg = MsgBox(StrTxt, 0, "Error")
                        Exit Sub
                    End If
                    
                    .TextMatrix(.Row, .Col) = "(+) Positive"
                End If
            End If
            
        End If
    End With
    
End Sub

Private Sub txtEditASC_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With AnalogGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditASC.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditASC = True
                    .Row = .Row - 1
                    bDoNotEditASC = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditASC = True
                    .Row = .Row + 1
                    bDoNotEditASC = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditASC_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 45, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub


'###############################################################
'
'   Histogram Control tab
'      Subroutines that are used to set energy binning parameters
'
'###############################################################

Private Sub HistogramGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartHC Then Exit Sub
    Call pEditHistogramGrid(32)
End Sub

Private Sub pEditHistogramGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditHC
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = HistogramGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With HistogramGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditHC.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditHC.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditHC.Visible = True
    txtEditHC.SetFocus
End Sub

Private Sub HistogramGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditHC Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With HistogramGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Energy low
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> EnergyLow(.Row - 1) Then
                        EnergyLow(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("EMIN", EnergyLow(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set EMIN in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("EMIN", EnergyLow(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read EMIN from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = EnergyLow(.Row - 1)
                    End If
                ElseIf .Col = 2 Then    ' Energy binning factor
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> EnergyBinFactor(.Row - 1) Then
                        EnergyBinFactor(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("BINFACTOR", EnergyBinFactor(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set BINFACTOR in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("BINFACTOR", EnergyBinFactor(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BINFACTOR from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = EnergyBinFactor(.Row - 1)
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With HistogramGrid
        If .Col = 1 Then    ' Restore energy low
            .TextMatrix(.Row, .Col) = EnergyLow(.Row - 1)
        ElseIf .Col = 2 Then    ' Restore energy binning factor
            .TextMatrix(.Row, .Col) = EnergyBinFactor(.Row - 1)
        End If
    End With
    
End Sub

Private Sub HistogramGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditHistogramGrid(KeyAscii)
End Sub

Private Sub HistogramGrid_LeaveCell()
    
    If bDoNotEditHC Then Exit Sub
    Call HistogramGrid_GotFocus
End Sub

Private Sub HistogramGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With HistogramGrid
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
        bOnFixedPartHC = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditHC_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With HistogramGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditHC.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditHC = True
                    .Row = .Row - 1
                    bDoNotEditHC = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditHC = True
                    .Row = .Row + 1
                    bDoNotEditHC = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditHC_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   Decay Time tab
'      Subroutines that are used to set decay time
'
'###############################################################

Private Sub DecayTimeGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartTAU Then Exit Sub
    Call pEditDecayTimeGrid(32)
End Sub

Private Sub pEditDecayTimeGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditTau
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = DecayTimeGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With DecayTimeGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditTau.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditTau.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditTau.Visible = True
    txtEditTau.SetFocus
End Sub

Private Sub DecayTimeGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditTAU Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With DecayTimeGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Preamp tau
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> PreampTau(.Row - 1) Then
                        PreampTau(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("TAU", PreampTau(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set TAU in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("TAU", PreampTau(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TAU from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(PreampTau(.Row - 1), "0.000")
                        
                        retval = Pixie16ReadSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BLCUT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        ' Update display
                        BaselineGrid.TextMatrix(.Row, 1) = BLcut(.Row - 1)
                         
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With DecayTimeGrid
        If .Col = 1 Then    ' Restore preamp tau
            .TextMatrix(.Row, .Col) = Format(PreampTau(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub DecayTimeGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditDecayTimeGrid(KeyAscii)
End Sub

Private Sub DecayTimeGrid_LeaveCell()
    
    If bDoNotEditTAU Then Exit Sub
    Call DecayTimeGrid_GotFocus
End Sub

Private Sub DecayTimeGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With DecayTimeGrid
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
        bOnFixedPartTAU = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditTau_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With DecayTimeGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditTau.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditTAU = True
                    .Row = .Row - 1
                    bDoNotEditTAU = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditTAU = True
                    .Row = .Row + 1
                    bDoNotEditTAU = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditTau_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   Pulse Shape Analysis tab
'      Subroutines that are used to set pulse shape analysis parameters
'
'###############################################################

Private Sub PSAGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartPSA Then Exit Sub
    Call pEditPSAGrid(32)
End Sub

Private Sub pEditPSAGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditPSA
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = PSAGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With PSAGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditPSA.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditPSA.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditPSA.Visible = True
    txtEditPSA.SetFocus
End Sub

Private Sub PSAGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditPSA Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With PSAGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Trace length
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> TraceLength(.Row - 1) Then
                        TraceLength(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("TRACE_LENGTH", TraceLength(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set TRACE_LENGTH in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("TRACE_LENGTH", TraceLength(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRACE_LENGTH from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(TraceLength(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 2 Then    ' Trace delay
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> TraceDelay(.Row - 1) Then
                        TraceDelay(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("TRACE_DELAY", TraceDelay(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set TRACE_DELAY in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("TRACE_DELAY", TraceDelay(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read TRACE_DELAY from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(TraceDelay(.Row - 1), "0.000")
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With PSAGrid
        If .Col = 1 Then    ' Restore trace length
            .TextMatrix(.Row, .Col) = Format(TraceLength(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore trace delay
            .TextMatrix(.Row, .Col) = Format(TraceDelay(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub PSAGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditPSAGrid(KeyAscii)
End Sub

Private Sub PSAGrid_LeaveCell()
    
    If bDoNotEditPSA Then Exit Sub
    Call PSAGrid_GotFocus
End Sub

Private Sub PSAGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With PSAGrid
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
        bOnFixedPartPSA = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditPSA_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With PSAGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditPSA.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditPSA = True
                    .Row = .Row - 1
                    bDoNotEditPSA = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditPSA = True
                    .Row = .Row + 1
                    bDoNotEditPSA = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditPSA_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   Baseline tab
'      Subroutines that are used to set baseline parameters
'
'###############################################################

Private Sub BaselineGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartBL Then Exit Sub
    Call pEditBaselineGrid(32)
End Sub

Private Sub pEditBaselineGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditBaseline
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = BaselineGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With BaselineGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditBaseline.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditBaseline.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditBaseline.Visible = True
    txtEditBaseline.SetFocus
End Sub

Private Sub BaselineGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditBL Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With BaselineGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Baseline cut
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> BLcut(.Row - 1) Then
                        BLcut(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set BLCUT in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BLCUT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = BLcut(.Row - 1)
                    End If
                ElseIf .Col = 2 Then    ' Baseline percent
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> BaselinePercent(.Row - 1) Then
                        BaselinePercent(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("BASELINE_PERCENT", BaselinePercent(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set BASELINE_PERCENT in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("BASELINE_PERCENT", BaselinePercent(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BASELINE_PERCENT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = BaselinePercent(.Row - 1)
                    End If
                ElseIf .Col = 3 Then    ' Baseline average
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> BaselineAverage(.Row - 1) Then
                        BaselineAverage(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("BASELINE_AVERAGE", BaselineAverage(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set BASELINE_AVERAGE in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("BASELINE_AVERAGE", BaselineAverage(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read BASELINE_AVERAGE from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = BaselineAverage(.Row - 1)
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With BaselineGrid
        If .Col = 1 Then    ' Restore baseline cut
            .TextMatrix(.Row, .Col) = BLcut(.Row - 1)
        ElseIf .Col = 2 Then    ' Restore baseline percent
            .TextMatrix(.Row, .Col) = BaselinePercent(.Row - 1)
        ElseIf .Col = 3 Then    ' Restore baseline average
            .TextMatrix(.Row, .Col) = BaselineAverage(.Row - 1)
        End If
    End With
    
End Sub

Private Sub BaselineGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditBaselineGrid(KeyAscii)
End Sub

Private Sub BaselineGrid_LeaveCell()
    
    If bDoNotEditBL Then Exit Sub
    Call BaselineGrid_GotFocus
End Sub

Private Sub BaselineGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With BaselineGrid
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
        bOnFixedPartBL = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditBaseline_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With BaselineGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditBaseline.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditBL = True
                    .Row = .Row - 1
                    bDoNotEditBL = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditBL = True
                    .Row = .Row + 1
                    bDoNotEditBL = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditBaseline_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub


'###############################################################
'
'   CFD trigger tab
'      Subroutines that are used to set CFD trigger parameters
'
'###############################################################

Private Sub CFDTriggerGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartCFD Then Exit Sub
    Call pEditCFDTriggerGrid(32)
End Sub

Private Sub pEditCFDTriggerGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditCFDTrigger
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = CFDTriggerGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With CFDTriggerGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditCFDTrigger.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditCFDTrigger.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditCFDTrigger.Visible = True
    txtEditCFDTrigger.SetFocus
End Sub

Private Sub CFDTriggerGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bDoNotEditCFD Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With CFDTriggerGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' CFD Delay
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> CFDDelay(.Row - 1) Then
                        CFDDelay(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CFDDelay", CFDDelay(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CFDDelay in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("CFDDelay", CFDDelay(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read CFDDelay from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(CFDDelay(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 2 Then    ' CFD Scale
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> CFDScale(.Row - 1) Then
                        CFDScale(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CFDScale", CFDScale(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CFDScale in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("CFDScale", CFDScale(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read CFDScale from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = CFDScale(.Row - 1)
                    End If
                ElseIf .Col = 3 Then    ' CFD Threshold
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> CFDThresh(.Row - 1) Then
                        CFDThresh(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("CFDThresh", CFDThresh(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set CFDThresh in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("CFDThresh", CFDThresh(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read CFDThresh from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = CFDThresh(.Row - 1)
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With CFDTriggerGrid
        If .Col = 1 Then    ' Restore CFD Delay
            .TextMatrix(.Row, .Col) = Format(CFDDelay(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore CFD Scale
            .TextMatrix(.Row, .Col) = CFDScale(.Row - 1)
        ElseIf .Col = 3 Then    ' Restore CFD Threshold
            .TextMatrix(.Row, .Col) = CFDThresh(.Row - 1)
        End If
    End With
    
End Sub

Private Sub CFDTriggerGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditCFDTriggerGrid(KeyAscii)
End Sub

Private Sub CFDTriggerGrid_LeaveCell()
    
    If bDoNotEditCFD Then Exit Sub
    Call CFDTriggerGrid_GotFocus
End Sub

Private Sub CFDTriggerGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With CFDTriggerGrid
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
        bOnFixedPartCFD = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditCFDTrigger_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With CFDTriggerGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditCFDTrigger.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditCFD = True
                    .Row = .Row - 1
                    bDoNotEditCFD = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditCFD = True
                    .Row = .Row + 1
                    bDoNotEditCFD = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditCFDTrigger_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub


'###############################################################
'
'   Trigger Stretch Lengths tab
'      Subroutines that are used to set Trigger Stretch Lengths parameters
'
'###############################################################

Private Sub TriggerLengthsGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartTL Then Exit Sub
    Call pEditTriggerLengthsGrid(32)
End Sub

Private Sub pEditTriggerLengthsGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditTrigLen
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = TriggerLengthsGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With TriggerLengthsGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditTrigLen.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditTrigLen.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditTrigLen.Visible = True
    txtEditTrigLen.SetFocus
End Sub

Private Sub TriggerLengthsGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bOnFixedPartTL Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With TriggerLengthsGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' External Trigger Stretch
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> ExtTrigStretch(.Row - 1) Then
                        ExtTrigStretch(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("ExtTrigStretch", ExtTrigStretch(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set ExtTrigStretch in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("ExtTrigStretch", ExtTrigStretch(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ExtTrigStretch from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(ExtTrigStretch(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 2 Then    ' Channel Trigger Stretch
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> ChanTrigStretch(.Row - 1) Then
                        ChanTrigStretch(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("ChanTrigStretch", ChanTrigStretch(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set ChanTrigStretch in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("ChanTrigStretch", ChanTrigStretch(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ChanTrigStretch from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(ChanTrigStretch(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 3 Then    ' Veto Stretch
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> VetoStretch(.Row - 1) Then
                        VetoStretch(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("VetoStretch", VetoStretch(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set VetoStretch in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("VetoStretch", VetoStretch(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read VetoStretch from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(VetoStretch(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 4 Then    ' Length of the Fast Trigger Transmitted on the BackPlane
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> FastTrigBackPlaneLen(.Row - 1) Then
                        FastTrigBackPlaneLen(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("FASTTRIGBACKLEN", FastTrigBackPlaneLen(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set FASTTRIGBACKLEN in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("FASTTRIGBACKLEN", FastTrigBackPlaneLen(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read FASTTRIGBACKLEN from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(FastTrigBackPlaneLen(.Row - 1), "0.000")
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With TriggerLengthsGrid
        If .Col = 1 Then    ' Restore External Trigger Stretch
            .TextMatrix(.Row, .Col) = Format(ExtTrigStretch(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore Channel Trigger Stretch
            .TextMatrix(.Row, .Col) = Format(ChanTrigStretch(.Row - 1), "0.000")
        ElseIf .Col = 3 Then    ' Restore Veto Stretch
            .TextMatrix(.Row, .Col) = Format(VetoStretch(.Row - 1), "0.000")
        ElseIf .Col = 4 Then    ' Restore Length of the Fast Trigger Transmitted on the BackPlane
            .TextMatrix(.Row, .Col) = Format(FastTrigBackPlaneLen(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub TriggerLengthsGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditTriggerLengthsGrid(KeyAscii)
End Sub

Private Sub TriggerLengthsGrid_LeaveCell()
    
    If bDoNotEditTL Then Exit Sub
    Call TriggerLengthsGrid_GotFocus
End Sub

Private Sub TriggerLengthsGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With TriggerLengthsGrid
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
        bOnFixedPartTL = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditTrigLen_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With TriggerLengthsGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditTrigLen.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditTL = True
                    .Row = .Row - 1
                    bDoNotEditTL = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditTL = True
                    .Row = .Row + 1
                    bDoNotEditTL = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditTrigLen_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   FIFO Delays tab
'      Subroutines that are used to set FIFO Delays parameters
'
'###############################################################

Private Sub FIFODelaysGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartFIFO Then Exit Sub
    Call pEditFIFODelaysGrid(32)
End Sub

Private Sub pEditFIFODelaysGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditFIFODelays
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = FIFODelaysGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With FIFODelaysGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditFIFODelays.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditFIFODelays.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditFIFODelays.Visible = True
    txtEditFIFODelays.SetFocus
End Sub

Private Sub FIFODelaysGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bOnFixedPartFIFO Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With FIFODelaysGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' External Delay Length
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> ExtDelayLength(.Row - 1) Then
                        ExtDelayLength(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("ExternDelayLen", ExtDelayLength(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set ExternDelayLen in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("ExternDelayLen", ExtDelayLength(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read ExternDelayLen from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(ExtDelayLength(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 2 Then    ' Delay of Fast Trigger transmitted on the Backplane
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> FastTrigBackplaneDelay(.Row - 1) Then
                        FastTrigBackplaneDelay(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("FtrigoutDelay", FastTrigBackplaneDelay(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set FtrigoutDelay in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("FtrigoutDelay", FastTrigBackplaneDelay(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read FtrigoutDelay from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(FastTrigBackplaneDelay(.Row - 1), "0.000")
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With FIFODelaysGrid
        If .Col = 1 Then    ' Restore External Delay Length
            .TextMatrix(.Row, .Col) = Format(ExtDelayLength(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore Delay of Fast Trigger transmitted on the Backplane
            .TextMatrix(.Row, .Col) = Format(FastTrigBackplaneDelay(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub FIFODelaysGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditFIFODelaysGrid(KeyAscii)
End Sub

Private Sub FIFODelaysGrid_LeaveCell()
    
    If bDoNotEditFIFO Then Exit Sub
    Call FIFODelaysGrid_GotFocus
End Sub

Private Sub FIFODelaysGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With FIFODelaysGrid
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
        bOnFixedPartFIFO = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditFIFODelays_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With FIFODelaysGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditFIFODelays.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditFIFO = True
                    .Row = .Row - 1
                    bDoNotEditFIFO = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditFIFO = True
                    .Row = .Row + 1
                    bDoNotEditFIFO = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditFIFODelays_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub



'###############################################################
'
'   Multiplicity tab
'      Subroutines that are used to set Multiplicity parameters
'
'###############################################################

Private Sub MultiplicityGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartMP Then Exit Sub
    Call pEditMultiplicityGrid(32)
End Sub

Private Sub pEditMultiplicityGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditMultiplicity
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = MultiplicityGrid
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
    With MultiplicityGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditMultiplicity.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditMultiplicity.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditMultiplicity.Visible = True
    txtEditMultiplicity.SetFocus
End Sub

Private Sub MultiplicityGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bOnFixedPartMP Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With MultiplicityGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Multiplicity Mask Low
                    ' Check if it is a new value
                    If FromHex(.TextMatrix(.Row, .Col)) <> MultiplicityMaskL(.Row - 1) Then
                        MultiplicityMaskL(.Row - 1) = FromHex(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("MultiplicityMaskL", MultiplicityMaskL(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set MultiplicityMaskL in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("MultiplicityMaskL", MultiplicityMaskL(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read MultiplicityMaskL from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = ToHex(MultiplicityMaskL(.Row - 1))
                    End If
                ElseIf .Col = 2 Then    ' Multiplicity Mask High
                    ' Check if it is a new value
                    If FromHex(.TextMatrix(.Row, .Col)) <> MultiplicityMaskH(.Row - 1) Then
                        MultiplicityMaskH(.Row - 1) = FromHex(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("MultiplicityMaskH", MultiplicityMaskH(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set MultiplicityMaskH in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("MultiplicityMaskH", MultiplicityMaskH(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read MultiplicityMaskH from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = ToHex(MultiplicityMaskH(.Row - 1))
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With MultiplicityGrid
        If .Col = 1 Then    ' Restore Multiplicity Mask Low
            .TextMatrix(.Row, .Col) = ToHex(MultiplicityMaskL(.Row - 1))
        ElseIf .Col = 2 Then    ' Restore Multiplicity Mask High
            .TextMatrix(.Row, .Col) = ToHex(MultiplicityMaskH(.Row - 1))
        End If
    End With
    
End Sub

Private Sub MultiplicityGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditMultiplicityGrid(KeyAscii)
End Sub

Private Sub MultiplicityGrid_LeaveCell()
    
    If bDoNotEditMP Then Exit Sub
    Call MultiplicityGrid_GotFocus
End Sub

Private Sub MultiplicityGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With MultiplicityGrid
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
        bOnFixedPartMP = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditMultiplicity_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With MultiplicityGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditMultiplicity.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditMP = True
                    .Row = .Row - 1
                    bDoNotEditMP = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditMP = True
                    .Row = .Row + 1
                    bDoNotEditMP = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditMultiplicity_KeyPress(KeyAscii As Integer)
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

Private Sub TrigConfigGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartTC Then Exit Sub
    Call pEditTrigConfigGrid(32)
End Sub

Private Sub pEditTrigConfigGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditTrigConfig
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = TrigConfigGrid
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
    With TrigConfigGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditTrigConfig.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditTrigConfig.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditTrigConfig.Visible = True
    txtEditTrigConfig.SetFocus
End Sub

Private Sub TrigConfigGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    Dim ConvertedNum As Double
    Dim NewNum As Long
    
    On Error GoTo RestoreValue
    
    If bOnFixedPartTC Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With TrigConfigGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Trigger configuration register #0
                    ' Check if it is a new value
                    ConvertedNum = FromHex(.TextMatrix(.Row, .Col))
                    If ConvertedNum > 2147483647 Then
                        NewNum = ConvertedNum - 4294967296#
                    Else
                        NewNum = ConvertedNum
                    End If
                    
                    If NewNum <> TrigConfig0(.Row - 1) Then
                        TrigConfig0(.Row - 1) = NewNum
                        
                        ' Write to module
                        retval = Pixie16WriteSglModPar("TrigConfig0", TrigConfig0(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglModPar failed to set TrigConfig0 in module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglModPar("TrigConfig0", TrigConfig0(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglModPar failed to read TrigConfig0 from module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        If TrigConfig0(.Row - 1) < 0 Then
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig0(.Row - 1)) + 4294967296#)
                        Else
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig0(.Row - 1)))
                        End If
                    End If
                ElseIf .Col = 2 Then    ' Trigger configuration register #1
                    ' Check if it is a new value
                    ConvertedNum = FromHex(.TextMatrix(.Row, .Col))
                    If ConvertedNum > 2147483647 Then
                        NewNum = ConvertedNum - 4294967296#
                    Else
                        NewNum = ConvertedNum
                    End If
                    
                    If NewNum <> TrigConfig1(.Row - 1) Then
                        TrigConfig1(.Row - 1) = NewNum
                        
                        ' Write to module
                        retval = Pixie16WriteSglModPar("TrigConfig1", TrigConfig1(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglModPar failed to set TrigConfig1 in module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglModPar("TrigConfig1", TrigConfig1(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglModPar failed to read TrigConfig1 from module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        If TrigConfig1(.Row - 1) < 0 Then
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig1(.Row - 1)) + 4294967296#)
                        Else
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig1(.Row - 1)))
                        End If
                    End If
                ElseIf .Col = 3 Then    ' Trigger configuration register #2
                    ' Check if it is a new value
                    ConvertedNum = FromHex(.TextMatrix(.Row, .Col))
                    If ConvertedNum > 2147483647 Then
                        NewNum = ConvertedNum - 4294967296#
                    Else
                        NewNum = ConvertedNum
                    End If
                    
                    If NewNum <> TrigConfig2(.Row - 1) Then
                        TrigConfig2(.Row - 1) = NewNum
                        
                        ' Write to module
                        retval = Pixie16WriteSglModPar("TrigConfig2", TrigConfig2(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglModPar failed to set TrigConfig2 in module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglModPar("TrigConfig2", TrigConfig2(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglModPar failed to read TrigConfig2 from module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        If TrigConfig2(.Row - 1) < 0 Then
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig2(.Row - 1)) + 4294967296#)
                        Else
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig2(.Row - 1)))
                        End If
                    End If
                ElseIf .Col = 4 Then    ' Trigger configuration register #3
                    ' Check if it is a new value
                    ConvertedNum = FromHex(.TextMatrix(.Row, .Col))
                    If ConvertedNum > 2147483647 Then
                        NewNum = ConvertedNum - 4294967296#
                    Else
                        NewNum = ConvertedNum
                    End If
                    
                    If NewNum <> TrigConfig3(.Row - 1) Then
                        TrigConfig3(.Row - 1) = NewNum
                        
                        ' Write to module
                        retval = Pixie16WriteSglModPar("TrigConfig3", TrigConfig3(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglModPar failed to set TrigConfig3 in module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglModPar("TrigConfig3", TrigConfig3(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglModPar failed to read TrigConfig3 from module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        If TrigConfig3(.Row - 1) < 0 Then
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig3(.Row - 1)) + 4294967296#)
                        Else
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig3(.Row - 1)))
                        End If
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With TrigConfigGrid
        If .Col = 1 Then    ' Restore trigger configuration register #0
            If TrigConfig0(.Row - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig0(.Row - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig0(.Row - 1)))
            End If
        ElseIf .Col = 2 Then    ' Restore trigger configuration register #1
            If TrigConfig1(.Row - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig1(.Row - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig1(.Row - 1)))
            End If
        ElseIf .Col = 3 Then    ' Restore trigger configuration register #2
            If TrigConfig2(.Row - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig2(.Row - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig2(.Row - 1)))
            End If
        ElseIf .Col = 4 Then    ' Restore trigger configuration register #3
            If TrigConfig3(.Row - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig3(.Row - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(TrigConfig3(.Row - 1)))
            End If
        End If
    End With
    
End Sub

Private Sub TrigConfigGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditTrigConfigGrid(KeyAscii)
End Sub

Private Sub TrigConfigGrid_LeaveCell()
    
    If bDoNotEditTC Then Exit Sub
    Call TrigConfigGrid_GotFocus
End Sub

Private Sub TrigConfigGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With TrigConfigGrid
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
        bOnFixedPartTC = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditTrigConfig_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With TrigConfigGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditTrigConfig.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditTC = True
                    .Row = .Row - 1
                    bDoNotEditTC = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditTC = True
                    .Row = .Row + 1
                    bDoNotEditTC = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditTrigConfig_KeyPress(KeyAscii As Integer)
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

Private Sub FTNNEnaGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartFTNN Then Exit Sub
    Call pEditFTNNEnaGrid(32)
End Sub

Private Sub pEditFTNNEnaGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditFTNNEna
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = FTNNEnaGrid
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
    With FTNNEnaGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditFTNNEna.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditFTNNEna.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditFTNNEna.Visible = True
    txtEditFTNNEna.SetFocus
End Sub

Private Sub FTNNEnaGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    Dim ConvertedNum As Double
    Dim NewNum As Long

    On Error GoTo RestoreValue
    
    If bOnFixedPartFTNN Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With FTNNEnaGrid

            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' Fast trigger nearest neighbor enable
                    ' Check if it is a new value
                    ConvertedNum = FromHex(.TextMatrix(.Row, .Col))
                    If ConvertedNum > 2147483647 Then
                        NewNum = ConvertedNum - 4294967296#
                    Else
                        NewNum = ConvertedNum
                    End If
                    
                    If NewNum <> FTNNEna(.Row - 1) Then
                        FTNNEna(.Row - 1) = NewNum
                        
                        ' Write to module
                        retval = Pixie16WriteSglModPar("FastTrigBackplaneEna", FTNNEna(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglModPar failed to set FastTrigBackplaneEna in module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglModPar("FastTrigBackplaneEna", FTNNEna(.Row - 1), .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglModPar failed to read FastTrigBackplaneEna from module # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        If FTNNEna(.Row - 1) < 0 Then
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(FTNNEna(.Row - 1)) + 4294967296#)
                        Else
                            .TextMatrix(.Row, .Col) = ToHex(CDbl(FTNNEna(.Row - 1)))
                        End If
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With FTNNEnaGrid
        If .Col = 1 Then    ' Restore fast trigger nearest neighbor enable
            If FTNNEna(.Row - 1) < 0 Then
                .TextMatrix(.Row, .Col) = ToHex(CDbl(FTNNEna(.Row - 1)) + 4294967296#)
            Else
                .TextMatrix(.Row, .Col) = ToHex(CDbl(FTNNEna(.Row - 1)))
            End If
        End If
    End With
    
End Sub

Private Sub FTNNEnaGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditFTNNEnaGrid(KeyAscii)
End Sub

Private Sub FTNNEnaGrid_LeaveCell()
    
    If bDoNotEditFTNN Then Exit Sub
    Call FTNNEnaGrid_GotFocus
End Sub

Private Sub FTNNEnaGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With FTNNEnaGrid
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
        bOnFixedPartFTNN = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditFTNNEna_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With FTNNEnaGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditFTNNEna.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditFTNN = True
                    .Row = .Row - 1
                    bDoNotEditFTNN = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditFTNN = True
                    .Row = .Row + 1
                    bDoNotEditFTNN = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditFTNNEna_KeyPress(KeyAscii As Integer)
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


'###############################################################
'
'   QDC tab
'      Subroutines that are used to set QDC parameters
'
'###############################################################

Private Sub QDCGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartQDC Then Exit Sub
    Call pEditQDCGrid(32)
End Sub

Private Sub pEditQDCGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditQDC
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = QDCGrid
                .SelStart = 0
                .SelLength = 1000
                
            Case 8, 46, 48 To 57
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
    With QDCGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditQDC.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditQDC.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditQDC.Visible = True
    txtEditQDC.SetFocus
End Sub

Private Sub QDCGrid_GotFocus()
    Dim retval As Long
    Dim StrTxt As String
    
    On Error GoTo RestoreValue
    
    If bOnFixedPartQDC Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    If SystemBooted = True Then
        With QDCGrid
        
            If .Row > 0 And .Col > 0 Then
                If .Col = 1 Then    ' QDCLen0
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen0(.Row - 1) Then
                        QDCLen0(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen0", QDCLen0(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen0 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen0", QDCLen0(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen0 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen0(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 2 Then    ' QDCLen1
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen1(.Row - 1) Then
                        QDCLen1(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen1", QDCLen1(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen1 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen1", QDCLen1(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen1 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen1(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 3 Then    ' QDCLen2
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen2(.Row - 1) Then
                        QDCLen2(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen2", QDCLen2(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen2 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen2", QDCLen2(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen2 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen2(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 4 Then    ' QDCLen3
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen3(.Row - 1) Then
                        QDCLen3(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen3", QDCLen3(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen3 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen3", QDCLen3(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen3 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen3(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 5 Then    ' QDCLen4
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen4(.Row - 1) Then
                        QDCLen4(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen4", QDCLen4(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen4 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen4", QDCLen4(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen4 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen4(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 6 Then    ' QDCLen5
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen5(.Row - 1) Then
                        QDCLen5(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen5", QDCLen5(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen5 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen5", QDCLen5(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen5 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen5(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 7 Then    ' QDCLen6
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen6(.Row - 1) Then
                        QDCLen6(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen6", QDCLen6(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen6 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen6", QDCLen6(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen6 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen6(.Row - 1), "0.000")
                    End If
                ElseIf .Col = 8 Then    ' QDCLen7
                    ' Check if it is a new value
                    If CDbl(.TextMatrix(.Row, .Col)) <> QDCLen7(.Row - 1) Then
                        QDCLen7(.Row - 1) = CDbl(.TextMatrix(.Row, .Col))
                        
                        ' Write to module
                        retval = Pixie16WriteSglChanPar("QDCLen7", QDCLen7(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16WriteSglChanPar failed to set QDCLen7 in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Read back from module
                        retval = Pixie16ReadSglChanPar("QDCLen7", QDCLen7(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                        If retval < 0 Then
                            StrTxt = "Pixie16ReadSglChanPar failed to read QDCLen7 from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Exit Sub
                        End If
                        
                        ' Update display
                        .TextMatrix(.Row, .Col) = Format(QDCLen7(.Row - 1), "0.000")
                    End If
                End If
            End If
        End With
    End If
    
    Exit Sub
    
RestoreValue:
    With QDCGrid
        If .Col = 1 Then    ' Restore QDCLen0
            .TextMatrix(.Row, .Col) = Format(QDCLen0(.Row - 1), "0.000")
        ElseIf .Col = 2 Then    ' Restore QDCLen1
            .TextMatrix(.Row, .Col) = Format(QDCLen1(.Row - 1), "0.000")
        ElseIf .Col = 3 Then    ' Restore QDCLen2
            .TextMatrix(.Row, .Col) = Format(QDCLen2(.Row - 1), "0.000")
        ElseIf .Col = 4 Then    ' Restore QDCLen3
            .TextMatrix(.Row, .Col) = Format(QDCLen3(.Row - 1), "0.000")
        ElseIf .Col = 5 Then    ' Restore QDCLen4
            .TextMatrix(.Row, .Col) = Format(QDCLen4(.Row - 1), "0.000")
        ElseIf .Col = 6 Then    ' Restore QDCLen5
            .TextMatrix(.Row, .Col) = Format(QDCLen5(.Row - 1), "0.000")
        ElseIf .Col = 7 Then    ' Restore QDCLen6
            .TextMatrix(.Row, .Col) = Format(QDCLen6(.Row - 1), "0.000")
        ElseIf .Col = 8 Then    ' Restore QDCLen7
            .TextMatrix(.Row, .Col) = Format(QDCLen7(.Row - 1), "0.000")
        End If
    End With
    
End Sub

Private Sub QDCGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditQDCGrid(KeyAscii)
End Sub

Private Sub QDCGrid_LeaveCell()
    
    If bDoNotEditQDC Then Exit Sub
    Call QDCGrid_GotFocus
End Sub

Private Sub QDCGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With QDCGrid
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
        bOnFixedPartQDC = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub txtEditQDC_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With QDCGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditQDC.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditQDC = True
                    .Row = .Row - 1
                    bDoNotEditQDC = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditQDC = True
                    .Row = .Row + 1
                    bDoNotEditQDC = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditQDC_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8, 46
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
End Sub


'###########################################
'   ModCSRB_Bits set/clear routines
'###########################################

Private Sub ModulePullUpsCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If ModulePullUpsCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_PULLUP, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_PULLUP, localModCSRB)
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

Private Sub DirectorModuleCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If DirectorModuleCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_DIRMOD, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_DIRMOD, localModCSRB)
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

Private Sub CrateMasterCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If CrateMasterCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_CHASSISMASTER, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_CHASSISMASTER, localModCSRB)
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

Private Sub EnableRunInhibitCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If EnableRunInhibitCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_INHIBITENA, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_INHIBITENA, localModCSRB)
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

Private Sub MultipleCratesCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim localModCSRB As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    localModCSRB = ModCSRB(ModuleNumber)
    
    If MultipleCratesCheck.Value = 1 Then
        localModCSRB = APP32_SetBit(MODCSRB_MULTCRATES, localModCSRB)
    Else
        localModCSRB = APP32_ClrBit(MODCSRB_MULTCRATES, localModCSRB)
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

'###########################################
'   ChanCSRA_Bits set/clear routines
'###########################################

Private Sub GoodCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If GoodCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_GOOD, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_GOOD, localChanCSRAlng)
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

Private Sub HistogrammingCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If HistogrammingCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_HISTOE, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_HISTOE, localChanCSRAlng)
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

Private Sub TraceCaptureCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If TraceCaptureCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_TRACEENA, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_TRACEENA, localChanCSRAlng)
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

Private Sub QDCCaptureCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If QDCCaptureCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_QDCENA, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_QDCENA, localChanCSRAlng)
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

Private Sub CFDTRigCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If CFDTRigCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_CFDMODE, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_CFDMODE, localChanCSRAlng)
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

Private Sub GlobalExternalTrigCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If GlobalExternalTrigCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_GLOBTRIG, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_GLOBTRIG, localChanCSRAlng)
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

Private Sub CaptureESumsCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If CaptureESumsCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_ESUMSENA, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_ESUMSENA, localChanCSRAlng)
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

Private Sub ChanTrigCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If ChanTrigCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_CHANTRIG, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_CHANTRIG, localChanCSRAlng)
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

Private Sub NormalPileupControlCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If NormalPileupControlCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_PILEUPCTRL, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_PILEUPCTRL, localChanCSRAlng)
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

Private Sub InversePileupControlCheck_Click()
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber, ChannelNumber As Integer
    Dim localChanCSRAlng As Long
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    ChannelNumber = CHANCSRAChanNumEdit.Value
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    If InversePileupControlCheck.Value = 1 Then
        localChanCSRAlng = APP32_SetBit(CCSRA_INVERSEPILEUP, localChanCSRAlng)
    Else
        localChanCSRAlng = APP32_ClrBit(CCSRA_INVERSEPILEUP, localChanCSRAlng)
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

'###########################################
'   SlowFilterRangeEdit_ValueChanged()
'       Set a new SlowFilterRange and update
'       all related parameters
'###########################################

Private Sub SlowFilterRangeEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer

    SlowFilterRangeEdit.BackColorText = &H8000000B
    SlowFilterRangeEdit.Enabled = False
    
    Pixie16MDIForm.OperationModeStatusBar.Panels(1).Text = "Adjusting energy filter range in progress ..."

    ModuleNumber = DAQParSetupModNumEdit.Value
    SlowFilterRange(ModuleNumber) = Value
    
    ' Write SlowFilterRange value to module
    retval = Pixie16WriteSglModPar("SLOW_FILTER_RANGE", SlowFilterRange(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglModPar failed to set SLOW_FILTER_RANGE in module # " & Str$(ModuleNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        SlowFilterRangeEdit.Enabled = True
        Exit Sub
    End If
    
    ' Update all relevant parameters
    retval = RetrieveChanParFromModule(ModuleNumber)
    If retval < 0 Then
        StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        SlowFilterRangeEdit.Enabled = True
        Exit Sub
    End If
    ' Update channel parameters displays
    retval = UpdateChanParCtrlsOnDAQForm()
    
    Pixie16MDIForm.OperationModeStatusBar.Panels(1).Text = " "
    
    SlowFilterRangeEdit.BackColorText = &H80000005
    SlowFilterRangeEdit.Enabled = True
End Sub


'###########################################
'   FastFilterRangeEdit_ValueChanged()
'       Set a new FastFilterRange and update
'       all related parameters
'###########################################

Private Sub FastFilterRangeEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    
    FastFilterRangeEdit.Enabled = False
    
    ModuleNumber = DAQParSetupModNumEdit.Value
    FastFilterRange(ModuleNumber) = Value

    ' Write FastFilterRange value to module
    retval = Pixie16WriteSglModPar("FAST_FILTER_RANGE", FastFilterRange(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16WriteSglModPar failed to set FAST_FILTER_RANGE in module # " & Str$(ModuleNumber)
        Msg = MsgBox(StrTxt, 0, "Error")
        FastFilterRangeEdit.Enabled = True
        Exit Sub
    End If
    
    ' Update all relevant parameters
    retval = RetrieveChanParFromModule(ModuleNumber)
    If retval < 0 Then
        StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        FastFilterRangeEdit.Enabled = True
        Exit Sub
    End If
    ' Update channel parameters displays
    retval = UpdateChanParCtrlsOnDAQForm()

    ' Update list mode trace interval
    LMTraceDT(ModuleNumber) = 2 ^ FastFilterRange(ModuleNumber) / SYSTEM_CLOCK_MHZ(ModuleNumber)

    FastFilterRangeEdit.Enabled = True
End Sub


'###########################################
'   DAQParSetupModNumEdit_ValueChanged()
'       Choose a new module number and update
'       all related parameters
'###########################################

Private Sub DAQParSetupModNumEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim retval As Long
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    
    DAQParSetupModNumEdit.Enabled = False
    
    ModuleNumber = Value

    ' Update all relevant parameters
    
    retval = RetrieveModParFromModule(ModuleNumber)
    If retval < 0 Then
        StrTxt = "RetrieveModParFromModule " & "(Module # " & Str$(ModuleNumber) & ") returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        DAQParSetupModNumEdit.Enabled = True
        Exit Sub
    End If
    
    retval = RetrieveChanParFromModule(ModuleNumber)
    If retval < 0 Then
        StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        DAQParSetupModNumEdit.Enabled = True
        Exit Sub
    End If
    
    ' Update module parameters displays for the currently selected module
    retval = UpdateModParCtrlsOnDAQForm(ModuleNumber)
    
    ' Update channel parameters displays
    retval = UpdateChanParCtrlsOnDAQForm()
    
    DAQParSetupModNumEdit.Enabled = True
End Sub


'###########################################
'   CHANCSRAChanNumEdit_ValueChanged()
'       Update all related ChanCSRA bits
'###########################################

Private Sub CHANCSRAChanNumEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim ChannelNumber As Integer
    Dim ModuleNumber As Integer
    Dim retval As Long
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim localChanCSRAlng As Long

    On Error GoTo ErrMSGOnConversion
    
    ChannelNumber = CHANCSRAChanNumEdit.Value
    ModuleNumber = DAQParSetupModNumEdit.Value

    retval = Pixie16ReadSglChanPar("CHANNEL_CSRA", ChanCSRA(ChannelNumber), ModuleNumber, ChannelNumber)
    If retval < 0 Then
        ErrTxt = "(Pixie16ReadSglChanPar) failed to read CHANNEL_CSRA. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        Exit Sub
    End If
    
    localChanCSRAlng = CLng(ChanCSRA(ChannelNumber))
    
    ' Update displays

    ' Good channel
    If APP32_TstBit(CCSRA_GOOD, localChanCSRAlng) = 1 Then
        GoodCheck.Value = 1
    Else
        GoodCheck.Value = 0
    End If
    GoodCheck.Refresh

    ' Histogram energy
    If APP32_TstBit(CCSRA_HISTOE, localChanCSRAlng) = 1 Then
        HistogrammingCheck.Value = 1
    Else
        HistogrammingCheck.Value = 0
    End If
    HistogrammingCheck.Refresh

    ' Capture trace
    If APP32_TstBit(CCSRA_TRACEENA, localChanCSRAlng) = 1 Then
        TraceCaptureCheck.Value = 1
    Else
        TraceCaptureCheck.Value = 0
    End If
    TraceCaptureCheck.Refresh

    ' Capture QDC sums
    If APP32_TstBit(CCSRA_QDCENA, localChanCSRAlng) = 1 Then
        QDCCaptureCheck.Value = 1
    Else
        QDCCaptureCheck.Value = 0
    End If
    QDCCaptureCheck.Refresh

    ' Enable CFD trigger
    If APP32_TstBit(CCSRA_CFDMODE, localChanCSRAlng) = 1 Then
        CFDTRigCheck.Value = 1
    Else
        CFDTRigCheck.Value = 0
    End If
    CFDTRigCheck.Refresh

    ' Require global external trigger for validation
    If APP32_TstBit(CCSRA_GLOBTRIG, localChanCSRAlng) = 1 Then
        GlobalExternalTrigCheck.Value = 1
    Else
        GlobalExternalTrigCheck.Value = 0
    End If
    GlobalExternalTrigCheck.Refresh

    ' Capture raw energy sums and baseline
    If APP32_TstBit(CCSRA_ESUMSENA, localChanCSRAlng) = 1 Then
        CaptureESumsCheck.Value = 1
    Else
        CaptureESumsCheck.Value = 0
    End If
    CaptureESumsCheck.Refresh

    ' Require channel external trigger for validation
    If APP32_TstBit(CCSRA_CHANTRIG, localChanCSRAlng) = 1 Then
        ChanTrigCheck.Value = 1
    Else
        ChanTrigCheck.Value = 0
    End If
    ChanTrigCheck.Refresh

    ' Normal pileup rejection
    If APP32_TstBit(CCSRA_PILEUPCTRL, localChanCSRAlng) = 1 Then
        NormalPileupControlCheck.Value = 1
    Else
        NormalPileupControlCheck.Value = 0
    End If
    NormalPileupControlCheck.Refresh

    ' Inverse pileup rejection
    If APP32_TstBit(CCSRA_INVERSEPILEUP, localChanCSRAlng) = 1 Then
        InversePileupControlCheck.Value = 1
    Else
        InversePileupControlCheck.Value = 0
    End If
    InversePileupControlCheck.Refresh

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
    
    Exit Sub
    
ErrMSGOnConversion:

    ErrTxt = "(CHANCSRAChanNumEdit_ValueChanged) failed to convert ChanCSRA"
    Msg = MsgBox(ErrTxt, 0, "Error")
    
End Sub


'###########################################
'   AutoFindTau_Click()
'       Find tau value for the selected channel
'###########################################

Private Sub AutoFindTau_Click()
    Dim retval As Long
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim ModNum As Integer
    
    ' Disable further button-pressing
    AutoFindTau.Enabled = False
    
    ModNum = DAQParSetupModNumEdit.Value
    retval = Pixie16TauFinder(ModNum, GetTauValues(0))
    If retval < 0 Then
        ErrTxt = "(Pixie16TauFinder) failed to find Tau value. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
        
        ' Enable button-pressing again
        AutoFindTau.Enabled = True
        
        Exit Sub
    End If
    
    ' Update the AutoTauFinderGrid
    With AutoTauFinderGrid
        .Redraw = False
        
        ' Put found Tau value in col 1
        For X = 1 To NUM_CHANNELS
            If GetTauValues(X - 1) = -1# Then
                .TextMatrix(X, 1) = ""
            Else
                .TextMatrix(X, 1) = Format(GetTauValues(X - 1), "0.000")
            End If
        Next X
    
        .Redraw = True
    End With
    
    ' Enable button-pressing again
    AutoFindTau.Enabled = True
    
    ' Enable AcceptTauValue since AutoFindTau is called
    AcceptTauValue.Enabled = True

End Sub


'###########################################
'   AcceptTauValue_Click()
'       Accept tau value for the selected channel
'       and download it to the module
'###########################################

Private Sub AcceptTauValue_Click()
    Dim retval As Long
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim count As Integer
    
    ' Disable further button-pressing
    AcceptTauValue.Enabled = False
    
    With DecayTimeGrid
        .Redraw = False

        For count = 0 To NUM_CHANNELS - 1
        
            If GetTauValues(count) <> -1# Then
            
                .Row = count + 1
                .Col = 1
                PreampTau(.Row - 1) = GetTauValues(count)
                            
                ' Write to module
                retval = Pixie16WriteSglChanPar("TAU", PreampTau(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                If retval < 0 Then
                    StrTxt = "Pixie16WriteSglChanPar failed to set TAU in module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                    Msg = MsgBox(StrTxt, 0, "Error")
                    
                    ' Enable button-pressing again
                    AcceptTauValue.Enabled = True
        
                    Exit Sub
                End If
                
                ' Read back from module
                retval = Pixie16ReadSglChanPar("TAU", PreampTau(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                If retval < 0 Then
                    StrTxt = "Pixie16ReadSglChanPar failed to read TAU from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                    Msg = MsgBox(StrTxt, 0, "Error")
                    
                    ' Enable button-pressing again
                    AcceptTauValue.Enabled = True
                    
                    Exit Sub
                End If
                
                ' Update display
                .TextMatrix(.Row, .Col) = Format(PreampTau(.Row - 1), "0.000")
                            
                retval = Pixie16ReadSglChanPar("BLCUT", BLcut(.Row - 1), DAQParSetupModNumEdit.Value, .Row - 1)
                If retval < 0 Then
                    StrTxt = "Pixie16ReadSglChanPar failed to read BLCUT from module # " & Str$(DAQParSetupModNumEdit.Value) & " channel # " & Str$(.Row - 1)
                    Msg = MsgBox(StrTxt, 0, "Error")
                    
                    ' Enable button-pressing again
                    AcceptTauValue.Enabled = True
                    
                    Exit Sub
                End If
                ' Update display
                BaselineGrid.TextMatrix(.Row, 1) = BLcut(.Row - 1)
                
            End If
        Next count
        
        .Redraw = True
    End With

    ' Enable button-pressing again
    AcceptTauValue.Enabled = True
End Sub
