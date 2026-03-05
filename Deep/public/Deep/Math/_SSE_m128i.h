#pragma once

#include "_MathTypes.h"
#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for __m128i SIMD type representing 4 independent values
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) SSE_m128i {
#if defined(DEEP_USE_SSE)
    using Type = __m128i;
#else
    using Type = struct {
        uint32 values[4];
    };
#endif

    //

    SSE_m128i() = default;
    SSE_m128i(const SSE_m128i&) = default;
    SSE_m128i& operator=(const SSE_m128i&) = default;
    Deep_Inline SSE_m128i(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
    Deep_Inline SSE_m128i(Type in_internal);

    //

    // Implicit conversion
    Deep_Inline operator Type() const;

    //

    // Convert into __m128, converting each component to a float
    Deep_Inline SSE_m128 ToFloat() const;

    // Reinterpret bytes as a __m128
    Deep_Inline SSE_m128 ReinterpretAsFloat() const;

    // Treats the 4 components as booleans where true is when the most significant bit is set
    // and returns it as a bit mask.
    //
    // Bit 0 is set if X is true, Bit 1 is set if Y is true, Bit 2 is set if Z is true and Bit 3 is set if W is true.
    Deep_Inline int32 ToBooleanBitMask() const;

    // Replicate the given value across all components
    static Deep_Inline SSE_m128i Replicate(int in_value);

    // Component wise logical AND
    static Deep_Inline SSE_m128i And(SSE_m128iArg in_a, SSE_m128iArg in_b);

    // Component wise logical XOR
    static Deep_Inline SSE_m128i Xor(SSE_m128iArg in_a, SSE_m128iArg in_b);

    // Returns a 4 component integer value representing the equality of each component
    // in `a` or `b`.
    //
    // True is represented by the most significant bit being set.
    static Deep_Inline SSE_m128i Equals(SSE_m128iArg in_a, SSE_m128iArg in_b);

    // Shift all components by `Count` bits to the left
    template<const uint32 Count>
    Deep_Inline SSE_m128i LogicalShiftLeft() const;

    // Shift all components by `Count` bits to the right
    template<const uint32 Count>
    Deep_Inline SSE_m128i LogicalShiftRight() const;

    // Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
    static Deep_Inline SSE_m128i Select(SSE_m128iArg in_a, SSE_m128iArg in_b, SSE_m128iArg in_control);

    //

    // Equality
    friend bool operator!=(SSE_m128iArg in_a, SSE_m128iArg in_b);
    friend bool operator==(SSE_m128iArg in_a, SSE_m128iArg in_b);

    // Component wise Add
    Deep_Inline SSE_m128i& operator+=(SSE_m128iArg in_other);
    friend Deep_Inline SSE_m128i operator+(SSE_m128i in_a, SSE_m128iArg in_b);

    // Component wise Sub
    Deep_Inline SSE_m128i& operator-=(SSE_m128iArg in_other);
    friend Deep_Inline SSE_m128i operator-(SSE_m128i in_a, SSE_m128iArg in_b);
    friend Deep_Inline SSE_m128i operator-(SSE_m128iArg in_a);

    // Component wise Mul
    Deep_Inline SSE_m128i& operator*=(SSE_m128iArg in_other);
    friend Deep_Inline SSE_m128i operator*(SSE_m128i in_a, SSE_m128iArg in_b);

    // Component wise Div
    Deep_Inline SSE_m128i& operator/=(SSE_m128iArg in_other);
    friend Deep_Inline SSE_m128i operator/(SSE_m128i in_a, SSE_m128iArg in_b);

    // Mul components with float
    Deep_Inline SSE_m128i& operator*=(int32 in_other);
    friend Deep_Inline SSE_m128i operator*(SSE_m128i in_vec, int32 in_val);
    friend Deep_Inline SSE_m128i operator*(int32 in_val, SSE_m128iArg in_vec);

    // Div components with float
    Deep_Inline SSE_m128i& operator/=(int32 in_other);
    friend Deep_Inline SSE_m128i operator/(SSE_m128i in_vec, int32 in_val);
    friend Deep_Inline SSE_m128i operator/(int32 in_val, SSE_m128iArg in_vec);

    //

    union {
        Type _internal;
        int32 values[4];
        struct {
            int32 x;
            int32 y;
            int32 z;
            int32 w;
        };
    };
};

static_assert(std::is_trivial<SSE_m128i>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<SSE_m128i>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END