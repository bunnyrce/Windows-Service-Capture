@echo off
REM Build script for Windows Service Background Capture
REM //madebybunnyrce
REM Requires Visual Studio and Windows SDK

if not defined VSINSTALLDIR (
    echo Visual Studio environment not found. Run "Developer Command Prompt for VS" first.
    goto :eof
)

echo Building Windows Service Background Capture...

if not exist bin mkdir bin

cl /EHsc /W3 /MD "service.cpp" /Fe:bin\service.exe /link advapi32.lib gdi32.lib user32.lib
if errorlevel 1 (
    echo Service build failed
    goto :eof
)

echo Build completed successfully!
echo To install: run install.bat as Administrator
echo To test in console: bin\service.exe console
pause