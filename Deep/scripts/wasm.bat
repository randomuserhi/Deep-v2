@echo off
setlocal enabledelayedexpansion

cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="E:/wasi-sdk/wasi-sdk-34.0-rc.2-x86_64-windows/share/cmake/wasi-sdk-p1.cmake" -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
if !ERRORLEVEL! NEQ 0 (
    echo.
    echo ^^! -- CMake configuration failed
) else (
    echo.
    cmake --build build -j 16
    if !ERRORLEVEL! NEQ 0 (
        echo.
        echo ^^! -- Build failed
    )
)

endlocal