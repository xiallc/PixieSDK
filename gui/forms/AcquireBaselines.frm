VERSION 5.00
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Begin VB.Form AcquireBaselinesForm 
   Caption         =   "Acquire Baselines"
   ClientHeight    =   10110
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11445
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   10110
   ScaleWidth      =   11445
   Begin VB.CommandButton GetBaselines 
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
      Top             =   9240
      Width           =   1935
   End
   Begin VB.CommandButton CloseAcquireBaselinesForm 
      Caption         =   "Close"
      Height          =   615
      Left            =   8880
      TabIndex        =   3
      Top             =   9240
      Width           =   1455
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
      Tabs            =   2
      TabHeight       =   520
      TabCaption(0)   =   "Baselines - single channel"
      TabPicture(0)   =   "AcquireBaselines.frx":0000
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "SglChBaselinesChanNumLabel"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "XPos"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "YPos"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "dX"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "dY"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "BaselinesGraph"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "SglChBaselinesChanNumEdit"
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
      Tab(0).Control(14)=   "HistogramBaselinesButton"
      Tab(0).Control(14).Enabled=   0   'False
      Tab(0).Control(15)=   "Autoscale"
      Tab(0).Control(15).Enabled=   0   'False
      Tab(0).ControlCount=   16
      TabCaption(1)   =   "Baselines- 16 channels"
      TabPicture(1)   =   "AcquireBaselines.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "BaselinesChs12To15"
      Tab(1).Control(1)=   "BaselinesChs8To11"
      Tab(1).Control(2)=   "BaselinesChs4To7"
      Tab(1).Control(3)=   "BaselinesChs0To3"
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
      Begin VB.CheckBox Channel15 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch15"
         Height          =   255
         Left            =   -66600
         TabIndex        =   44
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel14 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch14"
         Height          =   255
         Left            =   -67440
         TabIndex        =   43
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel13 
         BackColor       =   &H000000FF&
         Caption         =   "Ch13"
         Height          =   255
         Left            =   -68280
         TabIndex        =   42
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel12 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch12"
         Height          =   255
         Left            =   -69120
         TabIndex        =   41
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel11 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch11"
         Height          =   255
         Left            =   -72360
         TabIndex        =   40
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel10 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch10"
         Height          =   255
         Left            =   -73200
         TabIndex        =   39
         Top             =   4320
         Value           =   1  'Checked
         Width           =   735
      End
      Begin VB.CheckBox Channel9 
         BackColor       =   &H000000FF&
         Caption         =   "Ch9"
         Height          =   255
         Left            =   -73920
         TabIndex        =   38
         Top             =   4320
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel8 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch8"
         Height          =   255
         Left            =   -74640
         TabIndex        =   37
         Top             =   4320
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel4 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch4"
         Height          =   255
         Left            =   -69000
         TabIndex        =   35
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel5 
         BackColor       =   &H000000FF&
         Caption         =   "Ch5"
         Height          =   255
         Left            =   -68280
         TabIndex        =   34
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel6 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch6"
         Height          =   255
         Left            =   -67560
         TabIndex        =   33
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel7 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch7"
         Height          =   255
         Left            =   -66840
         TabIndex        =   32
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel0 
         BackColor       =   &H0000FF00&
         Caption         =   "Ch0"
         Height          =   255
         Left            =   -74640
         TabIndex        =   31
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel1 
         BackColor       =   &H000000FF&
         Caption         =   "Ch1"
         Height          =   255
         Left            =   -73920
         TabIndex        =   30
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel2 
         BackColor       =   &H0000FFFF&
         Caption         =   "Ch2"
         Height          =   255
         Left            =   -73200
         TabIndex        =   29
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CheckBox Channel3 
         BackColor       =   &H00FF00FF&
         Caption         =   "Ch3"
         Height          =   255
         Left            =   -72480
         TabIndex        =   28
         Top             =   240
         Value           =   1  'Checked
         Width           =   615
      End
      Begin VB.CommandButton AutoScaleChs12To15 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -65640
         TabIndex        =   27
         Top             =   4200
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs8To11 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -71160
         TabIndex        =   26
         Top             =   4200
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs4To7 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -65640
         TabIndex        =   25
         Top             =   120
         Width           =   1455
      End
      Begin VB.CommandButton AutoScaleChs0To3 
         Caption         =   "Autoscale"
         Height          =   375
         Left            =   -71280
         TabIndex        =   24
         Top             =   120
         Width           =   1455
      End
      Begin VB.CommandButton Autoscale 
         Caption         =   "Autoscale"
         Height          =   495
         Left            =   4920
         TabIndex        =   19
         Top             =   7200
         Width           =   1215
      End
      Begin VB.CommandButton HistogramBaselinesButton 
         Caption         =   "Histogram"
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
         Left            =   6600
         TabIndex        =   18
         Top             =   7200
         Width           =   1455
      End
      Begin VB.TextBox XPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   435
         TabIndex        =   13
         Top             =   6840
         Width           =   1095
      End
      Begin VB.TextBox dXVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   1875
         TabIndex        =   12
         Top             =   6840
         Width           =   1095
      End
      Begin VB.TextBox YPosVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   435
         TabIndex        =   11
         Top             =   7200
         Width           =   1095
      End
      Begin VB.TextBox dYVal 
         BackColor       =   &H8000000F&
         Height          =   285
         Left            =   1875
         TabIndex        =   10
         Top             =   7200
         Width           =   1095
      End
      Begin VB.ComboBox YScalingCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   9600
         Style           =   2  'Dropdown List
         TabIndex        =   9
         Top             =   240
         Width           =   1335
      End
      Begin VB.ComboBox XCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   1800
         Style           =   2  'Dropdown List
         TabIndex        =   8
         Top             =   240
         Width           =   1575
      End
      Begin VB.ComboBox ZoomCombo 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   240
         Style           =   2  'Dropdown List
         TabIndex        =   7
         Top             =   240
         Width           =   1575
      End
      Begin CWUIControlsLib.CWNumEdit SglChBaselinesChanNumEdit 
         Height          =   495
         Left            =   9960
         TabIndex        =   5
         Top             =   7200
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
      Begin CWUIControlsLib.CWGraph BaselinesGraph 
         Height          =   6255
         Left            =   240
         TabIndex        =   6
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
         szText_12       =   "Time (ms)"
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
         szText_19       =   "Baseline Amplitude"
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
      Begin CWUIControlsLib.CWGraph BaselinesChs0To3 
         Height          =   3375
         Left            =   -74760
         TabIndex        =   20
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
         szText_12       =   "Time (ms)"
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
         szText_19       =   "Baseline Amplitude"
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
      Begin CWUIControlsLib.CWGraph BaselinesChs4To7 
         Height          =   3375
         Left            =   -69240
         TabIndex        =   21
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
         szText_12       =   "Time (ms)"
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
         szText_19       =   "Baseline Amplitude"
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
      Begin CWUIControlsLib.CWGraph BaselinesChs8To11 
         Height          =   3375
         Left            =   -74760
         TabIndex        =   22
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
         szText_12       =   "Time (ms)"
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
         szText_19       =   "Baseline Amplitude"
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
      Begin CWUIControlsLib.CWGraph BaselinesChs12To15 
         Height          =   3375
         Left            =   -69240
         TabIndex        =   23
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
         szText_12       =   "Time (ms)"
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
         szText_19       =   "Baseline Amplitude"
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
      Begin VB.Label dY 
         Caption         =   "dY"
         Height          =   255
         Left            =   1605
         TabIndex        =   17
         Top             =   7290
         Width           =   255
      End
      Begin VB.Label dX 
         Caption         =   "dX"
         Height          =   255
         Left            =   1605
         TabIndex        =   16
         Top             =   6945
         Width           =   255
      End
      Begin VB.Label YPos 
         Caption         =   "Y"
         Height          =   255
         Left            =   255
         TabIndex        =   15
         Top             =   7290
         Width           =   255
      End
      Begin VB.Label XPos 
         Caption         =   "X"
         Height          =   255
         Left            =   240
         TabIndex        =   14
         Top             =   6930
         Width           =   255
      End
      Begin VB.Label SglChBaselinesChanNumLabel 
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
         Left            =   8280
         TabIndex        =   4
         Top             =   7320
         Width           =   1575
      End
   End
   Begin CWUIControlsLib.CWNumEdit BaselinesModNumEdit 
      Height          =   495
      Left            =   2280
      TabIndex        =   2
      Top             =   9240
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
   Begin VB.Label BaselinesModNumLabel 
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
      Top             =   9360
      Width           =   1815
   End
End
Attribute VB_Name = "AcquireBaselinesForm"
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
    
    BaselinesGraph.TrackMode = cwGTrackZoomRectXY
 
    ' Set graph axis limits from program
   ' BaselinesGraph.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    BaselinesGraph.Axes(2).Autoscale = True
    
    BaselinesChs0To3.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    BaselinesChs0To3.Axes(2).Autoscale = True
    
    BaselinesChs4To7.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    BaselinesChs4To7.Axes(2).Autoscale = True
    
    BaselinesChs8To11.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    BaselinesChs8To11.Axes(2).Autoscale = True
   
    BaselinesChs12To15.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    BaselinesChs12To15.Axes(2).Autoscale = True
     
    ' Update maximum number of modules
    BaselinesModNumEdit.Maximum = Number_Modules_Total - 1
   
End Sub


'###########################################
'   CloseAcquireBaselinesForm_Click()
'       Close the MCAHistogram Form
'###########################################

Private Sub CloseAcquireBaselinesForm_Click()
    Unload Me
End Sub


'###########################################
'   GetBaselines_Click()
'       Read baselines from module
'###########################################

Private Sub GetBaselines_Click()
    Dim retval As Long
    Dim ModuleNumber, ChannelNumber As Integer
    Dim StrTxt As String
    Dim Msg As Integer

    ' First acquire baselines for all channels
    ModuleNumber = BaselinesModNumEdit.Value
    retval = Pixie16AcquireBaselines(ModuleNumber)
    If retval < 0 Then
        StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    
    ' Read baselines for the single channel
    ChannelNumber = SglChBaselinesChanNumEdit.Value
    retval = Pixie16ReadSglChanBaselines(BaselinesSglCh(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
    If retval < 0 Then
        StrTxt = "Pixie16ReadSglChanBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
        Msg = MsgBox(StrTxt, 0, "Error")
        Exit Sub
    End If
    ' Plot the baselines
    'BaselinesGraph.PlotXvsY BaseTimeStamps, BaselinesSglCh
    'BaselinesGraph.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    BaselinesGraph.PlotY BaselinesSglCh
    BaselinesGraph.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    
    ' Read baselines for other channels if they are checked
    If Channel0.Value = 1 Then
        ChannelNumber = 0
        retval = Pixie16ReadSglChanBaselines(BaselinesCh0(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16ReadSglChanBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        'BaselinesChs0To3.Plots(1).PlotXvsY BaseTimeStamps, BaselinesCh0
        'BaselinesChs0To3.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
        BaselinesChs0To3.Plots(1).PlotY BaselinesCh0
        BaselinesChs0To3.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
    End If
    
    If Channel1.Value = 1 Then
        ChannelNumber = 1
        retval = Pixie16ReadSglChanBaselines(BaselinesCh1(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs0To3.Plots(2).PlotY BaselinesCh1
        BaselinesChs0To3.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs0To3.Plots(2).PlotXvsY BaseTimeStamps, BaselinesCh1
        'BaselinesChs0To3.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If
    
    If Channel2.Value = 1 Then
        ChannelNumber = 2
        retval = Pixie16ReadSglChanBaselines(BaselinesCh2(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs0To3.Plots(3).PlotY BaselinesCh2
        BaselinesChs0To3.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs0To3.Plots(3).PlotXvsY BaseTimeStamps, BaselinesCh2
        'BaselinesChs0To3.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel3.Value = 1 Then
        ChannelNumber = 3
        retval = Pixie16ReadSglChanBaselines(BaselinesCh3(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs0To3.Plots(4).PlotY BaselinesCh3
        BaselinesChs0To3.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs0To3.Plots(4).PlotXvsY BaseTimeStamps, BaselinesCh3
        'BaselinesChs0To3.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel4.Value = 1 Then
        ChannelNumber = 4
        retval = Pixie16ReadSglChanBaselines(BaselinesCh4(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs4To7.Plots(1).PlotY BaselinesCh4
        BaselinesChs4To7.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs4To7.Plots(1).PlotXvsY BaseTimeStamps, BaselinesCh4
        'BaselinesChs4To7.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel5.Value = 1 Then
        ChannelNumber = 5
        retval = Pixie16ReadSglChanBaselines(BaselinesCh5(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs4To7.Plots(2).PlotY BaselinesCh5
        BaselinesChs4To7.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs4To7.Plots(2).PlotXvsY BaseTimeStamps, BaselinesCh5
        'BaselinesChs4To7.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel6.Value = 1 Then
        ChannelNumber = 6
        retval = Pixie16ReadSglChanBaselines(BaselinesCh6(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs4To7.Plots(3).PlotY BaselinesCh6
        BaselinesChs4To7.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs4To7.Plots(3).PlotXvsY BaseTimeStamps, BaselinesCh6
        'BaselinesChs4To7.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel7.Value = 1 Then
        ChannelNumber = 7
        retval = Pixie16ReadSglChanBaselines(BaselinesCh7(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs4To7.Plots(4).PlotY BaselinesCh7
        BaselinesChs4To7.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs4To7.Plots(4).PlotXvsY BaseTimeStamps, BaselinesCh7
        'BaselinesChs4To7.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel8.Value = 1 Then
        ChannelNumber = 8
        retval = Pixie16ReadSglChanBaselines(BaselinesCh8(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs8To11.Plots(1).PlotY BaselinesCh8
        BaselinesChs8To11.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs8To11.Plots(1).PlotXvsY BaseTimeStamps, BaselinesCh8
        'BaselinesChs8To11.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If
    
    If Channel9.Value = 1 Then
        ChannelNumber = 9
        retval = Pixie16ReadSglChanBaselines(BaselinesCh9(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs8To11.Plots(2).PlotY BaselinesCh9
        BaselinesChs8To11.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs8To11.Plots(2).PlotXvsY BaseTimeStamps, BaselinesCh9
        'BaselinesChs8To11.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If
    
    If Channel10.Value = 1 Then
        ChannelNumber = 10
        retval = Pixie16ReadSglChanBaselines(BaselinesCh10(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs8To11.Plots(3).PlotY BaselinesCh10
        BaselinesChs8To11.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs8To11.Plots(3).PlotXvsY BaseTimeStamps, BaselinesCh10
        'BaselinesChs8To11.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If
    
    If Channel11.Value = 1 Then
        ChannelNumber = 11
        retval = Pixie16ReadSglChanBaselines(BaselinesCh11(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs8To11.Plots(4).PlotY BaselinesCh11
        BaselinesChs8To11.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs8To11.Plots(4).PlotXvsY BaseTimeStamps, BaselinesCh11
        'BaselinesChs8To11.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel2.Value = 1 Then
        ChannelNumber = 12
        retval = Pixie16ReadSglChanBaselines(BaselinesCh12(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs12To15.Plots(1).PlotY BaselinesCh12
        BaselinesChs12To15.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs12To15.Plots(1).PlotXvsY BaseTimeStamps, BaselinesCh12
        'BaselinesChs12To15.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If
    
    If Channel13.Value = 1 Then
        ChannelNumber = 13
        retval = Pixie16ReadSglChanBaselines(BaselinesCh13(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs12To15.Plots(2).PlotY BaselinesCh13
        BaselinesChs12To15.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs12To15.Plots(2).PlotXvsY BaseTimeStamps, BaselinesCh13
        'BaselinesChs12To15.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If
        
    If Channel14.Value = 1 Then
        ChannelNumber = 14
        retval = Pixie16ReadSglChanBaselines(BaselinesCh14(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs12To15.Plots(3).PlotY BaselinesCh14
        BaselinesChs12To15.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs12To15.Plots(3).PlotXvsY BaseTimeStamps, BaselinesCh14
        'BaselinesChs12To15.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

    If Channel15.Value = 1 Then
        ChannelNumber = 15
        retval = Pixie16ReadSglChanBaselines(BaselinesCh15(0), BaseTimeStamps(0), BASELINES_LENGTH, ModuleNumber, ChannelNumber)
        If retval < 0 Then
            StrTxt = "Pixie16AcquireBaselines " & " in module # " & Str$(ModuleNumber) & "  channel # " & Str$(ChannelNumber) & " returned unsuccessfully. Please check the log file"
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If
        BaselinesChs12To15.Plots(4).PlotY BaselinesCh15
        BaselinesChs12To15.Axes(1).SetMinMax 0, BASELINES_LENGTH - 1 ' X Axis
        'BaselinesChs12To15.Plots(4).PlotXvsY BaseTimeStamps, BaselinesCh15
        'BaselinesChs12To15.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis
    End If

End Sub


'###########################################
'   BaselinesGraph_CursorChange()
'       Respond to cursor position change
'###########################################

Private Sub BaselinesGraph_CursorChange(CursorIndex As Long, XPos As Variant, YPos As Variant, bTracking As Boolean)
    Dim index As Integer
    
    index = ZoomCombo.ListIndex
    'Display cursor position on user interface
    If CursorIndex = 1 Then
        XPosVal = Format(BaselinesGraph.Cursors(1).XPosition, "0.####") ' data from cursor
        YPosVal = Format(YPos, "0.####") ' data from event handler
    End If
    If (index = 3) Then
        Dim Amplitude As Double
        Dim Period As Double
        Amplitude = Format(Abs(BaselinesGraph.Cursors(2).YPosition - BaselinesGraph.Cursors(1).YPosition), "0.####")
        Period = Format(Abs(BaselinesGraph.Cursors(2).XPosition - BaselinesGraph.Cursors(1).XPosition), "0.####")
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
            BaselinesGraph.TrackMode = cwGTrackZoomRectXY
            BaselinesGraph.Cursors(1).Visible = False
            BaselinesGraph.Cursors(2).Visible = False
        Case 1
            BaselinesGraph.Cursors(1).Visible = False
            BaselinesGraph.Cursors(2).Visible = False
            BaselinesGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2
            BaselinesGraph.TrackMode = cwGTrackDragCursor
            BaselinesGraph.Cursors(1).Visible = True
            BaselinesGraph.Cursors(2).Visible = False
            BaselinesGraph.Cursors(1).XPosition = (BaselinesGraph.Axes(1).Minimum + BaselinesGraph.Axes(1).Maximum) / 2
            BaselinesGraph.Cursors(1).YPosition = (BaselinesGraph.Axes(2).Minimum + BaselinesGraph.Axes(2).Maximum) / 2
        Case 3
            BaselinesGraph.TrackMode = cwGTrackDragCursor
            BaselinesGraph.Cursors(1).Visible = True
            BaselinesGraph.Cursors(2).Visible = True

    End Select
End Sub


'###########################################
'   XPosition_ValueChanged()
'       Move cursor's X-position
'###########################################

Private Sub XPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    BaselinesGraph.Cursors(1).XPosition = XPosVal
    BaselinesGraph.Cursors(1).YPosition = YPosVal
End Sub


'###########################################
'   YPosition_ValueChanged()
'       move cursor's Y-position
'###########################################

Private Sub YPosition_ValueChanged(ByVal Value As Variant, ByVal PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' Set cursor position from program
    BaselinesGraph.Cursors(1).XPosition = XPosVal
    BaselinesGraph.Cursors(1).YPosition = YPosVal
End Sub


Private Sub HistogramBaselinesButton_Click()

    ' Check if the form is minimized or maximized
    If HistogramBaselines.WindowState <> vbNormal Then
        HistogramBaselines.WindowState = vbNormal
    End If

    CenterChild Pixie16MDIForm, HistogramBaselines
    HistogramBaselines.Height = 3600
    HistogramBaselines.Width = 5490
    
    HistogramBaselines.ZOrder 0

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
            BaselinesGraph.Cursors(1).SnapMode = cwCSnapFixed
            BaselinesGraph.Cursors(2).SnapMode = cwCSnapFixed
        Case 1
            BaselinesGraph.Cursors(1).SnapMode = cwCSnapFloating
            BaselinesGraph.Cursors(2).SnapMode = cwCSnapFloating
        Case 2
            BaselinesGraph.Cursors(1).SnapMode = cwCSnapNearestPoint
            BaselinesGraph.Cursors(2).SnapMode = cwCSnapNearestPoint
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
            BaselinesGraph.TrackMode = cwGTrackZoomRectXY
            BaselinesGraph.Cursors(1).Visible = False
            BaselinesGraph.Cursors(2).Visible = False
        Case 1      ' Pan
            BaselinesGraph.Cursors(1).Visible = False
            BaselinesGraph.Cursors(2).Visible = False
            BaselinesGraph.TrackMode = cwGTrackPanPlotAreaXY
        Case 2      ' Cursor Coordinates
            BaselinesGraph.TrackMode = cwGTrackDragCursor
            BaselinesGraph.Cursors(1).Visible = True
            BaselinesGraph.Cursors(2).Visible = False
            BaselinesGraph.Cursors(1).XPosition = (BaselinesGraph.Axes(1).Minimum + BaselinesGraph.Axes(1).Maximum) / 2
            BaselinesGraph.Cursors(1).YPosition = (BaselinesGraph.Axes(2).Minimum + BaselinesGraph.Axes(2).Maximum) / 2
        Case 3      ' Cursor Measurements
            BaselinesGraph.TrackMode = cwGTrackDragCursor
            BaselinesGraph.Cursors(1).Visible = True
            BaselinesGraph.Cursors(2).Visible = True
            BaselinesGraph.Cursors(1).XPosition = (BaselinesGraph.Axes(1).Minimum + BaselinesGraph.Axes(1).Maximum) / 2
            BaselinesGraph.Cursors(1).YPosition = (BaselinesGraph.Axes(2).Minimum + BaselinesGraph.Axes(2).Maximum) / 2
            BaselinesGraph.Cursors(2).XPosition = (BaselinesGraph.Axes(1).Minimum + BaselinesGraph.Axes(1).Maximum) / 2
            BaselinesGraph.Cursors(2).YPosition = (BaselinesGraph.Axes(2).Minimum + BaselinesGraph.Axes(2).Maximum) / 2
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
            BaselinesGraph.Axes(2).Log = False
        Case 1
            BaselinesGraph.Axes(2).Log = True
    End Select

End Sub


'###########################################
'   AutoScale_Click()
'       Auto scale all axis
'###########################################

Private Sub AutoScale_Click()
    Dim Axis As CWAxis
    
    For Each Axis In BaselinesGraph.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    BaselinesGraph.Axes(1).Minimum = 0
    BaselinesGraph.Axes(1).Maximum = BaseTimeStamps(BASELINES_LENGTH - 1)
End Sub


'###########################################
'   ChannelCheckBox Click routines
'       The following subroutines show or disable
'       the baselines display
'###########################################

Private Sub Channel0_Click()
    If Channel0.Value = 1 Then
        BaselinesChs0To3.Plots(1).Visible = True
    Else
        BaselinesChs0To3.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel1_Click()
    If Channel1.Value = 1 Then
        BaselinesChs0To3.Plots(2).Visible = True
    Else
        BaselinesChs0To3.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel2_Click()
    If Channel2.Value = 1 Then
        BaselinesChs0To3.Plots(3).Visible = True
    Else
        BaselinesChs0To3.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel3_Click()
    If Channel3.Value = 1 Then
        BaselinesChs0To3.Plots(4).Visible = True
    Else
        BaselinesChs0To3.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel4_Click()
    If Channel4.Value = 1 Then
        BaselinesChs4To7.Plots(1).Visible = True
    Else
        BaselinesChs4To7.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel5_Click()
    If Channel5.Value = 1 Then
        BaselinesChs4To7.Plots(2).Visible = True
    Else
        BaselinesChs4To7.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel6_Click()
    If Channel6.Value = 1 Then
        BaselinesChs4To7.Plots(3).Visible = True
    Else
        BaselinesChs4To7.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel7_Click()
    If Channel7.Value = 1 Then
        BaselinesChs4To7.Plots(4).Visible = True
    Else
        BaselinesChs4To7.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel8_Click()
    If Channel8.Value = 1 Then
        BaselinesChs8To11.Plots(1).Visible = True
    Else
        BaselinesChs8To11.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel9_Click()
    If Channel9.Value = 1 Then
        BaselinesChs8To11.Plots(2).Visible = True
    Else
        BaselinesChs8To11.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel10_Click()
    If Channel10.Value = 1 Then
        BaselinesChs8To11.Plots(3).Visible = True
    Else
        BaselinesChs8To11.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel11_Click()
    If Channel11.Value = 1 Then
        BaselinesChs8To11.Plots(4).Visible = True
    Else
        BaselinesChs8To11.Plots(4).Visible = False
    End If
End Sub

Private Sub Channel12_Click()
    If Channel12.Value = 1 Then
        BaselinesChs12To15.Plots(1).Visible = True
    Else
        BaselinesChs12To15.Plots(1).Visible = False
    End If
End Sub

Private Sub Channel13_Click()
    If Channel13.Value = 1 Then
        BaselinesChs12To15.Plots(2).Visible = True
    Else
        BaselinesChs12To15.Plots(2).Visible = False
    End If
End Sub

Private Sub Channel14_Click()
    If Channel14.Value = 1 Then
        BaselinesChs12To15.Plots(3).Visible = True
    Else
        BaselinesChs12To15.Plots(3).Visible = False
    End If
End Sub

Private Sub Channel15_Click()
    If Channel15.Value = 1 Then
        BaselinesChs12To15.Plots(4).Visible = True
    Else
        BaselinesChs12To15.Plots(4).Visible = False
    End If
End Sub

Private Sub AutoScaleChs0To3_Click()
    Dim Axis As CWAxis
    
    For Each Axis In BaselinesChs0To3.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    BaselinesChs0To3.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis

End Sub

Private Sub AutoScaleChs4To7_Click()
    Dim Axis As CWAxis
    
    For Each Axis In BaselinesChs4To7.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    BaselinesChs4To7.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis

End Sub

Private Sub AutoScaleChs8To11_Click()
    Dim Axis As CWAxis
    
    For Each Axis In BaselinesChs8To11.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next

    BaselinesChs8To11.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis

End Sub

Private Sub AutoScaleChs12To15_Click()
    Dim Axis As CWAxis
    
    For Each Axis In BaselinesChs12To15.Axes
        ' Force Graph Axis to Autoscale
        Axis.AutoScaleNow
    Next
    
    BaselinesChs12To15.Axes(1).SetMinMax 0, BaseTimeStamps(BASELINES_LENGTH - 1) ' X Axis

End Sub
