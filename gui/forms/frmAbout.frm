VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   ClientHeight    =   4530
   ClientLeft      =   255
   ClientTop       =   1410
   ClientWidth     =   9690
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   Icon            =   "frmAbout.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   ScaleHeight     =   4530
   ScaleWidth      =   9690
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdExit 
      Caption         =   "Close"
      Height          =   375
      Left            =   3960
      TabIndex        =   6
      Top             =   4080
      Width           =   1695
   End
   Begin VB.Frame Frame1 
      Height          =   3915
      Left            =   150
      TabIndex        =   0
      Top             =   60
      Width           =   9345
      Begin VB.Label LabelWebAddr 
         Caption         =   "www.xia.com"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   4920
         TabIndex        =   5
         Top             =   2880
         Width           =   1455
      End
      Begin VB.Image imgLogo 
         Height          =   1785
         Left            =   120
         Picture         =   "frmAbout.frx":000C
         Stretch         =   -1  'True
         Top             =   1920
         Width           =   4095
      End
      Begin VB.Label lblCopyright 
         Caption         =   "Copyright (2005 - 2020) XIA LLC"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   4920
         TabIndex        =   1
         Top             =   2520
         Width           =   4335
      End
      Begin VB.Label lblVersion 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         Caption         =   "Version 2.3.0"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   285
         Left            =   4920
         TabIndex        =   2
         Top             =   1920
         Width           =   1470
      End
      Begin VB.Label lblProductName 
         AutoSize        =   -1  'True
         Caption         =   "Pixie-16"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   32.25
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   765
         Left            =   2520
         TabIndex        =   4
         Top             =   840
         Width           =   2445
      End
      Begin VB.Label lblCompanyProduct 
         AutoSize        =   -1  'True
         Caption         =   "16-channel digital gamma-ray spectrometer"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   18
            Charset         =   0
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   435
         Left            =   1680
         TabIndex        =   3
         Top             =   240
         Width           =   7410
      End
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private Sub cmdExit_Click()
    Unload Me
End Sub

