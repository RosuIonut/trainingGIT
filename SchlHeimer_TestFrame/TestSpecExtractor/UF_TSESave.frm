VERSION 5.00
Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} UF_TSESave 
   Caption         =   "Test Spec Extractor"
   ClientHeight    =   2070
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   4710
   OleObjectBlob   =   "UF_TSESave.frx":0000
   StartUpPosition =   1  'Fenstermitte
End
Attribute VB_Name = "UF_TSESave"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit


Private Sub CMB_CANCEL_Click()
'stop programm
    UF_TSESave.Hide
    End
End Sub

Private Sub CMB_OK_Click()
'takeover path and name and proceed.
    TestSpecExtractor.MRTSPath = CB_PathAndName.text
    UF_TSESave.Hide
End Sub

