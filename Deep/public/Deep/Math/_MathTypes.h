#pragma once

#include "Deep.h"

// Vector alignment for vectorisation
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <immintrin.h>
DEEP_SUPPRESS_WARNINGS_STD_END

#define DEEP_VEC_ALIGNMENT Deep_AlignOf(__m128)

#else

#error Unsupported CPU architecture

#endif

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math

DEEP_NAMESPACE_BEGIN

struct SSE_m128;
struct SSE_m128i;

struct Vec3;
struct Vec3i;
struct Vec4;
struct Vec4i;
struct Quat;

struct Mat4;

// Types to use for passing arguments to functions.
// These types represent the fastest way to pass a given type as a read-only argument. (E.g passing Mat4 by reference
// will be faster than by value)
//
// If you need specific access patterns do not use these types.

using SSE_m128Arg = const SSE_m128;
using SSE_m128iArg = const SSE_m128i;

using Vec3Arg = const Vec3;
using Vec3iArg = const Vec3i;
using Vec4Arg = const Vec4;
using Vec4iArg = const Vec4i;
using QuatArg = const Quat;
using Mat4Arg = const Mat4&;

DEEP_NAMESPACE_END