Attribute VB_Name = "Pixie16_StartUp"

Sub main()
    
    Dim DELAYTIME As Long
    Dim intCounter As Long
    
    Call Load(frmSplash)
    Call frmSplash.Show
    CenterFrm frmSplash
    
    DELAYTIME = 150
    Do
        Sleep (1)
        DoEvents
        intCounter = intCounter + 1
    Loop Until (intCounter = DELAYTIME)
    
    Call Pixie16MDIForm.Show
    Call Unload(frmSplash)

End Sub


Sub CenterFrm(aForm As Form)
Dim X, Y
    
    X = (Screen.Width - aForm.Width) / 2
    Y = (Screen.Height - aForm.Height) / 2
    
    aForm.Move X, Y
    
End Sub
