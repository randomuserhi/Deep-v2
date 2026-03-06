#pragma once

#include "Deep.h"

#if defined(DEEP_USE_SSE)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <immintrin.h>
DEEP_SUPPRESS_WARNINGS_STD_END
#define DEEP_VEC_ALIGNMENT Deep_AlignOf(__m128)
#else
#define DEEP_VEC_ALIGNMENT Deep_AlignOf(uint32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct Xmm;
using XmmArg = Xmm;

struct Xmmi;
using XmmiArg = Xmmi;

// Abstraction layer for __m128i SIMD type representing 4 independent values
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Xmmi {
#if defined(DEEP_USE_SSE)
    using Type = __m128i;
#else
    using Type = struct {
        uint32 values[4];
    };
#endif

    //

    Xmmi() = default;
    Xmmi(const Xmmi&) = default;
    Xmmi& operator=(const Xmmi&) = default;
    Deep_Inline Xmmi(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
    Deep_Inline Xmmi(Type in_internal);

    //

    // Implicit conversion
    Deep_Inline operator Type() const;

    //

    // Convert into __m128, converting each component to a float
    Deep_Inline Xmm ToFloat() const;

    // Reinterpret bytes as a __m128
    Deep_Inline Xmm ReinterpretAsFloat() const;

    // Treats the 4 components as booleans where true is when the most significant bit is set
    // and returns it as a bit mask.
    //
    // Bit 0 is set if X is true, Bit 1 is set if Y is true, Bit 2 is set if Z is true and Bit 3 is set if W is true.
    Deep_Inline int32 ToBooleanBitMask() const;

    // Replicate the given value across all components
    static Deep_Inline Xmmi Replicate(int in_value);

    // Component wise logical AND
    static Deep_Inline Xmmi And(const XmmiArg in_a, const XmmiArg in_b);

    // Component wise logical XOR
    static Deep_Inline Xmmi Xor(const XmmiArg in_a, const XmmiArg in_b);

    // Returns a 4 component integer value representing the equality of each component
    // in `a` or `b`.
    //
    // True is represented by the most significant bit being set.
    static Deep_Inline Xmmi Equals(const XmmiArg in_a, const XmmiArg in_b);

    // Shift all components by `Count` bits to the left
    template<const uint32 Count>
    Deep_Inline Xmmi LogicalShiftLeft() const;

    // Shift all components by `Count` bits to the right
    template<const uint32 Count>
    Deep_Inline Xmmi LogicalShiftRight() const;

    // Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
    static Deep_Inline Xmmi Select(const XmmiArg in_a, const XmmiArg in_b, const XmmiArg in_control);

    //

    // Equality
    friend bool operator!=(const XmmiArg in_a, const XmmiArg in_b);
    friend bool operator==(const XmmiArg in_a, const XmmiArg in_b);

    // Component wise Add
    Deep_Inline Xmmi& operator+=(const XmmiArg in_other);
    friend Deep_Inline Xmmi operator+(Xmmi in_a, const XmmiArg in_b);

    // Component wise Sub
    Deep_Inline Xmmi& operator-=(const XmmiArg in_other);
    friend Deep_Inline Xmmi operator-(Xmmi in_a, const XmmiArg in_b);
    friend Deep_Inline Xmmi operator-(const XmmiArg in_a);

    // Component wise Mul
    Deep_Inline Xmmi& operator*=(const XmmiArg in_other);
    friend Deep_Inline Xmmi operator*(Xmmi in_a, const XmmiArg in_b);

    // Component wise Div
    Deep_Inline Xmmi& operator/=(const XmmiArg in_other);
    friend Deep_Inline Xmmi operator/(Xmmi in_a, const XmmiArg in_b);

    // Mul components with float
    Deep_Inline Xmmi& operator*=(int32 in_other);
    friend Deep_Inline Xmmi operator*(Xmmi in_vec, int32 in_val);
    friend Deep_Inline Xmmi operator*(int32 in_val, const XmmiArg in_vec);

    // Div components with float
    Deep_Inline Xmmi& operator/=(int32 in_other);
    friend Deep_Inline Xmmi operator/(Xmmi in_vec, int32 in_val);
    friend Deep_Inline Xmmi operator/(int32 in_val, const XmmiArg in_vec);

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

static_assert(std::is_trivial<Xmmi>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Xmmi>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Xmmi.inl" // IWYU pragma: export

#undef DEEP_VEC_ALIGNMENT
