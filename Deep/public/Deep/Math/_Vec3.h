#pragma once

#include "_MathTypes.h"
#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Vec3 {
    //

    Vec3() = default;
    Vec3(const Vec3&) = default;
    Vec3& operator=(const Vec3&) = default;
    Deep_Inline Vec3(float32 in_x, float32 in_y, float32 in_z);
    explicit Deep_Inline Vec3(SSE_m128 in_sse_m128);
    explicit Deep_Inline Vec3(Vec4Arg in_vec);

    //

    explicit Deep_Inline operator Vec3i() const;

    //

    Deep_Inline Vec3& Normalize();
    [[nodiscard]] Deep_Inline Vec3 normalized() const;
    [[nodiscard]] Deep_Inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

    [[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
    [[nodiscard]] Deep_Inline float32 magnitude() const;

    [[nodiscard]] static Deep_Inline float32 Dot(Vec3Arg in_a, Vec3Arg in_b);
    [[nodiscard]] static Deep_Inline Vec3 Cross(Vec3Arg in_a, Vec3Arg in_b);

    //

    // Equality
    friend bool operator!=(Vec3Arg in_a, Vec3Arg in_b);
    friend bool operator==(Vec3Arg in_a, Vec3Arg in_b);

    // Add vectors
    Deep_Inline Vec3& operator+=(Vec3Arg in_other);
    friend Deep_Inline Vec3 operator+(Vec3Arg in_a, Vec3Arg in_b);

    // Sub vectors
    Deep_Inline Vec3& operator-=(Vec3Arg in_other);
    friend Deep_Inline Vec3 operator-(Vec3Arg in_a, Vec3Arg in_b);
    friend Deep_Inline Vec3 operator-(Vec3Arg in_a);

    // Mul vectors
    Deep_Inline Vec3& operator*=(Vec3Arg in_other);
    friend Deep_Inline Vec3 operator*(Vec3Arg in_a, Vec3Arg in_b);

    // Div vectors
    Deep_Inline Vec3& operator/=(Vec3Arg in_other);
    friend Deep_Inline Vec3 operator/(Vec3Arg in_a, Vec3Arg in_b);

    // Mul vector and float
    Deep_Inline Vec3& operator*=(float32 in_other);
    friend Deep_Inline Vec3 operator*(Vec3Arg in_vec, float32 in_val);
    friend Deep_Inline Vec3 operator*(float32 in_val, Vec3Arg in_vec);

    // Div vector and float
    Deep_Inline Vec3& operator/=(float32 other);
    friend Deep_Inline Vec3 operator/(Vec3Arg in_vec, float32 in_val);
    friend Deep_Inline Vec3 operator/(float32 in_val, Vec3Arg in_vec);

    // Multiply a Matrix4x4 and Vector
    friend Deep_Inline Vec3 operator*(Mat4Arg in_mat, Vec3Arg in_vec); // NOTE(randomuserhi): Assumes Vec4 with w = 1

    // Apply a quaternion to a vector
    friend Deep_Inline Vec3 operator*(QuatArg in_quat, Vec3Arg in_vec);

    //

    // NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
    union {
        SSE_m128 sse_m128;
        SSE_m128i sse_m128i;
        float32 m_values[4];
        struct {
            float32 x;
            float32 y;
            float32 z;
            float32 _w;
        };
    };

    static const Vec3 k_zero;
    static const Vec3 k_one;

    static const Vec3 k_up;
    static const Vec3 k_down;
    static const Vec3 k_left;
    static const Vec3 k_right;
    static const Vec3 k_forwards;
    static const Vec3 k_backwards;
};

static_assert(std::is_trivial<Vec3>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec3>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END