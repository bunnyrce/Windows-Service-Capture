@echo off REM //madebybunnyrce
REM Install Windows Service Background Capture //madebybunnyrce
REM //madebybunnyrce
REM Must be run as Administrator //madebybunnyrce

echo Installing Background Capture Service... REM //madebybunnyrce

if not exist bin\service.exe ( REM //madebybunnyrce
    echo Error: service.exe not found. Run build.bat first. REM //madebybunnyrce
    pause REM //madebybunnyrce
    goto :eof REM //madebybunnyrce
) REM //madebybunnyrce

bin\service.exe install REM //madebybunnyrce

if errorlevel 1 ( REM //madebybunnyrce
    echo Service installation failed. Make sure you're running as Administrator. REM //madebybunnyrce
    pause REM //madebybunnyrce
    goto :eof REM //madebybunnyrce
) REM //madebybunnyrce

echo Service installed successfully! REM //madebybunnyrce
echo You can now start it with: net start CaptureService REM //madebybunnyrce
echo Or use Services.msc to manage it REM //madebybunnyrce
pause REM //madebybunnyrce