@echo off REM //madebybunnyrce
REM Uninstall Windows Service Background Capture //madebybunnyrce
REM //madebybunnyrce
REM Must be run as Administrator //madebybunnyrce

echo Uninstalling Background Capture Service... REM //madebybunnyrce

if not exist bin\service.exe ( REM //madebybunnyrce
    echo Error: service.exe not found. REM //madebybunnyrce
    pause REM //madebybunnyrce
    goto :eof REM //madebybunnyrce
) REM //madebybunnyrce

echo Stopping service if running... REM //madebybunnyrce
net stop CaptureService 2>nul REM //madebybunnyrce

bin\service.exe uninstall REM //madebybunnyrce

if errorlevel 1 ( REM //madebybunnyrce
    echo Service uninstallation failed. Make sure you're running as Administrator. REM //madebybunnyrce
    pause REM //madebybunnyrce
    goto :eof REM //madebybunnyrce
) REM //madebybunnyrce

echo Service uninstalled successfully! REM //madebybunnyrce
pause REM //madebybunnyrce