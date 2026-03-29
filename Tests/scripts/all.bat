@echo off
setlocal enabledelayedexpansion

set "COMMON_ARGS=-G Ninja -DCMAKE_CXX_COMPILER=clang++ -DUSE_ASSERTS=ON"

set "Build[1]_Name="
set "Build[1]_Path="
set "Build[1]_Args=!COMMON_ARGS!"

set "Build[2]_Name= NO_SIMD_INTRINSICS"
set "Build[2]_Path=-no-simd"
set "Build[2]_Args=!COMMON_ARGS! -DNO_SIMD_INTRINSICS=ON"

set "BUILD_COUNT=2"

set "Build_Type[1]=DEBUG"
set "Build_Type_Value[1]=Debug"
set "Build_Type_Name[1]=debug"
set "Build_Type[2]=RELEASE"
set "Build_Type_Value[2]=Release"
set "Build_Type_Name[2]=release"
set "Build_Type[3]=REL_WITH_DEB_INFO"
set "Build_Type_Value[3]=RelWithDebInfo"
set "Build_Type_Name[3]=relwithdebinfo"

set "BUILD_TYPE_COUNT=3"

set "Lib_Type[1]=STATIC"
set "Lib_Type_Args[1]=-DUSE_DYNAMIC_LINKING=OFF"
set "Lib_Type_Name[1]=static"
set "Lib_Type[2]=STATIC-LTO"
set "Lib_Type_Args[2]=-DUSE_DYNAMIC_LINKING=OFF -DINTERPROCEDURAL_OPTIMIZATION=ON -DCMAKE_LINKER_TYPE=LLD"
set "Lib_Type_Name[2]=static-lto"
set "Lib_Type[3]=SHARED"
set "Lib_Type_Args[3]=-DUSE_DYNAMIC_LINKING=ON"
set "Lib_Type_Name[3]=shared"

set "LIB_TYPE_COUNT=3"

set FAILS=

if not exist ".\all" mkdir ".\all"

for /L %%j in (1,1,%LIB_TYPE_COUNT%) do (
    for /L %%k in (1,1,%BUILD_TYPE_COUNT%) do (
        set "SKIP=0"
        if %%j==2 if %%k==1 set "SKIP=1"

        if "!SKIP!"=="0" (
            for /L %%i in (1,1,%BUILD_COUNT%) do (
                set "CURRENT_NAME=!Lib_Type[%%j]! !Build_Type[%%k]!!Build[%%i]_Name!"
                set "CURRENT_PATH=.\all\!Lib_Type_Name[%%j]!\!Build_Type_Name[%%k]!!Build[%%i]_Path!"
                set "CURRENT_ARGS=!Build[%%i]_Args! -DCMAKE_BUILD_TYPE=!Build_Type_Value[%%k]! !Lib_Type_Args[%%j]!"
                set "CURRENT_LOG_PATH=!CURRENT_PATH!\logs"
                set "EXE_PATH=!CURRENT_PATH!\bin\Tests.exe"
                
                echo.
                echo [!CURRENT_NAME!]

                if not exist !CURRENT_LOG_PATH! mkdir !CURRENT_LOG_PATH!

                cmake -S . -B "!CURRENT_PATH!" !CURRENT_ARGS! > "!CURRENT_LOG_PATH!\cmake_out.txt" 2>&1
                if !ERRORLEVEL! NEQ 0 (
                    echo ^^! -- CMake configuration failed for !CURRENT_PATH!
                    set "FAILS=!FAILS! "!CURRENT_NAME!""
                ) else (
                    cmake --build "!CURRENT_PATH!"  > "!CURRENT_LOG_PATH!\cmake_build.txt" 2>&1
                    
                    if !ERRORLEVEL! NEQ 0 (
                        echo ^^! -- Build failed for !CURRENT_PATH!
                        set "FAILS=!FAILS! "!CURRENT_NAME!""
                    ) else (
                        if exist "!EXE_PATH!" (
                            "!EXE_PATH!" > "!CURRENT_LOG_PATH!\test_out.txt" 2>&1
                            
                            if !ERRORLEVEL! NEQ 0 (
                                echo ^^! -- TEST FAILED
                                set "FAILS=!FAILS! "!CURRENT_NAME!""
                            ) else (
                                echo OK -- TEST PASSED
                            )
                        ) else (
                            echo ^^! -- ERROR: Executable not found.
                            set "FAILS=!FAILS! "!CURRENT_NAME!""
                        )
                    )
                )
            )
        )
    )
)

echo.
if not defined FAILS (
    echo All tests ran successfully.
) else (
    echo.
    echo Tests failed:
    for %%F in (%FAILS%) do (
        echo  - %%~F
    )
    exit /b 1
)

endlocal