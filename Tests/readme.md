```
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

```
scripts\all.bat
```

Use a separate build directory with `-DDEEP_NO_SIMD_INTRINSICS=ON` to check the scalar backend.
ARM64 builds enable Neon automatically; ARM32 builds enable Neon with `-mfpu=neon`.
For cross compilation, provide a CMake toolchain file and set `CMAKE_CROSSCOMPILING_EMULATOR`
to run the same tests on an emulator. Wasm tests use Node when it is available.

Possible TODO:
Use `-DCMAKE_CXX_FLAGS="-Rpass=inline"` flag with `Release` build and examine output to check certain methods inline properly
