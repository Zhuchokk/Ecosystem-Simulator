@echo off


rem Building a project
if not exist "build" (
    mkdir build
    echo "Directory 'build' is created"
)

cd build
@REM call vcvars64
cmake -G "Visual Studio 17 2022" -A x64 ..

@REM cmake --build . --config Release
@REM
@REM rem Run project
@REM cd ..
@REM
@REM set RES=".\\libs\\SFML-3.0.0\\bin"
@REM
@REM copy "%RES%\sfml-graphics-3.dll" ".\build\Release"
@REM copy "%RES%\sfml-window-3.dll" ".\build\Release"
@REM copy "%RES%\sfml-system-3.dll" ".\build\Release"
@REM start build/Debug/SFML_Project.exe


cmake --build . --config Debug

rem Run project
cd ..

@REM set RES=".\\libs\\SFML-3.0.0\\bin"
@REM
@REM copy "%RES%\sfml-graphics-d-3.dll" ".\build\Debug"
@REM copy "%RES%\sfml-window-d-3.dll" ".\build\Debug"
@REM copy "%RES%\sfml-system-d-3.dll" ".\build\Debug"
@REM

cd build/Debug
start SFML_Project.exe


