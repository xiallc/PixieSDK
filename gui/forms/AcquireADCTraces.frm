VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form AcquireADCTracesForm 
   Caption         =   "Acquire ADC Traces"
   ClientHeight    =   10605
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   12015
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   10605
   ScaleWidth      =   12015
   Begin VB.CommandButton AdjustOffsets 
      Caption         =   "Adjust Offsets"
      Height          =   615
      Left            =   3600
      TabIndex        =   57
      Top             =   9840
      Width           =   1935
   End
   Begin VB.CommandButton GetADCTraces 
      Caption         =   "Refresh"
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
      Left            =   6120
      TabIndex        =   36
      Top             =   9840
      Width           =   1935
   End
   Begin VB.CommandButton CloseADCForm 
      Caption         =   "Close"
      Height          =   615
      Left            =   8880
      TabIndex        =   3
      Top             =   9840
      Width           =   1455
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   9255
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   11775
      _ExtentX        =   20770
      _ExtentY        =   16325
      _Version        =   393216
      TabOrientation  =   1
      Tabs            =   2
      Tab             =   1
      TabHeight       =   520
      TabCaption(0)   =   "ADC Trace - single channel"
      TabPicture(0)   =   "AcquireADCTraces.frx":0000
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "DlgSaveADCTrace"
      Tab(0).Control(1)=   "Autoscale"
      Tab(0).Control(2)=   "FFTADCTracesButton"
      Tab(0).Control(3)=   "XPosVal"
      Tab(0).Control(4)=   "dXVal"
      Tab(0).Control(5)=   "YPosVal"
      Tab(0).Control(6)=   "dYVal"
      Tab(0).Control(7)=   "YScalingCombo"
      Tab(0).Control(8)=   "XCombo"
      Tab(0).Control(9)=   "ZoomCombo"
      Tab(0).Control(10)=   "SglChADCTracesChanNumEdit"
      Tab(0).Control(11)=   "ADCTraceGraph"
      Tab(0).Control(12)=   "SaveADCTraceDataToFile"
      Tab(0).Control(13)=   "dY"
      Tab(0).Control(14)=   "dX"
      Tab(0).Control(15)=   "YPos"
      Tab(0).Control(16)=   "XPos"
      Tab(0).Control(17)=   "SglChADCTracesChanNumLabel"
      Tab(0).ControlCount=   18
      TabCaption(1)   =   "ADC Trace - 16 channels"
      TabPicture(1)   =   "AcquireADCTraces.frx":001C
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "SampleIntervalLabelChs4to7"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "SampleIntervalLabelChs0to3"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "SampleIntervalLabelChs8to11"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).Control(3)=   "SampleIntervalLabelChs12to15"
      Tab(1).Control(3).Enabled=   0   'False
      Tab(1).Control(4)=   "ADCTracesChs12To15"
      Tab(1).Control(4).Enabled=   0   'False
      Tab(1).Control(5)=   "ADCTracesChs8To11"
      Tab(1).Control(5).Enabled=   0   'False
      Tab(1).Control(6)=   "ADCTracesChs4To7"
      Tab(1).Control(6).Enabled=   0   'False
      Tab(1).Control(7)=   "ADCTracesChs0To3"
      Tab(1).Control(7).Enabled=   0   'False
      Tab(1).Control(8)=   "AutoScaleChs0To3"
      Tab(1).Control(8).Enabled=   0   'False
      Tab(1).Control(9)=   "AutoScaleChs4To7"
      Tab(1).Control(9).Enabled=   0   'False
      Tab(1).Control(10)=   "AutoScaleChs8To11"
      Tab(1).Control(10).Enabled=   0   'False
      Tab(1).Control(11)=   "AutoScaleChs12To15"
      Tab(1).Control(11).Enabled=   0   'False
      Tab(1).Control(12)=   "Channel3"
      Tab(1).Control(12).Enabled=   0   'False
      Tab(1).Control(13)=   "Channel2"
      Tab(1).Control(13).Enabled=   0   'False
      Tab(1).Control(14)=   "Channel1"
      Tab(1).Control(14).Enabled=   0   'False
      Tab(1).Control(15)=   "Channel0"
      Tab(1).Control(15).Enabled=   0   'False
      Tab(1).Control(16)=   "Channel7"
      Tab(1).Control(16).Enabled=   0   'False
      Tab(1).Control(17)=   "Channel6"
      Tab(1).Control(17).Enabled=   0   'False
      Tab(1).Control(18)=   "Channel5"
      Tab(1).Control(18).Enabled=   0   'False
      Tab(1).Control(19)=   "Channel4"
      Tab(1).Control(19).Enabled=   0   'False
      Tab(1).Control(20)=   "Channel8"
      Tab(1).Control(20).Enabled=   0   'False
      Tab(1).Control(21)=   "Channel9"
      Tab(1).Control(21).Enabled=   0   'False
      Tab(1).Control(22)=   "Channel10"
      Tab(1).Control(22).Enabled=   0   'False
      Tab(1).Control(23)=   "Channel11"
      Tab(1).Control(23).Enabled=   0   'False
      Tab(1).Control(24)=   "Channel12"
      Tab(1).Control(24).Enabled=   0   'False
      Tab(1).Control(25)=   "Channel13"
      Tab(1).Control(25).Enabled=   0   'False
      Tab(1).Control(26)=   "Channel14"
      Tab(1).Control(26).Enabled=   0   'False
      Tab(1).Control(27)=   "Channel15"
      Tab(1).Control(27).Enabled=   0   'False
      Tab(1).Control(28)=   "SelectOrDeselectChs4to7"
      Tab(1).Control(28).Enabled=   0   'False
      Tab(1).Control(29)=   "SampleIntervalComboChs4to7"
      Tab(1).Control(29).Enabled=   0   'False
      Tab(1).Control(30)=   "SampleIntervalComboChs0to3"
      Tab(1).Control(30).Enabled=   0   'False
      Tab(1).Control(31)=   "SampleIntervalComboChs8to11"
      Tab(1).Control(31).Enabled=   0   'False
      Tab(1).Control(32)=   "SampleIntervalComboChs12to15"
      Tab(1).Control(32).Enabled=   0   'False
      Tab(1).Control(33)=   "SelectOrDeselectChs0to3"
      Tab(1).Control(33).Enabled=   0   'False
      Tab(1).Control(34)=   "SelectOrDeselectChs8to11"
      Tab(1).Control(34).Enabled=   0   'False
      Tab(1).Control(35)=   "SelectOrDeselectChs12to15"
      Tab(1).Control(35).Enabled=   0   'False
      Tab(1).ControlCount=   36
      Begin VB.CommandButton SelectOrDeselectChs12to15 
         Caption         =   "Deselect All"
         Height          =   375
         Left            =   6000
         TabIndex        =   56
         Top             =   4490
         Width           =   1020
      End
      Begin VB.CommandButton SelectOrDeselectChs8to11 
         Caption         =   "Deselect All"
         Height          =   375
         Left            =   240
         TabIndex        =   55
         Top             =   4490
         Width           =   1020
      End
      Begin VB.CommandButton SelectOrDeselectChs0to3 
         Caption         =   "Deselect All"
         Height          =   375
         Left            =   240
         TabIndex        =   54
         Top             =   120
         Width           =   1020
      End
      Begin VB.ComboBox SampleIntervalComboChs12to15 
         Height          =   315
         Left            =   10080
         TabIndex        =   53
         Text            =   "Sample Interval"
         Top             =   4850
         Width           =   1455
      End
      Begin VB.ComboBox SampleIntervalComboChs8to11 
         Height          =   315
         Left            =   4320
         TabIndex        =   52
         Text            =   "Sample Interval"
         Top             =   4850
         Width           =   1455
      End
      Begin VB.ComboBox SampleIntervalComboChs0to3 
         Height          =   315
         ItemData        =   "AcquireADCTraces.frx":0038
         Left            =   4320
         List            =   "AcquireADCTraces.frx":003A
         TabIndex        =   51
         Text            =   "Sample Interval"
         Top             =   480
         Width           =   1455
      End
      Begin VB.ComboBox SampleIntervalComboChs4to7 
         Height          =   315
         Left            =   10080
         TabIndex        =   46
         Text            =   "Sample Interval"
         Top             =   480
         Width           =   1455
      End
      Begin VB.CommandButton SelectOrDeselectChs4to7 
         Caption         =   "Deselect All"
         Height          =   375
         Left            =   6000
         TabIndex        =   45
         Top             =   120
         Width           =   1020
      End
      Begin MSComDlg.CommonDialog DlgSaveADCTrace 
         Left            =   -63960
         Top             =   7800
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
      End
      Begin VB.CheckBox Channel15 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch15"
         Height          =   255
         Left            =   8160
         TabIndex        =   44
         Top             =   4920
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel14 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch14"
         Height          =   255
         Left            =   7440
         TabIndex        =   43
         Top             =   4920
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel13 
         BackColor       =   &H000000FF&
         Caption         =   "Ch13"
         Height          =   255
         Left            =   6720
         TabIndex        =   42
         Top             =   4920
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel12 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch12"
         Height          =   255
         Left            =   6000
         TabIndex        =   41
         Top             =   4920
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel11 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch11"
         Height          =   255
         Left            =   2160
         TabIndex        =   40
         Top             =   4920
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel10 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch10"
         Height          =   255
         Left            =   1440
         TabIndex        =   39
         Top             =   4920
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel9 
         BackColor       =   &H000000FF&
         Caption         =   "Ch9"
         Height          =   255
         Left            =   840
         TabIndex        =   38
         Top             =   4920
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel8 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch8"
         Height          =   255
         Left            =   240
         TabIndex        =   37
         Top             =   4920
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel4 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch4"
         Height          =   255
         Left            =   6000
         TabIndex        =   35
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel5 
         BackColor       =   &H000000FF&
         Caption         =   "Ch5"
         Height          =   255
         Left            =   6600
         TabIndex        =   34
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel6 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch6"
         Height          =   255
         Left            =   7200
         TabIndex        =   33
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel7 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch7"
         Height          =   255
         Left            =   7800
         TabIndex        =   32
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel0 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch0"
         Height          =   255
         Left            =   240
         TabIndex        =   31
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel1 
         BackColor       =   &H000000FF&
         Caption         =   "Ch1"
         Height          =   255
         Left            =   840
         TabIndex        =   30
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel2 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch2"
         Height          =   255
         Left            =   1440
         TabIndex        =   29
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel3 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch3"
         Height          =   255
         Left            =   2040
         TabIndex        =   28
         Top             =   550
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CommandButton AutoScaleChs12To15 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   9000
         TabIndex        =   27
         Top             =   4680
         Width           =   900
      End
      Begin VB.CommandButton AutoScaleChs8To11 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   3120
         TabIndex        =   26
         Top             =   4680
         Width           =   900
      End
      Begin VB.CommandButton AutoScaleChs4To7 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   9000
         TabIndex        =   25
         Top             =   360
         Width           =   900
      End
      Begin VB.CommandButton AutoScaleChs0To3 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   3120
         TabIndex        =   24
         Top             =   360
         Width           =   900
      End
      Begin VB.CommandButton Autoscale 
         Caption         =   "Autoscale"
         Height          =   495
         Left            =   -71040
         TabIndex        =   19
         Top             =   7680
         Width           =   1215
      End
      Begin VB.CommandButton FFTADCTracesButton 
         Caption         =   "FFT"
         Height          =   495
         Left            =   -69480
         TabIndex        =   18
         Top             =   7680
         Width           =   1215
      End
      Begin VB.TextBox XPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   -74565
         TabIndex        =   13
         Top             =   7560
         Width           =   1095
      End
      Begin VB.TextBox dXVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   -73200
         TabIndex        =   12
         Top             =   7560
         Width           =   1095
      End
      Begin VB.TextBox YPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   -74565
         TabIndex        =   11
         Top             =   7920
         Width           =   1095
      End
      Begin VB.TextBox dYVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   -73200
         TabIndex        =   10
         Top             =   7920
         Width           =   1095
      End
      Begin VB.ComboBox YScalingCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   -64800
         Style           =   2  'Dropdown List
         TabIndex        =   9
         Top             =   240
         Width           =   1335
      End
      Begin VB.ComboBox XCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   -73200
         Style           =   2  'Dropdown List
         TabIndex        =   8
         Top             =   240
         Width           =   1575
      End
      Begin VB.ComboBox ZoomCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   -74760
         Style           =   2  'Dropdown List
         TabIndex        =   7
         Top             =   240
         Width           =   1575
      End
      Begin CWUIControlsLib.CWNumEdit SglChADCTracesChanNumEdit 
         Height          =   495
         Left            =   -66360
         TabIndex        =   5
         Top             =   7560
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
      Begin CWUIControlsLib.CWGraph ADCTraceGraph 
         Height          =   6735
         Left            =   -74760
         TabIndex        =   6
         Top             =   600
         Width           =   11295
         _Version        =   393218
         _ExtentX        =   19923
         _ExtentY        =   11880
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
         Format_7        =   "."
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   24576
         rMin_8          =   40
         rMax_8          =   741
         dMax_8          =   10
         discInterval_8  =   0.01
         discBase_8      =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   0.5
         minor_6         =   0.25
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   62
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Time (탎)"
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
         rMax_15         =   409
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
         szText_19       =   "ADC Steps"
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
      Begin CWUIControlsLib.CWGraph ADCTracesChs0To3 
         Height          =   3375
         Left            =   240
         TabIndex        =   20
         Top             =   840
         Width           =   5535
         _Version        =   393218
         _ExtentX        =   9763
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
         Format_7        =   "."
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   357
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
         szText_12       =   "Time (탎)"
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
         szText_19       =   "ADC Steps"
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
      Begin CWUIControlsLib.CWGraph ADCTracesChs4To7 
         Height          =   3375
         Left            =   6000
         TabIndex        =   21
         Top             =   840
         Width           =   5535
         _Version        =   393218
         _ExtentX        =   9763
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
         Format_7        =   "."
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   357
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
         szText_12       =   "Time (탎)"
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
         szText_19       =   "ADC Steps"
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
      Begin CWUIControlsLib.CWGraph ADCTracesChs8To11 
         Height          =   3375
         Left            =   240
         TabIndex        =   22
         Top             =   5210
         Width           =   5535
         _Version        =   393218
         _ExtentX        =   9763
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
         Format_7        =   "."
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   357
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
         szText_12       =   "Time (탎)"
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
         szText_19       =   "ADC Steps"
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
      Begin CWUIControlsLib.CWGraph ADCTracesChs12To15 
         Height          =   3375
         Left            =   6000
         TabIndex        =   23
         Top             =   5210
         Width           =   5535
         _Version        =   393218
         _ExtentX        =   9763
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
         Format_7        =   "."
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   357
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
         szText_12       =   "Time (탎)"
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
         szText_19       =   "ADC Steps"
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
      Begin VB.Label SampleIntervalLabelChs12to15 
         Caption         =   "Sample interval"
         Height          =   255
         Left            =   10080
         TabIndex        =   50
         Top             =   4610
         Width           =   1455
      End
      Begin VB.Label SampleIntervalLabelChs8to11 
         Caption         =   "Sample interval"
         Height          =   255
         Left            =   4320
         TabIndex        =   49
         Top             =   4610
         Width           =   1455
      End
      Begin VB.Label SampleIntervalLabelChs0to3 
         Caption         =   "Sample interval"
         Height          =   255
         Left            =   4320
         TabIndex        =   48
         Top             =   240
         Width           =   1455
      End
      Begin VB.Label SampleIntervalLabelChs4to7 
         Caption         =   "Sample interval"
         Height          =   255
         Left            =   10080
         TabIndex        =   47
         Top             =   240
         Width           =   1455
      End
      Begin VB.Image SaveADCTraceDataToFile 
         Height          =   225
         Left            =   -63720
         Picture         =   "AcquireADCTraces.frx":003C
         ToolTipText     =   "Save ADC trace to a text file"
         Top             =   7440
         Width           =   240
      End
      Begin VB.Label dY 
         Caption         =   "dY"
         Height          =   255
         Left            =   -73440
         TabIndex        =   17
         Top             =   7920
         Width           =   255
      End
      Begin VB.Label dX 
         Caption         =   "dX"
         Height          =   255
         Left            =   -73440
         TabIndex        =   16
         Top             =   7560
         Width           =   255
      End
      Begin VB.Label YPos 
         Caption         =   "Y"
         Height          =   255
         Left            =   -74760
         TabIndex        =   15
         Top             =   7920
         Width           =   255
      End
      Begin VB.Label XPos 
         Caption         =   "X"
         Height          =   255
         Left            =   -74760
         TabIndex        =   14
         Top             =   7560
         Width           =   255
      End
      Begin VB.Label SglChADCTracesChanNumLabel 
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
         Left            =   -67920
         TabIndex        =   4
         Top             =   7680
         Width           =   1575
      End
   End
   Begin CWUIControlsLib.CWNumEdit ADCTracesModNumEdit 
      Height          =   495
      Left            =   2280
      TabIndex        =   2
      Top             =   9840
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
      RangeMax_Val_1  =   6
      Bindings_1      =   4
      ClassName_4     =   "CCWBindingHolderArray"
      Editor_4        =   5
      ClassName_5     =   "CCWBindingHolderArrayEditor"
      Owner_5         =   1
   End
   Begin VB.Label ADCTracesModNumLabel 
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
      Left            =   480
      TabIndex        =   1
      Top             =   9960
      Width           =   1815
   End
End
Attribute VB_Name = "AcquireADCTracesForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'###########################################
'   Form_Load()
'       Load AcquireADCTracesForm form
'###########################################

Private Sub Form_Load()
    Dim X, ChanNum As Integer
    
    ' Update maximum number of modules
    ADCTracesModNumEdit.Maximum = Number_Modules_Total - 1

    ' Set FirstTimeSettingADCSamplingInterval to true
    FirstTimeSettingADCSamplingInterval = True
    
    ' Set X-axis limits for the single channel
    ChanNum = SglChADCTracesChanNumEdit.Value
    Select Case ChanNum
        Case 0 To 3
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(0) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(0)
        Case 4 To 7
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(1) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(1)
        Case 8 To 11
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(2) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(2)
        Case 12 To 15
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(3) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(3)
    End Select
    ADCTraceGraph.Axes(2).Autoscale = True
    
    ' Set X-axis limits for other channels
    ADCTracesChs0To3.Axes(1).SetMinMax 0, Xdt(0) * (ADC_TRACE_LENGTH - 1) ' X Axis
    ADCTracesChs0To3.Axes(2).Autoscale = True
    
    ADCTracesChs4To7.Axes(1).SetMinMax 0, Xdt(1) * (ADC_TRACE_LENGTH - 1) ' X Axis
    ADCTracesChs4To7.Axes(2).Autoscale = True
    
    ADCTracesChs8To11.Axes(1).SetMinMax 0, Xdt(2) * (ADC_TRACE_LENGTH - 1) ' X Axis
    ADCTracesChs8To11.Axes(2).Autoscale = True
   
    ADCTracesChs12To15.Axes(1).SetMinMax 0, Xdt(3) * (ADC_TRACE_LENGTH - 1) ' X Axis
    ADCTracesChs12To15.Axes(2).Autoscale = True
    
    ' Initialize Combo items
    ZoomCombo.AddItem "Zoom"
    ZoomCombo.AddItem "Pan"
    ZoomCombo.AddItem "Coordinates"
    ZoomCombo.AddItem "Measurements"
    ZoomCombo.ListIndex = 0
    
    XCombo.AddItem "Snap Fixed"
    XCombo.AddItem "Snap Float"
    XCombo.AddItem "Snap Nearest Pt"
    XCombo.ListIndex = 1
    
    YScalingCombo.AddItem "Linear"
    YScalingCombo.AddItem "Log"
    YScalingCombo.ListIndex = 0
    
    ' Set available ADC sample intervals based on module variant
    Call Set_ADC_Sampling_Intervals(ADCTracesModNumEdit.Value)
    
    ' Update ADC sample intervals
    Call UpdateADCSampleIntervals(ADCTracesModNumEdit.Value)

    ' Set FirstTimeSettingADCSamplingInterval to false
    FirstTimeSettingADCSamplingInterval = False

End Sub


'###########################################
'   ADCTracesModNumEdit_ValueChanged()
'       Update ADC sample intervals
'###########################################

Private Sub ADCTracesModNumEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    Dim ModNum As Integer
    Dim ChanNum As Integer
    Dim X, Y As Integer
    Dim ErrTxt As String
    Dim Msg As Integer
    Dim retval As Long
    Dim dT As Double
    
    ' Set current module number
    ModNum = ADCTracesModNumEdit.Value
    
    ' Set available ADC sample intervals based on module variant
    Call Set_ADC_Sampling_Intervals(ModNum)

    ' Get XDT
    For X = 0 To 3
        retval = Pixie16ReadSglChanPar("XDT", Xdt(X), ModNum, X * 4)
        If retval < 0 Then
            ErrTxt = "(ADCTracesModNumEdit_ValueChanged) failed to read XDT. Please check the log file"
            Msg = MsgBox(ErrTxt, 0, "Error")
            Exit Sub
        End If
        
        ' Check if the returned Xdt value is 0 - which is possible
        ' if a corrupted settings file is being used
        If Xdt(X) = 0 Or Xdt(X) < 0 Then
            ' Set Xdt to the minimum
            dT = XDT_SCALE_FACTOR(ModNum) / DSP_CLOCK_MHZ
            ' Write to module
            For Y = 0 To 3
                retval = Pixie16WriteSglChanPar("XDT", dT, ModNum, X * 4 + Y)
                If retval < 0 Then
                    StrTxt = "Pixie16WriteSglChanPar failed to set XDT in module # " & Str$(ModNum)
                    Msg = MsgBox(StrTxt, 0, "Error")
                    Exit Sub
                End If
            Next Y
            
            ' Read back from module
            retval = Pixie16ReadSglChanPar("XDT", Xdt(X), ModNum, X * 4)
            If retval < 0 Then
            ErrTxt = "(ADCTracesModNumEdit_ValueChanged) failed to read XDT. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        End If
    Next X
    
    ' Set X-axis limits for the single channel
    ChanNum = SglChADCTracesChanNumEdit.Value
    Select Case ChanNum
        Case 0 To 3
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(0) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(0)
        Case 4 To 7
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(1) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(1)
        Case 8 To 11
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(2) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(2)
        Case 12 To 15
            ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(3) * (ADC_TRACE_LENGTH - 1) ' X Axis
            XdtSglChan = Xdt(3)
    End Select
    
    ' Update ADC sample intervals
    Call UpdateADCSampleIntervals(ADCTracesModNumEdit.Value)
    
End Sub


'###########################################
'   GetADCTraces_Click()
'       Acquire ADC traces
'###########################################

Private Sub GetADCTraces_Click()
    Dim retval As Long
    Dim ModuleNumber, ChannelNumber As Integer
    Dim StrTxt As String
    Dim Msg As Integer
    Dim Cnt As Long

    ModuleNumber = ADCTracesModNumEdit.Value
    
    ' First read the ADC traces
    retval = Pixie16AcquireADCTrace(ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16AcquireADCTrace " & " in module # " & Str$(ModuleNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
       
    ' The single channel
    ChannelNumber = SglChADCTracesChanNumEdit.Value
    retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceSglCh(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceSglCh(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    ' Plot the ADC trace
    ADCTraceGraph.PlotY ADCTraceSglCh, 0, XdtSglChan

    ' Read ADC trace for other channels if they are checked
    If Channel0.Value = 1 Then
        ChannelNumber = 0
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs0To3.Plots(1).PlotY ADCTraceCh0, 0, Xdt(0)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh0(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh0(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    If Channel1.Value = 1 Then
        ChannelNumber = 1
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs0To3.Plots(2).PlotY ADCTraceCh1, 0, Xdt(0)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh1(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh1(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    If Channel2.Value = 1 Then
        ChannelNumber = 2
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs0To3.Plots(3).PlotY ADCTraceCh2, 0, Xdt(0)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh2(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh2(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel3.Value = 1 Then
        ChannelNumber = 3
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs0To3.Plots(4).PlotY ADCTraceCh3, 0, Xdt(0)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh3(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh3(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel4.Value = 1 Then
        ChannelNumber = 4
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs4To7.Plots(1).PlotY ADCTraceCh4, 0, Xdt(1)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh4(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh4(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel5.Value = 1 Then
        ChannelNumber = 5
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs4To7.Plots(2).PlotY ADCTraceCh5, 0, Xdt(1)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh5(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh5(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel6.Value = 1 Then
        ChannelNumber = 6
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs4To7.Plots(3).PlotY ADCTraceCh6, 0, Xdt(1)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh6(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh6(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel7.Value = 1 Then
        ChannelNumber = 7
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs4To7.Plots(4).PlotY ADCTraceCh7, 0, Xdt(1)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh7(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh7(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel8.Value = 1 Then
        ChannelNumber = 8
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs8To11.Plots(1).PlotY ADCTraceCh8, 0, Xdt(2)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh8(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh8(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    If Channel9.Value = 1 Then
        ChannelNumber = 9
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs8To11.Plots(2).PlotY ADCTraceCh9, 0, Xdt(2)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh9(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh9(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    If Channel10.Value = 1 Then
        ChannelNumber = 10
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs8To11.Plots(3).PlotY ADCTraceCh10, 0, Xdt(2)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh10(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh10(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    If Channel11.Value = 1 Then
        ChannelNumber = 11
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs8To11.Plots(4).PlotY ADCTraceCh11, 0, Xdt(2)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh11(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh11(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel2.Value = 1 Then
        ChannelNumber = 12
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs12To15.Plots(1).PlotY ADCTraceCh12, 0, Xdt(3)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh12(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh12(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
    
    If Channel13.Value = 1 Then
        ChannelNumber = 13
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs12To15.Plots(2).PlotY ADCTraceCh13, 0, Xdt(3)
    End If
     
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh13(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh13(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt
       
    If Channel14.Value = 1 Then
        ChannelNumber = 14
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs12To15.Plots(3).PlotY ADCTraceCh14, 0, Xdt(3)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh14(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh14(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

    If Channel15.Value = 1 Then
        ChannelNumber = 15
        retval = Pixie16ReadSglChanADCTrace(ADCTraceRdCh(0), ADC_TRACE_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanADCTrace " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        ADCTracesChs12To15.Plots(4).PlotY ADCTraceCh15, 0, Xdt(3)
    End If
    
    For Cnt = 0 To ADC_TRACE_LENGTH - 1
        If ADCTraceRdCh(Cnt) < 0 Then
            ADCTraceCh15(Cnt) = ADCTraceRdCh(Cnt) + 65536
        Else
            ADCTraceCh15(Cnt) = ADCTraceRdCh(Cnt)
        End If
    Next Cnt

End Sub


'###########################################
'   AdjustOffsets_Click()
'       Adjust DC offsets
'###########################################

Private Sub AdjustOffsets_Click()
    Dim retval As Long
    Dim ModuleNumber As Integer
    Dim StrTxt As String
    Dim Msg As Integer

    ' Disable further click
    AdjustOffsets.Enabled = False
    
    ModuleNumber = ADCTracesModNumEdit.Value
    
    ' Adjust DC offsets in this module
    retval = Pixie16AdjustOffsets(ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16AdjustOffsets " & " in module # " & Str$(ModuleNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    ' Update all relevant parameters
    If IsLoaded("DAQParametersSetup") Then
        If ModuleNumber = DAQParametersSetup.DAQParSetupModNumEdit.Value Then
            retval = RetrieveChanParFromModule(ModuleNumber)
            If retval < 0 Then
                StrTxt = "RetrieveChanParFromModule " & "(Module # " & Str$(ModuleNumber) & " ) returned unsuccessfully. Please check the log file"
                Msg = MsgBox(StrTxt, 0, "Error")
                Exit Sub
            End If
        
            ' Update channel parameters displays
            retval = UpdateChanParCtrlsOnDAQForm()
        End If
    End If

    ' Refresh ADC display after adjusting offsets
    Call GetADCTraces_Click
    
    ' Enable click
    AdjustOffsets.Enabled = True
End Sub

'###########################################
'   FFTADCTracesButton_Click()
'       FFT ADC trace
'###########################################

Private Sub FFTADCTracesButton_Click()
    Dim yDataFFT(16383) As Double
    Dim power(8191) As Double
    Dim i As Long
    Dim retval As Integer
    Dim myString As String
    
    For i = LBound(ADCTraceSglCh) To UBound(ADCTraceSglCh)
        yDataFFT(i * 2) = ADCTraceSglCh(i)
    Next i
    For i = (UBound(ADCTraceSglCh) + 1) To 8191
        yDataFFT(i * 2) = ADCTraceSglCh(UBound(ADCTraceSglCh))
    Next i
    
    ' Compute FFT
    retval = Pixie16complexFFT(yDataFFT(0), 8192)
    
    For i = 0 To (8191)
        power(i) = (yDataFFT(i * 2) ^ 2 + yDataFFT(i * 2 + 1) ^ 2) ^ 0.5 / 8192
    Next i
    'clear out the DC offset
    power(0) = 0#
    
    ' Plot FFT
    ADCTraceGraph.PlotY power, 0, 1 / (XdtSglChan * 0.001 * 8192)
    ADCTraceGraph.Axes(1).SetMinMax 0, 8192 / (XdtSglChan * 0.001 * 8192) ' X Axis
    ADCTraceGraph.Axes(2).Autoscale = True
    ADCTraceGraph.Axes(1).Caption = "Frequency (kHz)"
    ADCTraceGraph.Axes(2).Caption = "Amplitude"
End Sub


'###########################################
'   SaveADCTraceDataToFile_Click()
'       Save ADC trace data to a text file
'###########################################

Private Sub SaveADCTraceDataToFile_Click()
    Dim TraceFile
    Dim i As Integer
    
    On Error GoTo ErrHandler
    DlgSaveADCTrace.Filter = "Text Files (*.txt)|*.txt"
    DlgSaveADCTrace.Action = 2
    TraceFile = DlgSaveADCTrace.filename
    
    Open TraceFile For Output As #1
    For i = 0 To ADC_TRACE_LENGTH - 1
        Print #1, ADCTraceSglCh(i)
    Next i
    
    Close #1

ErrHandler:
End Sub


'###########################################
'   AutoScale_Click()
'       Auto scale all axes
'###########################################

Private Sub AutoScale_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ADCTraceGraph.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    
    ADCTraceGraph.Axes(1).SetMinMax 0, XdtSglChan * (ADC_TRACE_LENGTH - 1) ' X Axis

End Sub


'###########################################
'   ADCTraceGraph_CursorChange()
'       Display cursor position
'###########################################

Private Sub ADCTraceGraph_CursorChange(CursorIndex As Long, XPos As Variant, YPos As Variant, bTracking As Boolean)
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    'Display cursor position on user interface
    If CursorIndex = 1 Then
        XPosVal = Format(ADCTraceGraph.Cursors(1).XPosition, "0.####") ' data from cursor
        YPosVal = Format(YPos, "0.####") ' data from event handler
    End If
    If (index = 3) Then
        Dim Amplitude As Double
        Dim Period As Double
        Amplitude = Format(Abs(ADCTraceGraph.Cursors(2).YPosition - ADCTraceGraph.Cursors(1).YPosition), "0.####")
        Period = Format(Abs(ADCTraceGraph.Cursors(2).XPosition - ADCTraceGraph.Cursors(1).XPosition), "0.####")
        dYVal = Amplitude
        dXVal = Period
    End If
End Sub


'###########################################
'   GraphMode_PointerValueChanged()
'       Change TrackMode of graph and hide or show cursors
'###########################################

Private Sub GraphMode_PointerValueChanged(ByVal Pointer As Long, Value As Variant)
    ' Change TrackMode of graph
    ' Hide and show cursors
    Select Case Value
        Case 0
            ADCTraceGraph.TrackMode = cwGTrackZoomRectXY
            ADCTraceGraph.Cursors(1).Visible = False
            ADCTraceGraph.Cursors(2).Visible = False
        Case 1
            ADCTraceGraph.Cursors(1).Visible = False
            ADCTraceGraph.Cursors(2).Visible = False
            ADCTraceGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2
            ADCTraceGraph.TrackMode = cwGTrackDragCursor
            ADCTraceGraph.Cursors(1).Visible = True
            ADCTraceGraph.Cursors(2).Visible = False
            ADCTraceGraph.Cursors(1).XPosition = (ADCTraceGraph.Axes(1).Minimum + ADCTraceGraph.Axes(1).Maximum) / 2
            ADCTraceGraph.Cursors(1).YPosition = (ADCTraceGraph.Axes(2).Minimum + ADCTraceGraph.Axes(2).Maximum) / 2
        Case 3
            ADCTraceGraph.TrackMode = cwGTrackDragCursor
            ADCTraceGraph.Cursors(1).Visible = True
            ADCTraceGraph.Cursors(2).Visible = True

    End Select
End Sub


'###########################################
'   XPosition_ValueChanged(), YPosition_ValueChanged()
'       X and Y-position change
'###########################################

Private Sub XPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    ADCTraceGraph.Cursors(1).XPosition = XPosVal
    ADCTraceGraph.Cursors(1).YPosition = YPosVal
End Sub

Private Sub YPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    ADCTraceGraph.Cursors(1).XPosition = XPosVal
    ADCTraceGraph.Cursors(1).YPosition = YPosVal
End Sub


'###########################################
'   XCombo_Click()
'       X-axis Snap options
'###########################################

Private Sub XCombo_Click()
    Dim index As Integer
    
    index = XCombo.ListIndex
    Select Case index
        Case 0
            ADCTraceGraph.Cursors(1).SnapMode = cwCSnapFixed
            ADCTraceGraph.Cursors(2).SnapMode = cwCSnapFixed
        Case 1
            ADCTraceGraph.Cursors(1).SnapMode = cwCSnapFloating
            ADCTraceGraph.Cursors(2).SnapMode = cwCSnapFloating
        Case 2
            ADCTraceGraph.Cursors(1).SnapMode = cwCSnapNearestPoint
            ADCTraceGraph.Cursors(2).SnapMode = cwCSnapNearestPoint
    End Select
End Sub


'###########################################
'   YScalingCombo_Click()
'       Log or linear scaling for ADC traces
'###########################################

Private Sub YScalingCombo_Click()
    Dim index As Integer
    
    index = YScalingCombo.ListIndex
    Select Case index
        Case 0
            ADCTraceGraph.Axes(2).Log = False
        Case 1
            ADCTraceGraph.Axes(2).Log = True
    End Select

End Sub


'###########################################
'   ZoomCombo_Click()
'       Zoom, pan, cursors for ADC traces
'###########################################

Private Sub ZoomCombo_Click()
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    Select Case index
        Case 0      ' Zoom
            ADCTraceGraph.TrackMode = cwGTrackZoomRectXY
            ADCTraceGraph.Cursors(1).Visible = False
            ADCTraceGraph.Cursors(2).Visible = False
        Case 1      ' Pan
            ADCTraceGraph.Cursors(1).Visible = False
            ADCTraceGraph.Cursors(2).Visible = False
            ADCTraceGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2      ' Cursor Coordinates
            ADCTraceGraph.TrackMode = cwGTrackDragCursor
            ADCTraceGraph.Cursors(1).Visible = True
            ADCTraceGraph.Cursors(2).Visible = False
            ADCTraceGraph.Cursors(1).XPosition = (ADCTraceGraph.Axes(1).Minimum + ADCTraceGraph.Axes(1).Maximum) / 2
            ADCTraceGraph.Cursors(1).YPosition = (ADCTraceGraph.Axes(2).Minimum + ADCTraceGraph.Axes(2).Maximum) / 2
        Case 3      ' Cursor Measurements
            ADCTraceGraph.TrackMode = cwGTrackDragCursor
            ADCTraceGraph.Cursors(1).Visible = True
            ADCTraceGraph.Cursors(2).Visible = True
            ADCTraceGraph.Cursors(1).XPosition = (ADCTraceGraph.Axes(1).Minimum + ADCTraceGraph.Axes(1).Maximum) / 2
            ADCTraceGraph.Cursors(1).YPosition = (ADCTraceGraph.Axes(2).Minimum + ADCTraceGraph.Axes(2).Maximum) / 2
            ADCTraceGraph.Cursors(2).XPosition = (ADCTraceGraph.Axes(1).Minimum + ADCTraceGraph.Axes(1).Maximum) / 2
            ADCTraceGraph.Cursors(2).YPosition = (ADCTraceGraph.Axes(2).Minimum + ADCTraceGraph.Axes(2).Maximum) / 2
    End Select
End Sub


'###########################################
'   ChannelCheckBox Click routines
'       The following subroutines show or disable
'       the ADC traces display
'###########################################

Private Sub Channel0_Click()
    If Channel0.Value = 1 Then
        ADCTracesChs0To3.Plots(1).Visible = True
    Else
        ADCTracesChs0To3.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel1_Click()
    If Channel1.Value = 1 Then
        ADCTracesChs0To3.Plots(2).Visible = True
    Else
        ADCTracesChs0To3.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel2_Click()
    If Channel2.Value = 1 Then
        ADCTracesChs0To3.Plots(3).Visible = True
    Else
        ADCTracesChs0To3.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel3_Click()
    If Channel3.Value = 1 Then
        ADCTracesChs0To3.Plots(4).Visible = True
    Else
        ADCTracesChs0To3.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel4_Click()
    If Channel4.Value = 1 Then
        ADCTracesChs4To7.Plots(1).Visible = True
    Else
        ADCTracesChs4To7.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel5_Click()
    If Channel5.Value = 1 Then
        ADCTracesChs4To7.Plots(2).Visible = True
    Else
        ADCTracesChs4To7.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel6_Click()
    If Channel6.Value = 1 Then
        ADCTracesChs4To7.Plots(3).Visible = True
    Else
        ADCTracesChs4To7.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel7_Click()
    If Channel7.Value = 1 Then
        ADCTracesChs4To7.Plots(4).Visible = True
    Else
        ADCTracesChs4To7.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel8_Click()
    If Channel8.Value = 1 Then
        ADCTracesChs8To11.Plots(1).Visible = True
    Else
        ADCTracesChs8To11.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel9_Click()
    If Channel9.Value = 1 Then
        ADCTracesChs8To11.Plots(2).Visible = True
    Else
        ADCTracesChs8To11.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel10_Click()
    If Channel10.Value = 1 Then
        ADCTracesChs8To11.Plots(3).Visible = True
    Else
        ADCTracesChs8To11.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel11_Click()
    If Channel11.Value = 1 Then
        ADCTracesChs8To11.Plots(4).Visible = True
    Else
        ADCTracesChs8To11.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel12_Click()
    If Channel12.Value = 1 Then
        ADCTracesChs12To15.Plots(1).Visible = True
    Else
        ADCTracesChs12To15.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel13_Click()
    If Channel13.Value = 1 Then
        ADCTracesChs12To15.Plots(2).Visible = True
    Else
        ADCTracesChs12To15.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel14_Click()
    If Channel14.Value = 1 Then
        ADCTracesChs12To15.Plots(3).Visible = True
    Else
        ADCTracesChs12To15.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel15_Click()
    If Channel15.Value = 1 Then
        ADCTracesChs12To15.Plots(4).Visible = True
    Else
        ADCTracesChs12To15.Plots(4).Visible = False
    End If
End Sub


'###########################################
'   SelectOrDeselectChs Click routines
'       The following subroutines select or deselect
'       the ADC traces display in four channels
'###########################################

Private Sub SelectOrDeselectChs0to3_Click()

    If SelectOrDeselectChs0to3.Caption = "Deselect All" Then
        SelectOrDeselectChs0to3.Caption = "Select All"
        Channel0.Value = 0
        Channel1.Value = 0
        Channel2.Value = 0
        Channel3.Value = 0
        ADCTracesChs0To3.Plots(1).Visible = False
        ADCTracesChs0To3.Plots(2).Visible = False
        ADCTracesChs0To3.Plots(3).Visible = False
        ADCTracesChs0To3.Plots(4).Visible = False
    Else
        SelectOrDeselectChs0to3.Caption = "Deselect All"
        Channel0.Value = 1
        Channel1.Value = 1
        Channel2.Value = 1
        Channel3.Value = 1
        ADCTracesChs0To3.Plots(1).Visible = True
        ADCTracesChs0To3.Plots(2).Visible = True
        ADCTracesChs0To3.Plots(3).Visible = True
        ADCTracesChs0To3.Plots(4).Visible = True
    End If
    
End Sub

Private Sub SelectOrDeselectChs4to7_Click()

    If SelectOrDeselectChs4to7.Caption = "Deselect All" Then
        SelectOrDeselectChs4to7.Caption = "Select All"
        Channel4.Value = 0
        Channel5.Value = 0
        Channel6.Value = 0
        Channel7.Value = 0
        ADCTracesChs4To7.Plots(1).Visible = False
        ADCTracesChs4To7.Plots(2).Visible = False
        ADCTracesChs4To7.Plots(3).Visible = False
        ADCTracesChs4To7.Plots(4).Visible = False
    Else
        SelectOrDeselectChs4to7.Caption = "Deselect All"
        Channel4.Value = 1
        Channel5.Value = 1
        Channel6.Value = 1
        Channel7.Value = 1
        ADCTracesChs4To7.Plots(1).Visible = True
        ADCTracesChs4To7.Plots(2).Visible = True
        ADCTracesChs4To7.Plots(3).Visible = True
        ADCTracesChs4To7.Plots(4).Visible = True
    End If
    
End Sub

Private Sub SelectOrDeselectChs8to11_Click()

    If SelectOrDeselectChs8to11.Caption = "Deselect All" Then
        SelectOrDeselectChs8to11.Caption = "Select All"
        Channel8.Value = 0
        Channel9.Value = 0
        Channel10.Value = 0
        Channel11.Value = 0
        ADCTracesChs8To11.Plots(1).Visible = False
        ADCTracesChs8To11.Plots(2).Visible = False
        ADCTracesChs8To11.Plots(3).Visible = False
        ADCTracesChs8To11.Plots(4).Visible = False
    Else
        SelectOrDeselectChs8to11.Caption = "Deselect All"
        Channel8.Value = 1
        Channel9.Value = 1
        Channel10.Value = 1
        Channel11.Value = 1
        ADCTracesChs8To11.Plots(1).Visible = True
        ADCTracesChs8To11.Plots(2).Visible = True
        ADCTracesChs8To11.Plots(3).Visible = True
        ADCTracesChs8To11.Plots(4).Visible = True
    End If
    
End Sub

Private Sub SelectOrDeselectChs12to15_Click()

    If SelectOrDeselectChs12to15.Caption = "Deselect All" Then
        SelectOrDeselectChs12to15.Caption = "Select All"
        Channel12.Value = 0
        Channel13.Value = 0
        Channel14.Value = 0
        Channel15.Value = 0
        ADCTracesChs12To15.Plots(1).Visible = False
        ADCTracesChs12To15.Plots(2).Visible = False
        ADCTracesChs12To15.Plots(3).Visible = False
        ADCTracesChs12To15.Plots(4).Visible = False
    Else
        SelectOrDeselectChs12to15.Caption = "Deselect All"
        Channel12.Value = 1
        Channel13.Value = 1
        Channel14.Value = 1
        Channel15.Value = 1
        ADCTracesChs12To15.Plots(1).Visible = True
        ADCTracesChs12To15.Plots(2).Visible = True
        ADCTracesChs12To15.Plots(3).Visible = True
        ADCTracesChs12To15.Plots(4).Visible = True
    End If
    
End Sub


'###########################################
'   AutoScale Click routines
'       The following subroutines auto scale
'       the ADC traces display
'###########################################

Private Sub AutoScaleChs0To3_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ADCTracesChs0To3.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    ADCTracesChs0To3.Axes(1).SetMinMax 0, Xdt(0) * (ADC_TRACE_LENGTH - 1) ' X Axis

End Sub

Private Sub AutoScaleChs4To7_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ADCTracesChs4To7.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    ADCTracesChs4To7.Axes(1).SetMinMax 0, Xdt(1) * (ADC_TRACE_LENGTH - 1) ' X Axis

End Sub

Private Sub AutoScaleChs8To11_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ADCTracesChs8To11.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    ADCTracesChs8To11.Axes(1).SetMinMax 0, Xdt(2) * (ADC_TRACE_LENGTH - 1) ' X Axis

End Sub

Private Sub AutoScaleChs12To15_Click()
    Dim Axis As CWAxis
    
    For Each Axis In ADCTracesChs12To15.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    ADCTracesChs12To15.Axes(1).SetMinMax 0, Xdt(3) * (ADC_TRACE_LENGTH - 1) ' X Axis

End Sub


'###########################################
'   SampleIntervalCombo_Click()
'       Select ADC sample intervals
'###########################################

Private Sub SampleIntervalComboChs0to3_Click()
    Dim index, ChanNum As Integer
    Dim dT As Double
    Dim ModNum As Integer
    
    ModNum = ADCTracesModNumEdit.Value
    
    index = SampleIntervalComboChs0to3.ListIndex
    dT = 2 ^ index * XDT_SCALE_FACTOR(ModNum) / DSP_CLOCK_MHZ

    ' Write to module
    For ChanNum = 0 To 3
        retval = Pixie16WriteSglChanPar("XDT", dT, ADCTracesModNumEdit.Value, ChanNum)
        If retval < 0 Then
            StrTxt = "Pixie16WriteSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channel # " & Str$(ChanNum)
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
   Next ChanNum

    ' Read back from module
    retval = Pixie16ReadSglChanPar("XDT", Xdt(0), ADCTracesModNumEdit.Value, 0)
    If retval < 0 Then
        StrTxt = "Pixie16ReadSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channels # 0 to 3"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If

    Call AutoScaleChs0To3_Click
    
    If SglChADCTracesChanNumEdit.Value >= 0 And SglChADCTracesChanNumEdit.Value <= 3 Then
        ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(0) * (ADC_TRACE_LENGTH - 1) ' X Axis
        XdtSglChan = Xdt(0)
    End If

End Sub

Private Sub SampleIntervalComboChs4to7_Click()
    Dim index As Integer
    Dim dT As Double
    Dim ModNum As Integer
    
    ModNum = ADCTracesModNumEdit.Value
    
    index = SampleIntervalComboChs4to7.ListIndex
    dT = 2 ^ index * XDT_SCALE_FACTOR(ModNum) / DSP_CLOCK_MHZ

    ' Write to module
    For ChanNum = 4 To 7
        retval = Pixie16WriteSglChanPar("XDT", dT, ADCTracesModNumEdit.Value, ChanNum)
        If retval < 0 Then
            StrTxt = "Pixie16WriteSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channel # " & Str$(ChanNum)
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
   Next ChanNum
   
    ' Read back from module
    retval = Pixie16ReadSglChanPar("XDT", Xdt(1), ADCTracesModNumEdit.Value, 4)
    If retval < 0 Then
        StrTxt = "Pixie16ReadSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channels # 4 to 7"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    Call AutoScaleChs4To7_Click
    
    If SglChADCTracesChanNumEdit.Value >= 4 And SglChADCTracesChanNumEdit.Value <= 7 Then
        ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(1) * (ADC_TRACE_LENGTH - 1) ' X Axis
        XdtSglChan = Xdt(1)
    End If
    
End Sub

Private Sub SampleIntervalComboChs8to11_Click()
    Dim index As Integer
    Dim dT As Double
    Dim ModNum As Integer
    
    ModNum = ADCTracesModNumEdit.Value
    
    index = SampleIntervalComboChs8to11.ListIndex
    dT = 2 ^ index * XDT_SCALE_FACTOR(ModNum) / DSP_CLOCK_MHZ

    ' Write to module
   For ChanNum = 8 To 11
        retval = Pixie16WriteSglChanPar("XDT", dT, ADCTracesModNumEdit.Value, ChanNum)
        If retval < 0 Then
            StrTxt = "Pixie16WriteSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channel # " & Str$(ChanNum)
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
   Next ChanNum
    
    ' Read back from module
    retval = Pixie16ReadSglChanPar("XDT", Xdt(2), ADCTracesModNumEdit.Value, 8)
    If retval < 0 Then
        StrTxt = "Pixie16ReadSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channels # 8 to 11"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    Call AutoScaleChs8To11_Click
    
    If SglChADCTracesChanNumEdit.Value >= 8 And SglChADCTracesChanNumEdit.Value <= 11 Then
        ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(2) * (ADC_TRACE_LENGTH - 1) ' X Axis
        XdtSglChan = Xdt(2)
    End If
    
End Sub

Private Sub SampleIntervalComboChs12to15_Click()
    Dim index As Integer
    Dim dT As Double
    Dim ModNum As Integer
    
    ModNum = ADCTracesModNumEdit.Value
    
    index = SampleIntervalComboChs12to15.ListIndex
    dT = 2 ^ index * XDT_SCALE_FACTOR(ModNum) / DSP_CLOCK_MHZ

    ' Write to module
   For ChanNum = 12 To 15
        retval = Pixie16WriteSglChanPar("XDT", dT, ADCTracesModNumEdit.Value, ChanNum)
        If retval < 0 Then
            StrTxt = "Pixie16WriteSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channel # " & Str$(ChanNum)
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
   Next ChanNum
   
    ' Read back from module
    retval = Pixie16ReadSglChanPar("XDT", Xdt(3), ADCTracesModNumEdit.Value, 12)
    If retval < 0 Then
        StrTxt = "Pixie16ReadSglChanPar failed to set XDT in module # " & Str$(ADCTracesModNumEdit.Value) & " channels # 12 to 15"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    Call AutoScaleChs12To15_Click
    
    If SglChADCTracesChanNumEdit.Value >= 12 And SglChADCTracesChanNumEdit.Value <= 15 Then
        ADCTraceGraph.Axes(1).SetMinMax 0, Xdt(3) * (ADC_TRACE_LENGTH - 1) ' X Axis
        XdtSglChan = Xdt(3)
    End If
    
End Sub



Private Sub CloseADCForm_Click()
    Unload Me
End Sub

