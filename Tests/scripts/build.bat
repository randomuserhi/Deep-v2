@echo off
setlocal enabledelayedexpansion

cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
if !ERRORLEVEL! NEQ 0 (
    echo.
    echo ^^! -- CMake configuration failed
) else (
    echo.
    cmake --build build -j 16
    if !ERRORLEVEL! NEQ 0 (
        echo.
        echo ^^! -- Build failed
    ) else (
        echo.
        .\build\bin\Tests.exe       
    )
)

endlocal