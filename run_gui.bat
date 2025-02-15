@echo off


rem Building a project
if not exist "build" (
    mkdir build
    echo "Directory 'build' is created"
)

cd build
@REM call vcvars64
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Debug

rem Run project
cd ..

set RES=".\\libs\\SFML-3.0.0\\bin"
copy "%RES%\sfml-graphics-3.dll" ".\build\Debug"
copy "%RES%\sfml-window-3.dll" ".\build\Debug"
copy "%RES%\sfml-system-3.dll" ".\build\Debug"
start build/Debug/SFML_Project.exe
