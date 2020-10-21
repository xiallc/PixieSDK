VERSION 5.00
Begin VB.Form Pixie16MsgLogFile 
   Caption         =   "Pixie-16 Log File Content"
   ClientHeight    =   10470
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   11190
   Icon            =   "Pixie16MsgLogFile.frx":0000
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   10470
   ScaleWidth      =   11190
   Begin VB.CommandButton CloseLogFileForm 
      Caption         =   "Close"
      Height          =   615
      Left            =   7200
      TabIndex        =   2
      Top             =   9600
      Width           =   1575
   End
   Begin VB.CommandButton RefreshMsgText 
      Caption         =   "Refresh"
      Height          =   615
      Left            =   2400
      TabIndex        =   1
      Top             =   9600
      Width           =   1575
   End
   Begin VB.TextBox Pixie16MsgTextBox 
      Height          =   9255
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   120
      Width           =   10935
   End
End
Attribute VB_Name = "Pixie16MsgLogFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub CloseLogFileForm_Click()
    Unload Me
End Sub

Private Sub RefreshMsgText_Click()

    Dim Pixie16MsgTxt As String
    Pixie16MsgTxt = String(65535, vbNullChar)
    
    Pixie16ReadMSGFile (Pixie16MsgTxt)
    Pixie16MsgTextBox.Text = Pixie16MsgTxt

End Sub
