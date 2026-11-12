@echo off
rem Build Release d'Aseprite (Ninja + MSVC 2022 + Skia m124 precompile)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set "PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja;%PATH%"
cd /d "%~dp0"
if not exist build\build.ninja (
  cmake -S . -B build -G Ninja ^
    -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    -DLAF_BACKEND=skia ^
    -DSKIA_DIR="%~dp0.deps\skia-m124" ^
    -DSKIA_LIBRARY_DIR="%~dp0.deps\skia-m124\out\Release-x64" ^
    -DSKIA_LIBRARY="%~dp0.deps\skia-m124\out\Release-x64\skia.lib" || exit /b 1
)
ninja -C build aseprite
