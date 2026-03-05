# Developer Command Prompt for Visual Studio

```
cmake -S . -B out -G Ninja -DCMAKE_CXX_COMPILER=clang -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build out
```