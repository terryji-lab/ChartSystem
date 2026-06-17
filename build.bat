@echo off
setlocal

echo ==========================================
echo       Building ChartSystem Project...
echo ==========================================

REM Set compiler and flags
set CXX=g++
set CXXFLAGS=-std=c++14 -g
set INCLUDES=-I src

REM Set source files
set SRC_FILES=src\main.cpp src\chart\*.cpp src\common\*.cpp src\ui\*.cpp src\utils\*.cpp

REM Set libraries
set LIBS=-leasyx -lgdi32 -lole32 -loleaut32 -luuid -lwinmm -lmsimg32

REM Set output executable
set OUT_EXE=main.exe

echo Compiling...
%CXX% %CXXFLAGS% %INCLUDES% %SRC_FILES% %LIBS% -o %OUT_EXE%

if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Build completed successfully! Executable: %OUT_EXE%
) else (
    echo.
    echo [ERROR] Build failed! Please check the compilation errors.
)
endlocal
