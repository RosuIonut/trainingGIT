@echo off

echo ===== Remove old _testtools directory =====
del /F /Q _testtools\*.*
rmdir _testtools
echo.

echo ===== Create new _testtools directory =====
mkdir _testtools
echo.

echo ========== Copy TestTool package ==========
xcopy TestToolsPackage\*.sdh _testtools
echo.
xcopy TestToolsPackage\*.cmk _testtools
echo.
xcopy TestToolsPackage\*.h _testtools
echo.
xcopy TestToolsPackage\*.c _testtools
echo.
xcopy TestToolsPackage\*.cpp _testtools
echo.

echo ======== Copy Shared Memory files =========
xcopy TestFrameInterface\SHMEM_*.* _testtools
echo.

echo ================== Done ===================
echo.
pause
