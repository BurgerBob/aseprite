@echo off
setlocal
cd /d "%~dp0"

rem Target dir is defined in export-release.local.bat (not versioned): set "DEST=C:\path\to\aseprite"
if not exist "export-release.local.bat" (
	echo Missing export-release.local.bat. Create it next to this file with this content:
	echo     set "DEST=C:\path\to\export\dir"
	pause
	exit /b 1
)
call "%~dp0export-release.local.bat"
if "%DEST%"=="" (
	echo DEST is not defined in export-release.local.bat
	pause
	exit /b 1
)

echo === Building Aseprite ===
call "%~dp0build_release.cmd"
if errorlevel 1 goto :error

if not exist "build\bin\aseprite.exe" (
	echo Built executable not found in build\bin
	goto :error
)

echo === Copying to %DEST% ===
rem Only the files needed to run (no debug symbols, no build tools)
robocopy "build\bin" "%DEST%" /MIR /XF *.pdb gen.exe /NFL /NDL /NJH /NJS /NP
if %errorlevel% geq 8 goto :error

echo.
echo Done: %DEST%\aseprite.exe
pause
exit /b 0

:error
echo.
echo Export failed.
pause
exit /b 1
