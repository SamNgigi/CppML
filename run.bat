@echo off
setlocal enabledelayedexpansion

for %%i in ("%~dp0.") do set "dir_name=%%~ni"

set "exe_path=build\Debug\%dir_name%.exe"
set "test_exe_path=build\Debug\%dir_name%_test.exe"

set "app_args="
set "action="

rem Check if no arguments were provided
if "%~1"=="" (
    set "action=run"
    goto :parse_complete
)

:parse_args
if "%~1"=="" goto :parse_complete
if "%~1"=="build" (
    set "action=build"
    shift
    goto :parse_args
) else if "%~1"=="run" (
    set "action=run"
    shift
    goto :parse_args
) else if "%~1"=="test" (
    set "action=test"
    shift
    goto :parse_args
) else if "%~1"=="clean" (
    set "action=clean"
    shift
    goto :parse_args
) else (
    if not defined action set "action=run"
    call :add_arg "%~1"
    shift
    goto :parse_args
)

:parse_complete
if not defined action set "action=run"

if "%action%"=="build" goto :build
if "%action%"=="clean" goto :clean
if "%action%"=="test" goto :test
if "%action%"=="run" goto :run_app

echo Unknown action: %action%
exit /b 1

:add_arg
setlocal disabledelayedexpansion
set "arg=%~1"
endlocal & set "app_args=%app_args% %arg:!=^!%"
exit /b 0

:clean
echo Cleaning build directory...
if exist build (
    echo Removing existing build directory...
    rmdir /s /q build
    if !errorlevel! neq 0 (
        echo Failed to remove build directory.
        exit /b !errorlevel!
    )
)
echo Creating new build directory...
mkdir build
if !errorlevel! neq 0 (
    echo Failed to create build directory.
    exit /b !errorlevel!
)
echo Clean completed successfully.
exit /b 0

:build
if not exist build mkdir build
echo Building the project...
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=C:\Users\samtn\vcpkg\scripts\buildsystems\vcpkg.cmake ..
if !errorlevel! neq 0 (
    echo CMake configuration failed.
    exit /b !errorlevel!
)
cmake --build .
if !errorlevel! neq 0 (
    echo Build failed.
    exit /b !errorlevel!
)
cd ..
echo Build completed successfully.
exit /b 0

:test
echo Performing clean build and running tests...
call :clean
if !errorlevel! neq 0 exit /b !errorlevel!
call :build
if !errorlevel! neq 0 exit /b !errorlevel!
echo Running tests...
"%test_exe_path%"
exit /b 0

:run_app
if not exist "%exe_path%" (
    echo Executable not found. Building the project...
    call :build
    if !errorlevel! neq 0 exit /b !errorlevel!
)
echo Running executable with arguments:%app_args%
setlocal disabledelayedexpansion
"%exe_path%"%app_args%
endlocal
exit /b 0
