#pragma once

#include "_MathTypes.h"
#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Vec3i {
    //

    Vec3i() = default;
    Vec3i(const Vec3i&) = default;
    Vec3i& operator=(const Vec3i&) = default;
    Deep_Inline Vec3i(int32 in_x, int32 in_y, int32 in_z);
    explicit Deep_Inline Vec3i(SSE_m128i in_sse_m128i);
    explicit Deep_Inline Vec3i(Vec4iArg in_vec);

    //

    explicit Deep_Inline operator Vec3() const;

    //

    [[nodiscard]] Deep_Inline int32 manhattanDistance() const;

    [[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
    [[nodiscard]] Deep_Inline float32 magnitude() const;

    [[nodiscard]] static Deep_Inline int32 Dot(Vec3iArg in_a, Vec3iArg in_b);

    //

    // Equality
    friend bool operator!=(Vec3iArg in_a, Vec3iArg in_b);
    friend bool operator==(Vec3iArg in_a, Vec3iArg in_b);

    // Add vectors
    Deep_Inline Vec3i& operator+=(Vec3iArg in_other);
    friend Deep_Inline Vec3i operator+(Vec3iArg in_a, Vec3iArg in_b);

    // Sub vectors
    Deep_Inline Vec3i& operator-=(Vec3iArg in_other);
    friend Deep_Inline Vec3i operator-(Vec3iArg in_a, Vec3iArg in_b);
    friend Deep_Inline Vec3i operator-(Vec3iArg in_a);

    // Mul vectors
    Deep_Inline Vec3i& operator*=(Vec3iArg in_other);
    friend Deep_Inline Vec3i operator*(Vec3iArg in_a, Vec3iArg in_b);

    // Div vectors
    Deep_Inline Vec3i& operator/=(Vec3iArg in_other);
    friend Deep_Inline Vec3i operator/(Vec3iArg in_a, Vec3iArg in_b);

    // Mul vector and float
    Deep_Inline Vec3i& operator*=(int32 in_other);
    friend Deep_Inline Vec3i operator*(Vec3iArg in_vec, int32 in_val);
    friend Deep_Inline Vec3i operator*(int32 in_val, Vec3iArg in_vec);

    // Div vector and float
    Deep_Inline Vec3i& operator/=(int32 other);
    friend Deep_Inline Vec3i operator/(Vec3iArg in_vec, int32 in_val);
    friend Deep_Inline Vec3i operator/(int32 in_val, Vec3iArg in_vec);

    //

    // NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
    union {
        SSE_m128 sse_m128;
        SSE_m128i sse_m128i;
        int32 m_values[4];
        struct {
            int32 x;
            int32 y;
            int32 z;
            // int32 w;
        };
    };

    static const Vec3i k_zero;

    static const Vec3i k_up;
    static const Vec3i k_down;
    static const Vec3i k_left;
    static const Vec3i k_right;
    static const Vec3i k_forwards;
    static const Vec3i k_backwards;
};

static_assert(std::is_trivial<Vec3i>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec3i>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END