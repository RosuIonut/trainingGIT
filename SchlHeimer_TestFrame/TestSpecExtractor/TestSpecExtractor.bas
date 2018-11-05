Attribute VB_Name = "TestSpecExtractor"
'############################################################################################################################################
'############################################################# History ######################################################################
'############################################################################################################################################
'Version    |Date       |Name           |Change                                                                                             |
'1.0        |2014-05-09 |H. Hartmann    |First Draw for VAG FPK - SELF                                                                      |
'1.1        |2014-07-07 |H. Hartmann    |Removing of comments from Test Headlines implemented, to avoid printing the xml links.             |
'1.2        |2014-07-15 |H. Hartmann    |Bug in RemoveComment Funktion removed. Function returns original string, if nothing to remove.     |
'1.3        |2014-09-08 |H. Hartmann    |Correction: TestSpecExtarctor macro renamed in TestSpecExtractor. Modulname for checking testcases |
'                                       |taken from documenttitle now.                                                                      |
'1.4        |2014-10-02 |H. Hartmann    |Default Path changed. Extracted document named as word document now.                               |
'                                       |Checking if Test case changed to simple check without module name, to keep it generic. Function    |
'                                       |GetModulenameFromTitle() removed. boIsTestCase() reworked. Tidied up, not used functions removed.  |
'1.5        |2014-10-14 |H.Hartmann     |TestSpec Extractor Version wird jetzt in Extracted Testspec ausgegeben.                            |
'1.6        |2014-10-31 |H.Hartmann     |getRevisionFromHistoryTable() can handle connected cells in history table now.                     |
'############################################################################################################################################

Option Explicit

'Document / Makro Info

' DONT FORGET TO UPDATE VERSION IN TestSpecExtractorVersion AFTER UPDATING THE MACRO!
Const TestSpecExtractorVersion As String = "1.6"

Const MRTSFFormat As String = "MRTSF 1"
Const MaxHeaderLength As Integer = 80
Public MRTSPath As String
Public DefaultPath As String

'Enumsen für Spalten
Public Enum Columns
    nTestCaseID = 1
    nTestCaseName = 2
    nPrio = 3
    nInput = 4
    nExpectedResult = 5
End Enum
Const TestChapterName  As String = "Tests"
Dim NumberOfTestCases As Integer

Function GetDefaultPathFromSpecName() As String
    Dim DocumentName As String
    'Get TestspecName
    ' Get Documentname and replace filetype with txt:
    DocumentName = Mid(ThisDocument.Name, 1, InStr(1, ThisDocument.Name, ".")) & "txt"
    ' Set Default Path:
    GetDefaultPathFromSpecName = "C:\Temp\" & DocumentName
End Function


Sub TestSpecExtractor()
Dim tmp As Integer
'Main Macro, opens and closes output file and calls generating functions.
    NumberOfTestCases = 0

    'Get Default Path from Documents name
    DefaultPath = GetDefaultPathFromSpecName()
    'Open output file for extracted MRTS
    Call OpenFileDialog 'Get Path and Name for file from user.
    Open MRTSPath For Output As #1
    Call PrintHeading
    Call PrintTests
    Print #1, "End Testspec"
    Close #1
    MsgBox "Extraction of Testspec finished." & vbCrLf & "generated test cases: " & NumberOfTestCases, vbInformation
End Sub

Function OpenFileDialog()
'Check if Dialog is available, then open it. if not, use default value
    
    MRTSPath = DefaultPath
    If CheckIfUF_TSESaveIsAvailable Then
      OpenFileDialogFunc
    'Dialog is available. -> use it
    Else
      'Use default value
      MsgBox "Form UF_TSESave missing!" & vbCrLf & "Error in VBA Project" & vbCrLf & "proceeding with default path for generated document:" & vbCrLf & MRTSPath, vbCritical
    End If
    'If Path is empty, use default path
    If MRTSPath = "" Then
        MRTSPath = DefaultPath
    End If
End Function

Private Sub OpenFileDialogFunc()
'This function is used only if UF is available. otherwise this will not be compiled.
       UF_TSESave.CB_PathAndName.Text = MRTSPath 'Set default value. Can be changed by the user in dialog.
       UF_TSESave.Show
End Sub

Function PrintHeading()
'write header information to file
    Print #1, ("# Automatic generated file, created by TestspecExtractor " & TestSpecExtractorVersion)
    Print #1, ("Begin Testspec")
    Print #1, ("Format: " & MRTSFFormat)
    Print #1, ("Filename: " & ThisDocument.Name)
    Print #1, ("Revision: " & getRevisionFromHistoryTable())
    Print #1, ("Extracted: " & "on " & Format(Date, "yyyy-mm-dd") & " at " & Time & " by " & Environ$("Username") & " (TSExtractor " & TestSpecExtractorVersion & ")")
    Print #1, ("Annotated: ")
    Print #1, ("Evaluated: ")
    Print #1, ""
End Function

Function PrintTests()
'Main Printing function, searches test relevant items and forces writing into the output file.
Dim rBegin As Range
Dim rEnd As Range
Dim rSearch As Range
Dim I As Integer
Dim DocPara As Paragraph
Dim RangePara As Paragraph
Dim done As Boolean
Dim rEndSearch As Range
Dim t As Table
Dim rTest As Range
    
    TurnScreenUpdatingOff 'avoid flickering, if msword is on screen.
    'Search for all Test chapters of file.
    For Each DocPara In ActiveDocument.Paragraphs
       
     If Not DocPara.Range.Style Is Nothing Then
       If Left(DocPara.Range.Style, Len("Überschrift 1")) = "Überschrift 1" Then
        If (Not rBegin Is Nothing) And (rEnd Is Nothing) Then
          Set rEnd = DocPara.Range
        End If
        If Left(DocPara.Range.Text, Len(TestChapterName)) = TestChapterName Then
            Set rBegin = DocPara.Range
        End If
       End If
     End If
    Next
    TurnScreenUpdatingOn
    
    Set rSearch = rBegin
    'rSearch.Start = rBegin.Start
    rSearch.End = rEnd.Start
    done = False
    While Not done
        With rSearch.Find
            .Style = wdStyleHeading2
            If .Execute(Forward:=True) Then
                If InStr(rSearch.Text, "<Test") Then
                  rSearch.Select
                  Set rEndSearch = Selection.Range
                  rEndSearch.Start = rEndSearch.End
                  'Search for test tables in current chapter and mark them
                  With rEndSearch.Find
                    .Style = wdStyleHeading2
                    If .Execute(Forward:=True) Then
                      Selection.Start = rSearch.End
                      Selection.End = rEndSearch.Start
                    Else
                      Selection.Start = rSearch.End
                      Selection.End = ActiveDocument.Range.End
                    End If
                  End With
                  'Debug.Print rSearch.text
                  PrintTestHeading (rSearch.Text)
                  'Testcases für dieses Kapitel aus Tabelle holen
                  'get test cases from current chapter fom table
                  For Each t In Selection.Tables
                  'select test tables and force printing to output file
                    t.Select
                    If (IsTableTestCaseTable(t)) Then
                        Call PrintTestCases(t)
                    End If
                  Next
                  Print #1, "End Test"
                  Print #1, (GetFilledString("", "=", MaxHeaderLength))
                  Print #1, ""
                End If
            Else
                done = True
            End If
        End With
        rSearch.Start = rSearch.End
        rSearch.End = rEnd.Start
    Wend
    Set rSearch = Nothing
End Function

Function PrintTestHeading(Text As String)
'write test chapter header to file
Dim testID As String
Dim TestName As String
Dim tempStr As String
Dim leftpos As Long
Dim rightpos As Long
Dim TestHeader As String
    
    leftpos = InStr(Text, " <")
    TestName = Left(Text, leftpos - 1)
    tempStr = Right(Text, Len(Text) - leftpos)
    leftpos = InStr(tempStr, ":") + 1
    tempStr = Right(tempStr, Len(tempStr) - leftpos)
    rightpos = InStr(tempStr, ">") - 1
    testID = Left(tempStr, rightpos)
    
    TestName = ReplaceSpecialCharacters(TestName)
    TestHeader = (testID & ": " & TestName)
    TestHeader = GetFilledString(TestHeader, "=", MaxHeaderLength)
    Print #1, (TestHeader)
    Print #1, ("Begin Test " & testID)
    Print #1, ("Name: " & TestName)
    Print #1, ""
End Function

Function GetFilledString(HeadTxt As String, fillChar As String, MaxLength As Integer) As String
'Function gets a string and give back a filled by fillChar string of MaxLength.
Dim length As Integer
Dim I As Integer
Dim CharToFill As Integer
Dim tmpStr As String
Const MinFillChars As Integer = 10 'Minimum number of fill characters which have to be printed in fromnt of the headline text. (without comment sign # at start and whitespace at the end.)
 
    length = Len(HeadTxt)
    If (length >= MaxLength - MinFillChars) Then
        'If original string fills the max length of the target string, it'll be cut to fit in with MinFillChars.
        HeadTxt = Mid(HeadTxt, 1, MaxLength - MinFillChars - 2)
        length = Len(HeadTxt)
    End If
        'Fill String with commentsign and n fillChar
        CharToFill = MaxLength - length
        For I = 1 To (CharToFill - 2) '-2 because of comment sign at start and whitespace between last fill sign and start of text.
            If I = 1 Then
                tmpStr = "#"
            Else
                tmpStr = tmpStr + fillChar
            End If
        Next
        'If headline text is available, put whitespace after at the end of the fill characters. Put strings together.
        If HeadTxt = "" Then
            'bei Kommentarzeile ohne Überschrift, kein Leerzeichen hinten anfügen.
            'If commentrow is without headline text, don't put in the whitespace at the end.
            GetFilledString = (tmpStr + fillChar)
        Else
            GetFilledString = (tmpStr & " " & HeadTxt)
        End If
        'Debug.Print (GetFilledString)
End Function

Function IsTableTestCaseTable(t As Table) As Boolean
'return true, if Table is table with test cases.
Dim str As String
    
    str = strCleanup(t.Cell(1, nTestCaseID).Range.Text)
    IsTableTestCaseTable = False
    
    If (str = "TestCaseID") Then
        IsTableTestCaseTable = True
    End If
End Function

Function PrintTestCases(t As Table)
'print all test cases of table t to file
Dim TableRows As Integer
Dim I As Integer
Dim tmpStr As String

TableRows = t.Rows.Count
    'Debug.Print (strCleanup(t.Cell(1, 1).Range.text))
    
    For I = 1 To TableRows
    'iterate through the table and print all test cases
        'Check if TestID
        If (boIsTestCase(t.Cell(I, nTestCaseID).Range.Text)) Then
            'Debug.Print t.Cell(i, nTestCaseID).Range.text
            'set Counter for Information in finished dialogue
            NumberOfTestCases = NumberOfTestCases + 1
            'Write Headline for Testcase
            Print #1, (WriteTestCaseHeadline(t, I))
            'Copy test relevant Data to Extracted Testspec:
            'TestCaseID
            Call WriteTestDataToFile(t, I, nTestCaseID)
            'Test Case Name
            Call WriteTestDataToFile(t, I, nTestCaseName)
            'Prio
            Call WriteTestDataToFile(t, I, nPrio)
            'Input
            Call WriteTestDataToFile(t, I, nInput)
            'Expected result
            Call WriteTestDataToFile(t, I, nExpectedResult)
            'Output
            Print #1, "Begin Output"
            Print #1, "End Output"
            'Result
            Print #1, ""
            Print #1, "Result: "
            'End Test Case
            Print #1, "End Testcase"
            Print #1, (GetFilledString("", "-", MaxHeaderLength))
            Print #1, ""
        End If
    Next I
End Function

Public Function boIsTestCase(Text As String) As Boolean
'Function public, because used by TSTools also.
Dim tmp As Integer
Dim TestCaseModulId As String
    boIsTestCase = False
    'It's a testcase, if TestCase ID is framed by "[]:"
    If (InStr(1, Text, "[", vbTextCompare) And InStrRev(Text, "]:", -1, vbTextCompare) <> 0) Then
        boIsTestCase = True
    End If
End Function

Function WriteTestCaseHeadline(t As Table, row As Integer) As String
'make test case headline from TestCaseId, TestCaseName, comment and fill signs.

    WriteTestCaseHeadline = CleanTCIDStr(t.Cell(row, nTestCaseID).Range.Text) + ": " + t.Cell(row, nTestCaseName).Range.Text
    WriteTestCaseHeadline = strCleanup(WriteTestCaseHeadline)
    WriteTestCaseHeadline = RemoveComment(WriteTestCaseHeadline) 'Removes text from first "#" character
    WriteTestCaseHeadline = ReplaceSpecialCharacters(WriteTestCaseHeadline)
    WriteTestCaseHeadline = GetFilledString(WriteTestCaseHeadline, "-", MaxHeaderLength)
End Function

Function WriteTestDataToFile(Table As Object, row As Integer, Column As Integer)
Dim tempString As String
    
    'tempString = strCleanup(Table.Cell(row, Column).Range.text)
    tempString = Table.Cell(row, Column).Range.Text
    Select Case Column
        Case nTestCaseID
            tempString = strCleanup(Table.Cell(row, Column).Range.Text)
            tempString = ReplaceSpecialCharacters(tempString)
            tempString = RemoveComment(tempString) 'Removes text from first "#" character
            Call CleanTCIDStr(tempString)
            Print #1, "Begin Testcase " & tempString
        Case nTestCaseName
            tempString = strCleanup(Table.Cell(row, Column).Range.Text)
            tempString = ReplaceSpecialCharacters(tempString)
            Print #1, ("Name: " & tempString)
        Case nPrio
            tempString = strCleanup(Table.Cell(row, Column).Range.Text)
            Print #1, ("Prio: " & tempString)
            Print #1, ""
        Case nInput
            Print #1, "Begin Input"
            tempString = CleanAndConvertCR(tempString)
            tempString = ReplaceSpecialCharacters(tempString)
            Print #1, (ClearCRLF(tempString))
            Print #1, "End Input"
        Case nExpectedResult
            CleanAndConvertCR (tempString)
            tempString = ReplaceSpecialCharacters(tempString)
            Print #1, "Begin Expected"
            tempString = CleanAndConvertCR(tempString)
            Print #1, (ClearCRLF(tempString))
            Print #1, "End Expected"
    End Select
End Function

Function RemoveComment(s As String)
'Removes text from first "#" character
Dim pos As Integer
    pos = InStr(1, s, "#", vbTextCompare)
    If pos > 0 Then
        RemoveComment = Mid(s, 1, pos - 1)
    Else
    'Nothing to remove, return original string.
        RemoveComment = s
    End If
End Function

Function ClearCRLF(ByRef Text As String) As String
'Erase last CR LF at end of string
Dim length As Integer

    length = Len(Text)
    If (Right(Text, 2) = (Chr(13) + Chr(10))) Then
        Text = Mid(Text, 1, length - 2)
    End If
    ClearCRLF = Text
End Function

Function CleanAndConvertCR(ByRef Text As String) As String
'Function cleans Word Table BEL character and converts VT character to CRLF.
'Because word table cells alway's end with Chr(13)+Chr(7) (CR BEL)
    Text = Replace(Text, Chr(13) + Chr(7), "") 'Clear BEL
    Text = Replace(Text, Chr(13), Chr(13) + Chr(10)) 'Clear CR
    Text = Replace(Text, Chr(11), Chr(13) + Chr(10))
    CleanAndConvertCR = Text
End Function

 Function CleanTCIDStr(ByRef Text As String)
 'Function cleans TestCaseID from [,] and : characters
    Text = Replace(Text, "[", "")
    Text = Replace(Text, "]", "")
    Text = Replace(Text, ":", "")
    Text = Replace(Text, " ", "")
    CleanTCIDStr = Text
 End Function
 
 Function strCleanup(s As String) As String
 'Cleans string from not valid characters and unwanted word wraps.
    s = Replace(s, Chr(7), "") 'BEL
    s = Replace(s, Chr(10), "") 'LF
    s = Replace(s, Chr(11), "") 'VT (Kleiner Zeilenumbruch)
    s = Replace(s, Chr(13), "") 'CR
    s = Replace(s, Chr(150), Chr(45)) 'Gedankenstrich durch Minus ersetzen.
    strCleanup = s
End Function

Function ReplaceSpecialCharacters(s As String) As String
    'mutatet vowels
    s = Replace(s, "ä", "ae")
    s = Replace(s, "ö", "oe")
    s = Replace(s, "ü", "ue")
    'other characters
    s = Replace(s, "ß", "ss")
    s = Replace(s, Chr(180), Chr(39)) 'replace ´ with Chr(39)'
    s = Replace(s, Chr(146), Chr(39)) 'replace ` with Chr(39)'
    s = Replace(s, Chr(96), Chr(39)) 'replace ´ with Chr(39)'
    s = Replace(s, Chr(181), "micro")
    s = Replace(s, Chr(150), Chr(45)) 'replace dash by minus
    s = Replace(s, Chr(147), Chr(34)) 'replace start Quotation mark
    s = Replace(s, Chr(148), Chr(34)) 'replace end quotation mark
    ReplaceSpecialCharacters = s
End Function

Function getRevisionFromHistoryTable() As String
'Funktion sucht in History table letzten eintrag und liefert die Revision und das Datum als String zurück
Dim t As Table
Dim I As Integer
    Set t = ActiveDocument.Tables(2)
    t.Select
    If InStr(t.Cell(1, 1).Range.Text, "Revision") = 1 Then
      I = t.Rows.Count

      While I > 1
        On Error GoTo ErrorHandle 'If connected cells are used
        If strCleanup(t.Cell(I, 1).Range.Text) <> "" Then
          'tempStr = strCleanup(t.Cell(i, 1).Range.text) & " from " & strCleanup(t.Cell(i, 2).Range.text)
          getRevisionFromHistoryTable = strCleanup(t.Cell(I, 1).Range.Text)
          I = 0
        End If
        I = I - 1
      Wend
    End If
ErrorHandle:
'Error handling, if connected cells are used in history table. Jump over the connected cell.
    Select Case Err.Number
        Case 5941
            I = I - 1
            Resume 'resume to same place and check, if error is still there, because of more connected cells.
    End Select
End Function

Sub TurnScreenUpdatingOn()
'As function name says ... ;-)
    Application.ScreenUpdating = True
End Sub

Sub TurnScreenUpdatingOff()
'Screen updating off to avoid flickering of screen while using select.
    Application.ScreenUpdating = False
End Sub

Function CheckIfUF_TSESaveIsAvailable() As Boolean
'Check if UF is in project.
Dim objVBE As Object
    CheckIfUF_TSESaveIsAvailable = False
    For Each objVBE In ThisDocument.VBProject.VBComponents
      If objVBE.Type = 3 Then
            'MsgBox objVBE.Name
            If objVBE.Name = "UF_TSESave" Then
                CheckIfUF_TSESaveIsAvailable = True
            End If
        End If
    Next
End Function
