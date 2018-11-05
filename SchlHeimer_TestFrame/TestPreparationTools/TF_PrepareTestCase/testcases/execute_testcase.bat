@echo off
set BASEDIR=..\..\..
set TOOLDIR=%BASEDIR%\Debug
set TESTCASE=%1
set TESTCASEMRTS=%TESTCASE%.mrts
set REPORTDIR=%BASEDIR%\..\reports
set TESTCASESTART=%TESTCASE%_start.cfg
set TESTCASEHTML=%TESTCASE%.html
title A0 TEST CASE %TESTCASE%
rem **** prepare startup file for TF interface
echo load %TESTCASEMRTS% > %TESTCASESTART%
echo list >> %TESTCASESTART%
rem **** call TF interface in interactive mode
%TOOLDIR%\TFInterface.exe -s %TESTCASESTART% -i
del %TESTCASESTART%
rem *** generate test report and show it in browser
if NOT EXIST %REPORTDIR% goto NOREPORTDIR
%TOOLDIR%\TRGenerator.exe -o %REPORTDIR% -s %TESTCASEMRTS%
pushd %REPORTDIR%
if NOT EXIST %TESTCASEHTML% goto SKIPREMOVE
del %TESTCASEHTML% >NUL
:SKIPREMOVE
mv report.html %TESTCASEHTML%
start %TESTCASEHTML%
popd
goto:eof
:NOREPORTDIR
echo %REPORTDIR% not existing
pause
