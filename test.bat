@echo OFF
set a=%1
set b=%2
set/a c=%a%+%b%
echo %1 + %2 = %c%