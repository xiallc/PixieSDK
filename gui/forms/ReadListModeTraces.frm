VERSION 5.00
Object = "{5E9E78A0-531B-11CF-91F6-C2863C385E30}#1.0#0"; "MSFLXGRD.OCX"
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form ReadListModeTracesForm 
   Caption         =   "Read List Mode Traces"
   ClientHeight    =   10560
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   14805
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   10560
   ScaleWidth      =   14805
   Begin VB.CheckBox ComputeFilterResponsesCheckBox 
      Caption         =   "Compute Filter Responses"
      Height          =   375
      Left            =   11880
      TabIndex        =   59
      Top             =   8520
      Width           =   2295
   End
   Begin VB.CommandButton ReadListModeDataFromFile 
      Caption         =   "Read from file"
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
      Left            =   240
      TabIndex        =   47
      Top             =   9720
      Width           =   1935
   End
   Begin MSFlexGridLib.MSFlexGrid ListModeEventsEnergyGrid 
      Height          =   5205
      Left            =   11400
      TabIndex        =   46
      Top             =   240
      Width           =   3300
      _ExtentX        =   5821
      _ExtentY        =   9181
      _Version        =   393216
   End
   Begin VB.TextBox TotalNumEventsText 
      Height          =   375
      Left            =   5400
      TabIndex        =   42
      Top             =   9840
      Width           =   1575
   End
   Begin VB.TextBox ListModeDataFile 
      Height          =   285
      Left            =   1920
      TabIndex        =   40
      Top             =   9240
      Width           =   9015
   End
   Begin VB.CommandButton CloseListModeTracesForm 
      Caption         =   "Close"
      Height          =   615
      Left            =   11880
      TabIndex        =   1
      Top             =   6000
      Width           =   1695
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   8655
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   11175
      _ExtentX        =   19711
      _ExtentY        =   15266
      _Version        =   393216
      TabOrientation  =   1
      TabHeight       =   520
      TabCaption(0)   =   "List mode trace - single channel"
      TabPicture(0)   =   "ReadListModeTraces.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "SglChMCAHistogramChanNumLabel"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "XPos"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "YPos"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "dX"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "dY"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "ListModeTracesGraph"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "SglChListModeTracesChanNumEdit"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "ZoomCombo"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "XCombo"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "dYVal"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "YPosVal"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "dXVal"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "XPosVal"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "Autoscale"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).ControlCount=   14
      TabCaption(1)   =   "List mode trace - 16 channels"
      TabPicture(1)   =   "ReadListModeTraces.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "ListModeTracesChs12To15"
      Tab(1).Control(1)=   "ListModeTracesChs8To11"
      Tab(1).Control(2)=   "ListModeTracesChs4To7"
      Tab(1).Control(3)=   "ListModeTracesChs0To3"
      Tab(1).Control(4)=   "AutoScaleChs0To3"
      Tab(1).Control(5)=   "AutoScaleChs4To7"
      Tab(1).Control(6)=   "AutoScaleChs8To11"
      Tab(1).Control(7)=   "AutoScaleChs12To15"
      Tab(1).Control(8)=   "Channel3"
      Tab(1).Control(9)=   "Channel2"
      Tab(1).Control(10)=   "Channel1"
      Tab(1).Control(11)=   "Channel0"
      Tab(1).Control(12)=   "Channel7"
      Tab(1).Control(13)=   "Channel6"
      Tab(1).Control(14)=   "Channel5"
      Tab(1).Control(15)=   "Channel4"
      Tab(1).Control(16)=   "Channel8"
      Tab(1).Control(17)=   "Channel9"
      Tab(1).Control(18)=   "Channel10"
      Tab(1).Control(19)=   "Channel11"
      Tab(1).Control(20)=   "Channel12"
      Tab(1).Control(21)=   "Channel13"
      Tab(1).Control(22)=   "Channel14"
      Tab(1).Control(23)=   "Channel15"
      Tab(1).ControlCount=   24
      TabCaption(2)   =   "List mode trace - filter responses"
      TabPicture(2)   =   "ReadListModeTraces.frx":0038
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "TextSFFilter"
      Tab(2).Control(1)=   "TextSFTrace"
      Tab(2).Control(2)=   "TextFFCFD"
      Tab(2).Control(3)=   "TextFFFilter"
      Tab(2).Control(4)=   "TextFFTrace"
      Tab(2).Control(5)=   "LMTraceFastFilterAutoScale"
      Tab(2).Control(6)=   "LMTraceSlowFilterAutoScale"
      Tab(2).Control(7)=   "EventChanNumberText"
      Tab(2).Control(8)=   "LMTraceFastFilterGraph"
      Tab(2).Control(9)=   "LMTraceSlowFilterGraph"
      Tab(2).Control(10)=   "EventChannelNumberLabel"
      Tab(2).ControlCount=   11
      Begin VB.TextBox TextSFFilter 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H000000FF&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   -72480
         TabIndex        =   58
         Text            =   "Slow Filter"
         Top             =   4320
         Width           =   1095
      End
      Begin VB.TextBox TextSFTrace 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H0000C000&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   -73440
         TabIndex        =   57
         Text            =   "Trace"
         Top             =   4320
         Width           =   855
      End
      Begin VB.TextBox TextFFCFD 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H0000FFFF&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   -71280
         TabIndex        =   56
         Text            =   "CFD"
         Top             =   240
         Width           =   615
      End
      Begin VB.TextBox TextFFFilter 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H000000FF&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   -72480
         TabIndex        =   55
         Text            =   "Fast Filter"
         Top             =   240
         Width           =   1095
      End
      Begin VB.TextBox TextFFTrace 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H0000C000&
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   360
         Left            =   -73440
         TabIndex        =   54
         Text            =   "Trace"
         Top             =   240
         Width           =   855
      End
      Begin VB.CommandButton LMTraceFastFilterAutoScale 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -74760
         TabIndex        =   53
         Top             =   240
         Width           =   1215
      End
      Begin VB.CommandButton LMTraceSlowFilterAutoScale 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -74760
         TabIndex        =   52
         Top             =   4280
         Width           =   1215
      End
      Begin VB.TextBox EventChanNumberText 
         Height          =   375
         Left            =   -64800
         TabIndex        =   50
         Top             =   120
         Width           =   735
      End
      Begin VB.CheckBox Channel15 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch15"
         Height          =   255
         Left            =   -66600
         TabIndex        =   39
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel14 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch14"
         Height          =   255
         Left            =   -67440
         TabIndex        =   38
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel13 
         BackColor       =   &H000000FF&
         Caption         =   "Ch13"
         Height          =   255
         Left            =   -68280
         TabIndex        =   37
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel12 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch12"
         Height          =   255
         Left            =   -69120
         TabIndex        =   36
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel11 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch11"
         Height          =   255
         Left            =   -72360
         TabIndex        =   35
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel10 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch10"
         Height          =   255
         Left            =   -73200
         TabIndex        =   34
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel9 
         BackColor       =   &H000000FF&
         Caption         =   "Ch9"
         Height          =   255
         Left            =   -73920
         TabIndex        =   33
         Top             =   4320
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel8 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch8"
         Height          =   255
         Left            =   -74640
         TabIndex        =   32
         Top             =   4320
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel4 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch4"
         Height          =   255
         Left            =   -69000
         TabIndex        =   31
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel5 
         BackColor       =   &H000000FF&
         Caption         =   "Ch5"
         Height          =   255
         Left            =   -68280
         TabIndex        =   30
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel6 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch6"
         Height          =   255
         Left            =   -67560
         TabIndex        =   29
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel7 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch7"
         Height          =   255
         Left            =   -66840
         TabIndex        =   28
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel0 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch0"
         Height          =   255
         Left            =   -74640
         TabIndex        =   27
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel1 
         BackColor       =   &H000000FF&
         Caption         =   "Ch1"
         Height          =   255
         Left            =   -73920
         TabIndex        =   26
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel2 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch2"
         Height          =   255
         Left            =   -73200
         TabIndex        =   25
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel3 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch3"
         Height          =   255
         Left            =   -72480
         TabIndex        =   24
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CommandButton AutoScaleChs12To15 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -65640
         TabIndex        =   23
         Top             =   4200
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs8To11 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -71160
         TabIndex        =   22
         Top             =   4200
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs4To7 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -65640
         TabIndex        =   21
         Top             =   120
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs0To3 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -71280
         TabIndex        =   20
         Top             =   120
         Width           =   1455
      End
      Begin VB.CommandButton Autoscale 
         Caption         =   "Autoscale"
         Height          =   495
         Left            =   6120
         TabIndex        =   15
         Top             =   7080
         Width           =   1215
      End
      Begin VB.TextBox XPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   435
         TabIndex        =   10
         Top             =   6840
         Width           =   1095
      End
      Begin VB.TextBox dXVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   1875
         TabIndex        =   9
         Top             =   6840
         Width           =   1095
      End
      Begin VB.TextBox YPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   435
         TabIndex        =   8
         Top             =   7200
         Width           =   1095
      End
      Begin VB.TextBox dYVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   1875
         TabIndex        =   7
         Top             =   7200
         Width           =   1095
      End
      Begin VB.ComboBox XCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   1800
         Style           =   2  'Dropdown List
         TabIndex        =   6
         Top             =   240
         Width           =   1575
      End
      Begin VB.ComboBox ZoomCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   240
         Style           =   2  'Dropdown List
         TabIndex        =   5
         Top             =   240
         Width           =   1575
      End
      Begin CWUIControlsLib.CWNumEdit SglChListModeTracesChanNumEdit 
         Height          =   495
         Left            =   9720
         TabIndex        =   3
         Top             =   7080
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
         AccelInc_Val_1  =   1
         RangeMinVarType_1=   5
         RangeMaxVarType_1=   5
         RangeMax_Val_1  =   15
         Bindings_1      =   4
         ClassName_4     =   "CCWBindingHolderArray"
         Editor_4        =   5
         ClassName_5     =   "CCWBindingHolderArrayEditor"
         Owner_5         =   1
      End
      Begin CWUIControlsLib.CWGraph ListModeTracesGraph 
         Height          =   6255
         Left            =   240
         TabIndex        =   4
         Top             =   600
         Width           =   10695
         _Version        =   393218
         _ExtentX        =   18865
         _ExtentY        =   11033
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   1
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   701
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   1
         minor_6         =   0.5
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   11
         rMax_15         =   377
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   1
         minor_13        =   0.5
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC Units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Axes_1          =   20
         ClassName_20    =   "CCWAxes"
         Array_20        =   2
         Editor_20       =   21
         ClassName_21    =   "CCWGFAxisArrayEditor"
         Owner_21        =   1
         Array[0]_20     =   6
         Array[1]_20     =   13
         DefaultPlot_1   =   22
         ClassName_22    =   "CCWDataPlot"
         opts_22         =   4194367
         Name_22         =   "[Template]"
         C[0]_22         =   65280
         C[1]_22         =   255
         C[2]_22         =   16711680
         C[3]_22         =   16776960
         Event_22        =   2
         X_22            =   6
         Y_22            =   13
         LineStyle_22    =   2
         LineWidth_22    =   1
         BasePlot_22     =   0
         DefaultXInc_22  =   1
         DefaultPlotPerRow_22=   -1  'True
         Cursors_1       =   23
         ClassName_23    =   "CCWCursors"
         Array_23        =   2
         Editor_23       =   24
         ClassName_24    =   "CCWGFCursorArrayEditor"
         Owner_24        =   1
         Array[0]_23     =   25
         ClassName_25    =   "CCWCursor"
         opts_25         =   61
         Name_25         =   "Cursor-1"
         C[0]_25         =   255
         Event_25        =   2
         X_25            =   0
         Y_25            =   0
         XPos_25         =   1
         YPos_25         =   1
         PointIndex_25   =   -1
         ChrosshairStyle_25=   8
         LockPlot_25     =   0
         Array[1]_23     =   26
         ClassName_26    =   "CCWCursor"
         opts_26         =   61
         Name_26         =   "Cursor-2"
         C[0]_26         =   16776960
         Event_26        =   2
         X_26            =   0
         Y_26            =   0
         XPos_26         =   2
         YPos_26         =   2
         PointIndex_26   =   -1
         ChrosshairStyle_26=   8
         LockPlot_26     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   27
         ClassName_27    =   "CCWDrawObj"
         opts_27         =   62
         Image_27        =   28
         ClassName_28    =   "CCWPictImage"
         opts_28         =   1280
         Rows_28         =   1
         Cols_28         =   1
         F_28            =   -2147483633
         B_28            =   -2147483633
         ColorReplaceWith_28=   8421504
         ColorReplace_28 =   8421504
         Tolerance_28    =   2
         Animator_27     =   0
         Blinker_27      =   0
         PlotFrame_1     =   29
         ClassName_29    =   "CCWDrawObj"
         opts_29         =   62
         C[1]_29         =   0
         Image_29        =   30
         ClassName_30    =   "CCWPictImage"
         opts_30         =   1280
         Rows_30         =   1
         Cols_30         =   1
         Pict_30         =   1
         F_30            =   -2147483633
         B_30            =   0
         ColorReplaceWith_30=   8421504
         ColorReplace_30 =   8421504
         Tolerance_30    =   2
         Animator_29     =   0
         Blinker_29      =   0
         Caption_1       =   31
         ClassName_31    =   "CCWDrawObj"
         opts_31         =   62
         C[0]_31         =   -2147483640
         Image_31        =   32
         ClassName_32    =   "CCWTextImage"
         font_32         =   0
         Animator_31     =   0
         Blinker_31      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   33
         ClassName_33    =   "CCWBindingHolderArray"
         Editor_33       =   34
         ClassName_34    =   "CCWBindingHolderArrayEditor"
         Owner_34        =   1
         Annotations_1   =   35
         ClassName_35    =   "CCWAnnotations"
         Editor_35       =   36
         ClassName_36    =   "CCWAnnotationArrayEditor"
         Owner_36        =   1
         AnnotationTemplate_1=   37
         ClassName_37    =   "CCWAnnotation"
         opts_37         =   63
         Name_37         =   "[Template]"
         Plot_37         =   38
         ClassName_38    =   "CCWDataPlot"
         opts_38         =   4194367
         Name_38         =   "[Template]"
         C[0]_38         =   65280
         C[1]_38         =   255
         C[2]_38         =   16711680
         C[3]_38         =   16776960
         Event_38        =   2
         X_38            =   6
         Y_38            =   13
         LineStyle_38    =   1
         LineWidth_38    =   1
         BasePlot_38     =   0
         DefaultXInc_38  =   1
         DefaultPlotPerRow_38=   -1  'True
         Text_37         =   "[Template]"
         TextXPoint_37   =   6.7
         TextYPoint_37   =   6.7
         TextColor_37    =   16777215
         TextFont_37     =   39
         ClassName_39    =   "CCWFont"
         bFont_39        =   -1  'True
         BeginProperty Font_39 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_37 =   40
         ClassName_40    =   "CDataBuffer"
         Type_40         =   5
         m_cDims;_40     =   1
         m_cElts_40      =   1
         Element[0]_40   =   3.3
         ShapeYPoints_37 =   41
         ClassName_41    =   "CDataBuffer"
         Type_41         =   5
         m_cDims;_41     =   1
         m_cElts_41      =   1
         Element[0]_41   =   3.3
         ShapeFillColor_37=   16777215
         ShapeLineColor_37=   16777215
         ShapeLineWidth_37=   1
         ShapeLineStyle_37=   1
         ShapePointStyle_37=   10
         ShapeImage_37   =   42
         ClassName_42    =   "CCWDrawObj"
         opts_42         =   62
         Image_42        =   43
         ClassName_43    =   "CCWPictImage"
         opts_43         =   1280
         Rows_43         =   1
         Cols_43         =   1
         Pict_43         =   7
         F_43            =   -2147483633
         B_43            =   -2147483633
         ColorReplaceWith_43=   8421504
         ColorReplace_43 =   8421504
         Tolerance_43    =   2
         Animator_42     =   0
         Blinker_42      =   0
         ArrowVisible_37 =   -1  'True
         ArrowColor_37   =   16777215
         ArrowWidth_37   =   1
         ArrowLineStyle_37=   1
         ArrowHeadStyle_37=   1
      End
      Begin CWUIControlsLib.CWGraph ListModeTracesChs0To3 
         Height          =   3375
         Left            =   -74760
         TabIndex        =   16
         Top             =   600
         Width           =   5175
         _Version        =   393218
         _ExtentX        =   9128
         _ExtentY        =   5953
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   4
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   333
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   2
         minor_6         =   1
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   11
         rMax_15         =   185
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   2
         minor_13        =   1
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Array[1]_3      =   20
         ClassName_20    =   "CCWDataPlot"
         opts_20         =   4194367
         Name_20         =   "Plot-2"
         C[0]_20         =   255
         C[1]_20         =   255
         C[2]_20         =   16711680
         C[3]_20         =   16776960
         Event_20        =   2
         X_20            =   6
         Y_20            =   13
         LineStyle_20    =   2
         LineWidth_20    =   1
         BasePlot_20     =   0
         DefaultXInc_20  =   1
         DefaultPlotPerRow_20=   -1  'True
         Array[2]_3      =   21
         ClassName_21    =   "CCWDataPlot"
         opts_21         =   4194367
         Name_21         =   "Plot-3"
         C[0]_21         =   65535
         C[1]_21         =   255
         C[2]_21         =   16711680
         C[3]_21         =   16776960
         Event_21        =   2
         X_21            =   6
         Y_21            =   13
         LineStyle_21    =   2
         LineWidth_21    =   1
         BasePlot_21     =   0
         DefaultXInc_21  =   1
         DefaultPlotPerRow_21=   -1  'True
         Array[3]_3      =   22
         ClassName_22    =   "CCWDataPlot"
         opts_22         =   4194367
         Name_22         =   "Plot-4"
         C[0]_22         =   16711935
         C[1]_22         =   255
         C[2]_22         =   16711680
         C[3]_22         =   16776960
         Event_22        =   2
         X_22            =   6
         Y_22            =   13
         LineStyle_22    =   2
         LineWidth_22    =   1
         BasePlot_22     =   0
         DefaultXInc_22  =   1
         DefaultPlotPerRow_22=   -1  'True
         Axes_1          =   23
         ClassName_23    =   "CCWAxes"
         Array_23        =   2
         Editor_23       =   24
         ClassName_24    =   "CCWGFAxisArrayEditor"
         Owner_24        =   1
         Array[0]_23     =   6
         Array[1]_23     =   13
         DefaultPlot_1   =   25
         ClassName_25    =   "CCWDataPlot"
         opts_25         =   4194367
         Name_25         =   "[Template]"
         C[0]_25         =   65280
         C[1]_25         =   255
         C[2]_25         =   16711680
         C[3]_25         =   16776960
         Event_25        =   2
         X_25            =   6
         Y_25            =   13
         LineStyle_25    =   2
         LineWidth_25    =   1
         BasePlot_25     =   0
         DefaultXInc_25  =   1
         DefaultPlotPerRow_25=   -1  'True
         Cursors_1       =   26
         ClassName_26    =   "CCWCursors"
         Array_26        =   2
         Editor_26       =   27
         ClassName_27    =   "CCWGFCursorArrayEditor"
         Owner_27        =   1
         Array[0]_26     =   28
         ClassName_28    =   "CCWCursor"
         opts_28         =   61
         Name_28         =   "Cursor-1"
         C[0]_28         =   255
         Event_28        =   2
         X_28            =   0
         Y_28            =   0
         XPos_28         =   1
         YPos_28         =   1
         PointIndex_28   =   -1
         ChrosshairStyle_28=   8
         LockPlot_28     =   0
         Array[1]_26     =   29
         ClassName_29    =   "CCWCursor"
         opts_29         =   61
         Name_29         =   "Cursor-2"
         C[0]_29         =   16776960
         Event_29        =   2
         X_29            =   0
         Y_29            =   0
         XPos_29         =   2
         YPos_29         =   2
         PointIndex_29   =   -1
         ChrosshairStyle_29=   8
         LockPlot_29     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   30
         ClassName_30    =   "CCWDrawObj"
         opts_30         =   62
         Image_30        =   31
         ClassName_31    =   "CCWPictImage"
         opts_31         =   1280
         Rows_31         =   1
         Cols_31         =   1
         F_31            =   -2147483633
         B_31            =   -2147483633
         ColorReplaceWith_31=   8421504
         ColorReplace_31 =   8421504
         Tolerance_31    =   2
         Animator_30     =   0
         Blinker_30      =   0
         PlotFrame_1     =   32
         ClassName_32    =   "CCWDrawObj"
         opts_32         =   62
         C[1]_32         =   0
         Image_32        =   33
         ClassName_33    =   "CCWPictImage"
         opts_33         =   1280
         Rows_33         =   1
         Cols_33         =   1
         Pict_33         =   1
         F_33            =   -2147483633
         B_33            =   0
         ColorReplaceWith_33=   8421504
         ColorReplace_33 =   8421504
         Tolerance_33    =   2
         Animator_32     =   0
         Blinker_32      =   0
         Caption_1       =   34
         ClassName_34    =   "CCWDrawObj"
         opts_34         =   62
         C[0]_34         =   -2147483640
         Image_34        =   35
         ClassName_35    =   "CCWTextImage"
         font_35         =   0
         Animator_34     =   0
         Blinker_34      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   36
         ClassName_36    =   "CCWBindingHolderArray"
         Editor_36       =   37
         ClassName_37    =   "CCWBindingHolderArrayEditor"
         Owner_37        =   1
         Annotations_1   =   38
         ClassName_38    =   "CCWAnnotations"
         Editor_38       =   39
         ClassName_39    =   "CCWAnnotationArrayEditor"
         Owner_39        =   1
         AnnotationTemplate_1=   40
         ClassName_40    =   "CCWAnnotation"
         opts_40         =   63
         Name_40         =   "[Template]"
         Plot_40         =   41
         ClassName_41    =   "CCWDataPlot"
         opts_41         =   4194367
         Name_41         =   "[Template]"
         C[0]_41         =   65280
         C[1]_41         =   255
         C[2]_41         =   16711680
         C[3]_41         =   16776960
         Event_41        =   2
         X_41            =   6
         Y_41            =   13
         LineStyle_41    =   1
         LineWidth_41    =   1
         BasePlot_41     =   0
         DefaultXInc_41  =   1
         DefaultPlotPerRow_41=   -1  'True
         Text_40         =   "[Template]"
         TextXPoint_40   =   6.7
         TextYPoint_40   =   6.7
         TextColor_40    =   16777215
         TextFont_40     =   42
         ClassName_42    =   "CCWFont"
         bFont_42        =   -1  'True
         BeginProperty Font_42 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_40 =   43
         ClassName_43    =   "CDataBuffer"
         Type_43         =   5
         m_cDims;_43     =   1
         m_cElts_43      =   1
         Element[0]_43   =   3.3
         ShapeYPoints_40 =   44
         ClassName_44    =   "CDataBuffer"
         Type_44         =   5
         m_cDims;_44     =   1
         m_cElts_44      =   1
         Element[0]_44   =   3.3
         ShapeFillColor_40=   16777215
         ShapeLineColor_40=   16777215
         ShapeLineWidth_40=   1
         ShapeLineStyle_40=   1
         ShapePointStyle_40=   10
         ShapeImage_40   =   45
         ClassName_45    =   "CCWDrawObj"
         opts_45         =   62
         Image_45        =   46
         ClassName_46    =   "CCWPictImage"
         opts_46         =   1280
         Rows_46         =   1
         Cols_46         =   1
         Pict_46         =   7
         F_46            =   -2147483633
         B_46            =   -2147483633
         ColorReplaceWith_46=   8421504
         ColorReplace_46 =   8421504
         Tolerance_46    =   2
         Animator_45     =   0
         Blinker_45      =   0
         ArrowVisible_40 =   -1  'True
         ArrowColor_40   =   16777215
         ArrowWidth_40   =   1
         ArrowLineStyle_40=   1
         ArrowHeadStyle_40=   1
      End
      Begin CWUIControlsLib.CWGraph ListModeTracesChs4To7 
         Height          =   3375
         Left            =   -69240
         TabIndex        =   17
         Top             =   600
         Width           =   5175
         _Version        =   393218
         _ExtentX        =   9128
         _ExtentY        =   5953
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   4
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   333
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   2
         minor_6         =   1
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   11
         rMax_15         =   185
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   2
         minor_13        =   1
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Array[1]_3      =   20
         ClassName_20    =   "CCWDataPlot"
         opts_20         =   4194367
         Name_20         =   "Plot-2"
         C[0]_20         =   255
         C[1]_20         =   255
         C[2]_20         =   16711680
         C[3]_20         =   16776960
         Event_20        =   2
         X_20            =   6
         Y_20            =   13
         LineStyle_20    =   2
         LineWidth_20    =   1
         BasePlot_20     =   0
         DefaultXInc_20  =   1
         DefaultPlotPerRow_20=   -1  'True
         Array[2]_3      =   21
         ClassName_21    =   "CCWDataPlot"
         opts_21         =   4194367
         Name_21         =   "Plot-3"
         C[0]_21         =   65535
         C[1]_21         =   255
         C[2]_21         =   16711680
         C[3]_21         =   16776960
         Event_21        =   2
         X_21            =   6
         Y_21            =   13
         LineStyle_21    =   2
         LineWidth_21    =   1
         BasePlot_21     =   0
         DefaultXInc_21  =   1
         DefaultPlotPerRow_21=   -1  'True
         Array[3]_3      =   22
         ClassName_22    =   "CCWDataPlot"
         opts_22         =   4194367
         Name_22         =   "Plot-4"
         C[0]_22         =   16711935
         C[1]_22         =   255
         C[2]_22         =   16711680
         C[3]_22         =   16776960
         Event_22        =   2
         X_22            =   6
         Y_22            =   13
         LineStyle_22    =   2
         LineWidth_22    =   1
         BasePlot_22     =   0
         DefaultXInc_22  =   1
         DefaultPlotPerRow_22=   -1  'True
         Axes_1          =   23
         ClassName_23    =   "CCWAxes"
         Array_23        =   2
         Editor_23       =   24
         ClassName_24    =   "CCWGFAxisArrayEditor"
         Owner_24        =   1
         Array[0]_23     =   6
         Array[1]_23     =   13
         DefaultPlot_1   =   25
         ClassName_25    =   "CCWDataPlot"
         opts_25         =   4194367
         Name_25         =   "[Template]"
         C[0]_25         =   65280
         C[1]_25         =   255
         C[2]_25         =   16711680
         C[3]_25         =   16776960
         Event_25        =   2
         X_25            =   6
         Y_25            =   13
         LineStyle_25    =   2
         LineWidth_25    =   1
         BasePlot_25     =   0
         DefaultXInc_25  =   1
         DefaultPlotPerRow_25=   -1  'True
         Cursors_1       =   26
         ClassName_26    =   "CCWCursors"
         Array_26        =   2
         Editor_26       =   27
         ClassName_27    =   "CCWGFCursorArrayEditor"
         Owner_27        =   1
         Array[0]_26     =   28
         ClassName_28    =   "CCWCursor"
         opts_28         =   61
         Name_28         =   "Cursor-1"
         C[0]_28         =   255
         Event_28        =   2
         X_28            =   0
         Y_28            =   0
         XPos_28         =   1
         YPos_28         =   1
         PointIndex_28   =   -1
         ChrosshairStyle_28=   8
         LockPlot_28     =   0
         Array[1]_26     =   29
         ClassName_29    =   "CCWCursor"
         opts_29         =   61
         Name_29         =   "Cursor-2"
         C[0]_29         =   16776960
         Event_29        =   2
         X_29            =   0
         Y_29            =   0
         XPos_29         =   2
         YPos_29         =   2
         PointIndex_29   =   -1
         ChrosshairStyle_29=   8
         LockPlot_29     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   30
         ClassName_30    =   "CCWDrawObj"
         opts_30         =   62
         Image_30        =   31
         ClassName_31    =   "CCWPictImage"
         opts_31         =   1280
         Rows_31         =   1
         Cols_31         =   1
         F_31            =   -2147483633
         B_31            =   -2147483633
         ColorReplaceWith_31=   8421504
         ColorReplace_31 =   8421504
         Tolerance_31    =   2
         Animator_30     =   0
         Blinker_30      =   0
         PlotFrame_1     =   32
         ClassName_32    =   "CCWDrawObj"
         opts_32         =   62
         C[1]_32         =   0
         Image_32        =   33
         ClassName_33    =   "CCWPictImage"
         opts_33         =   1280
         Rows_33         =   1
         Cols_33         =   1
         Pict_33         =   1
         F_33            =   -2147483633
         B_33            =   0
         ColorReplaceWith_33=   8421504
         ColorReplace_33 =   8421504
         Tolerance_33    =   2
         Animator_32     =   0
         Blinker_32      =   0
         Caption_1       =   34
         ClassName_34    =   "CCWDrawObj"
         opts_34         =   62
         C[0]_34         =   -2147483640
         Image_34        =   35
         ClassName_35    =   "CCWTextImage"
         font_35         =   0
         Animator_34     =   0
         Blinker_34      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   36
         ClassName_36    =   "CCWBindingHolderArray"
         Editor_36       =   37
         ClassName_37    =   "CCWBindingHolderArrayEditor"
         Owner_37        =   1
         Annotations_1   =   38
         ClassName_38    =   "CCWAnnotations"
         Editor_38       =   39
         ClassName_39    =   "CCWAnnotationArrayEditor"
         Owner_39        =   1
         AnnotationTemplate_1=   40
         ClassName_40    =   "CCWAnnotation"
         opts_40         =   63
         Name_40         =   "[Template]"
         Plot_40         =   41
         ClassName_41    =   "CCWDataPlot"
         opts_41         =   4194367
         Name_41         =   "[Template]"
         C[0]_41         =   65280
         C[1]_41         =   255
         C[2]_41         =   16711680
         C[3]_41         =   16776960
         Event_41        =   2
         X_41            =   6
         Y_41            =   13
         LineStyle_41    =   1
         LineWidth_41    =   1
         BasePlot_41     =   0
         DefaultXInc_41  =   1
         DefaultPlotPerRow_41=   -1  'True
         Text_40         =   "[Template]"
         TextXPoint_40   =   6.7
         TextYPoint_40   =   6.7
         TextColor_40    =   16777215
         TextFont_40     =   42
         ClassName_42    =   "CCWFont"
         bFont_42        =   -1  'True
         BeginProperty Font_42 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_40 =   43
         ClassName_43    =   "CDataBuffer"
         Type_43         =   5
         m_cDims;_43     =   1
         m_cElts_43      =   1
         Element[0]_43   =   3.3
         ShapeYPoints_40 =   44
         ClassName_44    =   "CDataBuffer"
         Type_44         =   5
         m_cDims;_44     =   1
         m_cElts_44      =   1
         Element[0]_44   =   3.3
         ShapeFillColor_40=   16777215
         ShapeLineColor_40=   16777215
         ShapeLineWidth_40=   1
         ShapeLineStyle_40=   1
         ShapePointStyle_40=   10
         ShapeImage_40   =   45
         ClassName_45    =   "CCWDrawObj"
         opts_45         =   62
         Image_45        =   46
         ClassName_46    =   "CCWPictImage"
         opts_46         =   1280
         Rows_46         =   1
         Cols_46         =   1
         Pict_46         =   7
         F_46            =   -2147483633
         B_46            =   -2147483633
         ColorReplaceWith_46=   8421504
         ColorReplace_46 =   8421504
         Tolerance_46    =   2
         Animator_45     =   0
         Blinker_45      =   0
         ArrowVisible_40 =   -1  'True
         ArrowColor_40   =   16777215
         ArrowWidth_40   =   1
         ArrowLineStyle_40=   1
         ArrowHeadStyle_40=   1
      End
      Begin CWUIControlsLib.CWGraph ListModeTracesChs8To11 
         Height          =   3375
         Left            =   -74760
         TabIndex        =   18
         Top             =   4680
         Width           =   5175
         _Version        =   393218
         _ExtentX        =   9128
         _ExtentY        =   5953
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   4
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   333
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   2
         minor_6         =   1
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   11
         rMax_15         =   185
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   2
         minor_13        =   1
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Array[1]_3      =   20
         ClassName_20    =   "CCWDataPlot"
         opts_20         =   4194367
         Name_20         =   "Plot-2"
         C[0]_20         =   255
         C[1]_20         =   255
         C[2]_20         =   16711680
         C[3]_20         =   16776960
         Event_20        =   2
         X_20            =   6
         Y_20            =   13
         LineStyle_20    =   2
         LineWidth_20    =   1
         BasePlot_20     =   0
         DefaultXInc_20  =   1
         DefaultPlotPerRow_20=   -1  'True
         Array[2]_3      =   21
         ClassName_21    =   "CCWDataPlot"
         opts_21         =   4194367
         Name_21         =   "Plot-3"
         C[0]_21         =   65535
         C[1]_21         =   255
         C[2]_21         =   16711680
         C[3]_21         =   16776960
         Event_21        =   2
         X_21            =   6
         Y_21            =   13
         LineStyle_21    =   2
         LineWidth_21    =   1
         BasePlot_21     =   0
         DefaultXInc_21  =   1
         DefaultPlotPerRow_21=   -1  'True
         Array[3]_3      =   22
         ClassName_22    =   "CCWDataPlot"
         opts_22         =   4194367
         Name_22         =   "Plot-4"
         C[0]_22         =   16711935
         C[1]_22         =   255
         C[2]_22         =   16711680
         C[3]_22         =   16776960
         Event_22        =   2
         X_22            =   6
         Y_22            =   13
         LineStyle_22    =   2
         LineWidth_22    =   1
         BasePlot_22     =   0
         DefaultXInc_22  =   1
         DefaultPlotPerRow_22=   -1  'True
         Axes_1          =   23
         ClassName_23    =   "CCWAxes"
         Array_23        =   2
         Editor_23       =   24
         ClassName_24    =   "CCWGFAxisArrayEditor"
         Owner_24        =   1
         Array[0]_23     =   6
         Array[1]_23     =   13
         DefaultPlot_1   =   25
         ClassName_25    =   "CCWDataPlot"
         opts_25         =   4194367
         Name_25         =   "[Template]"
         C[0]_25         =   65280
         C[1]_25         =   255
         C[2]_25         =   16711680
         C[3]_25         =   16776960
         Event_25        =   2
         X_25            =   6
         Y_25            =   13
         LineStyle_25    =   2
         LineWidth_25    =   1
         BasePlot_25     =   0
         DefaultXInc_25  =   1
         DefaultPlotPerRow_25=   -1  'True
         Cursors_1       =   26
         ClassName_26    =   "CCWCursors"
         Array_26        =   2
         Editor_26       =   27
         ClassName_27    =   "CCWGFCursorArrayEditor"
         Owner_27        =   1
         Array[0]_26     =   28
         ClassName_28    =   "CCWCursor"
         opts_28         =   61
         Name_28         =   "Cursor-1"
         C[0]_28         =   255
         Event_28        =   2
         X_28            =   0
         Y_28            =   0
         XPos_28         =   1
         YPos_28         =   1
         PointIndex_28   =   -1
         ChrosshairStyle_28=   8
         LockPlot_28     =   0
         Array[1]_26     =   29
         ClassName_29    =   "CCWCursor"
         opts_29         =   61
         Name_29         =   "Cursor-2"
         C[0]_29         =   16776960
         Event_29        =   2
         X_29            =   0
         Y_29            =   0
         XPos_29         =   2
         YPos_29         =   2
         PointIndex_29   =   -1
         ChrosshairStyle_29=   8
         LockPlot_29     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   30
         ClassName_30    =   "CCWDrawObj"
         opts_30         =   62
         Image_30        =   31
         ClassName_31    =   "CCWPictImage"
         opts_31         =   1280
         Rows_31         =   1
         Cols_31         =   1
         F_31            =   -2147483633
         B_31            =   -2147483633
         ColorReplaceWith_31=   8421504
         ColorReplace_31 =   8421504
         Tolerance_31    =   2
         Animator_30     =   0
         Blinker_30      =   0
         PlotFrame_1     =   32
         ClassName_32    =   "CCWDrawObj"
         opts_32         =   62
         C[1]_32         =   0
         Image_32        =   33
         ClassName_33    =   "CCWPictImage"
         opts_33         =   1280
         Rows_33         =   1
         Cols_33         =   1
         Pict_33         =   1
         F_33            =   -2147483633
         B_33            =   0
         ColorReplaceWith_33=   8421504
         ColorReplace_33 =   8421504
         Tolerance_33    =   2
         Animator_32     =   0
         Blinker_32      =   0
         Caption_1       =   34
         ClassName_34    =   "CCWDrawObj"
         opts_34         =   62
         C[0]_34         =   -2147483640
         Image_34        =   35
         ClassName_35    =   "CCWTextImage"
         font_35         =   0
         Animator_34     =   0
         Blinker_34      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   36
         ClassName_36    =   "CCWBindingHolderArray"
         Editor_36       =   37
         ClassName_37    =   "CCWBindingHolderArrayEditor"
         Owner_37        =   1
         Annotations_1   =   38
         ClassName_38    =   "CCWAnnotations"
         Editor_38       =   39
         ClassName_39    =   "CCWAnnotationArrayEditor"
         Owner_39        =   1
         AnnotationTemplate_1=   40
         ClassName_40    =   "CCWAnnotation"
         opts_40         =   63
         Name_40         =   "[Template]"
         Plot_40         =   41
         ClassName_41    =   "CCWDataPlot"
         opts_41         =   4194367
         Name_41         =   "[Template]"
         C[0]_41         =   65280
         C[1]_41         =   255
         C[2]_41         =   16711680
         C[3]_41         =   16776960
         Event_41        =   2
         X_41            =   6
         Y_41            =   13
         LineStyle_41    =   1
         LineWidth_41    =   1
         BasePlot_41     =   0
         DefaultXInc_41  =   1
         DefaultPlotPerRow_41=   -1  'True
         Text_40         =   "[Template]"
         TextXPoint_40   =   6.7
         TextYPoint_40   =   6.7
         TextColor_40    =   16777215
         TextFont_40     =   42
         ClassName_42    =   "CCWFont"
         bFont_42        =   -1  'True
         BeginProperty Font_42 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_40 =   43
         ClassName_43    =   "CDataBuffer"
         Type_43         =   5
         m_cDims;_43     =   1
         m_cElts_43      =   1
         Element[0]_43   =   3.3
         ShapeYPoints_40 =   44
         ClassName_44    =   "CDataBuffer"
         Type_44         =   5
         m_cDims;_44     =   1
         m_cElts_44      =   1
         Element[0]_44   =   3.3
         ShapeFillColor_40=   16777215
         ShapeLineColor_40=   16777215
         ShapeLineWidth_40=   1
         ShapeLineStyle_40=   1
         ShapePointStyle_40=   10
         ShapeImage_40   =   45
         ClassName_45    =   "CCWDrawObj"
         opts_45         =   62
         Image_45        =   46
         ClassName_46    =   "CCWPictImage"
         opts_46         =   1280
         Rows_46         =   1
         Cols_46         =   1
         Pict_46         =   7
         F_46            =   -2147483633
         B_46            =   -2147483633
         ColorReplaceWith_46=   8421504
         ColorReplace_46 =   8421504
         Tolerance_46    =   2
         Animator_45     =   0
         Blinker_45      =   0
         ArrowVisible_40 =   -1  'True
         ArrowColor_40   =   16777215
         ArrowWidth_40   =   1
         ArrowLineStyle_40=   1
         ArrowHeadStyle_40=   1
      End
      Begin CWUIControlsLib.CWGraph ListModeTracesChs12To15 
         Height          =   3375
         Left            =   -69240
         TabIndex        =   19
         Top             =   4680
         Width           =   5175
         _Version        =   393218
         _ExtentX        =   9128
         _ExtentY        =   5953
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   4
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   333
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   2
         minor_6         =   1
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   11
         rMax_15         =   185
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   2
         minor_13        =   1
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Array[1]_3      =   20
         ClassName_20    =   "CCWDataPlot"
         opts_20         =   4194367
         Name_20         =   "Plot-2"
         C[0]_20         =   255
         C[1]_20         =   255
         C[2]_20         =   16711680
         C[3]_20         =   16776960
         Event_20        =   2
         X_20            =   6
         Y_20            =   13
         LineStyle_20    =   2
         LineWidth_20    =   1
         BasePlot_20     =   0
         DefaultXInc_20  =   1
         DefaultPlotPerRow_20=   -1  'True
         Array[2]_3      =   21
         ClassName_21    =   "CCWDataPlot"
         opts_21         =   4194367
         Name_21         =   "Plot-3"
         C[0]_21         =   65535
         C[1]_21         =   255
         C[2]_21         =   16711680
         C[3]_21         =   16776960
         Event_21        =   2
         X_21            =   6
         Y_21            =   13
         LineStyle_21    =   2
         LineWidth_21    =   1
         BasePlot_21     =   0
         DefaultXInc_21  =   1
         DefaultPlotPerRow_21=   -1  'True
         Array[3]_3      =   22
         ClassName_22    =   "CCWDataPlot"
         opts_22         =   4194367
         Name_22         =   "Plot-4"
         C[0]_22         =   16711935
         C[1]_22         =   255
         C[2]_22         =   16711680
         C[3]_22         =   16776960
         Event_22        =   2
         X_22            =   6
         Y_22            =   13
         LineStyle_22    =   2
         LineWidth_22    =   1
         BasePlot_22     =   0
         DefaultXInc_22  =   1
         DefaultPlotPerRow_22=   -1  'True
         Axes_1          =   23
         ClassName_23    =   "CCWAxes"
         Array_23        =   2
         Editor_23       =   24
         ClassName_24    =   "CCWGFAxisArrayEditor"
         Owner_24        =   1
         Array[0]_23     =   6
         Array[1]_23     =   13
         DefaultPlot_1   =   25
         ClassName_25    =   "CCWDataPlot"
         opts_25         =   4194367
         Name_25         =   "[Template]"
         C[0]_25         =   65280
         C[1]_25         =   255
         C[2]_25         =   16711680
         C[3]_25         =   16776960
         Event_25        =   2
         X_25            =   6
         Y_25            =   13
         LineStyle_25    =   2
         LineWidth_25    =   1
         BasePlot_25     =   0
         DefaultXInc_25  =   1
         DefaultPlotPerRow_25=   -1  'True
         Cursors_1       =   26
         ClassName_26    =   "CCWCursors"
         Array_26        =   2
         Editor_26       =   27
         ClassName_27    =   "CCWGFCursorArrayEditor"
         Owner_27        =   1
         Array[0]_26     =   28
         ClassName_28    =   "CCWCursor"
         opts_28         =   61
         Name_28         =   "Cursor-1"
         C[0]_28         =   255
         Event_28        =   2
         X_28            =   0
         Y_28            =   0
         XPos_28         =   1
         YPos_28         =   1
         PointIndex_28   =   -1
         ChrosshairStyle_28=   8
         LockPlot_28     =   0
         Array[1]_26     =   29
         ClassName_29    =   "CCWCursor"
         opts_29         =   61
         Name_29         =   "Cursor-2"
         C[0]_29         =   16776960
         Event_29        =   2
         X_29            =   0
         Y_29            =   0
         XPos_29         =   2
         YPos_29         =   2
         PointIndex_29   =   -1
         ChrosshairStyle_29=   8
         LockPlot_29     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   30
         ClassName_30    =   "CCWDrawObj"
         opts_30         =   62
         Image_30        =   31
         ClassName_31    =   "CCWPictImage"
         opts_31         =   1280
         Rows_31         =   1
         Cols_31         =   1
         F_31            =   -2147483633
         B_31            =   -2147483633
         ColorReplaceWith_31=   8421504
         ColorReplace_31 =   8421504
         Tolerance_31    =   2
         Animator_30     =   0
         Blinker_30      =   0
         PlotFrame_1     =   32
         ClassName_32    =   "CCWDrawObj"
         opts_32         =   62
         C[1]_32         =   0
         Image_32        =   33
         ClassName_33    =   "CCWPictImage"
         opts_33         =   1280
         Rows_33         =   1
         Cols_33         =   1
         Pict_33         =   1
         F_33            =   -2147483633
         B_33            =   0
         ColorReplaceWith_33=   8421504
         ColorReplace_33 =   8421504
         Tolerance_33    =   2
         Animator_32     =   0
         Blinker_32      =   0
         Caption_1       =   34
         ClassName_34    =   "CCWDrawObj"
         opts_34         =   62
         C[0]_34         =   -2147483640
         Image_34        =   35
         ClassName_35    =   "CCWTextImage"
         font_35         =   0
         Animator_34     =   0
         Blinker_34      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   36
         ClassName_36    =   "CCWBindingHolderArray"
         Editor_36       =   37
         ClassName_37    =   "CCWBindingHolderArrayEditor"
         Owner_37        =   1
         Annotations_1   =   38
         ClassName_38    =   "CCWAnnotations"
         Editor_38       =   39
         ClassName_39    =   "CCWAnnotationArrayEditor"
         Owner_39        =   1
         AnnotationTemplate_1=   40
         ClassName_40    =   "CCWAnnotation"
         opts_40         =   63
         Name_40         =   "[Template]"
         Plot_40         =   41
         ClassName_41    =   "CCWDataPlot"
         opts_41         =   4194367
         Name_41         =   "[Template]"
         C[0]_41         =   65280
         C[1]_41         =   255
         C[2]_41         =   16711680
         C[3]_41         =   16776960
         Event_41        =   2
         X_41            =   6
         Y_41            =   13
         LineStyle_41    =   1
         LineWidth_41    =   1
         BasePlot_41     =   0
         DefaultXInc_41  =   1
         DefaultPlotPerRow_41=   -1  'True
         Text_40         =   "[Template]"
         TextXPoint_40   =   6.7
         TextYPoint_40   =   6.7
         TextColor_40    =   16777215
         TextFont_40     =   42
         ClassName_42    =   "CCWFont"
         bFont_42        =   -1  'True
         BeginProperty Font_42 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_40 =   43
         ClassName_43    =   "CDataBuffer"
         Type_43         =   5
         m_cDims;_43     =   1
         m_cElts_43      =   1
         Element[0]_43   =   3.3
         ShapeYPoints_40 =   44
         ClassName_44    =   "CDataBuffer"
         Type_44         =   5
         m_cDims;_44     =   1
         m_cElts_44      =   1
         Element[0]_44   =   3.3
         ShapeFillColor_40=   16777215
         ShapeLineColor_40=   16777215
         ShapeLineWidth_40=   1
         ShapeLineStyle_40=   1
         ShapePointStyle_40=   10
         ShapeImage_40   =   45
         ClassName_45    =   "CCWDrawObj"
         opts_45         =   62
         Image_45        =   46
         ClassName_46    =   "CCWPictImage"
         opts_46         =   1280
         Rows_46         =   1
         Cols_46         =   1
         Pict_46         =   7
         F_46            =   -2147483633
         B_46            =   -2147483633
         ColorReplaceWith_46=   8421504
         ColorReplace_46 =   8421504
         Tolerance_46    =   2
         Animator_45     =   0
         Blinker_45      =   0
         ArrowVisible_40 =   -1  'True
         ArrowColor_40   =   16777215
         ArrowWidth_40   =   1
         ArrowLineStyle_40=   1
         ArrowHeadStyle_40=   1
      End
      Begin CWUIControlsLib.CWGraph LMTraceFastFilterGraph 
         Height          =   3795
         Left            =   -74760
         TabIndex        =   48
         Top             =   425
         Width           =   10695
         _Version        =   393218
         _ExtentX        =   18865
         _ExtentY        =   6703
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.26
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   3
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   701
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   1
         minor_6         =   0.5
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   24
         rMax_15         =   213
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   2
         minor_13        =   1
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Array[1]_3      =   20
         ClassName_20    =   "CCWDataPlot"
         opts_20         =   4194367
         Name_20         =   "Plot-2"
         C[0]_20         =   255
         C[1]_20         =   255
         C[2]_20         =   16711680
         C[3]_20         =   16776960
         Event_20        =   2
         X_20            =   6
         Y_20            =   13
         LineStyle_20    =   2
         LineWidth_20    =   1
         BasePlot_20     =   0
         DefaultXInc_20  =   1
         DefaultPlotPerRow_20=   -1  'True
         Array[2]_3      =   21
         ClassName_21    =   "CCWDataPlot"
         opts_21         =   4194367
         Name_21         =   "Plot-3"
         C[0]_21         =   65535
         C[1]_21         =   255
         C[2]_21         =   16711680
         C[3]_21         =   16776960
         Event_21        =   2
         X_21            =   6
         Y_21            =   13
         LineStyle_21    =   2
         LineWidth_21    =   1
         BasePlot_21     =   0
         DefaultXInc_21  =   1
         DefaultPlotPerRow_21=   -1  'True
         Axes_1          =   22
         ClassName_22    =   "CCWAxes"
         Array_22        =   2
         Editor_22       =   23
         ClassName_23    =   "CCWGFAxisArrayEditor"
         Owner_23        =   1
         Array[0]_22     =   6
         Array[1]_22     =   13
         DefaultPlot_1   =   24
         ClassName_24    =   "CCWDataPlot"
         opts_24         =   4194367
         Name_24         =   "[Template]"
         C[0]_24         =   65280
         C[1]_24         =   255
         C[2]_24         =   16711680
         C[3]_24         =   16776960
         Event_24        =   2
         X_24            =   6
         Y_24            =   13
         LineStyle_24    =   2
         LineWidth_24    =   1
         BasePlot_24     =   0
         DefaultXInc_24  =   1
         DefaultPlotPerRow_24=   -1  'True
         Cursors_1       =   25
         ClassName_25    =   "CCWCursors"
         Array_25        =   2
         Editor_25       =   26
         ClassName_26    =   "CCWGFCursorArrayEditor"
         Owner_26        =   1
         Array[0]_25     =   27
         ClassName_27    =   "CCWCursor"
         opts_27         =   61
         Name_27         =   "Cursor-1"
         C[0]_27         =   255
         Event_27        =   2
         X_27            =   0
         Y_27            =   0
         XPos_27         =   1
         YPos_27         =   1
         PointIndex_27   =   -1
         ChrosshairStyle_27=   8
         LockPlot_27     =   0
         Array[1]_25     =   28
         ClassName_28    =   "CCWCursor"
         opts_28         =   61
         Name_28         =   "Cursor-2"
         C[0]_28         =   16776960
         Event_28        =   2
         X_28            =   0
         Y_28            =   0
         XPos_28         =   2
         YPos_28         =   2
         PointIndex_28   =   -1
         ChrosshairStyle_28=   8
         LockPlot_28     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   29
         ClassName_29    =   "CCWDrawObj"
         opts_29         =   62
         Image_29        =   30
         ClassName_30    =   "CCWPictImage"
         opts_30         =   1280
         Rows_30         =   1
         Cols_30         =   1
         F_30            =   -2147483633
         B_30            =   -2147483633
         ColorReplaceWith_30=   8421504
         ColorReplace_30 =   8421504
         Tolerance_30    =   2
         Animator_29     =   0
         Blinker_29      =   0
         PlotFrame_1     =   31
         ClassName_31    =   "CCWDrawObj"
         opts_31         =   62
         C[1]_31         =   0
         Image_31        =   32
         ClassName_32    =   "CCWPictImage"
         opts_32         =   1280
         Rows_32         =   1
         Cols_32         =   1
         Pict_32         =   1
         F_32            =   -2147483633
         B_32            =   0
         ColorReplaceWith_32=   8421504
         ColorReplace_32 =   8421504
         Tolerance_32    =   2
         Animator_31     =   0
         Blinker_31      =   0
         Caption_1       =   33
         ClassName_33    =   "CCWDrawObj"
         opts_33         =   62
         C[0]_33         =   -2147483640
         Image_33        =   34
         ClassName_34    =   "CCWTextImage"
         szText_34       =   "Fast Filter Responses"
         font_34         =   0
         Animator_33     =   0
         Blinker_33      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   35
         ClassName_35    =   "CCWBindingHolderArray"
         Editor_35       =   36
         ClassName_36    =   "CCWBindingHolderArrayEditor"
         Owner_36        =   1
         Annotations_1   =   37
         ClassName_37    =   "CCWAnnotations"
         Editor_37       =   38
         ClassName_38    =   "CCWAnnotationArrayEditor"
         Owner_38        =   1
         AnnotationTemplate_1=   39
         ClassName_39    =   "CCWAnnotation"
         opts_39         =   63
         Name_39         =   "[Template]"
         Plot_39         =   40
         ClassName_40    =   "CCWDataPlot"
         opts_40         =   4194367
         Name_40         =   "[Template]"
         C[0]_40         =   65280
         C[1]_40         =   255
         C[2]_40         =   16711680
         C[3]_40         =   16776960
         Event_40        =   2
         X_40            =   6
         Y_40            =   13
         LineStyle_40    =   1
         LineWidth_40    =   1
         BasePlot_40     =   0
         DefaultXInc_40  =   1
         DefaultPlotPerRow_40=   -1  'True
         Text_39         =   "[Template]"
         TextXPoint_39   =   6.7
         TextYPoint_39   =   6.7
         TextColor_39    =   16777215
         TextFont_39     =   41
         ClassName_41    =   "CCWFont"
         bFont_41        =   -1  'True
         BeginProperty Font_41 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_39 =   42
         ClassName_42    =   "CDataBuffer"
         Type_42         =   5
         m_cDims;_42     =   1
         m_cElts_42      =   1
         Element[0]_42   =   3.3
         ShapeYPoints_39 =   43
         ClassName_43    =   "CDataBuffer"
         Type_43         =   5
         m_cDims;_43     =   1
         m_cElts_43      =   1
         Element[0]_43   =   3.3
         ShapeFillColor_39=   16777215
         ShapeLineColor_39=   16777215
         ShapeLineWidth_39=   1
         ShapeLineStyle_39=   1
         ShapePointStyle_39=   10
         ShapeImage_39   =   44
         ClassName_44    =   "CCWDrawObj"
         opts_44         =   62
         Image_44        =   45
         ClassName_45    =   "CCWPictImage"
         opts_45         =   1280
         Rows_45         =   1
         Cols_45         =   1
         Pict_45         =   7
         F_45            =   -2147483633
         B_45            =   -2147483633
         ColorReplaceWith_45=   8421504
         ColorReplace_45 =   8421504
         Tolerance_45    =   2
         Animator_44     =   0
         Blinker_44      =   0
         ArrowVisible_39 =   -1  'True
         ArrowColor_39   =   16777215
         ArrowWidth_39   =   1
         ArrowLineStyle_39=   1
         ArrowHeadStyle_39=   1
      End
      Begin CWUIControlsLib.CWGraph LMTraceSlowFilterGraph 
         Height          =   3795
         Left            =   -74760
         TabIndex        =   49
         Top             =   4440
         Width           =   10695
         _Version        =   393218
         _ExtentX        =   18865
         _ExtentY        =   6703
         _StockProps     =   71
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.26
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Reset_0         =   0   'False
         CompatibleVers_0=   393218
         Graph_0         =   1
         ClassName_1     =   "CCWGraphFrame"
         opts_1          =   30
         C[0]_1          =   0
         Event_1         =   2
         ClassName_2     =   "CCWGFPlotEvent"
         Owner_2         =   1
         Plots_1         =   3
         ClassName_3     =   "CCWDataPlots"
         Array_3         =   2
         Editor_3        =   4
         ClassName_4     =   "CCWGFPlotArrayEditor"
         Owner_4         =   1
         Array[0]_3      =   5
         ClassName_5     =   "CCWDataPlot"
         opts_5          =   4194367
         Name_5          =   "Plot-1"
         C[0]_5          =   65280
         C[1]_5          =   255
         C[2]_5          =   16711680
         C[3]_5          =   16776960
         Event_5         =   2
         X_5             =   6
         ClassName_6     =   "CCWAxis"
         opts_6          =   1599
         Name_6          =   "XAxis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   701
         dMax_8          =   10
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   1
         minor_6         =   0.5
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Trace Length (탎)"
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1599
         Name_13         =   "YAxis-1"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   24
         rMax_15         =   213
         dMax_15         =   10
         discInterval_15 =   1
         Radial_13       =   0
         Enum_13         =   16
         ClassName_16    =   "CCWEnum"
         Editor_16       =   17
         ClassName_17    =   "CCWEnumArrayEditor"
         Owner_17        =   13
         Font_13         =   0
         tickopts_13     =   2711
         major_13        =   2
         minor_13        =   1
         Caption_13      =   18
         ClassName_18    =   "CCWDrawObj"
         opts_18         =   62
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "ADC units"
         font_19         =   0
         Animator_18     =   0
         Blinker_18      =   0
         LineStyle_5     =   2
         LineWidth_5     =   1
         BasePlot_5      =   0
         DefaultXInc_5   =   1
         DefaultPlotPerRow_5=   -1  'True
         Array[1]_3      =   20
         ClassName_20    =   "CCWDataPlot"
         opts_20         =   4194367
         Name_20         =   "Plot-2"
         C[0]_20         =   255
         C[1]_20         =   255
         C[2]_20         =   16711680
         C[3]_20         =   16776960
         Event_20        =   2
         X_20            =   6
         Y_20            =   13
         LineStyle_20    =   2
         LineWidth_20    =   1
         BasePlot_20     =   0
         DefaultXInc_20  =   1
         DefaultPlotPerRow_20=   -1  'True
         Axes_1          =   21
         ClassName_21    =   "CCWAxes"
         Array_21        =   2
         Editor_21       =   22
         ClassName_22    =   "CCWGFAxisArrayEditor"
         Owner_22        =   1
         Array[0]_21     =   6
         Array[1]_21     =   13
         DefaultPlot_1   =   23
         ClassName_23    =   "CCWDataPlot"
         opts_23         =   4194367
         Name_23         =   "[Template]"
         C[0]_23         =   65280
         C[1]_23         =   255
         C[2]_23         =   16711680
         C[3]_23         =   16776960
         Event_23        =   2
         X_23            =   6
         Y_23            =   13
         LineStyle_23    =   2
         LineWidth_23    =   1
         BasePlot_23     =   0
         DefaultXInc_23  =   1
         DefaultPlotPerRow_23=   -1  'True
         Cursors_1       =   24
         ClassName_24    =   "CCWCursors"
         Array_24        =   2
         Editor_24       =   25
         ClassName_25    =   "CCWGFCursorArrayEditor"
         Owner_25        =   1
         Array[0]_24     =   26
         ClassName_26    =   "CCWCursor"
         opts_26         =   61
         Name_26         =   "Cursor-1"
         C[0]_26         =   255
         Event_26        =   2
         X_26            =   0
         Y_26            =   0
         XPos_26         =   1
         YPos_26         =   1
         PointIndex_26   =   -1
         ChrosshairStyle_26=   8
         LockPlot_26     =   0
         Array[1]_24     =   27
         ClassName_27    =   "CCWCursor"
         opts_27         =   61
         Name_27         =   "Cursor-2"
         C[0]_27         =   16776960
         Event_27        =   2
         X_27            =   0
         Y_27            =   0
         XPos_27         =   2
         YPos_27         =   2
         PointIndex_27   =   -1
         ChrosshairStyle_27=   8
         LockPlot_27     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   28
         ClassName_28    =   "CCWDrawObj"
         opts_28         =   62
         Image_28        =   29
         ClassName_29    =   "CCWPictImage"
         opts_29         =   1280
         Rows_29         =   1
         Cols_29         =   1
         F_29            =   -2147483633
         B_29            =   -2147483633
         ColorReplaceWith_29=   8421504
         ColorReplace_29 =   8421504
         Tolerance_29    =   2
         Animator_28     =   0
         Blinker_28      =   0
         PlotFrame_1     =   30
         ClassName_30    =   "CCWDrawObj"
         opts_30         =   62
         C[1]_30         =   0
         Image_30        =   31
         ClassName_31    =   "CCWPictImage"
         opts_31         =   1280
         Rows_31         =   1
         Cols_31         =   1
         Pict_31         =   1
         F_31            =   -2147483633
         B_31            =   0
         ColorReplaceWith_31=   8421504
         ColorReplace_31 =   8421504
         Tolerance_31    =   2
         Animator_30     =   0
         Blinker_30      =   0
         Caption_1       =   32
         ClassName_32    =   "CCWDrawObj"
         opts_32         =   62
         C[0]_32         =   -2147483640
         Image_32        =   33
         ClassName_33    =   "CCWTextImage"
         szText_33       =   "Slow Filter Responses"
         font_33         =   0
         Animator_32     =   0
         Blinker_32      =   0
         DefaultXInc_1   =   1
         DefaultPlotPerRow_1=   -1  'True
         Bindings_1      =   34
         ClassName_34    =   "CCWBindingHolderArray"
         Editor_34       =   35
         ClassName_35    =   "CCWBindingHolderArrayEditor"
         Owner_35        =   1
         Annotations_1   =   36
         ClassName_36    =   "CCWAnnotations"
         Editor_36       =   37
         ClassName_37    =   "CCWAnnotationArrayEditor"
         Owner_37        =   1
         AnnotationTemplate_1=   38
         ClassName_38    =   "CCWAnnotation"
         opts_38         =   63
         Name_38         =   "[Template]"
         Plot_38         =   39
         ClassName_39    =   "CCWDataPlot"
         opts_39         =   4194367
         Name_39         =   "[Template]"
         C[0]_39         =   65280
         C[1]_39         =   255
         C[2]_39         =   16711680
         C[3]_39         =   16776960
         Event_39        =   2
         X_39            =   6
         Y_39            =   13
         LineStyle_39    =   1
         LineWidth_39    =   1
         BasePlot_39     =   0
         DefaultXInc_39  =   1
         DefaultPlotPerRow_39=   -1  'True
         Text_38         =   "[Template]"
         TextXPoint_38   =   6.7
         TextYPoint_38   =   6.7
         TextColor_38    =   16777215
         TextFont_38     =   40
         ClassName_40    =   "CCWFont"
         bFont_40        =   -1  'True
         BeginProperty Font_40 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_38 =   41
         ClassName_41    =   "CDataBuffer"
         Type_41         =   5
         m_cDims;_41     =   1
         m_cElts_41      =   1
         Element[0]_41   =   3.3
         ShapeYPoints_38 =   42
         ClassName_42    =   "CDataBuffer"
         Type_42         =   5
         m_cDims;_42     =   1
         m_cElts_42      =   1
         Element[0]_42   =   3.3
         ShapeFillColor_38=   16777215
         ShapeLineColor_38=   16777215
         ShapeLineWidth_38=   1
         ShapeLineStyle_38=   1
         ShapePointStyle_38=   10
         ShapeImage_38   =   43
         ClassName_43    =   "CCWDrawObj"
         opts_43         =   62
         Image_43        =   44
         ClassName_44    =   "CCWPictImage"
         opts_44         =   1280
         Rows_44         =   1
         Cols_44         =   1
         Pict_44         =   7
         F_44            =   -2147483633
         B_44            =   -2147483633
         ColorReplaceWith_44=   8421504
         ColorReplace_44 =   8421504
         Tolerance_44    =   2
         Animator_43     =   0
         Blinker_43      =   0
         ArrowVisible_38 =   -1  'True
         ArrowColor_38   =   16777215
         ArrowWidth_38   =   1
         ArrowLineStyle_38=   1
         ArrowHeadStyle_38=   1
      End
      Begin VB.Label EventChannelNumberLabel 
         Caption         =   "Event channel number"
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
         Left            =   -66960
         TabIndex        =   51
         Top             =   170
         Width           =   2055
      End
      Begin VB.Label dY 
         Caption         =   "dY"
         Height          =   255
         Left            =   1605
         TabIndex        =   14
         Top             =   7290
         Width           =   255
      End
      Begin VB.Label dX 
         Caption         =   "dX"
         Height          =   255
         Left            =   1605
         TabIndex        =   13
         Top             =   6945
         Width           =   255
      End
      Begin VB.Label YPos 
         Caption         =   "Y"
         Height          =   255
         Left            =   255
         TabIndex        =   12
         Top             =   7290
         Width           =   255
      End
      Begin VB.Label XPos 
         Caption         =   "X"
         Height          =   255
         Left            =   240
         TabIndex        =   11
         Top             =   6930
         Width           =   255
      End
      Begin VB.Label SglChMCAHistogramChanNumLabel 
         Caption         =   "Channel Number"
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
         Left            =   8040
         TabIndex        =   2
         Top             =   7200
         Width           =   1575
      End
   End
   Begin CWUIControlsLib.CWNumEdit ListModeEventNumberEdit 
      Height          =   495
      Left            =   9600
      TabIndex        =   44
      Top             =   9840
      Width           =   1695
      _Version        =   393218
      _ExtentX        =   2990
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
      RangeMax_Val_1  =   2147483647
      Bindings_1      =   4
      ClassName_4     =   "CCWBindingHolderArray"
      Editor_4        =   5
      ClassName_5     =   "CCWBindingHolderArrayEditor"
      Owner_5         =   1
   End
   Begin VB.Label SelectEventNumLabel 
      Caption         =   "Select event number"
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
      Left            =   7320
      TabIndex        =   45
      Top             =   9950
      Width           =   2295
   End
   Begin VB.Label TotalListModeNumEventsLabel 
      Caption         =   "Total number of events"
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
      Left            =   3240
      TabIndex        =   43
      Top             =   9885
      Width           =   2175
   End
   Begin VB.Image OpenListModeFile 
      Height          =   225
      Left            =   11040
      Picture         =   "ReadListModeTraces.frx":0054
      Top             =   9240
      Width           =   240
   End
   Begin VB.Label ListModeFileLabel 
      Caption         =   "List mode data file"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   240
      TabIndex        =   41
      Top             =   9240
      Width           =   1815
   End
End
Attribute VB_Name = "ReadListModeTracesForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'###########################################
'   Form_Load()
'       Load the ReadListModeTraces Form
'###########################################

Private Sub Form_Load()

    ZoomCombo.AddItem "Zoom"
    ZoomCombo.AddItem "Pan"
    ZoomCombo.AddItem "Coordinates"
    ZoomCombo.AddItem "Measurements"
    ZoomCombo.ListIndex = 0
    
    XCombo.AddItem "Snap Fixed"
    XCombo.AddItem "Snap Float"
    XCombo.AddItem "Snap Nearest Pt"
    XCombo.ListIndex = 1
        
    ListModeTracesGraph.TrackMode = cwGTrackZoomRectXY
 
    '   Initialize ListModeEventsEnergy Grid
    With ListModeEventsEnergyGrid
        .Redraw = False
        
        .Cols = 3
        .Rows = NUM_CHANNELS + 1
        
        .FixedCols = 1
        .FixedRows = 1
        
        .ColWidth(0) = 500
        .ColWidth(1) = 1100
        .ColWidth(2) = 1600
        .TextMatrix(0, 0) = "Chan"
        .TextMatrix(0, 1) = "Energy (16bit)"
        .TextMatrix(0, 2) = "TimeStamp"
        
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
    
    ' Update list mode data file name
    ListModeDataFile.Text = Pixie16_ListModeDataFile_Name(0)

End Sub

'###########################################
'   ListModeEventNumberEdit_ValueChanged()
'       Change the event number and display traces
'###########################################

Private Sub ListModeEventNumberEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim retval As Long
    Dim EventNumber As Long
    Dim FileLoc As Long
    Dim NumWords As Long
    Dim count As Integer
    Dim arraysize As Long
    Dim ModuleNumber, ChannelNumber As Integer
    Dim ChanWords(NUM_CHANNELS - 1) As Long
    Dim Cnt As Long

    ' Assign module number: since each module has its own file, ModuleNumber should always be 0
    ModuleNumber = 0
    
    EventNumber = Value
    If EventNumber >= ModuleListModeEvents(ModuleNumber) Then
        Exit Sub
    End If
  
    NumWords = ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 10)
    FileLoc = ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 11)
  
    ' Read list mode data for the channel that has this event
    ChannelNumber = ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 1)
    
    ' Assign ChanWords
    For count = 0 To NUM_CHANNELS - 1
        If count = ChannelNumber Then
            ChanWords(count) = NumWords
        Else
            ChanWords(count) = 0
        End If
    Next count
      
    ' First the single channel
    If ChanWords(SglChListModeTracesChanNumEdit.Value) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceSglCh(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceSglCh(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceSglCh(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesGraph.PlotY ListModeTraceSglCh, 0, LMTraceDT(ModuleNumber)
        ListModeTracesGraph.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceSglCh(1)
        ListModeTracesGraph.PlotY ListModeTraceSglCh, 0, LMTraceDT(ModuleNumber)
    End If
            
    ' Now the 16-channel display
    
    If ChanWords(0) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh0(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh0(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh0(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs0To3.Plots(1).PlotY ListModeTraceCh0, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs0To3.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh0(1)
        ListModeTracesChs0To3.Plots(1).PlotY ListModeTraceCh0, 0, LMTraceDT(ModuleNumber)
    End If
            
    If ChanWords(1) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh1(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh1(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh1(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs0To3.Plots(2).PlotY ListModeTraceCh1, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs0To3.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh1(1)
        ListModeTracesChs0To3.Plots(2).PlotY ListModeTraceCh1, 0, LMTraceDT(ModuleNumber)
    End If
            
    If ChanWords(2) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh2(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh2(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh2(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs0To3.Plots(3).PlotY ListModeTraceCh2, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs0To3.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh2(1)
        ListModeTracesChs0To3.Plots(3).PlotY ListModeTraceCh2, 0, LMTraceDT(ModuleNumber)
    End If

    If ChanWords(3) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh3(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh3(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh3(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs0To3.Plots(4).PlotY ListModeTraceCh3, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs0To3.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh3(1)
        ListModeTracesChs0To3.Plots(4).PlotY ListModeTraceCh3, 0, LMTraceDT(ModuleNumber)
    End If
            
    If ChanWords(4) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh4(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh4(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh4(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs4To7.Plots(1).PlotY ListModeTraceCh4, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs4To7.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh4(1)
        ListModeTracesChs4To7.Plots(1).PlotY ListModeTraceCh4, 0, LMTraceDT(ModuleNumber)
    End If

    If ChanWords(5) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh5(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh5(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh5(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs4To7.Plots(2).PlotY ListModeTraceCh5, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs4To7.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh5(1)
        ListModeTracesChs4To7.Plots(2).PlotY ListModeTraceCh5, 0, LMTraceDT(ModuleNumber)
    End If

    If ChanWords(6) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh6(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh6(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh6(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs4To7.Plots(3).PlotY ListModeTraceCh6, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs4To7.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh6(1)
        ListModeTracesChs4To7.Plots(3).PlotY ListModeTraceCh6, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(7) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh7(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh7(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh7(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs4To7.Plots(4).PlotY ListModeTraceCh7, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs4To7.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh7(1)
        ListModeTracesChs4To7.Plots(4).PlotY ListModeTraceCh7, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(8) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh8(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh8(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh8(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs8To11.Plots(1).PlotY ListModeTraceCh8, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs8To11.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh8(1)
        ListModeTracesChs8To11.Plots(1).PlotY ListModeTraceCh8, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(9) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh9(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh9(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh9(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs8To11.Plots(2).PlotY ListModeTraceCh9, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs8To11.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh9(1)
        ListModeTracesChs8To11.Plots(2).PlotY ListModeTraceCh9, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(10) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh10(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh10(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh10(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs8To11.Plots(3).PlotY ListModeTraceCh10, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs8To11.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh10(1)
        ListModeTracesChs8To11.Plots(3).PlotY ListModeTraceCh10, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(11) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh11(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh11(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh11(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs8To11.Plots(4).PlotY ListModeTraceCh11, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs8To11.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh11(1)
        ListModeTracesChs8To11.Plots(4).PlotY ListModeTraceCh11, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(12) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh12(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh12(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh12(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs12To15.Plots(1).PlotY ListModeTraceCh12, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs12To15.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh12(1)
        ListModeTracesChs12To15.Plots(1).PlotY ListModeTraceCh12, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(13) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh13(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh13(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh13(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs12To15.Plots(2).PlotY ListModeTraceCh13, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs12To15.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh13(1)
        ListModeTracesChs12To15.Plots(2).PlotY ListModeTraceCh13, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(14) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh14(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh14(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh14(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs12To15.Plots(3).PlotY ListModeTraceCh14, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs12To15.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh14(1)
        ListModeTracesChs12To15.Plots(3).PlotY ListModeTraceCh14, 0, LMTraceDT(ModuleNumber)
    End If
    
    If ChanWords(15) > 0 Then
        ReDim ListModeTraceRdCh(NumWords - 1)
        retval = Pixie16ReadListModeTrace(ListModeDataFile.Text, ListModeTraceRdCh(0), NumWords, FileLoc)
        If retval < 0 Then
            StrTxt = "Pixie16ReadListModeTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        
        ReDim ListModeTraceCh15(NumWords - 1)
        For Cnt = 0 To NumWords - 1
            If ListModeTraceRdCh(Cnt) < 0 Then
                ListModeTraceCh15(Cnt) = ListModeTraceRdCh(Cnt) + 65536
            Else
                ListModeTraceCh15(Cnt) = ListModeTraceRdCh(Cnt)
            End If
        Next Cnt
        
        ' Plot the trace
        ListModeTracesChs12To15.Plots(4).PlotY ListModeTraceCh15, 0, LMTraceDT(ModuleNumber)
        ListModeTracesChs12To15.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    Else
        ' ReDim will clear the trace
        ReDim ListModeTraceCh15(1)
        ListModeTracesChs12To15.Plots(4).PlotY ListModeTraceCh15, 0, LMTraceDT(ModuleNumber)
    End If
        
    ' Update event energies and timestamps
    With ListModeEventsEnergyGrid
        .TextMatrix(ChannelNumber + 1, 1) = ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 9)
        
        If ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 7) < 0 Then
            .TextMatrix(ChannelNumber + 1, 2) = 4294967295# + ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 7) + 1 + ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 8) * 2147483648#
        Else
            .TextMatrix(ChannelNumber + 1, 2) = ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 7) + ListModeEventInformation(EventNumber * LMEVENTINFOLENGTH + 8) * 2147483648#
        End If
       
        For count = 0 To NUM_CHANNELS - 1
            If count <> ChannelNumber Then
                .TextMatrix(count + 1, 1) = 0
                .TextMatrix(count + 1, 2) = 0
            End If
        Next count
    End With
    
    ' Check whether compute filter responses
    If ComputeFilterResponses = 1 Then
        ' Compute fast and slow filter responses
        EventChanNumberText.Text = ChannelNumber
        
        ReDim LMTraceFilterRSTrace(NumWords - 1)
        ReDim LMTraceFastFilter(NumWords - 1)
        ReDim LMTraceCFD(NumWords - 1)
        ReDim LMTraceSlowFilter(NumWords - 1)
        
        If NumWords > 0 Then
            ' Fast filter responses
            retval = Pixie16ComputeFastFiltersOffline(ListModeDataFile.Text, ModuleNumber, ChannelNumber, FileLoc, NumWords, LMTraceFilterRSTrace(0), LMTraceFastFilter(0), LMTraceCFD(0))
            If retval < 0 Then
                StrTxt = "Pixie16ComputeFastFiltersOffline " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
            
            ' Slow filter responses
            retval = Pixie16ComputeSlowFiltersOffline(ListModeDataFile.Text, ModuleNumber, ChannelNumber, FileLoc, NumWords, LMTraceFilterRSTrace(0), LMTraceSlowFilter(0))
            If retval < 0 Then
                StrTxt = "Pixie16ComputeSlowFiltersOffline " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        End If
        
        ' Plot the fast filter responses
        LMTraceFastFilterGraph.Plots(1).PlotY LMTraceFilterRSTrace, 0, LMTraceDT(ModuleNumber)
        LMTraceFastFilterGraph.Plots(2).PlotY LMTraceFastFilter, 0, LMTraceDT(ModuleNumber)
        LMTraceFastFilterGraph.Plots(3).PlotY LMTraceCFD, 0, LMTraceDT(ModuleNumber)
        
        LMTraceFastFilterGraph.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
        
        ' Plot the slow filter responses
        LMTraceSlowFilterGraph.Plots(1).PlotY LMTraceFilterRSTrace, 0, LMTraceDT(ModuleNumber)
        LMTraceSlowFilterGraph.Plots(2).PlotY LMTraceSlowFilter, 0, LMTraceDT(ModuleNumber)
        
        LMTraceSlowFilterGraph.Axes(1).SetMinMax 0, (NumWords - 1) * LMTraceDT(ModuleNumber) ' X Axis
    End If
    
End Sub


'###########################################
'   CloseListModeTracesForm_Click()
'       Close the ListModeTraces Form
'###########################################

Private Sub CloseListModeTracesForm_Click()
    Unload Me
End Sub


'###########################################
'   ListModeTracesGraph_CursorChange()
'       Respond to cursor position change
'###########################################

Private Sub ListModeTracesGraph_CursorChange(CursorIndex As Long, XPos As Variant, YPos As Variant, bTracking As Boolean)
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    'Display cursor position on user interface
    If CursorIndex = 1 Then
        XPosVal = Format(ListModeTracesGraph.Cursors(1).XPosition, "0.####") ' data from cursor
        YPosVal = Format(YPos, "0.####") ' data from event handler
    End If
    If (index = 3) Then
        Dim Amplitude As Double
        Dim Period As Double
        Amplitude = Format(Abs(ListModeTracesGraph.Cursors(2).YPosition - ListModeTracesGraph.Cursors(1).YPosition), "0.####")
        Period = Format(Abs(ListModeTracesGraph.Cursors(2).XPosition - ListModeTracesGraph.Cursors(1).XPosition), "0.####")
        dYVal = Amplitude
        dXVal = Period
    End If
End Sub


'###########################################
'   GraphMode_PointerValueChanged()
'       Change TrackMode of graph
'###########################################

Private Sub GraphMode_PointerValueChanged(ByVal Pointer As Long, Value As Variant)
    ' Change TrackMode of graph
    ' Hide and show cursors
    Select Case Value
        Case 0
            ListModeTracesGraph.TrackMode = cwGTrackZoomRectXY
            ListModeTracesGraph.Cursors(1).Visible = False
            ListModeTracesGraph.Cursors(2).Visible = False
        Case 1
            ListModeTracesGraph.Cursors(1).Visible = False
            ListModeTracesGraph.Cursors(2).Visible = False
            ListModeTracesGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2
            ListModeTracesGraph.TrackMode = cwGTrackDragCursor
            ListModeTracesGraph.Cursors(1).Visible = True
            ListModeTracesGraph.Cursors(2).Visible = False
            ListModeTracesGraph.Cursors(1).XPosition = (ListModeTracesGraph.Axes(1).Minimum + ListModeTracesGraph.Axes(1).Maximum) / 2
            ListModeTracesGraph.Cursors(1).YPosition = (ListModeTracesGraph.Axes(2).Minimum + ListModeTracesGraph.Axes(2).Maximum) / 2
        Case 3
            ListModeTracesGraph.TrackMode = cwGTrackDragCursor
            ListModeTracesGraph.Cursors(1).Visible = True
            ListModeTracesGraph.Cursors(2).Visible = True

    End Select
End Sub


'###########################################
'   XPosition_ValueChanged()
'       Move cursor's X-position
'###########################################

Private Sub XPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    ListModeTracesGraph.Cursors(1).XPosition = XPosVal
    ListModeTracesGraph.Cursors(1).YPosition = YPosVal
End Sub


'###########################################
'   YPosition_ValueChanged()
'       move cursor's Y-position
'###########################################

Private Sub YPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    ListModeTracesGraph.Cursors(1).XPosition = XPosVal
    ListModeTracesGraph.Cursors(1).YPosition = YPosVal
End Sub


Private Sub LMTraceFastFilterAutoScale_Click()
    Dim Axis As CWAxis
    
    For Each Axis In LMTraceFastFilterGraph.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

End Sub

Private Sub LMTraceSlowFilterAutoScale_Click()
    Dim Axis As CWAxis
    
    For Each Axis In LMTraceSlowFilterGraph.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

End Sub

'###########################################
'   OpenListModeFile_Click()
'       Browse for list mode data file
'###########################################

Private Sub OpenListModeFile_Click()
    Dim getfile As String
    
    getfile = BrowseForFile(Me, "Select the list mode data file", ListModeDataFile.Text)
    If Len(getfile) = 0 Then Exit Sub  'user selected cancel
    ListModeDataFile = getfile
    
    ' Parse the file
    Call ParseListModeFile(ListModeDataFile.Text)
End Sub

Private Sub ListModeDataFile_KeyPress(KeyAscii As Integer)
    If (KeyAscii = 13) Then
        ' Parse the file
        Call ParseListModeFile(ListModeDataFile.Text)
    End If
End Sub

Private Sub ListModeDataFile_LostFocus()
    ' Parse the file
    Call ParseListModeFile(ListModeDataFile.Text)
End Sub

'###########################################
'   ParseListModeFile()
'       Parse list mode data file
'###########################################

Private Sub ParseListModeFile(ByVal ListModeDataFile As String)
    Dim retval As Long
    Dim StrTxt As String
    Dim count As Integer
    Dim ModuleNumber As Integer
    Dim Msg
    
    ' Assign module number: since each module has its own file, ModuleNumber should always be 0
    ModuleNumber = 0
    
    ' Clear array
    For count = 0 To Number_Modules_Total - 1
        ModuleListModeEvents(count) = 0
    Next count
    
    ' Parse the list mode data file to determine the number of events for each module
    retval = Pixie16GetModuleEvents(ListModeDataFile, ModuleListModeEvents(ModuleNumber))
    If retval < 0 Then
        StrTxt = "Pixie16GetModuleEvents failed. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    ' Set total number of events for the current module
    TotalNumEventsText.Text = Str$(ModuleListModeEvents(ModuleNumber))
    
    ' Update maximum number of events
    If ModuleListModeEvents(ModuleNumber) > 0 Then
        ListModeEventNumberEdit.Maximum = ModuleListModeEvents(ModuleNumber) - 1
    Else
        ListModeEventNumberEdit.Maximum = 0
    End If
    
    ' Read events information
    ReDim ListModeEventInformation(ModuleListModeEvents(ModuleNumber) * LMEVENTINFOLENGTH)
    retval = Pixie16GetEventsInfo(ListModeDataFile, ListModeEventInformation(ModuleNumber), ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16GetEventsInfo failed. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

End Sub


Private Sub ReadListModeDataFromFile_Click()
    Call ParseListModeFile(ListModeDataFile.Text)
End Sub

'###########################################
'   XCombo_Click()
'       X-axis snap modes
'###########################################

Private Sub XCombo_Click()
    Dim index As Integer
    
    index = XCombo.ListIndex
    Select Case index
        Case 0
            ListModeTracesGraph.Cursors(1).SnapMode = cwCSnapFixed
            ListModeTracesGraph.Cursors(2).SnapMode = cwCSnapFixed
        Case 1
            ListModeTracesGraph.Cursors(1).SnapMode = cwCSnapFloating
            ListModeTracesGraph.Cursors(2).SnapMode = cwCSnapFloating
        Case 2
            ListModeTracesGraph.Cursors(1).SnapMode = cwCSnapNearestPoint
            ListModeTracesGraph.Cursors(2).SnapMode = cwCSnapNearestPoint
    End Select
End Sub


'###########################################
'   ZoomCombo_Click()
'       Various zoom options
'###########################################

Private Sub ZoomCombo_Click()
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    Select Case index
        Case 0      ' Zoom
            ListModeTracesGraph.TrackMode = cwGTrackZoomRectXY
            ListModeTracesGraph.Cursors(1).Visible = False
            ListModeTracesGraph.Cursors(2).Visible = False
        Case 1      ' Pan
            ListModeTracesGraph.Cursors(1).Visible = False
            ListModeTracesGraph.Cursors(2).Visible = False
            ListModeTracesGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2      ' Cursor Coordinates
            ListModeTracesGraph.TrackMode = cwGTrackDragCursor
            ListModeTracesGraph.Cursors(1).Visible = True
            ListModeTracesGraph.Cursors(2).Visible = False
            ListModeTracesGraph.Cursors(1).XPosition = (ListModeTracesGraph.Axes(1).Minimum + ListModeTracesGraph.Axes(1).Maximum) / 2
            ListModeTracesGraph.Cursors(1).YPosition = (ListModeTracesGraph.Axes(2).Minimum + ListModeTracesGraph.Axes(2).Maximum) / 2
        Case 3      ' Cursor Measurements
            ListModeTracesGraph.TrackMode = cwGTrackDragCursor
            ListModeTracesGraph.Cursors(1).Visible = True
            ListModeTracesGraph.Cursors(2).Visible = True
            ListModeTracesGraph.Cursors(1).XPosition = (ListModeTracesGraph.Axes(1).Minimum + ListModeTracesGraph.Axes(1).Maximum) / 2
            ListModeTracesGraph.Cursors(1).YPosition = (ListModeTracesGraph.Axes(2).Minimum + ListModeTracesGraph.Axes(2).Maximum) / 2
            ListModeTracesGraph.Cursors(2).XPosition = (ListModeTracesGraph.Axes(1).Minimum + ListModeTracesGraph.Axes(1).Maximum) / 2
            ListModeTracesGraph.Cursors(2).YPosition = (ListModeTracesGraph.Axes(2).Minimum + ListModeTracesGraph.Axes(2).Maximum) / 2
    End Select

End Sub


'###########################################
'   AutoScale_Click()
'       Auto scale all axis
'###########################################

Private Sub AutoScale_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ListModeTracesGraph.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
End Sub


'###########################################
'   ChannelCheckBox Click routines
'       The following subroutines show or disable
'       the list mode traces display
'###########################################

Private Sub Channel0_Click()
    If Channel0.Value = 1 Then
        ListModeTracesChs0To3.Plots(1).Visible = True
    Else
        ListModeTracesChs0To3.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel1_Click()
    If Channel1.Value = 1 Then
        ListModeTracesChs0To3.Plots(2).Visible = True
    Else
        ListModeTracesChs0To3.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel2_Click()
    If Channel2.Value = 1 Then
        ListModeTracesChs0To3.Plots(3).Visible = True
    Else
        ListModeTracesChs0To3.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel3_Click()
    If Channel3.Value = 1 Then
        ListModeTracesChs0To3.Plots(4).Visible = True
    Else
        ListModeTracesChs0To3.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel4_Click()
    If Channel4.Value = 1 Then
        ListModeTracesChs4To7.Plots(1).Visible = True
    Else
        ListModeTracesChs4To7.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel5_Click()
    If Channel5.Value = 1 Then
        ListModeTracesChs4To7.Plots(2).Visible = True
    Else
        ListModeTracesChs4To7.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel6_Click()
    If Channel6.Value = 1 Then
        ListModeTracesChs4To7.Plots(3).Visible = True
    Else
        ListModeTracesChs4To7.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel7_Click()
    If Channel7.Value = 1 Then
        ListModeTracesChs4To7.Plots(4).Visible = True
    Else
        ListModeTracesChs4To7.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel8_Click()
    If Channel8.Value = 1 Then
        ListModeTracesChs8To11.Plots(1).Visible = True
    Else
        ListModeTracesChs8To11.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel9_Click()
    If Channel9.Value = 1 Then
        ListModeTracesChs8To11.Plots(2).Visible = True
    Else
        ListModeTracesChs8To11.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel10_Click()
    If Channel10.Value = 1 Then
        ListModeTracesChs8To11.Plots(3).Visible = True
    Else
        ListModeTracesChs8To11.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel11_Click()
    If Channel11.Value = 1 Then
        ListModeTracesChs8To11.Plots(4).Visible = True
    Else
        ListModeTracesChs8To11.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel12_Click()
    If Channel12.Value = 1 Then
        ListModeTracesChs12To15.Plots(1).Visible = True
    Else
        ListModeTracesChs12To15.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel13_Click()
    If Channel13.Value = 1 Then
        ListModeTracesChs12To15.Plots(2).Visible = True
    Else
        ListModeTracesChs12To15.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel14_Click()
    If Channel14.Value = 1 Then
        ListModeTracesChs12To15.Plots(3).Visible = True
    Else
        ListModeTracesChs12To15.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel15_Click()
    If Channel15.Value = 1 Then
        ListModeTracesChs12To15.Plots(4).Visible = True
    Else
        ListModeTracesChs12To15.Plots(4).Visible = False
    End If
End Sub

Private Sub AutoScaleChs0To3_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ListModeTracesChs0To3.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

End Sub

Private Sub AutoScaleChs12To15_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ListModeTracesChs12To15.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

End Sub

Private Sub AutoScaleChs4To7_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ListModeTracesChs4To7.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

End Sub

Private Sub AutoScaleChs8To11_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ListModeTracesChs8To11.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

End Sub

'###########################################
'   ComputeFilterResponsesCheckBox_Click()
'       Set ComputeFilterResponsesCheckBox CheckBox
'###########################################

Private Sub ComputeFilterResponsesCheckBox_Click()
    If ComputeFilterResponsesCheckBox.Value = 1 Then
        ComputeFilterResponses = 1
    Else
        ComputeFilterResponses = 0
    End If
End Sub

