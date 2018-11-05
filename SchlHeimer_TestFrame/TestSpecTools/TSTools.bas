Attribute VB_Name = "TSTools"
'############################################################################################################################################
'############################################################# History ######################################################################
'############################################################################################################################################
'Version    |Date       |Name           |Change                                                                                             |
'1.0        |2014-07-09 |H. Hartmann    |ColourUpComments Macro inserted                                                                    |
'1.1        |2014-07-22 |H. Hartmann    |Only color up if Table row is an test case. otherwise cells in rthis row will be ignored.          |
'1.2        |2014-10-02 |H.Hartmann     |Local boIsTestcase() function reworked. boIsTestcase() from TestSpecExtractor is called now.       |
'############################################################################################################################################
Option Explicit

Sub ColourUpComments()
Attribute ColourUpComments.VB_Description = "Macro formats all comments in test tables in dark green color."
Attribute ColourUpComments.VB_ProcData.VB_Invoke_Func = "Project.TSTools.ColourUpComments"
'Function searches for comments (Text after #) to CR or VT in Input and expected result columns and colours it in green.
'Should only be used in Test Tables and not in Test Case ID column, because of the XML Trace links.
'Coloring will be made for the whole table.

'Instructions:
'Set Cursor to Test table, which should be formated, then start the macro.

Const iIDCol = 1    'Column with TestCase ID
Const iStartRow = 2
Const iStartCol = 4
Const iEndCol = 5
Const lColorComment = 32768 ' #008000, dark green

Dim rngWork As Range
Dim iCurrCol As Integer
Dim iCurrRow As Integer
Dim iRowCount As Integer

Dim boCommentFound As Boolean

Dim lOldStart As Long
Dim lOldEnd As Long
Dim vTest As Variant

iRowCount = Selection.Tables(1).Rows.Count

' Iterate over all rows in this table
For iCurrRow = iStartRow To iRowCount
        'Bugfix: check if Testcase, then color upm the comments.
        If (boIsTestCase(Selection.Tables(1).Cell(iCurrRow, iIDCol).Range.Text) = True) Then
             ' Iterate over all columns in this row
            For iCurrCol = iStartCol To iEndCol
                ' Set selection on whole cell
                Selection.Tables(1).Cell(iCurrRow, iCurrCol).Range.Select
                ' Reduce selection by 1 character to select only text in cell
                Selection.MoveEnd unit:=wdCharacter, Count:=-1
                ' Assign the complete cell to the working range
                Set rngWork = Selection.Range
                ' Work on this range until end of cell is reached
                Do
                    boCommentFound = False
                    ' Find start of next comment
                    lOldStart = rngWork.Start
                    rngWork.MoveStartUntil CSet:="#", Count:=rngWork.End - rngWork.Start
                    ' Comment is found if either start was moved or was already placed on a comment
                    If (rngWork.Start <> lOldStart) Or (Left(rngWork.Text, 1) = "#") Then
                        boCommentFound = True
                        ' Remember old end position to restore it later
                        lOldEnd = rngWork.End
                        ' Move end to start for searching forward until the end of line
                        rngWork.End = rngWork.Start + 1
                        ' Reduce end of work range to contain comment line only
                        rngWork.MoveEndUntil CSet:=Chr(13) & Chr(11), Count:=lOldEnd - rngWork.Start
                        ' Color the string
                        rngWork.Font.Color = lColorComment
                        ' Move start position to current end position
                        rngWork.Start = rngWork.End
                        ' Restore old end position
                        rngWork.End = lOldEnd
                    End If
                Loop While boCommentFound
            Next iCurrCol
        End If
Next iCurrRow
End Sub
Private Function boIsTestCase(Text As String) As Boolean
    'Deliver True, if Testcase found.
    'Call boIsTestCase() from TestSpecExtractor.
    boIsTestCase = TestSpecExtractor.boIsTestCase(Text)
End Function


Sub ColourUp()
Attribute ColourUp.VB_ProcData.VB_Invoke_Func = "Project.TSTools.ColourUp"
'
' TSTools.ColourUp Makro
'
'call ColourUpComments makro by shortcut
    ColourUpComments
End Sub
