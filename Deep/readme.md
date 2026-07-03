# Developer Command Prompt for Visual Studio

```
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

# Naming convention shizzle

- Prefer `{}` initializers (e.g `A a{ ... }`)
  - Just be careful with initializer lists `std::vector v{ ... }` vs `std::vector v(...)`

- `g_camelCase` for globals
- `m_camelCase` for members
- `m_CamelCase` for getter/setter functions
- `k_camelCase` for constants
- `e_camelCase` for enum values
- `s_camelCase` for static members
- `CamelCase` for functions
- `s_CamelCase` for static member functions
- `CamelCase` for types
- `Constexpr_CamelCase` for functions that overload another function to provide a `constexpr` version of it
- `sConstexpr_CamelCase` for static function overloads that provide a `constexpr` version of it
- `Constexpr` as a member function to provide a `constexpr` constructor overload
- `in_camelCase` for in arguments
- `out_camelCase` for out arguments
- `io_camelCase` for in-out arguments

- `Arg_CamelCase` for argument types
  - These types provide the fastest way to pass an object as a const parameter.
  - This typically follows the rule, if the type is < 32 bytes pass by value (`const T`) otherwise pass by reference (`const T&`).

- `// TODO(<name>): ...` todo comments should be marked with author

Avoid class / struct level dll export:
```cpp
class DEEP_EXPORT A {}
struct DEEP_EXPORT A {}
```
Prefer exporting each function / static member individually.
This is to prevent issues with inline methods and shared libraries when compilers choose to not inline them and the dll does not emit a definition.

# Notes

The library provides `Arg_` types which give you the fastest way to pass a type as a function argument.
This typically follows the rule, if the type is < 32 bytes pass by value (`const T`) otherwise pass by reference (`const T&`).

Optimized for clang version >= 20.x.x
