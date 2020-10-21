VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.1#0"; "MSCOMCTL.OCX"
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.MDIForm Pixie16MDIForm 
   BackColor       =   &H00FFC0C0&
   Caption         =   "Pixie-16"
   ClientHeight    =   11835
   ClientLeft      =   165
   ClientTop       =   -660
   ClientWidth     =   13935
   Icon            =   "Pixie16MDIForm.frx":0000
   WindowState     =   2  'Maximized
   Begin VB.Timer DAQRunTimer 
      Left            =   5760
      Top             =   1560
   End
   Begin MSComDlg.CommonDialog dlgLoadDSPSettingsFile 
      Left            =   6360
      Top             =   1560
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.PictureBox Pixie16ControlBar 
      Align           =   3  'Align Left
      Height          =   11460
      Left            =   0
      ScaleHeight     =   11400
      ScaleWidth      =   3690
      TabIndex        =   5
      Top             =   0
      Width           =   3750
      Begin VB.CommandButton LoadLogFile 
         Caption         =   "Log"
         Height          =   495
         Left            =   960
         TabIndex        =   51
         Top             =   10560
         Width           =   1455
      End
      Begin TabDlg.SSTab SSTab1 
         Height          =   9660
         Left            =   30
         TabIndex        =   6
         Top             =   120
         Width           =   3645
         _ExtentX        =   6429
         _ExtentY        =   17039
         _Version        =   393216
         TabOrientation  =   1
         Tabs            =   4
         TabsPerRow      =   4
         TabHeight       =   520
         ForeColor       =   -2147483640
         TabCaption(0)   =   "Startup"
         TabPicture(0)   =   "Pixie16MDIForm.frx":0442
         Tab(0).ControlEnabled=   -1  'True
         Tab(0).Control(0)=   "NumPixie16ModulesLabel"
         Tab(0).Control(0).Enabled=   0   'False
         Tab(0).Control(1)=   "Label1"
         Tab(0).Control(1).Enabled=   0   'False
         Tab(0).Control(2)=   "TotalNumModulesEdit"
         Tab(0).Control(2).Enabled=   0   'False
         Tab(0).Control(3)=   "SetupPXISlotsGrid"
         Tab(0).Control(3).Enabled=   0   'False
         Tab(0).Control(4)=   "SelectConfigFiles"
         Tab(0).Control(4).Enabled=   0   'False
         Tab(0).Control(5)=   "BootPixie16Modules"
         Tab(0).Control(5).Enabled=   0   'False
         Tab(0).Control(6)=   "OfflineAnalysisCheckBox"
         Tab(0).Control(6).Enabled=   0   'False
         Tab(0).Control(7)=   "txtEditPXISlots"
         Tab(0).Control(7).Enabled=   0   'False
         Tab(0).ControlCount=   8
         TabCaption(1)   =   "Settings"
         TabPicture(1)   =   "Pixie16MDIForm.frx":045E
         Tab(1).ControlEnabled=   0   'False
         Tab(1).Control(0)=   "SetDAQParameters"
         Tab(1).Control(0).Enabled=   0   'False
         Tab(1).Control(1)=   "CopyDSPParameters"
         Tab(1).Control(1).Enabled=   0   'False
         Tab(1).Control(2)=   "Frame1"
         Tab(1).Control(2).Enabled=   0   'False
         Tab(1).Control(3)=   "SaveParameters"
         Tab(1).Control(3).Enabled=   0   'False
         Tab(1).Control(4)=   "LoadDSPParameters"
         Tab(1).Control(4).Enabled=   0   'False
         Tab(1).ControlCount=   5
         TabCaption(2)   =   "Run"
         TabPicture(2)   =   "Pixie16MDIForm.frx":047A
         Tab(2).ControlEnabled=   0   'False
         Tab(2).Control(0)=   "Frame2"
         Tab(2).Control(0).Enabled=   0   'False
         Tab(2).Control(1)=   "Frame3"
         Tab(2).Control(1).Enabled=   0   'False
         Tab(2).Control(2)=   "Frame4"
         Tab(2).Control(2).Enabled=   0   'False
         Tab(2).Control(3)=   "StartRun"
         Tab(2).Control(3).Enabled=   0   'False
         Tab(2).Control(4)=   "StopRun"
         Tab(2).Control(4).Enabled=   0   'False
         Tab(2).ControlCount=   5
         TabCaption(3)   =   "Results"
         TabPicture(3)   =   "Pixie16MDIForm.frx":0496
         Tab(3).ControlEnabled=   0   'False
         Tab(3).Control(0)=   "Label8"
         Tab(3).Control(0).Enabled=   0   'False
         Tab(3).Control(1)=   "Label9"
         Tab(3).Control(1).Enabled=   0   'False
         Tab(3).Control(2)=   "Label10"
         Tab(3).Control(2).Enabled=   0   'False
         Tab(3).Control(3)=   "StatisticsModNumEdit"
         Tab(3).Control(3).Enabled=   0   'False
         Tab(3).Control(4)=   "ProcessedEventsTextBox"
         Tab(3).Control(4).Enabled=   0   'False
         Tab(3).Control(5)=   "RealTimeTextBox"
         Tab(3).Control(5).Enabled=   0   'False
         Tab(3).Control(6)=   "SSTab2"
         Tab(3).Control(6).Enabled=   0   'False
         Tab(3).Control(7)=   "ShowMCASpectrumButton"
         Tab(3).Control(7).Enabled=   0   'False
         Tab(3).Control(8)=   "ShowListModeTracesButton"
         Tab(3).Control(8).Enabled=   0   'False
         Tab(3).ControlCount=   9
         Begin VB.CommandButton ShowListModeTracesButton 
            Caption         =   "Show List Mode Traces"
            Height          =   375
            Left            =   -74420
            TabIndex        =   54
            Top             =   8520
            Width           =   2415
         End
         Begin VB.CommandButton ShowMCASpectrumButton 
            Caption         =   "Show MCA Spectrum"
            Height          =   375
            Left            =   -74420
            TabIndex        =   53
            Top             =   8040
            Width           =   2415
         End
         Begin VB.TextBox txtEditPXISlots 
            Appearance      =   0  'Flat
            BackColor       =   &H80000000&
            BorderStyle     =   0  'None
            Height          =   495
            Left            =   1320
            TabIndex        =   52
            Top             =   3060
            Visible         =   0   'False
            Width           =   1455
         End
         Begin VB.CommandButton StopRun 
            Caption         =   "Stop"
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
            Left            =   -74180
            TabIndex        =   50
            Top             =   8100
            Width           =   1935
         End
         Begin VB.CommandButton StartRun 
            Caption         =   "Start"
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
            Left            =   -74180
            TabIndex        =   49
            Top             =   7140
            Width           =   1935
         End
         Begin TabDlg.SSTab SSTab2 
            Height          =   5925
            Left            =   -74780
            TabIndex        =   45
            Top             =   1800
            Width           =   3135
            _ExtentX        =   5530
            _ExtentY        =   10451
            _Version        =   393216
            TabOrientation  =   1
            TabHeight       =   520
            TabCaption(0)   =   "Live Time"
            TabPicture(0)   =   "Pixie16MDIForm.frx":04B2
            Tab(0).ControlEnabled=   -1  'True
            Tab(0).Control(0)=   "LiveTimeGrid"
            Tab(0).Control(0).Enabled=   0   'False
            Tab(0).ControlCount=   1
            TabCaption(1)   =   "ICR"
            TabPicture(1)   =   "Pixie16MDIForm.frx":04CE
            Tab(1).ControlEnabled=   0   'False
            Tab(1).Control(0)=   "ICRGrid"
            Tab(1).Control(0).Enabled=   0   'False
            Tab(1).ControlCount=   1
            TabCaption(2)   =   "OCR"
            Tab(2).ControlEnabled=   0   'False
            Tab(2).Control(0)=   "OCRGrid"
            Tab(2).ControlCount=   1
            Begin MSFlexGridLib.MSFlexGrid OCRGrid 
               Height          =   5205
               Left            =   -74775
               TabIndex        =   48
               Top             =   180
               Width           =   2700
               _ExtentX        =   4763
               _ExtentY        =   9181
               _Version        =   393216
            End
            Begin MSFlexGridLib.MSFlexGrid ICRGrid 
               Height          =   5205
               Left            =   -74775
               TabIndex        =   47
               Top             =   180
               Width           =   2700
               _ExtentX        =   4763
               _ExtentY        =   9181
               _Version        =   393216
            End
            Begin MSFlexGridLib.MSFlexGrid LiveTimeGrid 
               Height          =   5205
               Left            =   225
               TabIndex        =   46
               Top             =   180
               Width           =   2700
               _ExtentX        =   4763
               _ExtentY        =   9181
               _Version        =   393216
            End
         End
         Begin VB.TextBox RealTimeTextBox 
            Height          =   285
            Left            =   -73200
            TabIndex        =   43
            Text            =   "0"
            Top             =   1320
            Width           =   1455
         End
         Begin VB.TextBox ProcessedEventsTextBox 
            Height          =   285
            Left            =   -73200
            TabIndex        =   41
            Text            =   "0"
            Top             =   840
            Width           =   1455
         End
         Begin VB.Frame Frame4 
            Caption         =   "Run Control"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   3015
            Left            =   -74780
            TabIndex        =   30
            Top             =   300
            Width           =   3015
            Begin VB.CheckBox ResumeRunCheck 
               Caption         =   "Resume run?"
               Height          =   375
               Left            =   1560
               TabIndex        =   55
               Top             =   2520
               Width           =   1335
            End
            Begin CWUIControlsLib.CWNumEdit PresetRunTimeEdit 
               Height          =   375
               Left            =   1200
               TabIndex        =   33
               Top             =   840
               Width           =   1575
               _Version        =   393218
               _ExtentX        =   2778
               _ExtentY        =   661
               _StockProps     =   4
               BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
                  Name            =   "MS Sans Serif"
                  Size            =   8.25
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
               opts_1          =   393278
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
               RangeMax_Val_1  =   100
               Bindings_1      =   4
               ClassName_4     =   "CCWBindingHolderArray"
               Editor_4        =   5
               ClassName_5     =   "CCWBindingHolderArrayEditor"
               Owner_5         =   1
            End
            Begin VB.ComboBox RunTypeCombo 
               Height          =   315
               Left            =   800
               TabIndex        =   31
               Text            =   "Select run type"
               Top             =   360
               Width           =   2175
            End
            Begin CWUIControlsLib.CWNumEdit PollingIntervalEdit 
               Height          =   375
               Left            =   1200
               TabIndex        =   35
               Top             =   1440
               Width           =   1575
               _Version        =   393218
               _ExtentX        =   2778
               _ExtentY        =   661
               _StockProps     =   4
               BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
                  Name            =   "MS Sans Serif"
                  Size            =   8.25
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
               opts_1          =   62
               ButtonPosition_1=   1
               Mode_1          =   1
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
               RangeMax_Val_1  =   100
               Bindings_1      =   4
               ClassName_4     =   "CCWBindingHolderArray"
               Editor_4        =   5
               ClassName_5     =   "CCWBindingHolderArrayEditor"
               Owner_5         =   1
            End
            Begin CWUIControlsLib.CWNumEdit NumberOfFIFOFillsEdit 
               Height          =   375
               Left            =   1200
               TabIndex        =   37
               Top             =   2040
               Width           =   1575
               _Version        =   393218
               _ExtentX        =   2778
               _ExtentY        =   661
               _StockProps     =   4
               BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
                  Name            =   "MS Sans Serif"
                  Size            =   8.25
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
               opts_1          =   393278
               ButtonPosition_1=   1
               format_1        =   2
               ClassName_2     =   "CCWFormat"
               scale_1         =   3
               ClassName_3     =   "CCWScale"
               opts_3          =   65536
               dMax_3          =   10
               discInterval_3  =   1
               ValueVarType_1  =   5
               Value_Val_1     =   1
               IncValueVarType_1=   5
               IncValue_Val_1  =   1
               AccelIncVarType_1=   5
               AccelInc_Val_1  =   1
               RangeMinVarType_1=   5
               RangeMin_Val_1  =   1
               RangeMaxVarType_1=   5
               RangeMax_Val_1  =   65536
               Bindings_1      =   4
               ClassName_4     =   "CCWBindingHolderArray"
               Editor_4        =   5
               ClassName_5     =   "CCWBindingHolderArrayEditor"
               Owner_5         =   1
            End
            Begin VB.Label Label7 
               Caption         =   "Number of FIFO fills in Module 0"
               Height          =   615
               Left            =   240
               TabIndex        =   38
               Top             =   1920
               Width           =   855
            End
            Begin VB.Label Label6 
               Caption         =   "Run polling interval [s]"
               Height          =   495
               Left            =   240
               TabIndex        =   36
               Top             =   1440
               Width           =   855
            End
            Begin VB.Label Label5 
               Caption         =   "Run time / time out [s]"
               Height          =   495
               Left            =   240
               TabIndex        =   34
               Top             =   840
               Width           =   855
            End
            Begin VB.Label Label4 
               Caption         =   "Run type"
               Height          =   240
               Left            =   120
               TabIndex        =   32
               Top             =   375
               Width           =   855
            End
         End
         Begin VB.Frame Frame3 
            Caption         =   "Output File"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   1815
            Left            =   -74780
            TabIndex        =   24
            Top             =   4980
            Width           =   3015
            Begin VB.CheckBox AutoIncRunNumberCheckBox 
               Caption         =   "Auto increment run number"
               Height          =   255
               Left            =   360
               TabIndex        =   29
               Top             =   1440
               Width           =   2295
            End
            Begin VB.TextBox OutFileBaseNameText 
               Height          =   375
               Left            =   1080
               TabIndex        =   25
               Text            =   "Data"
               Top             =   360
               Width           =   1815
            End
            Begin CWUIControlsLib.CWNumEdit RunNumEdit 
               Height          =   405
               Left            =   1080
               TabIndex        =   27
               Top             =   840
               Width           =   1815
               _Version        =   393218
               _ExtentX        =   3201
               _ExtentY        =   714
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
               Value_Val_1     =   1
               IncValueVarType_1=   5
               IncValue_Val_1  =   1
               AccelIncVarType_1=   5
               AccelInc_Val_1  =   1
               RangeMinVarType_1=   5
               RangeMin_Val_1  =   1
               RangeMaxVarType_1=   5
               RangeMax_Val_1  =   9999
               Bindings_1      =   4
               ClassName_4     =   "CCWBindingHolderArray"
               Editor_4        =   5
               ClassName_5     =   "CCWBindingHolderArrayEditor"
               Owner_5         =   1
            End
            Begin VB.Label Label3 
               Caption         =   "Run number"
               Height          =   375
               Left            =   120
               TabIndex        =   28
               Top             =   910
               Width           =   975
            End
            Begin VB.Label Label2 
               Caption         =   "Base name"
               Height          =   375
               Left            =   120
               TabIndex        =   26
               Top             =   400
               Width           =   855
            End
         End
         Begin VB.Frame Frame2 
            Caption         =   "Synchronization"
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
            Left            =   -74780
            TabIndex        =   21
            Top             =   3540
            Width           =   3015
            Begin VB.CheckBox SynchClocksCheckBox 
               Caption         =   "Synchronize clocks in all modules?"
               Height          =   375
               Left            =   120
               TabIndex        =   23
               Top             =   750
               Width           =   2775
            End
            Begin VB.CheckBox SynchStartStopCheckBox 
               Caption         =   "Start/stop runs in all modules simultaneously?"
               Height          =   375
               Left            =   120
               TabIndex        =   22
               Top             =   300
               Width           =   2775
            End
         End
         Begin VB.CommandButton LoadDSPParameters 
            Caption         =   "Load Parameters"
            Height          =   615
            Left            =   -74300
            TabIndex        =   19
            Top             =   3060
            Width           =   2175
         End
         Begin VB.CommandButton SaveParameters 
            Caption         =   "Save Parameters"
            Height          =   615
            Left            =   -74300
            TabIndex        =   18
            Top             =   2340
            Width           =   2175
         End
         Begin VB.Frame Frame1 
            Caption         =   "Diagnostic Tools"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   1815
            Left            =   -74660
            TabIndex        =   16
            Top             =   4500
            Width           =   2895
            Begin VB.CommandButton AcquireBaselines 
               Caption         =   "Acquire baselines"
               Height          =   495
               Left            =   360
               TabIndex        =   20
               Top             =   1080
               Width           =   2175
            End
            Begin VB.CommandButton AcquireADCTraces 
               Caption         =   "Acquire ADC Traces"
               Height          =   495
               Left            =   360
               TabIndex        =   17
               Top             =   360
               Width           =   2175
            End
         End
         Begin VB.CommandButton CopyDSPParameters 
            Caption         =   "Copy Parameters"
            Height          =   615
            Left            =   -74300
            TabIndex        =   15
            Top             =   1620
            Width           =   2175
         End
         Begin VB.CommandButton SetDAQParameters 
            Caption         =   "Set DAQ Parameters"
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
            Left            =   -74315
            TabIndex        =   14
            Top             =   540
            Width           =   2175
         End
         Begin VB.CheckBox OfflineAnalysisCheckBox 
            Caption         =   "Offline Analysis"
            Height          =   375
            Left            =   1060
            TabIndex        =   13
            Top             =   7000
            Width           =   1455
         End
         Begin VB.CommandButton BootPixie16Modules 
            Caption         =   "Boot Pixie-16 Modules"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   0
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   855
            Left            =   655
            TabIndex        =   12
            Top             =   7500
            Width           =   2295
         End
         Begin VB.CommandButton SelectConfigFiles 
            Caption         =   "Select Configuration Files"
            Height          =   855
            Left            =   655
            TabIndex        =   11
            Top             =   5800
            Width           =   2295
         End
         Begin MSFlexGridLib.MSFlexGrid SetupPXISlotsGrid 
            Height          =   4300
            Left            =   75
            TabIndex        =   7
            Top             =   1190
            Width           =   3500
            _ExtentX        =   6165
            _ExtentY        =   7594
            _Version        =   393216
         End
         Begin CWUIControlsLib.CWNumEdit TotalNumModulesEdit 
            Height          =   405
            Left            =   2405
            TabIndex        =   8
            Top             =   300
            Width           =   735
            _Version        =   393218
            _ExtentX        =   1296
            _ExtentY        =   706
            _StockProps     =   4
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "MS Sans Serif"
               Size            =   11.86
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
            Value_Val_1     =   1
            IncValueVarType_1=   5
            IncValue_Val_1  =   1
            AccelIncVarType_1=   5
            AccelInc_Val_1  =   1
            RangeMinVarType_1=   5
            RangeMin_Val_1  =   1
            RangeMaxVarType_1=   5
            RangeMax_Val_1  =   24
            Bindings_1      =   4
            ClassName_4     =   "CCWBindingHolderArray"
            Editor_4        =   5
            ClassName_5     =   "CCWBindingHolderArrayEditor"
            Owner_5         =   1
         End
         Begin CWUIControlsLib.CWNumEdit StatisticsModNumEdit 
            Height          =   405
            Left            =   -73200
            TabIndex        =   39
            Top             =   240
            Width           =   735
            _Version        =   393218
            _ExtentX        =   1296
            _ExtentY        =   706
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
         Begin VB.Label Label10 
            Caption         =   "Real time [s]"
            Height          =   255
            Left            =   -74280
            TabIndex        =   44
            Top             =   1320
            Width           =   975
         End
         Begin VB.Label Label9 
            Caption         =   "Processed events"
            Height          =   255
            Left            =   -74640
            TabIndex        =   42
            Top             =   840
            Width           =   1455
         End
         Begin VB.Label Label8 
            Caption         =   "Module Number"
            Height          =   255
            Left            =   -74400
            TabIndex        =   40
            Top             =   360
            Width           =   1215
         End
         Begin VB.Label Label1 
            Caption         =   "Specifiy PXI Slot Numbers"
            Height          =   255
            Left            =   120
            TabIndex        =   10
            Top             =   950
            Width           =   2175
         End
         Begin VB.Label NumPixie16ModulesLabel 
            Caption         =   "Number of Pixie-16 Modules"
            Height          =   375
            Left            =   320
            TabIndex        =   9
            Top             =   420
            Width           =   2175
         End
      End
   End
   Begin VB.PictureBox Picture2 
      Align           =   2  'Align Bottom
      Height          =   375
      Left            =   0
      ScaleHeight     =   315
      ScaleWidth      =   13875
      TabIndex        =   0
      Top             =   11460
      Width           =   13935
      Begin MSComctlLib.StatusBar OperationModeStatusBar 
         Height          =   300
         Left            =   3840
         TabIndex        =   4
         Top             =   0
         Width           =   4175
         _ExtentX        =   7355
         _ExtentY        =   529
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   1
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   7232
               MinWidth        =   7232
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.ProgressBar Pixie16ProgressBar 
         Height          =   300
         Left            =   8000
         TabIndex        =   3
         Top             =   0
         Width           =   5775
         _ExtentX        =   10186
         _ExtentY        =   529
         _Version        =   393216
         Appearance      =   1
         Scrolling       =   1
      End
      Begin MSComctlLib.StatusBar Pixie16RunStatusBar 
         Height          =   300
         Left            =   2040
         TabIndex        =   2
         Top             =   0
         Width           =   1785
         _ExtentX        =   3149
         _ExtentY        =   529
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   1
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3528
               MinWidth        =   3528
               Text            =   "No run in progress"
               TextSave        =   "No run in progress"
            EndProperty
         EndProperty
      End
      Begin MSComctlLib.StatusBar Pixie16InitStatusBar 
         Height          =   300
         Left            =   0
         TabIndex        =   1
         Top             =   0
         Width           =   2055
         _ExtentX        =   3625
         _ExtentY        =   529
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   1
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
               Object.Width           =   3528
               MinWidth        =   3528
               Text            =   "Pixie-16 not initialized"
               TextSave        =   "Pixie-16 not initialized"
            EndProperty
         EndProperty
      End
   End
   Begin VB.Menu FilePath 
      Caption         =   "&File"
      Begin VB.Menu Exit 
         Caption         =   "&Exit"
         Shortcut        =   ^Q
      End
   End
   Begin VB.Menu RunResults 
      Caption         =   "&Run Results"
      Begin VB.Menu ListModeTraces 
         Caption         =   "List Mode Traces"
         Shortcut        =   ^L
      End
      Begin VB.Menu MCAHistogram 
         Caption         =   "MCA Histogram"
         Shortcut        =   ^H
      End
   End
   Begin VB.Menu windows 
      Caption         =   "&Windows"
      WindowList      =   -1  'True
   End
   Begin VB.Menu AvancedOptions 
      Caption         =   "&Advanced"
      Begin VB.Menu ShowDSPpar 
         Caption         =   "Show DSP par"
         Shortcut        =   ^D
      End
      Begin VB.Menu DispDSPMemBlock0 
         Caption         =   "Show DSP Memory Block 0"
         Shortcut        =   ^W
      End
      Begin VB.Menu DispDSPMemBlock1 
         Caption         =   "Show DSP Memory Block 1"
         Shortcut        =   ^O
      End
   End
   Begin VB.Menu About 
      Caption         =   "Abo&ut"
   End
End
Attribute VB_Name = "Pixie16MDIForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function QueryPerformanceFrequency Lib "kernel32" (lpFrequency As Currency) As Long
Private Declare Function QueryPerformanceCounter Lib "kernel32" (lpPerformanceCount As Currency) As Long

Dim bDoNotEditPXISlots    As Boolean
Dim bOnFixedPartPXISlots  As Boolean

'###########################################
'   MDIForm_Initialize()
'       Initialize the Pixie16 MDI form
'###########################################

Private Sub MDIForm_Initialize()
    Dim index As Integer
    Dim X As Integer
    Dim Y As Integer

    '*******************************************
    '   Load settings from VBSettingsFile
    '*******************************************
    
    StartUpDir = CurDir
    Call LoadVBSettingsOnStartUp

    '*******************************************
    '   Initialize status indicators
    '*******************************************
    
    Load StatusPics
    Pixie16MDIForm.Pixie16InitStatusBar.Panels(1).Picture = StatusPics.RedSquare
    Pixie16MDIForm.Pixie16InitStatusBar.Panels(1).Text = "Pixie-16 not initialized"
    Pixie16MDIForm.Pixie16RunStatusBar.Panels(1).Picture = StatusPics.RedSquare
    Pixie16MDIForm.Pixie16RunStatusBar.Panels(1).Text = "No run in progress"
    
    DSPValIsHexadecimal = True
    DSPMemBlk0IsHexadecimal = True
    DSPMemBlk1IsHexadecimal = True
    SystemBooted = False
    
    '*******************************************
    '   Disable close button
    '*******************************************
    
    RemoveCloseMenuItem (Me.hWnd)
    
    '*******************************************
    '   Initialize Startup tab
    '*******************************************
    
    '   We need at least one module
    If Number_Modules_Total < 1 Then
        Number_Modules_Total = 1
    End If
    
    If IsOfflineOperation = 1 Then
        OfflineAnalysisCheckBox.Value = 1
    Else
        OfflineAnalysisCheckBox.Value = 0
    End If
        
    '   Initialize SetupPXISlots Grid
    With SetupPXISlotsGrid
        .Redraw = False
        
        .Cols = 6
        .Rows = Number_Modules_Total + 1
        
        .FixedCols = 5
        .FixedRows = 1
        
        .ColWidth(0) = 750
        .ColWidth(1) = 400
        .ColWidth(2) = 500
        .ColWidth(3) = 400
        .ColWidth(4) = 540
        .ColWidth(5) = 800
        
        .TextMatrix(0, 0) = "Module #"
        .TextMatrix(0, 1) = "REV"
        .TextMatrix(0, 2) = "S/N"
        .TextMatrix(0, 3) = "Bits"
        .TextMatrix(0, 4) = "MSPS"
        .TextMatrix(0, 5) = "PXI Slot #"
        
        .RowHeight(0) = 300
        For X = 1 To .Rows - 1
            .RowHeight(X) = 300
            .TextMatrix(X, 0) = X - 1
            .TextMatrix(X, 1) = ""
            .TextMatrix(X, 2) = ""
            .TextMatrix(X, 3) = ""
            .TextMatrix(X, 4) = ""
            .TextMatrix(X, 5) = PXISlotMap(X - 1)
        Next X
        
        For X = 0 To .Cols - 1
            For Y = 0 To .Rows - 1
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
        
        .Redraw = True
    End With
   
    '   Update maximum number of modules
    TotalNumModulesEdit.Value = Number_Modules_Total
    StatisticsModNumEdit.Maximum = Number_Modules_Total - 1
   
    '*******************************************
    '   Initialize Run tab
    '*******************************************
    
    '   Run type
    RunTypeCombo.AddItem "0x301: MCA mode"
    RunTypeCombo.AddItem "0x100: List mode"
    
    If RunType = &H301 Then
        RunTypeCombo.ListIndex = 0
        RunPollingInterval = 1#
        PollingIntervalEdit.Value = 1#
    ElseIf RunType = &H100 Then
        RunTypeCombo.ListIndex = 1
        RunPollingInterval = 0.001
        PollingIntervalEdit.Value = 0.001
    End If
      
    '   Preset run time
    PresetRunTimeEdit.Value = PresetRunTime
    
    '   Run status polling interval
    PollingIntervalEdit.Value = RunPollingInterval
    
    '   Number of FIFO fills for list mode run
    NumberOfFIFOFillsEdit.Value = NumberOfFIFOFills
    
    '   Output file base name
    OutFileBaseNameText.Text = OutputFileBaseName
    
    '   Run number
    RunNumEdit.Value = RunNumber
       
    '*******************************************
    '   Initialize Statistics tab
    '*******************************************
   
    '   Initialize LiveTime Grid
    With LiveTimeGrid
        .Redraw = False
        
        .Cols = 2
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 850
        .ColWidth(1) = 1750
        .TextMatrix(0, 0) = "Channel #"
        .TextMatrix(0, 1) = "Live time [s]"
        
        .RowHeight(0) = 300
        For X = 1 To .Rows - 1
            .RowHeight(X) = 300
            .TextMatrix(X, 0) = X - 1
        Next X
        
        For X = 0 To .Cols - 1
            For Y = 0 To .Rows - 1
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
        
        .Redraw = True
    End With
    
    '   Initialize ICR Grid
    With ICRGrid
        .Redraw = False
        
        .Cols = 2
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 850
        .ColWidth(1) = 1750
        .TextMatrix(0, 0) = "Channel #"
        .TextMatrix(0, 1) = "Input count rate [cps]"
        
        .RowHeight(0) = 300
        For X = 1 To .Rows - 1
            .RowHeight(X) = 300
            .TextMatrix(X, 0) = X - 1
        Next X
        
        For X = 0 To .Cols - 1
            For Y = 0 To .Rows - 1
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
        
        .Redraw = True
    End With
    
    '   Initialize OCR Grid
    With OCRGrid
        .Redraw = False
        
        .Cols = 2
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 850
        .ColWidth(1) = 1750
        .TextMatrix(0, 0) = "Channel #"
        .TextMatrix(0, 1) = "Output count rate [cps]"
        
        .RowHeight(0) = 300
        For X = 1 To .Rows - 1
            .RowHeight(X) = 300
            .TextMatrix(X, 0) = X - 1
        Next X
        
        For X = 0 To .Cols - 1
            For Y = 0 To .Rows - 1
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
        
        .Redraw = True
    End With
    
    ' Initialize SYSTEM_CLOCK_MHZ
    For X = 0 To Number_Modules_Total - 1
        SYSTEM_CLOCK_MHZ(X) = 100   ' System clock in MHz
    Next X
    
    ' Initialize XDT_SCALE_FACTOR
    For X = 0 To Number_Modules_Total - 1
        XDT_SCALE_FACTOR(X) = 6   ' XDT scale factor in DSP clock cycles
    Next X

    ' Initialize Xdt
    Xdt(0) = 0.06
    Xdt(1) = 0.06
    Xdt(2) = 0.06
    Xdt(3) = 0.06
    
End Sub

Private Sub NumberOfFIFOFillsEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    If DAQRunTimer.Enabled = False Then
        NumberOfFIFOFills = NumberOfFIFOFillsEdit.Value
    End If
End Sub

Private Sub PresetRunTimeEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim StrTxt As String
    Dim Msg As Integer
    Dim retval As Long
    
    If DAQRunTimer.Enabled = False Then
        PresetRunTime = PresetRunTimeEdit.Value
        
        If SystemBooted = True Then
            '*******************************************
            '   Broadcast HOST_RT_PRESET to all modules
            '*******************************************
            HostRunTimePreset(0) = Decimal2IEEEFloating(CDbl(PresetRunTimeEdit.Value))
            retval = Pixie16WriteSglModPar("HOST_RT_PRESET", HostRunTimePreset(0), 0)
            If retval < 0 Then
                StrTxt = "(Pixie16WriteSglModPar) failed to write HOST_RT_PRESET. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        End If
    End If
End Sub

'###########################################
'   ResumeRunCheck_Click()
'       Choose whether to resume a run
'###########################################

Private Sub ResumeRunCheck_Click()

    If ResumeRunCheck.Value = 1 Then
        ResumeRun = True
    Else
        ResumeRun = False
    End If

End Sub

'###########################################
'   TotalNumModulesEdit_ValueChanged()
'       Select the total number of Pixie-16
'       modules in the system.
'###########################################

Private Sub TotalNumModulesEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim X As Integer
    Dim Y As Integer
        
    ' Update the total number of Pixie-16 modules in the system
    Number_Modules_Total = Value
    
    ' Update PXI slot number grid
    With SetupPXISlotsGrid
        .Redraw = False
        
        .Rows = Number_Modules_Total + 1
        For X = 1 To .Rows - 1
            .RowHeight(X) = 300
            .TextMatrix(X, 0) = X - 1
            .TextMatrix(X, 1) = ""
            .TextMatrix(X, 2) = ""
            .TextMatrix(X, 3) = ""
            .TextMatrix(X, 4) = ""
            .TextMatrix(X, 5) = PXISlotMap(X - 1)
        Next X
        
        For X = 0 To .Cols - 1
            For Y = 0 To .Rows - 1
                .Row = Y
                .Col = X
                .CellAlignment = flexAlignCenterCenter
            Next Y
        Next X
        
        .Redraw = True
    End With
    
    '   Update maximum number of modules
    If IsLoaded("DAQParametersSetup") Then
        DAQParametersSetup.DAQParSetupModNumEdit.Maximum = Number_Modules_Total - 1
    End If
    If IsLoaded("AcquireADCTracesForm") Then
        AcquireADCTracesForm.ADCTracesModNumEdit.Maximum = Number_Modules_Total - 1
    End If
    If IsLoaded("AcquireBaselinesForm") Then
        AcquireBaselinesForm.BaselinesModNumEdit.Maximum = Number_Modules_Total - 1
    End If
    If IsLoaded("ReadMCAHistogramsForm") Then
        ReadMCAHistogramsForm.ReadMCAHistogramsModNumEdit.Maximum = Number_Modules_Total - 1
    End If

End Sub


'###########################################
'   SetupPXISlots Grid Controls
'       Subroutines that are used to set PXI
'       slot number
'###########################################

Private Sub SetupPXISlotsGrid_Click()
    '
    ' Display the textbox if the user clicked
    ' on a non-fixed row or column.
    '
    If bOnFixedPartPXISlots Then Exit Sub
    Call pEditPXISlotsGrid(32)
End Sub

Private Sub pEditPXISlotsGrid(KeyAscii As Integer)
    '
    ' Populate the textbox and position it.
    '
    With txtEditPXISlots
        Select Case KeyAscii
            Case 0 To 32
                '
                ' Edit the current text.
                '
                .Text = SetupPXISlotsGrid
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
    With SetupPXISlotsGrid
        If .CellWidth < 0 Then Exit Sub
        txtEditPXISlots.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth, .CellHeight
        '
        ' NOTE:
        '   Depending on the style of the Grid Lines that you set, you
        '   may need to adjust the textbox position slightly. For example
        '   if you use raised grid lines use the following:
        '
        'txtEditPXISlots.Move .Left + .CellLeft, .Top + .CellTop, .CellWidth - 8, .CellHeight - 8
    End With
    
    txtEditPXISlots.Visible = True
    txtEditPXISlots.SetFocus
End Sub

Private Sub SetupPXISlotsGrid_GotFocus()
    
    If bDoNotEditPXISlots Then Exit Sub
    '
    ' Copy the textbox's value to the grid
    ' and hide the textbox.
    '
    Call pSetCellValue
    
    ' Update PXISlotMap
    If SetupPXISlotsGrid.Col > 4 And SetupPXISlotsGrid.Row > 0 Then
        PXISlotMap(SetupPXISlotsGrid.Row - 1) = SetupPXISlotsGrid.TextMatrix(SetupPXISlotsGrid.Row, 5)
    End If
    
End Sub

Private Sub SetupPXISlotsGrid_KeyPress(KeyAscii As Integer)
    '
    ' Display the textbox.
    '
    Call pEditPXISlotsGrid(KeyAscii)
End Sub

Private Sub SetupPXISlotsGrid_LeaveCell()
    
    If bDoNotEditPXISlots Then Exit Sub
    Call SetupPXISlotsGrid_GotFocus
End Sub

Private Sub SetupPXISlotsGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim k      As Long
    Dim lWidth As Long
    Dim lHeight As Long
    
    With SetupPXISlotsGrid
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
        bOnFixedPartPXISlots = (X < .ColWidth(0)) Or _
                       (X > lWidth) Or _
                       (Y < .RowHeight(0)) Or _
                       (Y > lHeight)
    End With
End Sub

Private Sub SetupPXISlotsGrid_Scroll()
    Call SetupPXISlotsGrid_GotFocus
End Sub

Private Sub txtEditPXISlots_KeyDown(KeyCode As Integer, Shift As Integer)
    '
    ' See what key was pressed in the textbox.
    '
    With SetupPXISlotsGrid
        Select Case KeyCode
            Case 13   'ENTER
                .SetFocus
            Case 27   'ESC
                 txtEditPXISlots.Visible = False
                .SetFocus
            Case 38   'Up arrow
                .SetFocus
                DoEvents
                If .Row > .FixedRows Then
                    bDoNotEditPXISlots = True
                    .Row = .Row - 1
                    bDoNotEditPXISlots = False
                End If
            Case 40   'Down arrow
                .SetFocus
                DoEvents
                If .Row < .Rows - 1 Then
                    bDoNotEditPXISlots = True
                    .Row = .Row + 1
                    bDoNotEditPXISlots = False
                End If
        End Select
    End With
End Sub

Private Sub txtEditPXISlots_KeyPress(KeyAscii As Integer)
    '
    ' Delete carriage returns to get rid of beep
    ' and only allow numbers plus 'A' to 'F'.
    '
    Select Case KeyAscii
        Case Asc(vbCr)
            KeyAscii = 0
        Case 8
        Case 48 To 57
        Case Else
            KeyAscii = 0
    End Select
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
    
    ' PXI slots setup
    If txtEditPXISlots.Visible Then
        SetupPXISlotsGrid.Text = txtEditPXISlots.Text
        txtEditPXISlots.Visible = False
    End If
End Sub


'###########################################
'   SelectConfigFiles_Click()
'       Set Pixie-16 configuration files
'###########################################

Private Sub SelectConfigFiles_Click()

    ' Check if the form is minimized or maximized
    If dlgConfigFilesDataPaths.WindowState <> vbNormal Then
        dlgConfigFilesDataPaths.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, dlgConfigFilesDataPaths
    dlgConfigFilesDataPaths.Height = 7185
    dlgConfigFilesDataPaths.Width = 8085
    
    dlgConfigFilesDataPaths.ZOrder 0

End Sub


'###########################################
'   BootPixie16Modules_Click()
'       Boot all Pixie-16 modules
'###########################################

Private Sub BootPixie16Modules_Click()
    Dim retval As Long
    Dim Msg As Integer
    Dim count As Integer
    Dim StrTxt As String
    Dim BootPattern As Integer
    Dim Pixie16_Com_FPGA_File As String
    Dim Pixie16_SP_FPGA_File As String
    Dim Pixie16_Trig_FPGA_File As String
    Dim Pixie16_DSP_Code_File As String
    Dim Pixie16_DSP_Var_File As String
    Dim fso
        
    '*******************************************
    '   Disable BootPixie16Modules button until boot completes
    '*******************************************
    BootPixie16Modules.Enabled = False
        
    Pixie16MDIForm.Pixie16InitStatusBar.Panels(1).Picture = StatusPics.RedSquare
    Pixie16MDIForm.Pixie16InitStatusBar.Panels(1).Text = "Pixie-16 not initialized"
    
    ' Check if SystemBooted is True; if yes, we need to close PCI devices first
    If SystemBooted = True Then
    
        ' Close the PCI devices that are already opened
        For count = 0 To Number_Modules_Booted - 1
            retval = Pixie16ExitSystem(count)
            If retval < 0 Then
                StrTxt = "Pixie16ExitSystem failed in module # " & count
                Msg = MsgBox(StrTxt, 0, "Error")
            End If
        Next count
        
    End If

    ' Open PCI communication to the modules
    retval = Pixie16InitSystem(Number_Modules_Total, PXISlotMap(0), IsOfflineOperation)
    If retval < 0 Then
        Msg = MsgBox("Pixie16InitSystem returned unsuccessfully. Please check the log file", 0, "Error")
        GoTo AbortBoot
    End If

    ' Set modules' offline variant if running in offline mode
    ' Use module's PXI Slot # as the variant identifier
    '     Slot #        Variant
    '     1             Rev D
    '     2             Rev F, 14-bit, 100 MHz
    '     3             Rev F, 12-bit, 250 MHz
    '     4             Rev F, 14-bit, 250 MHz
    '     5             Rev F, 12-bit, 500 MHz
    '     6             Rev F, 14-bit, 500 MHz
    '     7             Rev F, 16-bit, 250 MHz
    If IsOfflineOperation = 1 Then
        For count = 0 To Number_Modules_Total - 1
            ' check if module offline variant is valid
            If PXISlotMap(count) > 0 And PXISlotMap(count) < 8 Then
                retval = Pixie16SetOfflineVariant(count, PXISlotMap(count))
            Else
                StrTxt = "Wrong module offline variant set in PXI Slot # for module " & Str$(count)
                Msg = MsgBox(StrTxt, 0, "Error")
                GoTo AbortBoot
            End If
        Next count
    End If

    ' Retrieve module hardware information
    For count = 0 To Number_Modules_Total - 1
        retval = Pixie16ReadModuleInfo(count, ModRev(count), ModSerNum(count), ModADCBits(count), ModADCMSPS(count))
        If retval < 0 Then
            StrTxt = "Pixie16ReadModuleInfo " & "(Module # " & Str$(count) & ") returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            GoTo AbortBoot
        End If
    Next count
        
    ' Update module hardware information in the PXI Slots Grid
    Call UpdatePXISlotsGrid
        
    ' Set Pixie16_Trig_FPGA_File to a dummy string since it is no longer in use - just a place holder for software compatibility
    Pixie16_Trig_FPGA_File = "pixie16trigger.bin"
            
    ' Boot all Pixie16 modules
    BootPattern = &H7F
    
    ' Create a file system object
    Set fso = CreateObject("Scripting.FileSystemObject")

    For count = 0 To Number_Modules_Total - 1
    
        ' Select firmware and dsp files based on hardware variant
        Select Case ModRev(count)
            Case 11, 12, 13
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevBCD
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevBCD
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevBCD
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevBCD
            Case 15
                If ModADCBits(count) = 14 And ModADCMSPS(count) = 100 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF14b100m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF14b100m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF14b100m
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b100m
                ElseIf ModADCBits(count) = 16 And ModADCMSPS(count) = 100 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF16b100m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF16b100m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF16b100m
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF16b100m
                ElseIf ModADCBits(count) = 12 And ModADCMSPS(count) = 250 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF12b250m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF12b250m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF12b250m
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF12b250m
                ElseIf ModADCBits(count) = 14 And ModADCMSPS(count) = 250 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF14b250m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF14b250m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF14b250m
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF14b250m
                ElseIf ModADCBits(count) = 16 And ModADCMSPS(count) = 250 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF16b250m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF16b250m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF16b250m
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF16b250m
                ElseIf ModADCBits(count) = 12 And ModADCMSPS(count) = 500 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF12b500m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF12b500m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF12b500m
                    Pixie16_DSP_Var_File = Pixie16_DSP_Var_File_RevF12b500m
                ElseIf ModADCBits(count) = 14 And ModADCMSPS(count) = 500 Then
                    Pixie16_Com_FPGA_File = Pixie16_Com_FPGA_File_RevF14b500m
                    Pixie16_SP_FPGA_File = Pixie16_SP_FPGA_File_RevF14b500m
                    Pixie16_DSP_Code_File = Pixie16_DSP_Code_File_RevF14b500m
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
    
        ' Boot the module
        retval = Pixie16BootModule(Pixie16_Com_FPGA_File, Pixie16_SP_FPGA_File, Pixie16_Trig_FPGA_File, _
                                Pixie16_DSP_Code_File, Pixie16_DSP_Par_File, Pixie16_DSP_Var_File, _
                                count, BootPattern)
        If retval < 0 Then
            StrTxt = "Pixie16BootModule returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            GoTo AbortBoot
        End If
    Next count
    
    ' Update status display
    Pixie16MDIForm.Pixie16InitStatusBar.Panels(1).Picture = StatusPics.GreenSquare
    Pixie16MDIForm.Pixie16InitStatusBar.Panels(1).Text = "Pixie-16 initialized"
    
    ' Set SYSTEM_CLOCK_MHZ
    Call Set_SYSTEM_CLOCK_MHZ
    
    ' Set XDT_SCALE_FACTOR
    Call Set_XDT_SCALE_FACTOR
    
    ' Retrieve module parameters
    For count = 0 To Number_Modules_Total - 1
        retval = RetrieveModParFromModule(count)
        If retval < 0 Then
            StrTxt = "RetrieveModParFromModule " & "(Module # " & Str$(count) & ") returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            GoTo AbortBoot
        End If
    Next count
    
    ' Retrieve channel parameters for the currently selected module
    If IsLoaded("DAQParametersSetup") Then
        retval = RetrieveChanParFromModule(DAQParametersSetup.DAQParSetupModNumEdit.Value)
        If retval < 0 Then
            StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(DAQParametersSetup.DAQParSetupModNumEdit.Value) & ") returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            GoTo AbortBoot
        End If
    Else
        retval = RetrieveChanParFromModule(0)
        If retval < 0 Then
            StrTxt = "RetrieveChanParFromModule " & "(Module # 0) returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            GoTo AbortBoot
        End If
    End If
       
    ' Update IN_SYNCH and SYNCH_WAIT - here we use module #0
    ' since all other modules should be the same
    retval = UpdateModParCtrlsOnMDIForm(0)
    
    If IsLoaded("DAQParametersSetup") = True Then
        ' Update maximum number of modules for the DAQ Parameters Setup form
        DAQParametersSetup.DAQParSetupModNumEdit.Maximum = Number_Modules_Total - 1
    
        ' Update module parameters on the DAQ parameters setup form
        retval = UpdateModParCtrlsOnDAQForm(DAQParametersSetup.DAQParSetupModNumEdit.Value)
        
        ' Update channel parameters displays for the currently selected module
        retval = UpdateChanParCtrlsOnDAQForm()
    End If
   
    If IsLoaded("AcquireADCTracesForm") = True Then
        ' Update maximum number of modules for the ADC trace form
        AcquireADCTracesForm.ADCTracesModNumEdit.Maximum = Number_Modules_Total - 1
    
        ' Set available ADC sample intervals based on module variant
        Call Set_ADC_Sampling_Intervals(AcquireADCTracesForm.ADCTracesModNumEdit.Value)
        
        ' Update ADC sample intervals
        Call UpdateADCSampleIntervals(AcquireADCTracesForm.ADCTracesModNumEdit.Value)
    End If
   
    SystemBooted = True
    Number_Modules_Booted = Number_Modules_Total
    StatisticsModNumEdit.Maximum = Number_Modules_Total - 1
    
AbortBoot:
    '*******************************************
    '   Eanble BootPixie16Modules button
    '*******************************************
    BootPixie16Modules.Enabled = True

End Sub


'###########################################
'   StartRun_Click()
'       Start a data acquisition run
'###########################################

Private Sub StartRun_Click()
    Dim retval As Long
    Dim Msg As Integer
    Dim StrTxt As String
    Dim count As Integer
    Dim index As Integer
    
    '*******************************************
    '   Get the requested run type
    '*******************************************
    index = RunTypeCombo.ListIndex
    Select Case index
        Case 0
            RunType = &H301
            RunPollingInterval = 1#
            PollingIntervalEdit.Value = 1#
        Case 1
            RunType = &H100
            RunPollingInterval = 0.001
            PollingIntervalEdit.Value = 0.001
    End Select

    '*******************************************
    '   Make data and settings files names
    '*******************************************
    If RunType = &H301 Then ' MCA run
        For count = 0 To Number_Modules_Total - 1
            Pixie16_HistogramDataFile_Name(count) = Pixie16_HistogramDataFile_Path & OutputFileBaseName & Format(RunNumber, "0000") & "_mod" & Format(count, "00") & ".mca"
        Next count
        
        Pixie16_SettingsFile_Name = Pixie16_HistogramDataFile_Path & OutputFileBaseName & Format(RunNumber, "0000") & ".set"
        If IsLoaded("ReadMCAHistogramsForm") Then
            ReadMCAHistogramsForm.HistogramDataFile = Pixie16_HistogramDataFile_Name(0)
        End If
    Else ' List mode run
        For count = 0 To Number_Modules_Total - 1
            Pixie16_HistogramDataFile_Name(count) = Pixie16_ListModeDataFile_Path & OutputFileBaseName & Format(RunNumber, "0000") & "_mod" & Format(count, "00") & ".mca"
            Pixie16_ListModeDataFile_Name(count) = Pixie16_ListModeDataFile_Path & OutputFileBaseName & Format(RunNumber, "0000") & "_mod" & Format(count, "00") & ".bin"
        Next count
        
        Pixie16_SettingsFile_Name = Pixie16_ListModeDataFile_Path & OutputFileBaseName & Format(RunNumber, "0000") & ".set"
        If IsLoaded("ReadListModeTracesForm") Then
            ReadListModeTracesForm.ListModeDataFile = Pixie16_ListModeDataFile_Name(0)
        End If
    End If
    
    '*******************************************
    '   Initialize run control parameters
    '*******************************************
    PresetRunTime = PresetRunTimeEdit.Value
    NumberOfFIFOFills = NumberOfFIFOFillsEdit.Value
    
    For count = 0 To Number_Modules_Total - 1
        nFIFOWords(count) = 0
    Next count

    '*******************************************
    '   Broadcast SYNCH_WAIT to all modules
    '*******************************************
    If SynchStartStopCheckBox.Value = 1 Then
        SynchWait(0) = 1
    Else
        SynchWait(0) = 0
    End If
    retval = Pixie16WriteSglModPar("SYNCH_WAIT", SynchWait(0), 0)
    If retval < 0 Then
        StrTxt = "(Pixie16WriteSglModPar) failed to write SYNCH_WAIT. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
        
    '*******************************************
    '   Broadcast IN_SYNCH to all modules
    '*******************************************
    If SynchClocksCheckBox.Value = 1 Then
        InSynch(0) = 0
    Else
        InSynch(0) = 1
    End If
    retval = Pixie16WriteSglModPar("IN_SYNCH", InSynch(0), 0)
    If retval < 0 Then
        StrTxt = "(Pixie16WriteSglModPar) failed to write IN_SYNCH. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
        
    '*******************************************
    '   Broadcast HOST_RT_PRESET to all modules
    '*******************************************
    HostRunTimePreset(0) = Decimal2IEEEFloating(CDbl(PresetRunTimeEdit.Value))
    retval = Pixie16WriteSglModPar("HOST_RT_PRESET", HostRunTimePreset(0), 0)
    If retval < 0 Then
        StrTxt = "(Pixie16WriteSglModPar) failed to write HOST_RT_PRESET. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    '*******************************************
    '   Start the run now
    '*******************************************
    If RunType = &H301 Then ' MCA run
        If ResumeRunCheck.Value = 0 Then
            retval = Pixie16StartHistogramRun(Number_Modules_Total, NEW_RUN)
            If retval < 0 Then
                StrTxt = "Pixie16StartHistogramRun (NEW_RUN) failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        Else
            retval = Pixie16StartHistogramRun(Number_Modules_Total, RESUME_RUN)
            If retval < 0 Then
                StrTxt = "Pixie16StartHistogramRun (RESUME_RUN) failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        End If
    Else ' List mode run
        If ResumeRunCheck.Value = 0 Then
            retval = Pixie16StartListModeRun(Number_Modules_Total, RunType, NEW_RUN)
            If retval < 0 Then
                StrTxt = "Pixie16StartListModeRun (NEW_RUN) failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        Else
            retval = Pixie16StartListModeRun(Number_Modules_Total, RunType, RESUME_RUN)
            If retval < 0 Then
                StrTxt = "Pixie16StartListModeRun (RESUME_RUN) failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        End If
    End If
    
    '*******************************************
    '   Set run status indicators
    '*******************************************
    Pixie16MDIForm.Pixie16RunStatusBar.Panels(1).Picture = StatusPics.GreenSquare
    Pixie16MDIForm.Pixie16RunStatusBar.Panels(1).Text = "Run in progress"
    
    '*******************************************
    '   Check system timer frequency and record
    '   run start time
    '*******************************************
    QueryPerformanceFrequency SysTimerFreq ' Get the timer frequency
    QueryPerformanceCounter RunStartTime   ' Get the start time
  
    '*******************************************
    '   Activate the DAQ run timer
    '*******************************************
    DAQRunTimer.Interval = RunPollingInterval * 1000
    DAQRunTimer.Enabled = True
    
    '*******************************************
    '   Disable controls during the DAQ run
    '*******************************************
    StartRun.Enabled = False
    
End Sub


'###########################################
'   DAQRunTimer_Timer()
'       Timer function to check run status
'###########################################

Private Sub DAQRunTimer_Timer()
    Dim retval As Long
    Dim ModuleNumber, count As Integer
    Dim Msg As Integer
    Dim StrTxt As String
    Dim FIFOWordsRead As Long
    Dim EndOfRunRead As Integer

    '*******************************************
    '   First, decrement the run time by the elapsed
    '   run time
    '*******************************************
    QueryPerformanceCounter CurrentRunTime    ' Get the current time
    ElapsedRunTime = (CurrentRunTime - RunStartTime) / SysTimerFreq
    PresetRunTimeEdit.Value = PresetRunTime - ElapsedRunTime

    If RunType = &H301 Then ' MCA run
        ' MCA run: only check run status since
        ' DSP will check run time and stop run
        
        '*******************************************
        '   Now, check the run status. Here we only
        '   check the status of module #0 since we assume
        '   a multi-module system should always be synchronized
        '*******************************************
        ModuleNumber = 0
        retval = Pixie16CheckRunStatus(ModuleNumber)
        If retval = 0 Or ElapsedRunTime >= PresetRunTime Then
            ' Make sure run in all modules is stopped
            retval = Pixie16EndRun(Number_Modules_Total)
            If retval < 0 Then
                ' Stop the timer
                DAQRunTimer.Enabled = False
                
                StrTxt = "Pixie16EndRun failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Call Restore_DAQ_Controls
                Exit Sub
            End If
            
            ' Stop the timer
            DAQRunTimer.Enabled = False

            Call Post_DAQ_Processing
            Call Restore_DAQ_Controls
        Else
            ' Update run statistics
            ModuleNumber = StatisticsModNumEdit.Value
            Call ComputeRunStatistics(ModuleNumber)
            
            ' Update MCA spectra
            If IsLoaded("ReadMCAHistogramsForm") Then
                Call ReadMCAHistogramsFromModule
            End If
        End If
    Else
        ' List mode run: read data from external FIFO
        EndOfRunRead = 0
        For count = 0 To Number_Modules_Total - 1
            retval = Pixie16SaveExternalFIFODataToFile(Pixie16_ListModeDataFile_Name(count), FIFOWordsRead, count, EndOfRunRead)
            If retval < 0 Then
                StrTxt = "Pixie16SaveExternalFIFODataToFile failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Call Restore_DAQ_Controls
                Exit Sub
            End If
            
            ' Accumulate the number of words read from the external FIFO so far
            nFIFOWords(count) = nFIFOWords(count) + FIFOWordsRead
        Next count
        
        ' Update remaining number of accumulated FIFO fills
        NumberOfFIFOFillsEdit.Value = NumberOfFIFOFills - CLng(nFIFOWords(0) / EXTFIFO_LENGTH)

        ' List mode run: check accumulated number of FIFO fills, timeout, and run status
        
        '*******************************************
        '   Now, check the run status. Here we only
        '   check the status of module #0 since we assume
        '   a multi-module system should always be synchronized
        '*******************************************
        ModuleNumber = 0
        retval = Pixie16CheckRunStatus(ModuleNumber)

        If NumberOfFIFOFillsEdit.Value = 0 Or ElapsedRunTime > PresetRunTime Or retval = 0 Then
            ' Make sure run in all modules is stopped successfully
            retval = Pixie16EndRun(Number_Modules_Total)
            For ModuleNumber = 0 To Number_Modules_Total - 1
                count = 0
                Do While count < 1000
                    retval = Pixie16CheckRunStatus(ModuleNumber)
                    If retval <> 0 Then
                        EndOfRunRead = 1
                        retval = Pixie16SaveExternalFIFODataToFile(Pixie16_ListModeDataFile_Name(ModuleNumber), FIFOWordsRead, ModuleNumber, EndOfRunRead)
                        If retval < 0 Then
                            StrTxt = "Pixie16SaveExternalFIFODataToFile failed. Please check the log file"
                            Msg = MsgBox(StrTxt, 0, "Error")
                            Call Restore_DAQ_Controls
                            Exit Sub
                        End If
                    
                        ' Accumulate the number of words read from the external FIFO so far
                        nFIFOWords(ModuleNumber) = nFIFOWords(ModuleNumber) + FIFOWordsRead
                    Else
                        Exit Do
                    End If
                    
                    count = count + 1
                Loop
                
                If count = 1000 Then
                    StrTxt = "Pixie16EndRun failed. Please check the log file"
                    Msg = MsgBox(StrTxt, 0, "Error")
                    Call Restore_DAQ_Controls
                    Exit Sub
                End If
            Next ModuleNumber
            
            ' All modules have their run stopped successfully. Now read out the possible last words from the external FIFO
            For ModuleNumber = 0 To Number_Modules_Total - 1
                EndOfRunRead = 1
                retval = Pixie16SaveExternalFIFODataToFile(Pixie16_ListModeDataFile_Name(ModuleNumber), FIFOWordsRead, ModuleNumber, EndOfRunRead)
                If retval < 0 Then
                    StrTxt = "Pixie16SaveExternalFIFODataToFile failed. Please check the log file"
                    Msg = MsgBox(StrTxt, 0, "Error")
                    Call Restore_DAQ_Controls
                    Exit Sub
                End If
            
                ' Accumulate the number of words read from the external FIFO so far
                nFIFOWords(ModuleNumber) = nFIFOWords(ModuleNumber) + FIFOWordsRead
            Next ModuleNumber
            
            ' Stop the timer
            DAQRunTimer.Enabled = False

            Call Post_DAQ_Processing
            Call Restore_DAQ_Controls
                
        End If
    End If
    
End Sub


'###########################################
'   Post_DAQ_Processing()
'       Post-processing routines after the run is ended
'###########################################

Private Sub Post_DAQ_Processing()
    Dim retval As Long
    Dim count As Integer
    Dim Msg As Integer
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    
    ' Save histogram data to file
    For count = 0 To Number_Modules_Total - 1
        retval = Pixie16SaveHistogramToFile(Pixie16_HistogramDataFile_Name(count), count)
        If retval < 0 Then
            StrTxt = "Pixie16SaveHistogramToFile failed. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
    Next count

    ' Save DSP parameters to file
    retval = Pixie16SaveDSPParametersToFile(Pixie16_SettingsFile_Name)
    If retval < 0 Then
        StrTxt = "Pixie16SaveDSPParametersToFile failed. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    ' Update run statistics data
    
    ModuleNumber = StatisticsModNumEdit.Value
    Call ComputeRunStatistics(ModuleNumber)
    
    If RunType = &H301 Then ' MCA run
        ' Load ReadMCAHistogramsForm if it not loaded yet
        ' Check if the form is minimized or maximized
        If ReadMCAHistogramsForm.WindowState <> vbNormal Then
            ReadMCAHistogramsForm.WindowState = vbNormal
        End If
    
        CenterChild Pixie16MDIForm, ReadMCAHistogramsForm
        ReadMCAHistogramsForm.Height = 11235
        ReadMCAHistogramsForm.Width = 11565
    Else ' List mode run
    
        ' Clear array
        For count = 0 To Number_Modules_Total - 1
            ModuleListModeEvents(count) = 0
        Next count

        ' Load ReadListModeTracesForm if it not loaded yet
        ' Check if the form is minimized or maximized
        If ReadListModeTracesForm.WindowState <> vbNormal Then
            ReadListModeTracesForm.WindowState = vbNormal
        End If
    
        CenterChild Pixie16MDIForm, ReadListModeTracesForm
        ReadListModeTracesForm.Height = 11070
        ReadListModeTracesForm.Width = 14925
        
        ReadListModeTracesForm.ZOrder 0
    End If

End Sub


'###########################################
'   Restore_DAQ_Controls()
'       Restore DAQ controls after the run is ended
'###########################################

Private Sub Restore_DAQ_Controls()

    StartRun.Enabled = True
    PresetRunTimeEdit.Value = PresetRunTime
    NumberOfFIFOFillsEdit.Value = NumberOfFIFOFills
    
    If AutoIncRunNumber = True Then
        RunNumber = RunNumber + 1
        RunNumEdit.Value = RunNumber
    End If
    
    Pixie16MDIForm.Pixie16RunStatusBar.Panels(1).Picture = StatusPics.RedSquare
    Pixie16MDIForm.Pixie16RunStatusBar.Panels(1).Text = "No run in progress"

End Sub


'###########################################
'   StopRun_Click()
'       User requests to stop the run
'###########################################

Private Sub StopRun_Click()
    Dim retval As Long
    Dim count As Integer
    Dim Msg
    Dim StrTxt As String
    Dim FIFOWordsRead As Long
    Dim EndOfRunRead As Integer
    Dim ModuleNumber As Integer
    
    ' Stop the timer
    DAQRunTimer.Enabled = False
    
    ' Stop run in all modules
    retval = Pixie16EndRun(Number_Modules_Total)
    If retval < 0 Then
        StrTxt = "Pixie16EndRun failed. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
    End If

    If RunType <> &H301 Then ' Not MCA run, i.e. list mode run
        ' Make sure run in all modules is stopped successfully
        For ModuleNumber = 0 To Number_Modules_Total - 1
            count = 0
            Do While count < 10
                retval = Pixie16CheckRunStatus(ModuleNumber)
                If retval <> 0 Then
                    EndOfRunRead = 1
                    retval = Pixie16SaveExternalFIFODataToFile(Pixie16_ListModeDataFile_Name(ModuleNumber), FIFOWordsRead, ModuleNumber, EndOfRunRead)
                    If retval < 0 Then
                        StrTxt = "Pixie16SaveExternalFIFODataToFile failed. Please check the log file"
                        Msg = MsgBox(StrTxt, 0, "Error")
                        Call Restore_DAQ_Controls
                        Exit Sub
                    End If
                
                    ' Accumulate the number of words read from the external FIFO so far
                    nFIFOWords(ModuleNumber) = nFIFOWords(ModuleNumber) + FIFOWordsRead
                Else
                    Exit Do
                End If
                
                count = count + 1
            Loop
            If count = 10 Then
                StrTxt = "Pixie16EndRun failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Call Restore_DAQ_Controls
                Exit Sub
            End If
        Next ModuleNumber
        
        ' All modules have their run stopped successfully. Now read out the possible last words from the external FIFO
        For ModuleNumber = 0 To Number_Modules_Total - 1
            EndOfRunRead = 1
            retval = Pixie16SaveExternalFIFODataToFile(Pixie16_ListModeDataFile_Name(ModuleNumber), FIFOWordsRead, ModuleNumber, EndOfRunRead)
            If retval < 0 Then
                StrTxt = "Pixie16SaveExternalFIFODataToFile failed. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Call Restore_DAQ_Controls
                Exit Sub
            End If
        
            ' Accumulate the number of words read from the external FIFO so far
            nFIFOWords(ModuleNumber) = nFIFOWords(ModuleNumber) + FIFOWordsRead
        Next ModuleNumber
    End If
    
    Call Post_DAQ_Processing
    Call Restore_DAQ_Controls
    
End Sub


'###########################################
'   StatisticsModNumEdit_ValueChanged()
'       Display run statistics for the selected
'       module
'###########################################

Private Sub StatisticsModNumEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim ModuleNumber As Integer
    
    ModuleNumber = StatisticsModNumEdit.Value
    Call ComputeRunStatistics(ModuleNumber)

End Sub


'###########################################
'   StatisticsModNumEdit_ValueChanged()
'       Display run statistics for the selected
'       module
'###########################################

Private Sub ComputeRunStatistics(ByVal ModuleNumber As Integer)
    Dim retval As Long
    Dim count As Integer
    Dim Msg As Integer
    Dim StrTxt As String
    Dim realtime As Double
    
    ' Read run statistics data from the selected module
    retval = Pixie16ReadStatisticsFromModule(StatisticsData(0), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16ReadStatisticsFromModule failed. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    ' Compute number of processed events
    ProcessedEventsTextBox = Str$(Pixie16ComputeProcessedEvents(StatisticsData(0), ModuleNumber))
    
    ' Compute real time
    realtime = Pixie16ComputeRealTime(StatisticsData(0), ModuleNumber)
    RealTimeTextBox = Format(realtime, "0.000")
    
    ' Compute live time
    With LiveTimeGrid
        For count = 0 To NUM_CHANNELS - 1
            .TextMatrix(count + 1, 1) = Format(Pixie16ComputeLiveTime(StatisticsData(0), ModuleNumber, count), "0.000")
        Next count
    End With

    ' Compute ICR
    With ICRGrid
        For count = 0 To NUM_CHANNELS - 1
            .TextMatrix(count + 1, 1) = Format(Pixie16ComputeInputCountRate(StatisticsData(0), ModuleNumber, count), "0.000")
        Next count
    End With

    ' Compute OCR
    With OCRGrid
        For count = 0 To NUM_CHANNELS - 1
            .TextMatrix(count + 1, 1) = Format(Pixie16ComputeOutputCountRate(StatisticsData(0), ModuleNumber, count), "0.000")
        Next count
    End With

End Sub


'###########################################
'   SaveVBSettingsToFile()
'       Save VB_GUI settings to a text file
'###########################################

Public Sub SaveVBSettingsToFile()
    Dim Pixie16SettingsFile
    Dim LineStr As String
    Dim i As Integer
    Dim ErrTxt As String
    Dim fso, Msg
    
    ' Create a file system object
    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Pixie16SettingsFile = StartUpDir + "\" + PIXIE16_VB_SETTINGS_FILE
    
    ' Check if Pixie16SettingsFile exists
    If (fso.FileExists(Pixie16SettingsFile)) Then
        Open Pixie16SettingsFile For Output As #1
    Else    ' file doesn't exist
        ErrTxt = "Pixie16 Settings File " & Pixie16SettingsFile & " doesn't exist."
        Msg = MsgBox(ErrTxt, 0, "Error")
        Exit Sub
    End If
    
    Print #1, "[Number of Pixie-16 modules]"
    Print #1, Number_Modules_Total
    Print #1,
    
    Print #1, "[Module PXI slot number]"
    For i = 0 To Number_Modules_Total - 1
        Print #1, PXISlotMap(i)
    Next
    Print #1,
        
    Print #1, "[FPGA Firmware files]"
    
    Print #1, "*** Rev-B/C/D ***"
    Print #1, Pixie16_Com_FPGA_File_RevBCD
    Print #1, Pixie16_SP_FPGA_File_RevBCD
    
    Print #1, "*** Rev-F-14Bit-100MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF14b100m
    Print #1, Pixie16_SP_FPGA_File_RevF14b100m
    
    Print #1, "*** Rev-F-16Bit-100MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF16b100m
    Print #1, Pixie16_SP_FPGA_File_RevF16b100m
    
    Print #1, "*** Rev-F-12Bit-250MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF12b250m
    Print #1, Pixie16_SP_FPGA_File_RevF12b250m
    
    Print #1, "*** Rev-F-14Bit-250MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF14b250m
    Print #1, Pixie16_SP_FPGA_File_RevF14b250m
    
    Print #1, "*** Rev-F-16Bit-250MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF16b250m
    Print #1, Pixie16_SP_FPGA_File_RevF16b250m
    
    Print #1, "*** Rev-F-12Bit-500MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF12b500m
    Print #1, Pixie16_SP_FPGA_File_RevF12b500m
    
    Print #1, "*** Rev-F-14Bit-500MSPS ***"
    Print #1, Pixie16_Com_FPGA_File_RevF14b500m
    Print #1, Pixie16_SP_FPGA_File_RevF14b500m
    
    Print #1,
    
    Print #1, "[DSP Code files]"
    
    Print #1, "*** Rev-B/C/D ***"
    Print #1, Pixie16_DSP_Code_File_RevBCD
    Print #1, Pixie16_DSP_Var_File_RevBCD
    Print #1, Pixie16_DSP_List_File_RevBCD
    
    Print #1, "*** Rev-F-14Bit-100MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF14b100m
    Print #1, Pixie16_DSP_Var_File_RevF14b100m
    Print #1, Pixie16_DSP_List_File_RevF14b100m
    
    Print #1, "*** Rev-F-16Bit-100MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF16b100m
    Print #1, Pixie16_DSP_Var_File_RevF16b100m
    Print #1, Pixie16_DSP_List_File_RevF16b100m
    
    Print #1, "*** Rev-F-12Bit-250MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF12b250m
    Print #1, Pixie16_DSP_Var_File_RevF12b250m
    Print #1, Pixie16_DSP_List_File_RevF12b250m
    
    Print #1, "*** Rev-F-14Bit-250MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF14b250m
    Print #1, Pixie16_DSP_Var_File_RevF14b250m
    Print #1, Pixie16_DSP_List_File_RevF14b250m
    
    Print #1, "*** Rev-F-16Bit-250MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF16b250m
    Print #1, Pixie16_DSP_Var_File_RevF16b250m
    Print #1, Pixie16_DSP_List_File_RevF16b250m
    
    Print #1, "*** Rev-F-12Bit-500MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF12b500m
    Print #1, Pixie16_DSP_Var_File_RevF12b500m
    Print #1, Pixie16_DSP_List_File_RevF12b500m
    
    Print #1, "*** Rev-F-14Bit-500MSPS ***"
    Print #1, Pixie16_DSP_Code_File_RevF14b500m
    Print #1, Pixie16_DSP_Var_File_RevF14b500m
    Print #1, Pixie16_DSP_List_File_RevF14b500m
    
    Print #1,
        
    Print #1, "[DSP parameters file]"
    Print #1, Pixie16_DSP_Par_File
    Print #1,
   
    Print #1, "[Output data file paths]"
    Print #1, Pixie16_ListModeDataFile_Path
    Print #1, Pixie16_HistogramDataFile_Path
    Print #1,
    
    Print #1, "[Offline analysis mode]"
    If IsOfflineOperation = 1 Then
        Print #1, "1"
    Else
        Print #1, "0"
    End If
    Print #1,
    
    Print #1, "[Run type]"
    Print #1, RunType
    Print #1,
    
    Print #1, "[Run time [s]]"
    Print #1, PresetRunTime
    Print #1,
    
    Print #1, "[Run status polling interval [s]]"
    Print #1, RunPollingInterval
    Print #1,

    Print #1, "[Number of FIFO fills for Module #0]"
    Print #1, NumberOfFIFOFills
    Print #1,

    Print #1, "[Output file base name]"
    Print #1, OutputFileBaseName
    Print #1,

    Print #1, "[Run number]"
    Print #1, RunNumber
    Print #1,

    Print #1, "[Auto increment run number]"
    If AutoIncRunNumber = True Then
        Print #1, "1"
    Else
        Print #1, "0"
    End If
    Print #1,
    
    Print #1, "[Resume run]"
    If ResumeRun = True Then
        Print #1, "1"
    Else
        Print #1, "0"
    End If
    Print #1,
    
    Close #1
    
End Sub


'###########################################
'   LoadVBSettingsOnStartUp()
'       Load VB_GUI settings from a text file
'###########################################

Private Sub LoadVBSettingsOnStartUp()
    Dim Pixie16SettingsFile As String
    Dim LineStr As String
    Dim i As Integer
    Dim ErrTxt As String
    Dim fso, Msg
    
    ' Create a file system object
    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Pixie16SettingsFile = StartUpDir + "\" + PIXIE16_VB_SETTINGS_FILE
    
    ' Check if Pixie16SettingsFile exists
    If (fso.FileExists(Pixie16SettingsFile)) Then
        Open Pixie16SettingsFile For Input As #1
    Else    ' file doesn't exist
        ErrTxt = "Pixie16 Settings File " & Pixie16SettingsFile & " doesn't exist."
        Msg = MsgBox(ErrTxt, 0, "Error")
        Exit Sub
    End If
    
    Do While Not EOF(1)
    
        Line Input #1, LineStr
    
        If (LineStr = "[Number of Pixie-16 modules]") Then
            Line Input #1, LineStr
            Number_Modules_Total = LineStr
                        
        ElseIf (LineStr = "[Module PXI slot number]") Then
            For i = 0 To Number_Modules_Total - 1
                Line Input #1, LineStr
                PXISlotMap(i) = LineStr
            Next

        ElseIf (LineStr = "[FPGA Firmware files]") Then
            For i = 0 To 7
                Line Input #1, LineStr
                If (LineStr = "*** Rev-B/C/D ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevBCD = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevBCD = LineStr
               ElseIf (LineStr = "*** Rev-F-14Bit-100MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF14b100m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF14b100m = LineStr
               ElseIf (LineStr = "*** Rev-F-16Bit-100MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF16b100m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF16b100m = LineStr
               ElseIf (LineStr = "*** Rev-F-12Bit-250MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF12b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF12b250m = LineStr
               ElseIf (LineStr = "*** Rev-F-14Bit-250MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF14b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF14b250m = LineStr
               ElseIf (LineStr = "*** Rev-F-16Bit-250MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF16b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF16b250m = LineStr
               ElseIf (LineStr = "*** Rev-F-12Bit-500MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF12b500m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF12b500m = LineStr
               ElseIf (LineStr = "*** Rev-F-14Bit-500MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_Com_FPGA_File_RevF14b500m = LineStr
                    Line Input #1, LineStr
                    Pixie16_SP_FPGA_File_RevF14b500m = LineStr
               End If
            Next
        
        ElseIf (LineStr = "[DSP Code files]") Then
            For i = 0 To 7
                Line Input #1, LineStr
                If (LineStr = "*** Rev-B/C/D ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevBCD = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevBCD = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevBCD = LineStr
               ElseIf (LineStr = "*** Rev-F-14Bit-100MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF14b100m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF14b100m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF14b100m = LineStr
               ElseIf (LineStr = "*** Rev-F-16Bit-100MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF16b100m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF16b100m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF16b100m = LineStr
               ElseIf (LineStr = "*** Rev-F-12Bit-250MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF12b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF12b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF12b250m = LineStr
               ElseIf (LineStr = "*** Rev-F-14Bit-250MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF14b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF14b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF14b250m = LineStr
               ElseIf (LineStr = "*** Rev-F-16Bit-250MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF16b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF16b250m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF16b250m = LineStr
               ElseIf (LineStr = "*** Rev-F-12Bit-500MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF12b500m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF12b500m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF12b500m = LineStr
               ElseIf (LineStr = "*** Rev-F-14Bit-500MSPS ***") Then
                    Line Input #1, LineStr
                    Pixie16_DSP_Code_File_RevF14b500m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_Var_File_RevF14b500m = LineStr
                    Line Input #1, LineStr
                    Pixie16_DSP_List_File_RevF14b500m = LineStr
               End If
            Next
        
        ElseIf (LineStr = "[DSP parameters file]") Then
            Line Input #1, LineStr
            Pixie16_DSP_Par_File = LineStr
            
        ElseIf (LineStr = "[Output data file paths]") Then
            Line Input #1, LineStr
            Pixie16_ListModeDataFile_Path = LineStr
            Line Input #1, LineStr
            Pixie16_HistogramDataFile_Path = LineStr
       
        ElseIf (LineStr = "[Offline analysis mode]") Then
            Line Input #1, LineStr
            If CInt(LineStr) = 1 Then
                IsOfflineOperation = 1
            Else
                IsOfflineOperation = 0
            End If
            
         ElseIf (LineStr = "[Run type]") Then
            Line Input #1, LineStr
            RunType = LineStr
                        
         ElseIf (LineStr = "[Run time [s]]") Then
            Line Input #1, LineStr
            PresetRunTime = LineStr
                        
         ElseIf (LineStr = "[Run status polling interval [s]]") Then
            Line Input #1, LineStr
            RunPollingInterval = LineStr
                       
        ElseIf (LineStr = "[Number of FIFO fills for Module #0]") Then
            Line Input #1, LineStr
            NumberOfFIFOFills = LineStr
                       
        ElseIf (LineStr = "[Output file base name]") Then
            Line Input #1, LineStr
            OutputFileBaseName = LineStr
            
        ElseIf (LineStr = "[Run number]") Then
            Line Input #1, LineStr
            RunNumber = LineStr
            
        ElseIf (LineStr = "[Auto increment run number]") Then
            Line Input #1, LineStr
            If CInt(LineStr) = 1 Then
                AutoIncRunNumber = True
                AutoIncRunNumberCheckBox.Value = 1
            Else
                AutoIncRunNumber = False
                AutoIncRunNumberCheckBox.Value = 0
            End If
        ElseIf (LineStr = "[Resume run]") Then
            Line Input #1, LineStr
            If CInt(LineStr) = 1 Then
                ResumeRun = True
                ResumeRunCheck.Value = 1
            Else
                ResumeRun = False
                ResumeRunCheck.Value = 0
            End If
            
        End If
    Loop
    
    Close #1
    
End Sub


'###########################################
'   OfflineAnalysisCheckBox_Click()
'       Set OfflineAnalysis CheckBox
'###########################################

Private Sub OfflineAnalysisCheckBox_Click()
    If OfflineAnalysisCheckBox.Value = 1 Then
        IsOfflineOperation = 1
    Else
        IsOfflineOperation = 0
    End If
End Sub


'###########################################
'   LoadLogFile_Click()
'       Load contents from the Pixie-16 log
'       file (in line-reverse order)
'###########################################

Private Sub LoadLogFile_Click()

    Dim Pixie16MsgTxt As String
    Pixie16MsgTxt = String(65535, vbNullChar)
    
    ' Check if the form is minimized or maximized
    If Pixie16MsgLogFile.WindowState <> vbNormal Then
        Pixie16MsgLogFile.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, Pixie16MsgLogFile
    Pixie16MsgLogFile.Height = 10980
    Pixie16MsgLogFile.Width = 11310
    
    Pixie16MsgLogFile.ZOrder 0

    ' Load file contents
    Pixie16ReadMSGFile (Pixie16MsgTxt)
    Pixie16MsgLogFile.Pixie16MsgTextBox.Text = Pixie16MsgTxt

End Sub


'###########################################
'   SetDAQParameters_Click()
'       Load form DAQParametersSetup
'###########################################

Private Sub SetDAQParameters_Click()

    ' Check if the form is minimized or maximized
    If DAQParametersSetup.WindowState <> vbNormal Then
        DAQParametersSetup.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, DAQParametersSetup
    DAQParametersSetup.Height = 10965
    DAQParametersSetup.Width = 10260
    
    DAQParametersSetup.ZOrder 0
    
End Sub


'###########################################
'   CopyDSPParameters_Click()
'       Load form CopySettings
'###########################################

Private Sub CopyDSPParameters_Click()

    ' Check if the form is minimized or maximized
    If CopySettings.WindowState <> vbNormal Then
        CopySettings.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, CopySettings
    CopySettings.Height = 7425
    CopySettings.Width = 13590
    
    CopySettings.ZOrder 0

End Sub


'###########################################
'   SaveParameters_Click()
'       Load form SaveSettingsForm
'###########################################

Private Sub SaveParameters_Click()

    ' Check if the form is minimized or maximized
    If SaveSettingsForm.WindowState <> vbNormal Then
        SaveSettingsForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, SaveSettingsForm
    SaveSettingsForm.Height = 3705
    SaveSettingsForm.Width = 6960
    
    SaveSettingsForm.ZOrder 0
    
    SaveSettingsForm.DSPSettingsCheckBox.SetFocus
End Sub


'###########################################
'   LoadDSPParameters_Click()
'       Load DSP parameters from a file
'###########################################

Private Sub LoadDSPParameters_Click()
    Dim retval As Long
    Dim Msg As Integer
    Dim StrTxt As String
    Dim ModuleNumber As Integer
    Dim count As Integer
   
    If SystemBooted = False Then
        Exit Sub
    End If
    
    ' Browse for the DSP parameters file
    dlgLoadDSPSettingsFile.filename = vbNullString
    dlgLoadDSPSettingsFile.Filter = "Binary Files (*.set)|*.set|All|*.*"
    dlgLoadDSPSettingsFile.ShowOpen
    If dlgLoadDSPSettingsFile.filename = vbNullString Then
       Exit Sub
    End If
    
    ' Update relevant file names and displays
    Pixie16_DSP_Par_File = dlgLoadDSPSettingsFile.filename
    If IsLoaded("dlgConfigFilesDataPaths") Then
        dlgConfigFilesDataPaths.DSPParfile.Text = Pixie16_DSP_Par_File
    End If
    If IsLoaded("SaveSettingsForm") Then
        SaveSettingsForm.DSPSettingsFileText.Text = Pixie16_DSP_Par_File
    End If
    
    ' Download settings in the file to all modules
    retval = Pixie16LoadDSPParametersFromFile(Pixie16_DSP_Par_File)
    If retval < 0 Then
        StrTxt = "Pixie16LoadDSPParametersFromFile returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    ' Retrieve module parameters
    For count = 0 To Number_Modules_Total - 1
        retval = RetrieveModParFromModule(count)
        If retval < 0 Then
            StrTxt = "RetrieveModParFromModule " & "(Module # " & Str$(count) & ") returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
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
            Exit Sub
        End If
    
        ' Update module parameters displays for the currently selected module
        retval = UpdateModParCtrlsOnDAQForm(ModuleNumber)
        
        ' Update channel parameters displays
        retval = UpdateChanParCtrlsOnDAQForm()
    End If
    
End Sub


'###########################################
'   AcquireADCTraces_Click()
'       Acquire ADC traces for one or more channels
'###########################################

Private Sub AcquireADCTraces_Click()

    ' Check if the form is minimized or maximized
    If AcquireADCTracesForm.WindowState <> vbNormal Then
        AcquireADCTracesForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, AcquireADCTracesForm
    AcquireADCTracesForm.Height = 11115
    AcquireADCTracesForm.Width = 12135
    
    AcquireADCTracesForm.ZOrder 0

End Sub


'###########################################
'   AcquireBaselines_Click()
'       Acquire baselines for one or more channels
'###########################################

Private Sub AcquireBaselines_Click()

    ' Check if the form is minimized or maximized
    If AcquireBaselinesForm.WindowState <> vbNormal Then
        AcquireBaselinesForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, AcquireBaselinesForm
    AcquireBaselinesForm.Height = 10620
    AcquireBaselinesForm.Width = 11565
    
    AcquireBaselinesForm.ZOrder 0

End Sub


'###########################################
'   RunTypeCombo_Click()
'       Choose run type
'###########################################

Private Sub RunTypeCombo_Click()
    Dim index As Integer
    Dim retval As Long
    Dim Msg As Integer
    Dim count As Integer
    Dim StrTxt As String

    index = RunTypeCombo.ListIndex
    Select Case index
        Case 0
            RunType = &H301
            RunPollingInterval = 1#
            PollingIntervalEdit.Value = 1#
        Case 1
            RunType = &H100
            RunPollingInterval = 0.001
            PollingIntervalEdit.Value = 0.001
    End Select
    
End Sub


'###########################################
'   SynchStartStopCheckBox_Click()
'       Enable or disalbe start/stop run simutaneously
'###########################################

Private Sub SynchStartStopCheckBox_Click()
    Dim retval As Long
    Dim ErrTxt As String
    Dim Msg As Integer
    
    '====================================
    '   Here we only set SYNCH_WAIT for module #0
    '   since the C library will broadcast SYNCH_WAIT to
    '   all other modules.
    '====================================
    If SynchStartStopCheckBox.Value = 1 Then
        SynchWait(0) = 1
    Else
        SynchWait(0) = 0
    End If
    
    retval = Pixie16WriteSglModPar("SYNCH_WAIT", SynchWait(0), 0)
    If retval < 0 Then
        ErrTxt = "(Pixie16WriteSglModPar) failed to write SYNCH_WAIT. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
    End If

End Sub


'###########################################
'   SynchClocksCheckBox_Click()
'       Enable or disable clock synchronization
'###########################################

Private Sub SynchClocksCheckBox_Click()
    Dim retval As Long
    Dim ErrTxt As String
    Dim Msg As Integer
    
    '====================================
    '   Here we only set IN_SYNCH for module #0
    '   since the C library will broadcast IN_SYNCH to
    '   all other modules.
    '====================================
    If SynchClocksCheckBox.Value = 1 Then
        InSynch(0) = 0
    Else
        InSynch(0) = 1
    End If
    
    retval = Pixie16WriteSglModPar("IN_SYNCH", InSynch(0), 0)
    If retval < 0 Then
        ErrTxt = "(Pixie16WriteSglModPar) failed to write IN_SYNCH. Please check the log file"
        Msg = MsgBox(ErrTxt, 0, "Error")
    End If

End Sub


'###########################################
'   OutFileBaseNameText
'       Change output file base name
'###########################################

Private Sub OutFileBaseNameText_KeyPress(KeyAscii As Integer)
    If (KeyAscii < 48 Or ((KeyAscii > 57) And (KeyAscii < 65)) Or ((KeyAscii > 90) And (KeyAscii < 97)) Or (KeyAscii > 123)) Then
        If (KeyAscii <> 8 And KeyAscii <> 13 And KeyAscii <> 32 And KeyAscii <> 45 And KeyAscii <> 46 And KeyAscii <> 95) Then
            KeyAscii = 0
        End If
    End If
    If (KeyAscii = 13) Then
        OutputFileBaseName = OutFileBaseNameText.Text
    End If
End Sub

Private Sub OutFileBaseNameText_LostFocus()
    OutputFileBaseName = OutFileBaseNameText.Text
End Sub


'###########################################
'   RunNumEdit_ValueChanged()
'       Choose number of spills for list mode run
'###########################################

Private Sub RunNumEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    RunNumber = Value
End Sub


'###########################################
'   AutoIncRunNumberCheckBox_Click()
'       Choose whether to automatically increment run number
'###########################################

Private Sub AutoIncRunNumberCheckBox_Click()
    If AutoIncRunNumberCheckBox.Value = 1 Then
        AutoIncRunNumber = True
    Else
        AutoIncRunNumber = False
    End If

End Sub


'###########################################
'   ShowMCASpectrumButton_Click()
'       Read MCA spectrum for one or more channels
'###########################################

Private Sub ShowMCASpectrumButton_Click()

    ' Check if the form is minimized or maximized
    If ReadMCAHistogramsForm.WindowState <> vbNormal Then
        ReadMCAHistogramsForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, ReadMCAHistogramsForm
    ReadMCAHistogramsForm.Height = 11235
    ReadMCAHistogramsForm.Width = 11565
    
    ReadMCAHistogramsForm.ZOrder 0

End Sub


'###########################################
'   MCAHistogram_Click()
'       Read MCA spectrum for one or more channels
'###########################################

Private Sub MCAHistogram_Click()

    ' Check if the form is minimized or maximized
    If ReadMCAHistogramsForm.WindowState <> vbNormal Then
        ReadMCAHistogramsForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, ReadMCAHistogramsForm
    ReadMCAHistogramsForm.Height = 11235
    ReadMCAHistogramsForm.Width = 11565
    
    ReadMCAHistogramsForm.ZOrder 0
    
End Sub


'###########################################
'   ShowListModeTracesButton_Click()
'       Read list mode traces for one or more channels
'###########################################

Private Sub ShowListModeTracesButton_Click()

    ' Check if the form is minimized or maximized
    If ReadListModeTracesForm.WindowState <> vbNormal Then
        ReadListModeTracesForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, ReadListModeTracesForm
    ReadListModeTracesForm.Height = 11070
    ReadListModeTracesForm.Width = 14925
    
    ReadListModeTracesForm.ZOrder 0

End Sub


'###########################################
'   ListModeTraces_Click()
'       Read list mode traces for one or more channels
'###########################################

Private Sub ListModeTraces_Click()

    ' Check if the form is minimized or maximized
    If ReadListModeTracesForm.WindowState <> vbNormal Then
        ReadListModeTracesForm.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, ReadListModeTracesForm
    ReadListModeTracesForm.Height = 11070
    ReadListModeTracesForm.Width = 14925
    
    ReadListModeTracesForm.ZOrder 0

End Sub


'###########################################
'   Exit_Click()
'       Exit the Pixie-16 VB GUI
'###########################################

Private Sub Exit_Click()
    Dim intCount As Integer
    Dim retval As Long
    Dim StrTxt As String
    Dim Msg
    
    While Forms.count > 1
        ' Find first form besides "me" to unload
        intCount = 0
        
        While Forms(intCount).Caption = Me.Caption
            intCount = intCount + 1
        Wend
        
        Unload Forms(intCount)
    Wend
    
    ' Check if the Pixie-16 modules have been booted
    If SystemBooted = True Then
    
        ' Now close the previously opened PCI devices
        For intCount = 0 To Number_Modules_Total - 1
            retval = Pixie16ExitSystem(intCount)
            If retval < 0 Then
                StrTxt = "Pixie16ExitSystem for module # " & intCount & " returned unsuccessfully"
                Msg = MsgBox(StrTxt, 0, "Error")
            End If
        Next intCount
        
    End If
    
    ' Last thing to be done...
    Unload Me
    
End Sub


'###########################################
'   ShowDSPpar_Click()
'       Display DSP parameters in a table
'###########################################

Private Sub ShowDSPpar_Click()
    showDSPval.Show
    
    ' Check if the form is minimized or maximized
    If showDSPval.WindowState <> vbNormal Then
        showDSPval.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, showDSPval
    showDSPval.Height = 10125
    showDSPval.Width = 12200
    
    showDSPval.ZOrder 0
End Sub


'###########################################
'   DispDSPMemBlock0_Click()
'       Display DSP memory block #0 in a table
'###########################################

Private Sub DispDSPMemBlock0_Click()
    showDSPMemBlock0.Show
    
    ' Check if the form is minimized or maximized
    If showDSPMemBlock0.WindowState <> vbNormal Then
        showDSPMemBlock0.WindowState = vbNormal
    End If
    
    CenterChild Pixie16MDIForm, showDSPMemBlock0
    showDSPMemBlock0.Height = 9975
    showDSPMemBlock0.Width = 7935
    
    showDSPMemBlock0.ZOrder 0

End Sub


'###########################################
'   DispDSPMemBlock1_Click()
'       Display DSP memory block #1 in a table
'###########################################

Private Sub DispDSPMemBlock1_Click()
    showDSPMemBlock1.Show

    ' Check if the form is minimized or maximized
    If showDSPMemBlock1.WindowState <> vbNormal Then
        showDSPMemBlock1.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, showDSPMemBlock1
    showDSPMemBlock1.Height = 9975
    showDSPMemBlock1.Width = 7935
    
    showDSPMemBlock1.ZOrder 0
    
End Sub


'###########################################
'   About_Click()
'       Display About form
'###########################################

Private Sub About_Click()
    ' Check if the form is minimized or maximized
    If frmAbout.WindowState <> vbNormal Then
        frmAbout.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, frmAbout
    frmAbout.Height = 4680
    frmAbout.Width = 9780
    
    frmAbout.ZOrder 0
End Sub

'###########################################
'   ReadMCAHistogramsFromModule()
'       Read MCA histograms from module
'###########################################

Private Sub ReadMCAHistogramsFromModule()
    Dim retval As Long
    Dim ModuleNumber, ChannelNumber As Integer
    Dim StrTxt As String
    Dim Msg As Integer

    ModuleNumber = ReadMCAHistogramsForm.ReadMCAHistogramsModNumEdit.Value
    
    ' First the single channel
    ChannelNumber = ReadMCAHistogramsForm.SglChMCAHistogramsChanNumEdit.Value
    
    retval = Pixie16ReadHistogramFromModule(MCASpecSglCh(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
   
    ' Plot the MCA histogram
    ReadMCAHistogramsForm.MCAHistogramGraph.PlotY MCASpecSglCh
    'ReadMCAHistogramsForm.MCAHistogramGraph.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

    ' Read histograms for other channels if they are checked
    If ReadMCAHistogramsForm.Channel0.Value = 1 Then
        ChannelNumber = 0
        retval = Pixie16ReadHistogramFromModule(MCASpecCh0(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs0To3.Plots(1).PlotY MCASpecCh0
    End If
    
    If ReadMCAHistogramsForm.Channel1.Value = 1 Then
        ChannelNumber = 1
        retval = Pixie16ReadHistogramFromModule(MCASpecCh1(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs0To3.Plots(2).PlotY MCASpecCh1
    End If
    
    If ReadMCAHistogramsForm.Channel2.Value = 1 Then
        ChannelNumber = 2
        retval = Pixie16ReadHistogramFromModule(MCASpecCh2(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs0To3.Plots(3).PlotY MCASpecCh2
    End If

    If ReadMCAHistogramsForm.Channel3.Value = 1 Then
        ChannelNumber = 3
        retval = Pixie16ReadHistogramFromModule(MCASpecCh3(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs0To3.Plots(4).PlotY MCASpecCh3
    End If

    If ReadMCAHistogramsForm.Channel4.Value = 1 Then
        ChannelNumber = 4
        retval = Pixie16ReadHistogramFromModule(MCASpecCh4(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs4To7.Plots(1).PlotY MCASpecCh4
    End If

    If ReadMCAHistogramsForm.Channel5.Value = 1 Then
        ChannelNumber = 5
        retval = Pixie16ReadHistogramFromModule(MCASpecCh5(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs4To7.Plots(2).PlotY MCASpecCh5
    End If

    If ReadMCAHistogramsForm.Channel6.Value = 1 Then
        ChannelNumber = 6
        retval = Pixie16ReadHistogramFromModule(MCASpecCh6(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs4To7.Plots(3).PlotY MCASpecCh6
    End If

    If ReadMCAHistogramsForm.Channel7.Value = 1 Then
        ChannelNumber = 7
        retval = Pixie16ReadHistogramFromModule(MCASpecCh7(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs4To7.Plots(4).PlotY MCASpecCh7
    End If

    If ReadMCAHistogramsForm.Channel8.Value = 1 Then
        ChannelNumber = 8
        retval = Pixie16ReadHistogramFromModule(MCASpecCh8(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs8To11.Plots(1).PlotY MCASpecCh8
    End If
    
    If ReadMCAHistogramsForm.Channel9.Value = 1 Then
        ChannelNumber = 9
        retval = Pixie16ReadHistogramFromModule(MCASpecCh9(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs8To11.Plots(2).PlotY MCASpecCh9
    End If
    
    If ReadMCAHistogramsForm.Channel10.Value = 1 Then
        ChannelNumber = 10
        retval = Pixie16ReadHistogramFromModule(MCASpecCh10(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs8To11.Plots(3).PlotY MCASpecCh10
    End If
    
    If ReadMCAHistogramsForm.Channel11.Value = 1 Then
        ChannelNumber = 11
        retval = Pixie16ReadHistogramFromModule(MCASpecCh11(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs8To11.Plots(4).PlotY MCASpecCh11
    End If

    If ReadMCAHistogramsForm.Channel2.Value = 1 Then
        ChannelNumber = 12
        retval = Pixie16ReadHistogramFromModule(MCASpecCh12(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs12To15.Plots(1).PlotY MCASpecCh12
    End If
    
    If ReadMCAHistogramsForm.Channel13.Value = 1 Then
        ChannelNumber = 13
        retval = Pixie16ReadHistogramFromModule(MCASpecCh13(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs12To15.Plots(2).PlotY MCASpecCh13
    End If
        
    If ReadMCAHistogramsForm.Channel14.Value = 1 Then
        ChannelNumber = 14
        retval = Pixie16ReadHistogramFromModule(MCASpecCh14(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs12To15.Plots(3).PlotY MCASpecCh14
    End If

    If ReadMCAHistogramsForm.Channel15.Value = 1 Then
        ChannelNumber = 15
        retval = Pixie16ReadHistogramFromModule(MCASpecCh15(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ReadMCAHistogramsForm.MCAHistogramsChs12To15.Plots(4).PlotY MCASpecCh15
    End If

End Sub
