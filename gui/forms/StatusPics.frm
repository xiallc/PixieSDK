VERSION 5.00
Begin VB.Form StatusPics 
   Caption         =   "Status Pictures"
   ClientHeight    =   900
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   2190
   LinkTopic       =   "Form1"
   ScaleHeight     =   900
   ScaleWidth      =   2190
   Begin VB.Image GraySquare 
      Height          =   240
      Left            =   1560
      Picture         =   "StatusPics.frx":0000
      Stretch         =   -1  'True
      Top             =   240
      Width           =   240
   End
   Begin VB.Image RedSquare 
      Height          =   240
      Left            =   960
      Picture         =   "StatusPics.frx":0342
      Top             =   240
      Width           =   240
   End
   Begin VB.Image GreenSquare 
      Height          =   240
      Left            =   360
      Picture         =   "StatusPics.frx":0444
      Top             =   240
      Width           =   240
   End
End
Attribute VB_Name = "StatusPics"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
