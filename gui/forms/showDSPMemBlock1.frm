VERSION 5.00
Object = "{D940E4E4-6079-11CE-88CB-0020AF6845F6}#1.6#0"; "cwui.ocx"
Object = "{DE8CE233-DD83-481D-844C-C07B96589D3A}#1.1#0"; "vbalSGrid6.ocx"
Begin VB.Form showDSPMemBlock1 
   Caption         =   "Show DSP Memory Block 1 (0x00050000 - 0x0005FFFF)"
   ClientHeight    =   9465
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7815
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   9465
   ScaleWidth      =   7815
   Begin VB.CommandButton ConvertDataType 
      Caption         =   "Hexadecimal"
      Height          =   375
      Left            =   2880
      TabIndex        =   3
      Top             =   8760
      Width           =   1215
   End
   Begin vbAcceleratorSGrid6.vbalGrid vbalGrid1 
      Height          =   7935
      Left            =   360
      TabIndex        =   2
      Top             =   240
      Width           =   7095
      _ExtentX        =   12515
      _ExtentY        =   13996
      GridLines       =   -1  'True
      BackgroundPictureHeight=   0
      BackgroundPictureWidth=   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      DisableIcons    =   -1  'True
   End
   Begin VB.CommandButton CloseDSPMemBlock1 
      Caption         =   "Close"
      Height          =   615
      Left            =   6120
      TabIndex        =   1
      Top             =   8520
      Width           =   1575
   End
   Begin VB.CommandButton RefreshDSPMemBlock1 
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
      Left            =   4320
      TabIndex        =   0
      Top             =   8520
      Width           =   1575
   End
   Begin CWUIControlsLib.CWNumEdit showDSPMemBlock1ModNumEdit 
      Height          =   495
      Left            =   1680
      TabIndex        =   4
      Top             =   8640
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
   Begin VB.Label ShowDSPMemBlock1ModNumLabel 
      Caption         =   "Module Number"
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
      Left            =   120
      TabIndex        =   5
      Top             =   8760
      Width           =   1575
   End
End
Attribute VB_Name = "showDSPMemBlock1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub CloseDSPMemBlock1_Click()
    showDSPMemBlock1.Hide
End Sub


Private Sub Form_Load()
    
    ' Update maximum number of modules
    showDSPMemBlock1ModNumEdit.Maximum = Number_Modules_Total - 1
    
    ' Load DSP list and the grids - use default module 0
    Call Load_DSPlsts1(0)

    ' Clear the ProgressBar control display
    Pixie16MDIForm.Pixie16ProgressBar.Value = 0

End Sub

Private Sub RefreshDSPMemBlock1_Click()
    
    Dim retval As Long
    Dim i As Long
    Dim Msg As Integer
    Dim StrTxt As String
    Dim startaddr As Long
    Dim NumWords As Long
    Dim index As Long
    Dim ModuleNumber As Integer
    
    ModuleNumber = CInt(showDSPMemBlock1ModNumEdit.Text)
    
    ' Calculate the starting address of the DSP IOparams
    startaddr = &H50000
    
    'Calculate the number of words to read from the DSP
    NumWords = &H10000
    
    ' Check if we are using online mode
    If IsOfflineOperation = 0 Then
    
        ' Read module parameters
        retval = Pixie16IMbufferIO(DSPMemBlock1(0), NumWords, startaddr, MOD_READ, ModuleNumber)
        If retval < 0 Then
            StrTxt = "RefreshDSPMemBlock1: Pixie16IMbufferIO for module # " & ModuleNumber & " failed, retval = " & retval
            Msg = MsgBox(StrTxt, 0, "Error")
            Exit Sub
        End If

    Else
    
        ' In offline mode we clear DSPMemBlock0
        For index = 0 To NumWords - 1
            DSPMemBlock1(index) = 0
        Next index
    
    End If
    
    With vbalGrid1
        .Redraw = False
        
        For i = 1 To .Rows - 1
            If DSPMemBlock1(i - 1) < 0 Then
                If DSPMemBlk1IsHexadecimal = True Then
                    .CellText(i, 3) = ToHex(CDbl(DSPMemBlock1(i - 1)) + 4294967296#)
                Else
                    .CellText(i, 3) = CDbl(DSPMemBlock1(i - 1)) + 4294967296#
                End If
            Else
                If DSPMemBlk1IsHexadecimal = True Then
                    .CellText(i, 3) = ToHex(CDbl(DSPMemBlock1(i - 1)))
                Else
                    .CellText(i, 3) = CDbl(DSPMemBlock1(i - 1))
                End If
            End If
        
             ' Update the ProgressBar control.
            If i Mod 4096 = 0 Then
                If i + 4096 > .Rows Then
                    Pixie16MDIForm.Pixie16ProgressBar.Value = 100
                Else
                    Pixie16MDIForm.Pixie16ProgressBar.Value = (i + 4096) / .Rows * 100
                End If
            End If
        Next i
        
        .Redraw = True
    End With
    
    ' Clear the ProgressBar control display
    Pixie16MDIForm.Pixie16ProgressBar.Value = 0
    
End Sub


Private Sub ConvertDataType_Click()
    Dim i As Long
    
    ' Update data type indicator
    If DSPMemBlk1IsHexadecimal = True Then
        DSPMemBlk1IsHexadecimal = False
        ConvertDataType.Caption = "Decimal"
    Else
        DSPMemBlk1IsHexadecimal = True
        ConvertDataType.Caption = "Hexadecimal"
    End If
    
    With vbalGrid1
        .Redraw = False

        For i = 1 To .Rows - 1
            If DSPMemBlock1(i - 1) < 0 Then
                If DSPMemBlk1IsHexadecimal = True Then
                    .CellText(i, 3) = ToHex(CDbl(DSPMemBlock1(i - 1)) + 4294967296#)
                Else
                    .CellText(i, 3) = CDbl(DSPMemBlock1(i - 1)) + 4294967296#
                End If
            Else
                If DSPMemBlk1IsHexadecimal = True Then
                    .CellText(i, 3) = ToHex(CDbl(DSPMemBlock1(i - 1)))
                Else
                    .CellText(i, 3) = CDbl(DSPMemBlock1(i - 1))
                End If
            End If
        
            ' Update the ProgressBar control.
            If i Mod 4096 = 0 Then
                If i + 4096 > .Rows Then
                    Pixie16MDIForm.Pixie16ProgressBar.Value = 100
                Else
                    Pixie16MDIForm.Pixie16ProgressBar.Value = (i + 4096) / .Rows * 100
                End If
            End If
        Next i
        
        .Redraw = True
    End With
    
    ' Set appropriate caption
    If DSPMemBlk1IsHexadecimal = True Then
        ConvertDataType.Caption = "Hexadecimal"
    Else
        ConvertDataType.Caption = "Decimal"
    End If
    
    ' Clear the ProgressBar control display
    Pixie16MDIForm.Pixie16ProgressBar.Value = 0
    
End Sub


Private Sub Load_DSPlsts1(ModNum As Integer)
    Dim X As Double
    Dim LineStr1 As String
    Dim LineStr2 As String
    Dim startingaddr As Long
    Dim currentaddr As Long
    Dim nextaddr As Long
    Dim addrdiff As Long
    Dim Msg As Integer
    Dim ErrTxt As String
    Dim fso
    Dim Pixie16_DSP_List_File As String
    
    ' Create a file system object
    Set fso = CreateObject("Scripting.FileSystemObject")
    
    ' Select Pixie16_DSP_List_File
    Select Case ModRev(ModNum)
        Case 11, 12, 13
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevBCD
        Case 15
            If ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 100 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF14b100m
            ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 100 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF16b100m
            ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 250 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF12b250m
            ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 250 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF14b250m
            ElseIf ModADCBits(ModNum) = 16 And ModADCMSPS(ModNum) = 250 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF16b250m
            ElseIf ModADCBits(ModNum) = 12 And ModADCMSPS(ModNum) = 500 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF12b500m
            ElseIf ModADCBits(ModNum) = 14 And ModADCMSPS(ModNum) = 500 Then
                Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF14b500m
            End If
    End Select
    
    ' Make sure DSP code variable/list files exist in the Offline mode
    ' Default module type in Offline mode is 12-bit, 250 MSPS
    If IsOfflineOperation = 1 Then
        If (fso.FileExists(Pixie16_DSP_List_File_RevF12b250m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF12b250m
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevBCD)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevBCD
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevF14b100m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF14b100m
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevF16b100m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF16b100m
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevF14b250m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF14b250m
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevF16b250m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF16b250m
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevF12b500m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF12b500m
        ElseIf (fso.FileExists(Pixie16_DSP_List_File_RevF14b500m)) Then
            Pixie16_DSP_List_File = Pixie16_DSP_List_File_RevF14b500m
        End If
    End If
    
    ' Check if Pixie16_DSP_List_File exists
    If (fso.FileExists(Pixie16_DSP_List_File)) Then
        Open Pixie16_DSP_List_File For Input As #1
    Else    ' file doesn't exist
        ErrTxt = "Pixie16 DSP List File " & Pixie16_DSP_List_File & " doesn't exist. Please check boot file paths."
        Msg = MsgBox(ErrTxt, 0, "Error")
        Exit Sub
    End If
   
    With vbalGrid1
    
        .Redraw = False

        ' Add the columns
        .AddColumn "colAddr", "Address", ecgHdrTextALignCentre, , 100, True, True, , , , False, , False
        .AddColumn "colVal", "DSP Val", ecgHdrTextALignCentre, , 200, True, True
        .AddColumn "colPar", "DSP Par", ecgHdrTextALignCentre, , 150, True, True
        
        .Rows = 65536
        
        .DefaultRowHeight = 30
   
        ' Read lines from the file until we reach the starting address of memory block 1
        
        Do While 1
            If Not EOF(1) Then
                Line Input #1, LineStr1
            Else
                ErrTxt = "DSP list file " & Pixie16_DSP_List_File & " doesn't contain valid parameters"
                Msg = MsgBox(ErrTxt, 0, "Error")
                ' Close the Pixie16_DSP_List_File
                Close #1
                Exit Sub
            End If
        
            ' Extract the starting address
            startingaddr = Val("&H" & Mid(LineStr1, 6, 5))
            If startingaddr = &H50000 Then
                currentaddr = startingaddr
                Exit Do
            End If
        Loop
                
        ' Read the DSP variable names from the Pixie16_DSP_List_File
        Do While 1
                
            ' Read the next line
            If Not EOF(1) Then
                Line Input #1, LineStr2
            Else
                LineStr2 = ""   ' to indicate the end of file
            End If
            
            If LineStr2 <> "" Then
                nextaddr = Val("&H" & Mid(LineStr2, 6, 5))
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = nextaddr - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .CellText(currentaddr - startingaddr + 1 + X, 2) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .CellText(currentaddr - startingaddr + 1 + X, 2) = "[" & Format(X, "00") & "]"
                    Next X
                Else
                    .CellText(currentaddr - startingaddr + 1, 2) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
            Else
                ' we already reach the end of file
                
                ' Check if this DSP variable has mutliple entries, e.g. channel dependent parameter
                addrdiff = &H60000 - currentaddr
                If addrdiff > 1 Then
                    ' Include entry number in the DSP variable name
                    X = 0
                    .CellText(currentaddr - startingaddr + 1 + X, 2) = Right(LineStr1, (Len(LineStr1) - 12)) & "[" & Format(X, "00") & "]"
                    ' Use only entry number in other entries
                    For X = 1 To addrdiff - 1
                        .CellText(currentaddr - startingaddr + 1 + X, 2) = "[" & Format(X, "00") & "]"
                    Next X
                    
                Else
                    .CellText(currentaddr - startingaddr + 1, 2) = Right(LineStr1, (Len(LineStr1) - 12))
                End If
                
                LineStr1 = LineStr2
                Exit Do

            End If
            
            ' Update current address and linestr
            currentaddr = nextaddr
            LineStr1 = LineStr2
            
        Loop
       
        For X = 1 To .Rows
            .CellText(X, 1) = "0x0005" & Right(ToHex(X - 1), 4)
            
            For Y = 1 To .Columns
                .CellTextAlign(X, Y) = DT_RIGHT
            Next Y
            
            ' Update the ProgressBar control.
            If X Mod 4096 = 0 Then
                If X + 4096 > .Rows Then
                    Pixie16MDIForm.Pixie16ProgressBar.Value = 100
                Else
                    Pixie16MDIForm.Pixie16ProgressBar.Value = (X + 4096) / .Rows * 100
                End If
            End If
        Next X
    
        .Redraw = True
    End With
    
    ' Close the Pixie16_DSP_List_File
    Close #1

End Sub

Private Sub showDSPMemBlock1_ModNumEdit_ValueChanged(Value As Variant, PreviousValue As Variant, ByVal OutOfRange As Boolean)
    ' re-load DSP list
    Call Load_DSPlsts1(showDSPMemBlock1_ModNumEdit.Value)
End Sub
