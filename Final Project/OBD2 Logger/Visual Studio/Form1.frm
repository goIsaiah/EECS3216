VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   Caption         =   "Speed & RPM"
   ClientHeight    =   3150
   ClientLeft      =   120
   ClientTop       =   465
   ClientWidth     =   6135
   LinkTopic       =   "Form1"
   ScaleHeight     =   3150
   ScaleWidth      =   6135
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdLoad 
      Caption         =   "Load"
      Height          =   495
      Left            =   3120
      TabIndex        =   10
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdStartStop 
      BackColor       =   &H000000FF&
      Caption         =   "Stop"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1095
      Left            =   4680
      MaskColor       =   &H0000FFFF&
      TabIndex        =   6
      Top             =   960
      Width           =   1095
   End
   Begin VB.CommandButton cmdCustom 
      Caption         =   "Custom"
      Height          =   375
      Left            =   4680
      TabIndex        =   5
      Top             =   2160
      Width           =   1095
   End
   Begin VB.CommandButton cmdVIN 
      Caption         =   "VIN"
      Height          =   375
      Left            =   4680
      TabIndex        =   4
      Top             =   2640
      Width           =   1095
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   2520
      Top             =   720
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      CommPort        =   6
      DTREnable       =   -1  'True
   End
   Begin VB.TextBox Text1 
      BeginProperty Font 
         Name            =   "Terminal"
         Size            =   9
         Charset         =   255
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   360
      TabIndex        =   2
      Text            =   "OBDII Request"
      Top             =   120
      Width           =   5415
   End
   Begin VB.CommandButton cmdRPM 
      Caption         =   "RPM"
      Height          =   495
      Left            =   360
      TabIndex        =   1
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdSpeed 
      Caption         =   "Speed"
      Height          =   495
      Left            =   1800
      TabIndex        =   0
      Top             =   2520
      Width           =   1095
   End
   Begin MSCommLib.MSComm MSCommArduino 
      Left            =   3240
      Top             =   720
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      CommPort        =   7
      DTREnable       =   -1  'True
      BaudRate        =   38400
   End
   Begin VB.Label lblResponse 
      Caption         =   "OBDII Response"
      BeginProperty Font 
         Name            =   "Terminal"
         Size            =   9
         Charset         =   255
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   360
      TabIndex        =   9
      Top             =   600
      Width           =   5415
   End
   Begin VB.Label lblLoad 
      Alignment       =   2  'Center
      Caption         =   "-"
      BeginProperty Font 
         Name            =   "Goudy Stout"
         Size            =   15.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Left            =   3240
      TabIndex        =   8
      Top             =   1440
      Width           =   1095
   End
   Begin VB.Label lblRPM 
      Alignment       =   2  'Center
      Caption         =   "-"
      BeginProperty Font 
         Name            =   "Goudy Stout"
         Size            =   15.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Left            =   360
      TabIndex        =   7
      Top             =   1440
      Width           =   1095
   End
   Begin VB.Label lblSpeed 
      Alignment       =   2  'Center
      Caption         =   "-"
      BeginProperty Font 
         Name            =   "Goudy Stout"
         Size            =   15.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Left            =   1800
      TabIndex        =   3
      Top             =   1440
      Width           =   1095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const OBD2_SEND_DELAY = 100
Const ARDUINO_SEND_DELAY = 100

Const FIRST_INDEX = 1
Private Enum Measurements
    rpm = FIRST_INDEX
    speed
    Load
End Enum
Private mcolQueue As New Collection
Private mbytQueue As Byte
Private mbytBluetoothRequest As Byte

Dim Mystrg As String
Dim Temp As String
Dim B1 As String
Dim B2 As String
Dim B3 As Integer
Dim B4 As Integer

#If includeCars > 0 Then
Private Enum Cars
    ford = FIRST_INDEX
    Chevrolet
    [Chrysler 300]
    [Honda Civic]
    Subaru
    Toyota
    Mazda
    BMW
    Mercedes
    Tesla
End Enum
#End If

Private Sub Form_Load()
    mcolQueue.Add "010C" ', Measurements.rpm
    mcolQueue.Add "010D" ', Measurements.speed
    mcolQueue.Add "0104" ', Measurements.load
    mbytQueue = FIRST_INDEX
    mbytBluetoothRequest = FIRST_INDEX
    If DEBUG_MODE = 1 Then
        cmdStartStop.Caption = "Raw Data"
    Else
        Mystrg = Space(255)
        If DEBUG_MODE = 0 Then
            With MSComm1
                .Settings = "9600,n,8,1"
                .RThreshold = 1
                .PortOpen = True
            End With
        End If
        With MSCommArduino
            .Settings = "57600,n,8,1"
            .RThreshold = 1
            .PortOpen = True
        End With
        cmdStartStop.Caption = "Conn OK"
    End If
End Sub

Private Sub cmdStartStop_Click()
    gblnStartStop = Not gblnStartStop
    With cmdStartStop
        If gblnStartStop Then
            .Caption = "Started"
            Me.BackColor = vbGreen
            Call sendOBD2Msg
        Else
            .Caption = "Stopped"
            Me.BackColor = vbRed
        End If
    End With
End Sub

Private Sub MSComm1_OnComm()
    Dim strResponse As String
    Select Case MSComm1.CommEvent
    Case comEvReceive
        strResponse = MSComm1.Input
        logRaw strResponse
        If receiveOBD2Msg(strResponse) Then Call sendOBD2Msg
    End Select
End Sub

Public Function receiveOBD2Msg(strResponse As String) As Boolean
    Dim intResponse As Integer, strResponseA As String, strResponseB As String, _
        vntResponse As Variant, intCount As Integer
    lblResponse.Caption = strResponse
    If checkError(strResponse) Then
        MsgBox strResponse, vbCritical, "Error from OBDII"
    Else
        'Remove spaces, since some cars have spaces and others do not
        'e.g. MiEV has spaces, XL-7 does not.
        strResponse = Replace(strResponse, " ", "")
        'ignore responses that are just 2 characters "vbCr" and ">"
        vntResponse = Split(strResponse, vbCr)
        For intCount = LBound(vntResponse) To UBound(vntResponse)
            strResponse = vntResponse(intCount)
            If Len(strResponse) > 4 Then ' Val(Mid$(strResponse, 5, 2)) > 0 Then
                strResponseA = Mid$(strResponse, 5, 2)
                strResponseB = Mid$(strResponse, 7, 2)
                intResponse = Val("&h" & strResponseA & strResponseB)
                Select Case Left$(strResponse, 4)
                Case "410C" 'RPM
                    intResponse = intResponse / 4
                    lblRPM.Caption = intResponse
                Case "410D" 'Speed
                    lblSpeed.Caption = intResponse
                Case "4104" 'Load
                    lblLoad.Caption = CInt(intResponse / 255 * 100) & "%"
                    logData Val(lblRPM.Caption) & vbTab & _
                            Val(lblSpeed.Caption) & vbTab & _
                            Val(lblLoad.Caption)
                    Call sendArduinoMsg
                End Select
                receiveOBD2Msg = True 'Call sendOBD2Msg
                Exit For
            End If
        Next
    End If
    DoEvents
End Function

Private Sub sendOBD2Msg()
    If gblnStartStop Then
        Text1.Text = mcolQueue.Item(mbytQueue)
        Sleep OBD2_SEND_DELAY
        MSComm1.Output = mcolQueue.Item(mbytQueue) & vbCr
        mbytQueue = mbytQueue + 1
        If mbytQueue > mcolQueue.Count Then mbytQueue = FIRST_INDEX
    End If
End Sub

Private Sub sendArduinoDelayed(bytData As Byte)
    MSCommArduino.Output = Chr$(bytData)
    Sleep ARDUINO_SEND_DELAY
End Sub

Public Sub sendArduinoMsg()
    #If useold > 0 Then
    Dim strData As String
    Select Case mbytBluetoothRequest
    Case Measurements.rpm
        strData = lblRPM.Caption
    Case Measurements.speed
        strData = lblSpeed.Caption
    Case Measurements.Load
        strData = lblLoad.Caption
    End Select
    MSCommArduino.Output = CStr(CInt(Val(strData)))
    #End If
    Dim intTach As Integer, intSpeed As Integer, intLoad As Integer, _
        bytTach1 As Byte, bytTach2 As Byte, bytSpeed As Byte, bytLoad As Byte
    intTach = Val(lblRPM.Caption)
    intSpeed = Val(lblSpeed.Caption)
    intLoad = Val(lblLoad.Caption)
    bytTach1 = intTach / 256
    bytTach2 = intTach Mod 256
    bytSpeed = intSpeed
    bytLoad = intLoad
    sendArduinoDelayed 255
    sendArduinoDelayed 255
    sendArduinoDelayed bytTach1
    sendArduinoDelayed bytTach2
    sendArduinoDelayed bytSpeed
    sendArduinoDelayed bytLoad
End Sub

Private Function checkError(strMsg As String) As Boolean
    Dim vntErrors As Variant, vntPause As Variant, intCount As Integer
    vntErrors = Array("NO DATA", "SEARCHING...")
    vntPause = Array(10, 30)
    If InStr(1, strMsg, "UNABLE TO CONNECT") Then
        checkError = True
    Else
        For intCount = LBound(vntErrors) To UBound(vntErrors)
            If InStr(1, strMsg, vntErrors(intCount), vbTextCompare) Then
                'checkError = True
                If DEBUG_MODE = 0 Then Sleep vntPause(intCount) * 1000
                Exit For
            End If
        Next
    End If
End Function



















Private Sub cmdCustom_Click()
    Mystrg = ""
    MSComm1.Output = Text1.Text & vbCr
    'Attente (500)
    Mystrg = MSComm1.Input
    Text1.Text = Mystrg
End Sub

Private Sub cmdVIN_Click()
    Dim intRPM As Integer
    Mystrg = ""
    MSComm1.Output = "0902" & vbCr
    'Attente (500)
    Mystrg = MSComm1.Input
    Text1.Text = Mystrg
    Temp = Mid(Mystrg, 15, 3)
    If InStr(1, Temp, ">") <> 0 Then
        'If Mid(MyStrg, 15, 3) = ">" Then
        B1 = Mid(Mystrg, 7, 2)
        B2 = Mid(Mystrg, 10, 2)
        'On Error Resume Next
        B3 = ("&H" & B1) 'Convert to Hex
        B4 = ("&H" & B2)
        intRPM = (((B3 * 256) + B4) / 4)
        intRPM = Format(rpm, "0")
        lblRPM.Caption = intRPM & " RPM" 'Output RPM
    Else
        'Do nothing
    End If
End Sub

'RPM code 010C
Private Sub cmdRPM_Click()
'010C
'NO Data
'
'>


    Dim intRPM As Integer
    Mystrg = ""
    MSComm1.Output = "010C" & vbCr
    'Attente (500)
    Mystrg = MSComm1.Input
    Text1.Text = Mystrg
    Temp = Mid(Mystrg, 15, 3)
    If InStr(1, Temp, ">") <> 0 Then
        'If Mid(MyStrg, 15, 3) = ">" Then
        B1 = Mid(Mystrg, 9, 2)
        B2 = Mid(Mystrg, 12, 2)
        'On Error Resume Next
        B3 = ("&H" & B1) 'Convert to Hex
        B4 = ("&H" & B2)
        intRPM = (((B3 * 256) + B4) / 4)
        intRPM = Format(rpm, "0")
        lblRPM.Caption = intRPM 'Output RPM
    Else
        'Do nothing
    End If
End Sub


Private Sub cmdSpeed_Click()
'Speed Code
'010D
'41 0D 00
'
'>
Dim intSpeed As Integer
Mystrg = ""
MSComm1.Output = "010D" & vbCr
Mystrg = MSComm1.Input
    Text1.Text = Mystrg
    Temp = Mid(Mystrg, 12, 3)
    If InStr(1, Temp, ">") <> 0 Then
    'If Mid(MyStrg, 12, 1) = ">" Then
        B1 = Mid(Mystrg, 7, 2)
        'convert selection into decimal
        B3 = ("&H" & B1)
        'Do calculation for speed
        intSpeed = (B3 * 0.6215)
        intSpeed = Format(intSpeed, "0.0")
        'display it!
        lblSpeed.Caption = intSpeed
    Else
        'Do nothing
    End If
End Sub

Private Sub cmdLoad_Click()
    Dim intLoad As Integer
    Mystrg = ""
    MSComm1.Output = "0104" & vbCr
    Mystrg = MSComm1.Input
    Text1.Text = Mystrg
    Temp = Mid(Mystrg, 12, 3)
    If InStr(1, Temp, ">") <> 0 Then
    'If Mid(MyStrg, 12, 1) = ">" Then
        B1 = Mid(Mystrg, 7, 2)
        'convert selection into decimal
        B3 = ("&H" & B1)
        'Do calculation for speed
        intLoad = B3
        lblLoad.Caption = intLoad
    Else
        'Do nothing
    End If
End Sub

