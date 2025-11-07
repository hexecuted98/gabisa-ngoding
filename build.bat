@echo off
setlocal ENABLEDELAYEDEXPANSION
title Gabisa-Ngoding Windows Installer
chcp 65001 >nul

echo.
echo ===============================================
echo 🧩 Gabisa-Ngoding CLI Installer (Windows)
echo ===============================================

:: -------------------------------------------------
:: 🔍 Deteksi Compiler
:: -------------------------------------------------
set "COMPILERS="
set "COMPILER_COUNT=0"

where cl.exe >nul 2>nul
if %ERRORLEVEL%==0 (
    set /a COMPILER_COUNT+=1
    set "COMPILERS=!COMPILERS!MSVC "
)

where clang++.exe >nul 2>nul
if %ERRORLEVEL%==0 (
    set /a COMPILER_COUNT+=1
    set "COMPILERS=!COMPILERS!Clang "
)

where g++.exe >nul 2>nul
if %ERRORLEVEL%==0 (
    set /a COMPILER_COUNT+=1
    set "COMPILERS=!COMPILERS!MinGW "
)

if %COMPILER_COUNT%==0 (
    echo ⚠️  Tidak ada compiler C++ yang terdeteksi.
    echo Silakan install salah satu dari:
    echo  - Visual Studio dengan C++ build tools
    echo  - LLVM / Clang
    echo  - MinGW-w64
    pause
    exit /b
)

echo.
echo 🔍 Compiler yang terdeteksi:
set /a INDEX=0
for %%C in (%COMPILERS%) do (
    set /a INDEX+=1
    echo   !INDEX!. %%C
)

set "COMPILER_CHOICE="
if %COMPILER_COUNT% GTR 1 (
    echo.
    set /p COMPILER_CHOICE=⚙️  Pilih compiler [1-%COMPILER_COUNT%]: 
) else (
    set COMPILER_CHOICE=1
)

set /a INDEX=0
for %%C in (%COMPILERS%) do (
    set /a INDEX+=1
    if "!INDEX!"=="%COMPILER_CHOICE%" (
        set "CHOSEN_COMPILER=%%C"
    )
)

if "%CHOSEN_COMPILER%"=="" (
    echo ❌ Pilihan tidak valid.
    exit /b
)

echo.
echo ✅ Menggunakan compiler: %CHOSEN_COMPILER%

:: -------------------------------------------------
:: 🔧 Deteksi CMake
:: -------------------------------------------------
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ❌ CMake tidak ditemukan.
    echo Silakan install CMake dan tambahkan ke PATH.
    pause
    exit /b
)

:: -------------------------------------------------
:: 🧱 Menu Instalasi
:: -------------------------------------------------
echo.
echo ---------- GABISA-NGODING INSTALLER ----------
echo 1. Install cepat (static release)
echo 2. Install shared library
echo 3. Install mode debug
echo 0. Keluar
echo ----------------------------------------------
set /p OPT=🧩 Pilih opsi: 

if "%OPT%"=="0" (
    echo Keluar dari installer.
    exit /b
)

:: -------------------------------------------------
:: 🏗️ Proses Build
:: -------------------------------------------------
if not exist build (
    mkdir build
)
cd build

if "%CHOSEN_COMPILER%"=="MSVC" (
    set "CMAKE_GEN=Visual Studio 17 2022"
) else if "%CHOSEN_COMPILER%"=="Clang" (
    set "CMAKE_GEN=Ninja"
    set "CMAKE_CXX_COMPILER=clang++"
) else if "%CHOSEN_COMPILER%"=="MinGW" (
    set "CMAKE_GEN=MinGW Makefiles"
)

set "CMAKE_BUILD_TYPE=Release"
set "DEBUG_FLAG=0"
set "SHARED_FLAG=OFF"

if "%OPT%"=="2" (
    set "SHARED_FLAG=ON"
)
if "%OPT%"=="3" (
    set "DEBUG_FLAG=1"
    set "CMAKE_BUILD_TYPE=Debug"
)

echo.
echo ⚙️  Menjalankan konfigurasi CMake...
cmake -G "%CMAKE_GEN%" -DCMAKE_CXX_COMPILER=%CMAKE_CXX_COMPILER% ^
      -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
      -DBUILD_SHARED_LIBS=%SHARED_FLAG% ^
      -DDEBUG_AKTIF=%DEBUG_FLAG% ..

if %ERRORLEVEL% neq 0 (
    echo ❌ Gagal mengkonfigurasi proyek.
    pause
    exit /b
)

echo.
echo 🔨 Membangun proyek...
cmake --build . --config %CMAKE_BUILD_TYPE%

if %ERRORLEVEL% neq 0 (
    echo ❌ Gagal membangun proyek.
    pause
    exit /b
)

echo.
echo 📦 Menginstall (lokal ke C:\Program Files\Gabisa-Ngoding)...
cmake --install . --prefix "C:\Program Files\Gabisa-Ngoding"

echo.
echo ✅ Instalasi selesai!
echo Binary dan library terpasang di:
echo   C:\Program Files\Gabisa-Ngoding\bin
echo   C:\Program Files\Gabisa-Ngoding\lib

pause
exit /b
