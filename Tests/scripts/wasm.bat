@echo off
setlocal enabledelayedexpansion

cmake -S . -B build-wasm -DCMAKE_TOOLCHAIN_FILE="C:/wasi-sdk/wasi-sdk-34.0-rc.2-x86_64-windows/share/cmake/wasi-sdk-p1.cmake" -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DDEEP_NO_SIMD_INTRINSICS=OFF
if !ERRORLEVEL! NEQ 0 (
    echo.
    echo ^^! -- CMake configuration failed
) else (
    echo.
    cmake --build build-wasm -j 16
    if !ERRORLEVEL! NEQ 0 (
        echo.
        echo ^^! -- Build failed
    ) else (
        echo.
        node .\build-wasm\bin\test.js       
    )
)

endlocal