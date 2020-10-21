VERSION 5.00
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form ReadMCAHistogramsForm 
   Caption         =   "Read Histograms"
   ClientHeight    =   10830
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11445
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   10830
   ScaleWidth      =   11445
   Begin VB.CommandButton ReadMCAHistogramsFromFile 
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
      Left            =   120
      TabIndex        =   50
      Top             =   9720
      Width           =   1935
   End
   Begin VB.TextBox HistogramDataFile 
      Height          =   285
      Left            =   1920
      TabIndex        =   48
      Top             =   9240
      Width           =   9015
   End
   Begin VB.CommandButton ReadMCAHistogramsFromModule 
      Caption         =   "Refresh (read from module)"
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
      Left            =   6720
      TabIndex        =   34
      Top             =   9840
      Width           =   2775
   End
   Begin VB.CommandButton CloseMCAHistogramForm 
      Caption         =   "Close"
      Height          =   615
      Left            =   9720
      TabIndex        =   3
      Top             =   9840
      Width           =   1455
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   8655
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   11175
      _ExtentX        =   19711
      _ExtentY        =   15266
      _Version        =   393216
      TabOrientation  =   1
      Tabs            =   2
      TabHeight       =   520
      TabCaption(0)   =   "MCA histogram - single channel"
      TabPicture(0)   =   "ReadMCAHistograms.frx":0000
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
      Tab(0).Control(5)=   "MCAHistogramGraph"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "SglChMCAHistogramsChanNumEdit"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "ZoomCombo"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "XCombo"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "YScalingCombo"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).Control(10)=   "dYVal"
      Tab(0).Control(10).Enabled=   0   'False
      Tab(0).Control(11)=   "YPosVal"
      Tab(0).Control(11).Enabled=   0   'False
      Tab(0).Control(12)=   "dXVal"
      Tab(0).Control(12).Enabled=   0   'False
      Tab(0).Control(13)=   "XPosVal"
      Tab(0).Control(13).Enabled=   0   'False
      Tab(0).Control(14)=   "Autoscale"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "ROIButton"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).Control(16)=   "PeakAreaText"
      Tab(0).Control(16).Enabled=   0   'False
      Tab(0).Control(17)=   "FWHMText"
      Tab(0).Control(17).Enabled=   0   'False
      Tab(0).Control(18)=   "PeakPosText"
      Tab(0).Control(18).Enabled=   0   'False
      Tab(0).ControlCount=   19
      TabCaption(1)   =   "MCA histogram - 16 channels"
      TabPicture(1)   =   "ReadMCAHistograms.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Channel15"
      Tab(1).Control(1)=   "Channel14"
      Tab(1).Control(2)=   "Channel13"
      Tab(1).Control(3)=   "Channel12"
      Tab(1).Control(4)=   "Channel11"
      Tab(1).Control(5)=   "Channel10"
      Tab(1).Control(6)=   "Channel9"
      Tab(1).Control(7)=   "Channel8"
      Tab(1).Control(8)=   "Channel4"
      Tab(1).Control(9)=   "Channel5"
      Tab(1).Control(10)=   "Channel6"
      Tab(1).Control(11)=   "Channel7"
      Tab(1).Control(12)=   "Channel0"
      Tab(1).Control(13)=   "Channel1"
      Tab(1).Control(14)=   "Channel2"
      Tab(1).Control(15)=   "Channel3"
      Tab(1).Control(16)=   "AutoScaleChs12To15"
      Tab(1).Control(17)=   "AutoScaleChs8To11"
      Tab(1).Control(18)=   "AutoScaleChs4To7"
      Tab(1).Control(19)=   "AutoScaleChs0To3"
      Tab(1).Control(20)=   "MCAHistogramsChs0To3"
      Tab(1).Control(21)=   "MCAHistogramsChs4To7"
      Tab(1).Control(22)=   "MCAHistogramsChs8To11"
      Tab(1).Control(23)=   "MCAHistogramsChs12To15"
      Tab(1).ControlCount=   24
      Begin VB.TextBox PeakPosText 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   7335
         Locked          =   -1  'True
         TabIndex        =   46
         Text            =   "Peak Pos:"
         Top             =   6840
         Width           =   1575
      End
      Begin VB.TextBox FWHMText 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   8895
         Locked          =   -1  'True
         TabIndex        =   45
         Text            =   "FWHM:"
         Top             =   6840
         Width           =   2055
      End
      Begin VB.TextBox PeakAreaText 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   5535
         Locked          =   -1  'True
         TabIndex        =   44
         Text            =   "Peak Area:"
         Top             =   6840
         Width           =   1815
      End
      Begin VB.CommandButton ROIButton 
         Caption         =   "ROI"
         Height          =   255
         Left            =   5040
         TabIndex        =   43
         Top             =   6870
         Width           =   495
      End
      Begin VB.CheckBox Channel15 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch15"
         Height          =   255
         Left            =   -66600
         TabIndex        =   42
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel14 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch14"
         Height          =   255
         Left            =   -67440
         TabIndex        =   41
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel13 
         BackColor       =   &H000000FF&
         Caption         =   "Ch13"
         Height          =   255
         Left            =   -68280
         TabIndex        =   40
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel12 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch12"
         Height          =   255
         Left            =   -69120
         TabIndex        =   39
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel11 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch11"
         Height          =   255
         Left            =   -72360
         TabIndex        =   38
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel10 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch10"
         Height          =   255
         Left            =   -73200
         TabIndex        =   37
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel9 
         BackColor       =   &H000000FF&
         Caption         =   "Ch9"
         Height          =   255
         Left            =   -73920
         TabIndex        =   36
         Top             =   4320
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel8 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch8"
         Height          =   255
         Left            =   -74640
         TabIndex        =   35
         Top             =   4320
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel4 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch4"
         Height          =   255
         Left            =   -69000
         TabIndex        =   33
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel5 
         BackColor       =   &H000000FF&
         Caption         =   "Ch5"
         Height          =   255
         Left            =   -68280
         TabIndex        =   32
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel6 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch6"
         Height          =   255
         Left            =   -67560
         TabIndex        =   31
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel7 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch7"
         Height          =   255
         Left            =   -66840
         TabIndex        =   30
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel0 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch0"
         Height          =   255
         Left            =   -74640
         TabIndex        =   29
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel1 
         BackColor       =   &H000000FF&
         Caption         =   "Ch1"
         Height          =   255
         Left            =   -73920
         TabIndex        =   28
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel2 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch2"
         Height          =   255
         Left            =   -73200
         TabIndex        =   27
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel3 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch3"
         Height          =   255
         Left            =   -72480
         TabIndex        =   26
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CommandButton AutoScaleChs12To15 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -65640
         TabIndex        =   25
         Top             =   4200
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs8To11 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -71160
         TabIndex        =   24
         Top             =   4200
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs4To7 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -65640
         TabIndex        =   23
         Top             =   120
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs0To3 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -71280
         TabIndex        =   22
         Top             =   120
         Width           =   1455
      End
      Begin VB.CommandButton Autoscale 
         Caption         =   "Autoscale"
         Height          =   495
         Left            =   5040
         TabIndex        =   17
         Top             =   7320
         Width           =   1215
      End
      Begin VB.TextBox XPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   435
         TabIndex        =   12
         Top             =   6840
         Width           =   1095
      End
      Begin VB.TextBox dXVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   1875
         TabIndex        =   11
         Top             =   6840
         Width           =   1095
      End
      Begin VB.TextBox YPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   435
         TabIndex        =   10
         Top             =   7200
         Width           =   1095
      End
      Begin VB.TextBox dYVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   1875
         TabIndex        =   9
         Top             =   7200
         Width           =   1095
      End
      Begin VB.ComboBox YScalingCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   9600
         Style           =   2  'Dropdown List
         TabIndex        =   8
         Top             =   240
         Width           =   1335
      End
      Begin VB.ComboBox XCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   1800
         Style           =   2  'Dropdown List
         TabIndex        =   7
         Top             =   240
         Width           =   1575
      End
      Begin VB.ComboBox ZoomCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   240
         Style           =   2  'Dropdown List
         TabIndex        =   6
         Top             =   240
         Width           =   1575
      End
      Begin CWUIControlsLib.CWNumEdit SglChMCAHistogramsChanNumEdit 
         Height          =   495
         Left            =   9600
         TabIndex        =   5
         Top             =   7320
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
      Begin CWUIControlsLib.CWGraph MCAHistogramsChs0To3 
         Height          =   3375
         Left            =   -74760
         TabIndex        =   18
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
         szText_12       =   "Histogram Bin Number"
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
         szText_19       =   "Counts"
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
      Begin CWUIControlsLib.CWGraph MCAHistogramsChs4To7 
         Height          =   3375
         Left            =   -69240
         TabIndex        =   19
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
         szText_12       =   "Histogram Bin Number"
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
         szText_19       =   "Counts"
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
      Begin CWUIControlsLib.CWGraph MCAHistogramsChs8To11 
         Height          =   3375
         Left            =   -74760
         TabIndex        =   20
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
         szText_12       =   "Histogram Bin Number"
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
         szText_19       =   "Counts"
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
      Begin CWUIControlsLib.CWGraph MCAHistogramsChs12To15 
         Height          =   3375
         Left            =   -69240
         TabIndex        =   21
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
         szText_12       =   "Histogram Bin Number"
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
         szText_19       =   "Counts"
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
      Begin CWUIControlsLib.CWGraph MCAHistogramGraph 
         Height          =   6015
         Left            =   240
         TabIndex        =   47
         Top             =   600
         Width           =   10695
         _Version        =   393218
         _ExtentX        =   18865
         _ExtentY        =   10610
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
         opts_6          =   543
         Name_6          =   "X-Axis"
         Orientation_6   =   2944
         format_6        =   7
         ClassName_7     =   "CCWFormat"
         Scale_6         =   8
         ClassName_8     =   "CCWScale"
         opts_8          =   90112
         rMin_8          =   40
         rMax_8          =   697
         dMax_8          =   100
         discInterval_8  =   1
         Radial_6        =   0
         Enum_6          =   9
         ClassName_9     =   "CCWEnum"
         Editor_9        =   10
         ClassName_10    =   "CCWEnumArrayEditor"
         Owner_10        =   6
         Font_6          =   0
         tickopts_6      =   2711
         major_6         =   10
         minor_6         =   5
         Caption_6       =   11
         ClassName_11    =   "CCWDrawObj"
         opts_11         =   30
         C[0]_11         =   16711680
         Image_11        =   12
         ClassName_12    =   "CCWTextImage"
         szText_12       =   "Histogram Bin number"
         style_12        =   1701016175
         font_12         =   0
         Animator_11     =   0
         Blinker_11      =   0
         Y_5             =   13
         ClassName_13    =   "CCWAxis"
         opts_13         =   1567
         Name_13         =   "Y-Axis"
         Orientation_13  =   2067
         format_13       =   14
         ClassName_14    =   "CCWFormat"
         Scale_13        =   15
         ClassName_15    =   "CCWScale"
         opts_15         =   122880
         rMin_15         =   11
         rMax_15         =   361
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
         opts_18         =   30
         C[0]_18         =   16711680
         Image_18        =   19
         ClassName_19    =   "CCWTextImage"
         szText_19       =   "Counts"
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
         opts_20         =   4194365
         Name_20         =   "Plot-2"
         C[0]_20         =   65280
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
         Array_24        =   4
         Editor_24       =   25
         ClassName_25    =   "CCWGFCursorArrayEditor"
         Owner_25        =   1
         Array[0]_24     =   26
         ClassName_26    =   "CCWCursor"
         opts_26         =   29
         Name_26         =   "Cursor-1"
         C[0]_26         =   255
         Event_26        =   2
         X_26            =   6
         Y_26            =   13
         PointIndex_26   =   9
         ChrosshairStyle_26=   8
         LockPlot_26     =   0
         Array[1]_24     =   27
         ClassName_27    =   "CCWCursor"
         opts_27         =   29
         Name_27         =   "Cursor-2"
         C[0]_27         =   16776960
         Event_27        =   2
         X_27            =   6
         Y_27            =   13
         XPos_27         =   20
         YPos_27         =   2
         PointIndex_27   =   -1
         ChrosshairStyle_27=   8
         LockPlot_27     =   0
         Array[2]_24     =   28
         ClassName_28    =   "CCWCursor"
         opts_28         =   61
         Name_28         =   "Cursor-3"
         C[0]_28         =   16711680
         Event_28        =   2
         X_28            =   0
         Y_28            =   0
         XPos_28         =   20
         YPos_28         =   2
         PointIndex_28   =   -1
         ChrosshairStyle_28=   2
         LockPlot_28     =   0
         Array[3]_24     =   29
         ClassName_29    =   "CCWCursor"
         opts_29         =   61
         Name_29         =   "Cursor-4"
         C[0]_29         =   16711680
         Event_29        =   2
         X_29            =   0
         Y_29            =   0
         XPos_29         =   30
         YPos_29         =   3
         PointIndex_29   =   -1
         ChrosshairStyle_29=   2
         LockPlot_29     =   0
         TrackMode_1     =   6
         GraphBackground_1=   0
         GraphFrame_1    =   30
         ClassName_30    =   "CCWDrawObj"
         opts_30         =   30
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
         opts_32         =   30
         C[1]_32         =   0
         Image_32        =   33
         ClassName_33    =   "CCWPictImage"
         opts_33         =   1024
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
         opts_34         =   30
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
         opts_41         =   4194335
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
         TextXPoint_40   =   67
         TextYPoint_40   =   67
         TextColor_40    =   16777215
         TextFont_40     =   42
         ClassName_42    =   "CCWFont"
         bFont_42        =   -1  'True
         BeginProperty Font_42 {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ShapeXPoints_40 =   43
         ClassName_43    =   "CDataBuffer"
         Type_43         =   5
         m_cDims;_43     =   1
         ShapeYPoints_40 =   44
         ClassName_44    =   "CDataBuffer"
         Type_44         =   5
         m_cDims;_44     =   1
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
      Begin VB.Label dY 
         Caption         =   "dY"
         Height          =   255
         Left            =   1605
         TabIndex        =   16
         Top             =   7290
         Width           =   255
      End
      Begin VB.Label dX 
         Caption         =   "dX"
         Height          =   255
         Left            =   1605
         TabIndex        =   15
         Top             =   6945
         Width           =   255
      End
      Begin VB.Label YPos 
         Caption         =   "Y"
         Height          =   255
         Left            =   255
         TabIndex        =   14
         Top             =   7290
         Width           =   255
      End
      Begin VB.Label XPos 
         Caption         =   "X"
         Height          =   255
         Left            =   240
         TabIndex        =   13
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
         Left            =   7920
         TabIndex        =   4
         Top             =   7440
         Width           =   1575
      End
   End
   Begin CWUIControlsLib.CWNumEdit ReadMCAHistogramsModNumEdit 
      Height          =   495
      Left            =   5880
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
   Begin VB.Label HistogramDataFileLabel 
      Caption         =   "Histogram data file"
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
      Left            =   120
      TabIndex        =   49
      Top             =   9240
      Width           =   1815
   End
   Begin VB.Image OpenHistogramDataFile 
      Height          =   225
      Left            =   11040
      Picture         =   "ReadMCAHistograms.frx":0038
      Top             =   9240
      Width           =   240
   End
   Begin VB.Label MCAHistogramModNumLabel 
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
      Left            =   4080
      TabIndex        =   1
      Top             =   9960
      Width           =   1815
   End
End
Attribute VB_Name = "ReadMCAHistogramsForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

'###########################################
'   Form_Load()
'       Load the MCAHistogram Form
'###########################################

Private Sub Form_Load()

    ZoomCombo.AddItem "Zoom"
    ZoomCombo.AddItem "ROI"
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
    
    MCAHistogramGraph.TrackMode = cwGTrackZoomRectXY
 
    ' Set graph axis limits from program
    MCAHistogramGraph.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis
    MCAHistogramGraph.Axes(2).Autoscale = True
    
    MCAHistogramsChs0To3.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis
    MCAHistogramsChs0To3.Axes(2).Autoscale = True
    
    MCAHistogramsChs4To7.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis
    MCAHistogramsChs4To7.Axes(2).Autoscale = True
    
    MCAHistogramsChs8To11.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis
    MCAHistogramsChs8To11.Axes(2).Autoscale = True
   
    MCAHistogramsChs12To15.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis
    MCAHistogramsChs12To15.Axes(2).Autoscale = True
     
    ' Update maximum number of modules
    ReadMCAHistogramsModNumEdit.Maximum = Number_Modules_Total - 1
   
    ' Update histogram data file name
    HistogramDataFile = Pixie16_HistogramDataFile_Name(0)
   
End Sub


'###########################################
'   CloseMCAHistogramForm_Click()
'       Close the MCAHistogram Form
'###########################################

Private Sub CloseMCAHistogramForm_Click()
    Unload Me
End Sub

'###########################################
'   OpenHistogramDataFile_Click()
'       Browse for histogram data file
'###########################################

Private Sub OpenHistogramDataFile_Click()
    Dim getfile As String
    
    getfile = BrowseForFile(Me, "Select the histogram data file", HistogramDataFile)
    If Len(getfile) = 0 Then Exit Sub  'user selected cancel
    HistogramDataFile = getfile
End Sub

'###########################################
'   ReadMCAHistogramsFromModule_Click()
'       Read MCA histograms from module
'###########################################

Private Sub ReadMCAHistogramsFromModule_Click()
    Dim retval As Long
    Dim ModuleNumber, ChannelNumber As Integer
    Dim StrTxt As String
    Dim Msg As Integer

    ModuleNumber = ReadMCAHistogramsModNumEdit.Value
    
    ' First the single channel
    ChannelNumber = SglChMCAHistogramsChanNumEdit.Value
    
    retval = Pixie16ReadHistogramFromModule(MCASpecSglCh(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
   
    ' Plot the MCA histogram
    MCAHistogramGraph.PlotY MCASpecSglCh
    'MCAHistogramGraph.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

    ' Read histograms for other channels if they are checked
    If Channel0.Value = 1 Then
        ChannelNumber = 0
        retval = Pixie16ReadHistogramFromModule(MCASpecCh0(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(1).PlotY MCASpecCh0
    End If
    
    If Channel1.Value = 1 Then
        ChannelNumber = 1
        retval = Pixie16ReadHistogramFromModule(MCASpecCh1(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(2).PlotY MCASpecCh1
    End If
    
    If Channel2.Value = 1 Then
        ChannelNumber = 2
        retval = Pixie16ReadHistogramFromModule(MCASpecCh2(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(3).PlotY MCASpecCh2
    End If

    If Channel3.Value = 1 Then
        ChannelNumber = 3
        retval = Pixie16ReadHistogramFromModule(MCASpecCh3(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(4).PlotY MCASpecCh3
    End If

    If Channel4.Value = 1 Then
        ChannelNumber = 4
        retval = Pixie16ReadHistogramFromModule(MCASpecCh4(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(1).PlotY MCASpecCh4
    End If

    If Channel5.Value = 1 Then
        ChannelNumber = 5
        retval = Pixie16ReadHistogramFromModule(MCASpecCh5(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(2).PlotY MCASpecCh5
    End If

    If Channel6.Value = 1 Then
        ChannelNumber = 6
        retval = Pixie16ReadHistogramFromModule(MCASpecCh6(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(3).PlotY MCASpecCh6
    End If

    If Channel7.Value = 1 Then
        ChannelNumber = 7
        retval = Pixie16ReadHistogramFromModule(MCASpecCh7(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(4).PlotY MCASpecCh7
    End If

    If Channel8.Value = 1 Then
        ChannelNumber = 8
        retval = Pixie16ReadHistogramFromModule(MCASpecCh8(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(1).PlotY MCASpecCh8
    End If
    
    If Channel9.Value = 1 Then
        ChannelNumber = 9
        retval = Pixie16ReadHistogramFromModule(MCASpecCh9(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(2).PlotY MCASpecCh9
    End If
    
    If Channel10.Value = 1 Then
        ChannelNumber = 10
        retval = Pixie16ReadHistogramFromModule(MCASpecCh10(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(3).PlotY MCASpecCh10
    End If
    
    If Channel11.Value = 1 Then
        ChannelNumber = 11
        retval = Pixie16ReadHistogramFromModule(MCASpecCh11(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(4).PlotY MCASpecCh11
    End If

    If Channel2.Value = 1 Then
        ChannelNumber = 12
        retval = Pixie16ReadHistogramFromModule(MCASpecCh12(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(1).PlotY MCASpecCh12
    End If
    
    If Channel13.Value = 1 Then
        ChannelNumber = 13
        retval = Pixie16ReadHistogramFromModule(MCASpecCh13(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(2).PlotY MCASpecCh13
    End If
        
    If Channel14.Value = 1 Then
        ChannelNumber = 14
        retval = Pixie16ReadHistogramFromModule(MCASpecCh14(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(3).PlotY MCASpecCh14
    End If

    If Channel15.Value = 1 Then
        ChannelNumber = 15
        retval = Pixie16ReadHistogramFromModule(MCASpecCh15(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromModule " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(4).PlotY MCASpecCh15
    End If

End Sub


'###########################################
'   ReadMCAHistogramsFromFile_Click()
'       Read MCA histograms from a saved data file
'###########################################

Private Sub ReadMCAHistogramsFromFile_Click()
    Dim retval As Long
    Dim ModuleNumber, ChannelNumber As Integer
    Dim StrTxt As String
    Dim Msg As Integer

    ModuleNumber = 0
    
    ' First the single channel
    ChannelNumber = SglChMCAHistogramsChanNumEdit.Value
    
    retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecSglCh(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
   
    ' Plot the MCA histogram
    MCAHistogramGraph.PlotY MCASpecSglCh
    MCAHistogramGraph.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

    ' Read histograms for other channels if they are checked
    If Channel0.Value = 1 Then
        ChannelNumber = 0
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh0(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(1).PlotY MCASpecCh0
    End If
    
    If Channel1.Value = 1 Then
        ChannelNumber = 1
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh1(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(2).PlotY MCASpecCh1
    End If
    
    If Channel2.Value = 1 Then
        ChannelNumber = 2
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh2(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(3).PlotY MCASpecCh2
    End If

    If Channel3.Value = 1 Then
        ChannelNumber = 3
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh3(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs0To3.Plots(4).PlotY MCASpecCh3
    End If

    If Channel4.Value = 1 Then
        ChannelNumber = 4
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh4(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(1).PlotY MCASpecCh4
    End If

    If Channel5.Value = 1 Then
        ChannelNumber = 5
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh5(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(2).PlotY MCASpecCh5
    End If

    If Channel6.Value = 1 Then
        ChannelNumber = 6
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh6(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(3).PlotY MCASpecCh6
    End If

    If Channel7.Value = 1 Then
        ChannelNumber = 7
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh7(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs4To7.Plots(4).PlotY MCASpecCh7
    End If

    If Channel8.Value = 1 Then
        ChannelNumber = 8
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh8(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(1).PlotY MCASpecCh8
    End If
    
    If Channel9.Value = 1 Then
        ChannelNumber = 9
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh9(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(2).PlotY MCASpecCh9
    End If
    
    If Channel10.Value = 1 Then
        ChannelNumber = 10
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh10(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(3).PlotY MCASpecCh10
    End If
    
    If Channel11.Value = 1 Then
        ChannelNumber = 11
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh11(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs8To11.Plots(4).PlotY MCASpecCh11
    End If

    If Channel2.Value = 1 Then
        ChannelNumber = 12
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh12(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(1).PlotY MCASpecCh12
    End If
    
    If Channel13.Value = 1 Then
        ChannelNumber = 13
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh13(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(2).PlotY MCASpecCh13
    End If
        
    If Channel14.Value = 1 Then
        ChannelNumber = 14
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh14(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(3).PlotY MCASpecCh14
    End If

    If Channel15.Value = 1 Then
        ChannelNumber = 15
        retval = Pixie16ReadHistogramFromFile(HistogramDataFile, MCASpecCh15(0), MAX_HISTOGRAM_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadHistogramFromFile " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        MCAHistogramsChs12To15.Plots(4).PlotY MCASpecCh15
    End If

End Sub

'###########################################
'   MCAHistogramGraph_MouseUp()
'       Respond to MouseUp event
'###########################################

Private Sub MCAHistogramGraph_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim pos As Double
    Dim fwhm As Double
    Dim cursor1 As Long
    Dim cursor2 As Long
    Dim i As Long
    Dim index As Integer
    Dim fitdata(MAX_HISTOGRAM_LENGTH - 1) As Double
    
    index = ZoomCombo.ListIndex
    
    If (index = 1) Then
        cursor1 = MCAHistogramGraph.Cursors(3).XPosition
        cursor2 = MCAHistogramGraph.Cursors(4).XPosition
        
        For i = 0 To MAX_HISTOGRAM_LENGTH - 1
            fitdata(i) = MCASpecSglCh(i)
        Next i
        
        If (cursor2 > cursor1) Then
            Call fitGauss(fitdata, cursor1, cursor2, pos, fwhm)
        ElseIf (cursor1 > cursor2) Then
            Call fitGauss(fitdata, cursor2, cursor1, pos, fwhm)
        End If
        
        PeakPosText = "Peak Pos: " + CStr(Format(pos, "0.##"))
        If (pos > 0) Then
            FWHMText = "FWHM: " + CStr(Format(fwhm, "0.##")) + ",  " + _
                        CStr(Format((fwhm / pos) * 100, "0.####")) + "%"
        Else
            FWHMText = "FWHM: 0,  0%"
        End If
    End If

End Sub


'###########################################
'   MCAHistogramGraph_CursorChange()
'       Respond to cursor position change
'###########################################

Private Sub MCAHistogramGraph_CursorChange(CursorIndex As Long, XPos As Variant, YPos As Variant, bTracking As Boolean)
    Dim cursor1 As Long
    Dim cursor2 As Long
    Dim X As Long
    Dim count As Long
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    If index <> 1 Then
        'Display cursor position on user interface
        If CursorIndex = 1 Then
            XPosVal = Format(MCAHistogramGraph.Cursors(1).XPosition, "0.####") ' data from cursor
            YPosVal = Format(YPos, "0.####") ' data from event handler
        End If
        If index = 4 Then
            Dim Amplitude As Double
            Dim Period As Double
            Amplitude = Format(Abs(MCAHistogramGraph.Cursors(2).YPosition - MCAHistogramGraph.Cursors(1).YPosition), "0.####")
            Period = Format(Abs(MCAHistogramGraph.Cursors(2).XPosition - MCAHistogramGraph.Cursors(1).XPosition), "0.####")
            dYVal = Amplitude
            dXVal = Period
        End If
    Else
        ' Get current cursor positions
        cursor1 = MCAHistogramGraph.Cursors.Item(3).XPosition
        cursor2 = MCAHistogramGraph.Cursors.Item(4).XPosition
    
        ' Get the difference in position between to 2 cursors and read
        ' that amout of data form the roiHistogram using the smaller
        ' cursor value as the starting point.  Plot this data on the
        ' roi plot starting at the smaller cursors position
        If cursor2 > cursor1 Then
            ReDim roi(cursor2 - cursor1)
            For X = 0 To (cursor2 - cursor1)
                roi(X) = MCASpecSglCh(cursor1 + X)
            Next X
            MCAHistogramGraph.Plots(2).ClearData
            MCAHistogramGraph.Plots(2).PlotY roi, cursor1
            MCAHistogramGraph.Plots(2).Visible = True
        ElseIf cursor1 > cursor2 Then
            ReDim roi(cursor1 - cursor2)
            For X = 0 To (cursor1 - cursor2)
                roi(X) = MCASpecSglCh(cursor2 + X)
            Next X
            MCAHistogramGraph.Plots(2).ClearData
            MCAHistogramGraph.Plots(2).PlotY roi, cursor2
            MCAHistogramGraph.Plots(2).Visible = True
        End If
        
        ' Calculate the peak area between the 2 cursors and
        ' store in the variable count
        If cursor1 > cursor2 Then
            For X = cursor2 To cursor1
                count = count + MCASpecSglCh(X)
            Next X
        ElseIf cursor2 > cursor1 Then
            For X = cursor1 To cursor2
                count = count + MCASpecSglCh(X)
            Next X
        End If
        
        PeakAreaText = "Peak Area: " + CStr(count)
        
        'Display cursor position on user interface
        If CursorIndex = 3 Then
            XPosVal = Format(MCAHistogramGraph.Cursors(3).XPosition, "0.####") ' data from cursor
        ElseIf CursorIndex = 4 Then
            XPosVal = Format(MCAHistogramGraph.Cursors(4).XPosition, "0.####")
        End If
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
            MCAHistogramGraph.TrackMode = cwGTrackZoomRectXY
            MCAHistogramGraph.Cursors(1).Visible = False
            MCAHistogramGraph.Cursors(2).Visible = False
        Case 1
            MCAHistogramGraph.Cursors(1).Visible = False
            MCAHistogramGraph.Cursors(2).Visible = False
            MCAHistogramGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2
            MCAHistogramGraph.TrackMode = cwGTrackDragCursor
            MCAHistogramGraph.Cursors(1).Visible = True
            MCAHistogramGraph.Cursors(2).Visible = False
            MCAHistogramGraph.Cursors(1).XPosition = (MCAHistogramGraph.Axes(1).Minimum + MCAHistogramGraph.Axes(1).Maximum) / 2
            MCAHistogramGraph.Cursors(1).YPosition = (MCAHistogramGraph.Axes(2).Minimum + MCAHistogramGraph.Axes(2).Maximum) / 2
        Case 3
            MCAHistogramGraph.TrackMode = cwGTrackDragCursor
            MCAHistogramGraph.Cursors(1).Visible = True
            MCAHistogramGraph.Cursors(2).Visible = True

    End Select
End Sub


'###########################################
'   XPosition_ValueChanged()
'       Move cursor's X-position
'###########################################

Private Sub XPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    MCAHistogramGraph.Cursors(1).XPosition = XPosVal
    MCAHistogramGraph.Cursors(1).YPosition = YPosVal
End Sub


'###########################################
'   YPosition_ValueChanged()
'       move cursor's Y-position
'###########################################

Private Sub YPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    MCAHistogramGraph.Cursors(1).XPosition = XPosVal
    MCAHistogramGraph.Cursors(1).YPosition = YPosVal
End Sub


'###########################################
'   ROIButton_Click()
'       Display ROI settings
'###########################################

Private Sub ROIButton_Click()
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    If index = 1 Then
        ZoomCombo.ListIndex = 0
        PeakAreaText = "Peak Area:"
        PeakPosText = "Peak Pos:"
        FWHMText = "FWHM:"
    Else
        ZoomCombo.ListIndex = 1
    End If
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
            MCAHistogramGraph.Cursors(1).SnapMode = cwCSnapFixed
            MCAHistogramGraph.Cursors(2).SnapMode = cwCSnapFixed
            MCAHistogramGraph.Cursors(3).SnapMode = cwCSnapFixed
            MCAHistogramGraph.Cursors(4).SnapMode = cwCSnapFixed
            
        Case 1
            MCAHistogramGraph.Cursors(1).SnapMode = cwCSnapFloating
            MCAHistogramGraph.Cursors(2).SnapMode = cwCSnapFloating
            MCAHistogramGraph.Cursors(3).SnapMode = cwCSnapFloating
            MCAHistogramGraph.Cursors(4).SnapMode = cwCSnapFloating
            
        Case 2
            MCAHistogramGraph.Cursors(1).SnapMode = cwCSnapNearestPoint
            MCAHistogramGraph.Cursors(2).SnapMode = cwCSnapNearestPoint
            MCAHistogramGraph.Cursors(3).SnapMode = cwCSnapNearestPoint
            MCAHistogramGraph.Cursors(4).SnapMode = cwCSnapNearestPoint
            
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
            MCAHistogramGraph.TrackMode = cwGTrackZoomRectXY
            MCAHistogramGraph.Cursors(1).Visible = False
            MCAHistogramGraph.Cursors(2).Visible = False
    
            MCAHistogramGraph.Plots(2).Visible = False
            MCAHistogramGraph.Cursors.Item(3).Visible = False
            MCAHistogramGraph.Cursors.Item(4).Visible = False
            
        Case 1      ' ROI
            MCAHistogramGraph.TrackMode = cwGTrackDragCursor
            Call DisplayROI
            
        Case 2      ' Pan
            MCAHistogramGraph.Cursors(1).Visible = False
            MCAHistogramGraph.Cursors(2).Visible = False
            MCAHistogramGraph.TrackMode = cwGTrackPanPlotAreaXY
    
            MCAHistogramGraph.Plots(2).Visible = False
            MCAHistogramGraph.Cursors.Item(3).Visible = False
            MCAHistogramGraph.Cursors.Item(4).Visible = False
            
        Case 3      ' Cursor Coordinates
            MCAHistogramGraph.TrackMode = cwGTrackDragCursor
            MCAHistogramGraph.Cursors(1).Visible = True
            MCAHistogramGraph.Cursors(2).Visible = False
            MCAHistogramGraph.Cursors(1).XPosition = (MCAHistogramGraph.Axes(1).Minimum + MCAHistogramGraph.Axes(1).Maximum) / 2
            MCAHistogramGraph.Cursors(1).YPosition = (MCAHistogramGraph.Axes(2).Minimum + MCAHistogramGraph.Axes(2).Maximum) / 2
    
            MCAHistogramGraph.Plots(2).Visible = False
            MCAHistogramGraph.Cursors.Item(3).Visible = False
            MCAHistogramGraph.Cursors.Item(4).Visible = False
            
        Case 4      ' Cursor Measurments
            MCAHistogramGraph.TrackMode = cwGTrackDragCursor
            MCAHistogramGraph.Cursors(1).Visible = True
            MCAHistogramGraph.Cursors(2).Visible = True
            MCAHistogramGraph.Cursors(1).XPosition = (MCAHistogramGraph.Axes(1).Minimum + MCAHistogramGraph.Axes(1).Maximum) / 2
            MCAHistogramGraph.Cursors(1).YPosition = (MCAHistogramGraph.Axes(2).Minimum + MCAHistogramGraph.Axes(2).Maximum) / 2
            MCAHistogramGraph.Cursors(2).XPosition = (MCAHistogramGraph.Axes(1).Minimum + MCAHistogramGraph.Axes(1).Maximum) / 2
            MCAHistogramGraph.Cursors(2).YPosition = (MCAHistogramGraph.Axes(2).Minimum + MCAHistogramGraph.Axes(2).Maximum) / 2
    
            MCAHistogramGraph.Plots(2).Visible = False
            MCAHistogramGraph.Cursors.Item(3).Visible = False
            MCAHistogramGraph.Cursors.Item(4).Visible = False
            
    End Select
End Sub


'###########################################
'   YScalingCombo_Click()
'       Set proper scaling for the Y-axis
'###########################################

Private Sub YScalingCombo_Click()
    Dim index As Integer
    
    index = YScalingCombo.ListIndex
    Select Case index
        Case 0
            MCAHistogramGraph.Axes(2).Log = False
            MCAHistogramGraph.Axes(2).Minimum = 0
        Case 1
            MCAHistogramGraph.Axes(2).Log = True
            MCAHistogramGraph.Axes(2).Minimum = 0.5
    End Select

End Sub


'###########################################
'   AutoScale_Click()
'       Auto scale all axis
'###########################################

Private Sub AutoScale_Click()
    Dim Axis As CWAxis
    
    For Each Axis In MCAHistogramGraph.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    MCAHistogramGraph.Axes(1).Minimum = 0
    MCAHistogramGraph.Axes(1).Maximum = MAX_HISTOGRAM_LENGTH - 1
End Sub


'###########################################
'   DisplayROI()
'       Display a Region of Interest (ROI)
'###########################################

Private Sub DisplayROI()
    Dim Maximum As Long
    Dim Minimum As Long
    Dim cursor1 As Long
    Dim cursor2 As Long
    Dim X As Integer
    
    MCAHistogramGraph.Cursors.Item(1).Visible = False
    MCAHistogramGraph.Cursors.Item(2).Visible = False

    ' Setup parameters for the ROI plot
    MCAHistogramGraph.Plots(2).LineColor = vbBlue
    MCAHistogramGraph.Plots(2).FillColor = vbBlue
    MCAHistogramGraph.Plots(2).FillToBase = True
    MCAHistogramGraph.Plots(2).Visible = True
    Maximum = MCAHistogramGraph.Axes(1).Maximum
    Minimum = MCAHistogramGraph.Axes(1).Minimum
    
    MCAHistogramGraph.Cursors(3).XPosition = (Maximum - Minimum) * 0.333 + Minimum
    MCAHistogramGraph.Cursors(4).XPosition = (Maximum - Minimum) * 0.666 + Minimum
    MCAHistogramGraph.Cursors(3).Visible = True
    MCAHistogramGraph.Cursors(4).Visible = True
    MCAHistogramGraph.SetFocus
        
    ' Get the cursor positions
    cursor1 = MCAHistogramGraph.Cursors(3).XPosition
    cursor2 = MCAHistogramGraph.Cursors(4).XPosition
    
    ' Use the cursor positions to determine which block of histogram
    ' data we need, then plot the data starting at the first cursor
    ' position
    If (cursor2 > cursor1) Then
    
        ReDim roi(cursor2 - cursor1)
        For X = 0 To (cursor2 - cursor1)
            roi(X) = MCASpecSglCh(cursor1 + X)
        Next X
        MCAHistogramGraph.Plots(2).PlotY roi, cursor1
        MCAHistogramGraph.Plots(2).Visible = True
        
    ElseIf (cursor1 > cursor2) Then
    
        ReDim roi(cursor1 - cursor2)
        For X = 0 To (cursor1 - cursor2)
            roi(X) = MCASpecSglCh(cursor2 + X)
        Next X
        MCAHistogramGraph.Plots(2).PlotY roi, cursor1
        MCAHistogramGraph.Plots(2).Visible = True
        
    End If
End Sub


'###########################################
'   ChannelCheckBox Click routines
'       The following subroutines show or disable
'       the MCA histograms display
'###########################################

Private Sub Channel0_Click()
    If Channel0.Value = 1 Then
        MCAHistogramsChs0To3.Plots(1).Visible = True
    Else
        MCAHistogramsChs0To3.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel1_Click()
    If Channel1.Value = 1 Then
        MCAHistogramsChs0To3.Plots(2).Visible = True
    Else
        MCAHistogramsChs0To3.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel2_Click()
    If Channel2.Value = 1 Then
        MCAHistogramsChs0To3.Plots(3).Visible = True
    Else
        MCAHistogramsChs0To3.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel3_Click()
    If Channel3.Value = 1 Then
        MCAHistogramsChs0To3.Plots(4).Visible = True
    Else
        MCAHistogramsChs0To3.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel4_Click()
    If Channel4.Value = 1 Then
        MCAHistogramsChs4To7.Plots(1).Visible = True
    Else
        MCAHistogramsChs4To7.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel5_Click()
    If Channel5.Value = 1 Then
        MCAHistogramsChs4To7.Plots(2).Visible = True
    Else
        MCAHistogramsChs4To7.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel6_Click()
    If Channel6.Value = 1 Then
        MCAHistogramsChs4To7.Plots(3).Visible = True
    Else
        MCAHistogramsChs4To7.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel7_Click()
    If Channel7.Value = 1 Then
        MCAHistogramsChs4To7.Plots(4).Visible = True
    Else
        MCAHistogramsChs4To7.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel8_Click()
    If Channel8.Value = 1 Then
        MCAHistogramsChs8To11.Plots(1).Visible = True
    Else
        MCAHistogramsChs8To11.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel9_Click()
    If Channel9.Value = 1 Then
        MCAHistogramsChs8To11.Plots(2).Visible = True
    Else
        MCAHistogramsChs8To11.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel10_Click()
    If Channel10.Value = 1 Then
        MCAHistogramsChs8To11.Plots(3).Visible = True
    Else
        MCAHistogramsChs8To11.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel11_Click()
    If Channel11.Value = 1 Then
        MCAHistogramsChs8To11.Plots(4).Visible = True
    Else
        MCAHistogramsChs8To11.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel12_Click()
    If Channel12.Value = 1 Then
        MCAHistogramsChs12To15.Plots(1).Visible = True
    Else
        MCAHistogramsChs12To15.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel13_Click()
    If Channel13.Value = 1 Then
        MCAHistogramsChs12To15.Plots(2).Visible = True
    Else
        MCAHistogramsChs12To15.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel14_Click()
    If Channel14.Value = 1 Then
        MCAHistogramsChs12To15.Plots(3).Visible = True
    Else
        MCAHistogramsChs12To15.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel15_Click()
    If Channel15.Value = 1 Then
        MCAHistogramsChs12To15.Plots(4).Visible = True
    Else
        MCAHistogramsChs12To15.Plots(4).Visible = False
    End If
End Sub

Private Sub AutoScaleChs0To3_Click()
    Dim Axis As CWAxis
    
    For Each Axis In MCAHistogramsChs0To3.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    MCAHistogramsChs0To3.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

End Sub

Private Sub AutoScaleChs4To7_Click()
    Dim Axis As CWAxis
    
    For Each Axis In MCAHistogramsChs4To7.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    
    MCAHistogramsChs4To7.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

End Sub

Private Sub AutoScaleChs8To11_Click()
    Dim Axis As CWAxis
    
    For Each Axis In MCAHistogramsChs8To11.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    
    MCAHistogramsChs8To11.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

End Sub

Private Sub AutoScaleChs12To15_Click()
    Dim Axis As CWAxis
    
    For Each Axis In MCAHistogramsChs12To15.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    
    MCAHistogramsChs12To15.Axes(1).SetMinMax 0, MAX_HISTOGRAM_LENGTH - 1 ' X Axis

End Sub
