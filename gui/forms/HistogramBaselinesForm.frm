VERSION 5.00
Object = "{E7BC3920-33D4-11D0-8B73-0020AF31CEF9}#1.4#0"; "cwanalysis.ocx"
Begin VB.Form HistogramBaselines 
   Caption         =   "Histogramming Baselines"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   5370
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3090
   ScaleWidth      =   5370
   Begin VB.CommandButton CloseHistBaselines 
      Caption         =   "Close"
      Height          =   495
      Left            =   4200
      TabIndex        =   8
      Top             =   2400
      Width           =   975
   End
   Begin CWAnalysisControlsLib.CWStat CWStatBaseline 
      Left            =   720
      Top             =   2400
      _Version        =   65540
      _ExtentX        =   847
      _ExtentY        =   847
      _StockProps     =   0
   End
   Begin CWAnalysisControlsLib.CWArray CWArrayBaseline 
      Left            =   120
      Top             =   2400
      _Version        =   65540
      _ExtentX        =   847
      _ExtentY        =   847
      _StockProps     =   0
   End
   Begin VB.CommandButton HistogramBaselinesCmd 
      Caption         =   "Do It!"
      Height          =   495
      Left            =   1560
      TabIndex        =   7
      Top             =   2400
      Width           =   1575
   End
   Begin VB.TextBox NumIntervalsText 
      Height          =   375
      Left            =   2040
      TabIndex        =   6
      Top             =   1680
      Width           =   1815
   End
   Begin VB.TextBox MaxBaseText 
      Height          =   375
      Left            =   2040
      TabIndex        =   4
      Top             =   1200
      Width           =   1815
   End
   Begin VB.TextBox MinBaseText 
      Height          =   375
      Left            =   2040
      TabIndex        =   1
      Top             =   720
      Width           =   1815
   End
   Begin VB.Label NumIntervalsLabel 
      Caption         =   "Number of intervals"
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
      Left            =   240
      TabIndex        =   5
      Top             =   1680
      Width           =   1935
   End
   Begin VB.Label MaxBaseTextLabel 
      Caption         =   "Maximum"
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
      Left            =   1080
      TabIndex        =   3
      Top             =   1200
      Width           =   975
   End
   Begin VB.Label MinBaseTextLabel 
      Caption         =   "Minimum"
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
      Left            =   1080
      TabIndex        =   2
      Top             =   720
      Width           =   975
   End
   Begin VB.Label BaselineHistogramSettingsLabel 
      Caption         =   "Set baselines histogramming parameters below:"
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
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   5175
   End
End
Attribute VB_Name = "HistogramBaselines"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim HistMinBaselineValue As Variant
Dim HistMaxBaselineValue As Variant
Dim HistNumIntervals As Variant


Private Sub CloseHistBaselines_Click()
    Unload Me
End Sub

Private Sub HistogramBaselinesCmd_Click()
    Dim hist As Variant
    Dim Axis As Variant
    
    On Error GoTo StopHistogramming
    CWStatBaseline.Histogram BaselinesSglCh, HistMinBaselineValue, HistMaxBaselineValue, HistNumIntervals, hist, Axis
    
    ' Plot the histogram
    AcquireBaselinesForm.BaselinesGraph.PlotXvsY Axis, hist

StopHistogramming:
End Sub

Private Sub MinBaseText_KeyPress(KeyAscii As Integer)
    On Error GoTo SetDefaultMinValue

    If (KeyAscii = 13) Then
         HistMinBaselineValue = CVar(MinBaseText.Text)
    End If
    Exit Sub
    
SetDefaultMinValue:
    Dim max As Variant, min As Variant, imax As Variant, imin As Variant
    CWArrayBaseline.MaxMin1D BaselinesSglCh, max, imax, min, imin
    HistMinBaselineValue = min
    MinBaseText.Text = HistMinBaselineValue
End Sub

Private Sub MinBaseText_LostFocus()
    On Error GoTo SetDefaultMinValue
    HistMinBaselineValue = CVar(MinBaseText.Text)
    Exit Sub
    
SetDefaultMinValue:
    Dim max As Variant, min As Variant, imax As Variant, imin As Variant
    CWArrayBaseline.MaxMin1D BaselinesSglCh, max, imax, min, imin
    HistMinBaselineValue = min
    MinBaseText.Text = HistMinBaselineValue
End Sub

Private Sub MaxBaseText_KeyPress(KeyAscii As Integer)
    On Error GoTo SetDefaultMaxValue

    If (KeyAscii = 13) Then
         HistMaxBaselineValue = CVar(MaxBaseText.Text)
    End If
    Exit Sub
    
SetDefaultMaxValue:
    Dim max As Variant, min As Variant, imax As Variant, imin As Variant
    CWArrayBaseline.MaxMin1D BaselinesSglCh, max, imax, min, imin
    HistMaxBaselineValue = max
    MaxBaseText.Text = HistMaxBaselineValue
End Sub

Private Sub MaxBaseText_LostFocus()
    On Error GoTo SetDefaultMaxValue
    HistMaxBaselineValue = CVar(MaxBaseText.Text)
    Exit Sub
    
SetDefaultMaxValue:
    Dim max As Variant, min As Variant, imax As Variant, imin As Variant
    CWArrayBaseline.MaxMin1D BaselinesSglCh, max, imax, min, imin
    HistMaxBaselineValue = max
    MaxBaseText.Text = HistMaxBaselineValue
End Sub

Private Sub NumIntervalsText_KeyPress(KeyAscii As Integer)
    On Error GoTo SetDefaultNumIntervals

    If (KeyAscii = 13) Then
         HistNumIntervals = CVar(NumIntervalsText.Text)
    End If
    Exit Sub
    
SetDefaultNumIntervals:
    HistNumIntervals = 1024
    NumIntervalsText.Text = HistNumIntervals
End Sub

Private Sub NumIntervalsText_LostFocus()
    On Error GoTo SetDefaultNumIntervals
    HistNumIntervals = CVar(NumIntervalsText.Text)
    Exit Sub
    
SetDefaultNumIntervals:
    HistNumIntervals = 1024
    NumIntervalsText.Text = HistNumIntervals
End Sub

