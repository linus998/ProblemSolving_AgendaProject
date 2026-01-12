@echo off
set OUTPUT=agenda.exe
gcc main.c agenda.c day.c month.c task.c year.c helpers.c -Wall -g -o %OUTPUT%

IF %ERRORLEVEL% EQU 0 (
    echo Compilation successful! Run %OUTPUT%
) ELSE (
    echo Compilation failed!
)
pause
