#pragma once

#include "_MathTypes.h"
#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Vec4i {
    //

    Vec4i() = default;
    Vec4i(const Vec4i&) = default;
    Vec4i& operator=(const Vec4i&) = default;
    Deep_Inline Vec4i(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
    explicit Deep_Inline Vec4i(SSE_m128i in_sse_m128i);
    explicit Deep_Inline Vec4i(Vec3iArg in_xyz, int32 in_w);

    //

    explicit Deep_Inline operator Vec4() const;

    //

    [[nodiscard]] Deep_Inline int32 manhattanDistance() const;

    [[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
    [[nodiscard]] Deep_Inline float32 magnitude() const;

    [[nodiscard]] static Deep_Inline int32 Dot(Vec4iArg in_a, Vec4iArg in_b);

    //

    // Equality
    friend bool operator!=(Vec4iArg in_a, Vec4iArg in_b);
    friend bool operator==(Vec4iArg in_a, Vec4iArg in_b);

    // Add vectors
    Deep_Inline Vec4i& operator+=(Vec4iArg in_other);
    friend Deep_Inline Vec4i operator+(Vec4iArg in_a, Vec4iArg in_b);

    // Sub vectors
    Deep_Inline Vec4i& operator-=(Vec4iArg in_other);
    friend Deep_Inline Vec4i operator-(Vec4iArg in_a, Vec4iArg in_b);
    friend Deep_Inline Vec4i operator-(Vec4iArg in_a);

    // Mul vectors
    Deep_Inline Vec4i& operator*=(Vec4iArg in_other);
    friend Deep_Inline Vec4i operator*(Vec4iArg in_a, Vec4iArg in_b);

    // Div vectors
    Deep_Inline Vec4i& operator/=(Vec4iArg in_other);
    friend Deep_Inline Vec4i operator/(Vec4iArg in_a, Vec4iArg in_b);

    // Mul vector and int
    Deep_Inline Vec4i& operator*=(int32 in_other);
    friend Deep_Inline Vec4i operator*(Vec4iArg in_vec, int32 in_val);
    friend Deep_Inline Vec4i operator*(int32 in_val, Vec4iArg in_vec);

    // Div vector and int
    Deep_Inline Vec4i& operator/=(int32 in_other);
    friend Deep_Inline Vec4i operator/(Vec4iArg in_vec, int32 in_val);
    friend Deep_Inline Vec4i operator/(int32 in_val, Vec4iArg in_vec);

    //

    union {
        SSE_m128 sse_m128;
        SSE_m128i sse_m128i;
        int32 m_values[4];
        struct {
            int32 x;
            int32 y;
            int32 z;
            int32 w;
        };
    };

    static const Vec4i k_zero;
};

static_assert(std::is_trivial<Vec4i>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec4i>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END