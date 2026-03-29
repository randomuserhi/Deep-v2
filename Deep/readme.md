# Developer Command Prompt for Visual Studio

```
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

# Naming convention shizzle

`m_camelCase` for members
`k_camelCase` for constants
`e_camelCase` for enum values
`s_camelCase` for static
`CamelCase` for functions
`CamelCase` for types
`Arg_CamelCase` for types specific to `const in_` arguments
`Constexpr_CamelCase` for functions that overload another function to provide a `constexpr` version of it
`Constexpr` as a member function to provide a `constexpr` constructor overload
`in_camelCase` for in arguments
`out_camelCase` for out arguments
`io_camelCase` for in-out arguments

# Notes

The library provides `Arg_` types which give you the fastest way to pass a type as a function argument.
This typically follows the rule, if the type is < 32 bytes pass by value (`const T`) otherwise pass by reference (`const T&`).