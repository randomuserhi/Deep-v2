```
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

```
scripts\all.bat
```

Possible TODO:
Use `-DCMAKE_CXX_FLAGS="-Rpass=inline"` flag with `Release` build and examine output to check certain methods inline properly