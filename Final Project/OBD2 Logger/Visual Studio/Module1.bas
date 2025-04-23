Attribute VB_Name = "Module1"
'OBD2 pin 1234

Option Explicit

Global Const DEBUG_MODE = 0 '2 '0=live; 1=RawDataFile; 2=TestArduino

Global gblnStartStop As Boolean
Dim mintData As Integer
Dim mintRaw As Integer
Dim i As Integer

Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Sub Main()
    mintData = FreeFile
    Open "OBD2Data_" & Day(Now) & ".txt" For Output As #mintData
    mintRaw = FreeFile
    If DEBUG_MODE = 0 Then
        Open "OBD2Raw.txt" For Append As #mintRaw
        Form1.Show vbModal
        gblnStartStop = False
    Else
        Open "OBD2Raw.txt" For Input As #mintRaw
        Form1.Show
        If DEBUG_MODE = 1 Or DEBUG_MODE = 2 Then
            Call processRawData
        End If
        Unload Form1
    End If
    Close mintRaw
    Close mintData
End Sub

Public Function CStrHex(ByVal strMsg As String) As String
    For i = 1 To Len(strMsg)
        CStrHex = CStrHex & Right$("00" & Hex(Asc(Mid$(strMsg, i, 1))), 2) & " "
    Next
End Function

Public Sub logRaw(ByVal strMsg As String)
    Print #mintRaw, Now & vbTab & CStrHex(strMsg) & vbTab & strMsg
End Sub

Public Sub logData(ByVal strMsg As String)
    Print #mintData, Format(Now, "hh:mm:ss." & Right(Format(Timer, "#0.00"), 2)) & vbTab & strMsg
End Sub

Private Sub processRawData()
    Dim strRawData As String, vntRawData As Variant
    Do While Not EOF(mintRaw)
        Input #mintRaw, strRawData
        Form1.Text1.Text = strRawData
        vntRawData = Split(strRawData, vbTab)
        If UBound(vntRawData) > 1 Then
            strRawData = vntRawData(2)
            Form1.receiveOBD2Msg (strRawData)
        End If
        DoEvents
        Sleep 10
    Loop
End Sub

