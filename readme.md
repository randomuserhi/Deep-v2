A refactor of the original **Deep** library.

The design aims to be much simpler and moves away from Jolt's design philosophy.

Notably:
- SIMD vectorization of math types like vectors are handled by the developer, not the library itself.
  - This is better as SIMD has best performance over large amounts of data compared to single operaitons.
  - The library does provide SIMD optimized types if you wish to perform SIMD on individual operations, like matrix ones.
    - But this is not the default.

- Does not offer its own threading solution, this should be done by the library users. (Might change in the future)

- The goal of the library is to provide a Cpp framework that handles fundemental operations to provide most flexibility to developers.
  - E.g Basic collision detection is provided, but resolution has to be done by the developers

- The library itself is Cpp only (C wrapper can be made separately)