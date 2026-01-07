@echo off
REM Name of the final executable
set OUTPUT=agenda_program.exe

REM Compile all C files
gcc main.c agenda.c Structs\day.c Structs\month.c Structs\task.c Structs\year.c Structs\contains.c -Wall -g -o %OUTPUT%

IF %ERRORLEVEL% EQU 0 (
    echo Compilation successful! Run %OUTPUT%
) ELSE (
    echo Compilation failed!
)
pause
