@echo off
echo =======================================================
echo Compiling Aditya University - Student Record Manager...
echo =======================================================

where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] g++ compiler not found in PATH!
    echo Please install MinGW or ensure g++ is in your system PATH.
    exit /b 1
)

g++ -std=c++17 -O2 -Iinclude ^
    src/main.cpp ^
    src/models/Student.cpp ^
    src/models/AcademicRecord.cpp ^
    src/repository/FileStudentRepository.cpp ^
    src/service/StudentService.cpp ^
    src/service/AttendanceManager.cpp ^
    src/service/FeeManager.cpp ^
    src/service/UserManager.cpp ^
    src/service/ReportGenerator.cpp ^
    src/http/HttpRequest.cpp ^
    src/http/HttpResponse.cpp ^
    src/http/Router.cpp ^
    src/http/HttpServer.cpp ^
    src/cli/CliController.cpp ^
    -o student_manager.exe -lws2_32

if %errorlevel% equ 0 (
    echo.
    echo =======================================================
    echo [SUCCESS] Build completed successfully!
    echo Output executable: student_manager.exe
    echo.
    echo Run using: student_manager.exe
    echo or run batch script: run.bat
    echo =======================================================
) else (
    echo.
    echo [ERROR] Compilation failed. Please check the errors above.
    exit /b %errorlevel%
)

