@echo off
if not exist student_manager.exe (
    echo student_manager.exe not found. Building now...
    call build.bat
)

if exist student_manager.exe (
    student_manager.exe %*
)

