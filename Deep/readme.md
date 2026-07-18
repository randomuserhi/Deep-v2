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
- `c_CamelCase` for concepts
- `CamelCase` for namespaces
- `impl_CamelCase` for implementation specific namespaces often used to hide implementation specific templates used in meta-programming.

For templates where the template parameter acts more like a function parameter as opposed to a type declaration, use `in_CamelCase` name scheme:
```cpp
// `T` is used like a type
template<typename T>
class Vector {};

// `T` is used like a type, but `in_TypeList` is used like a function parameter
template<typename T, typename... in_TypeList>
constexpr static inline size_t CountOccurences() {
	return (0 + ... + std::is_same_v<T, in_TypeList>);
}
```

- `Arg_CamelCase` for argument types
  - These types provide the fastest way to pass an object as a const parameter.
  - This typically follows the rule, if the type is < 32 bytes pass by value (`const T`) otherwise pass by reference (`const T&`).

- `// TODO(<name>): ...` todo comments should be marked with author

Avoid class / struct level dll export:
```cpp
// Don't
class ENGINE_EXPORT A { ... }
struct ENGINE_EXPORT A { ... }

// Do
class A {
  ENGINE_EXPORT void Method();
}
struct A {
  ENGINE_EXPORT void Method();
}
```
This is to prevent issues with inline methods and shared libraries when compilers choose to not inline them and the dll does not emit a definition.

# Notes

The library provides `Arg_` types which give you the fastest way to pass a type as a function argument.
This typically follows the rule, if the type is < 32 bytes pass by value (`const T`) otherwise pass by reference (`const T&`).

Optimized for clang version >= 20.x.x
