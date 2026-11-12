@echo off
setlocal
cd /d "%~dp0"

echo === Building Aseprite ===
call "%~dp0build_release.cmd"
if errorlevel 1 goto :error

echo === Launching Aseprite ===
start "" "build\bin\aseprite.exe" %*
exit /b 0

:error
echo.
echo Build failed.
pause
exit /b 1
