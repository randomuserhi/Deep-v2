#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Math/Xmm.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmm) Vec4 {
    //

    Vec4() = default;
    Vec4(const Vec4&) = default;
    Vec4& operator=(const Vec4&) = default;
    Deep_Inline Vec4(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
    explicit Deep_Inline Vec4(Xmm in_xmm);
    explicit Deep_Inline Vec4(Vec3Arg in_xyz, float32 in_w);

    //

    explicit Deep_Inline operator Vec4i() const;

    //

    Deep_Inline Vec4& Normalize();
    [[nodiscard]] Deep_Inline Vec4 normalized() const;
    [[nodiscard]] Deep_Inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

    [[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
    [[nodiscard]] Deep_Inline float32 magnitude() const;

    [[nodiscard]] static Deep_Inline float32 Dot(Vec4Arg in_a, Vec4Arg in_b);

    //

    // Equality
    friend bool operator!=(Vec4Arg in_a, Vec4Arg in_b);
    friend bool operator==(Vec4Arg in_a, Vec4Arg in_b);

    // Add vectors
    Deep_Inline Vec4& operator+=(Vec4Arg in_other);
    friend Deep_Inline Vec4 operator+(Vec4Arg in_a, Vec4Arg in_b);

    // Sub vectors
    Deep_Inline Vec4& operator-=(Vec4Arg in_other);
    friend Deep_Inline Vec4 operator-(Vec4Arg in_a, Vec4Arg in_b);
    friend Deep_Inline Vec4 operator-(Vec4Arg in_a);

    // Mul vectors
    Deep_Inline Vec4& operator*=(Vec4Arg in_other);
    friend Deep_Inline Vec4 operator*(Vec4Arg in_a, Vec4Arg in_b);

    // Div vectors
    Deep_Inline Vec4& operator/=(Vec4Arg in_other);
    friend Deep_Inline Vec4 operator/(Vec4Arg in_a, Vec4Arg in_b);

    // Mul vector and float
    Deep_Inline Vec4& operator*=(float32 in_other);
    friend Deep_Inline Vec4 operator*(Vec4Arg in_vec, float32 in_val);
    friend Deep_Inline Vec4 operator*(float32 in_val, Vec4Arg in_vec);

    // Div vector and float
    Deep_Inline Vec4& operator/=(float32 in_other);
    friend Deep_Inline Vec4 operator/(Vec4Arg in_vec, float32 in_val);
    friend Deep_Inline Vec4 operator/(float32 in_val, Vec4Arg in_vec);

    // Multiply a Matrix4x4 and Vector
    friend Deep_Inline Vec4 operator*(Mat4Arg in_mat, Vec4Arg in_vec);

    //

    union {
        Xmm xmm;
        Xmmi xmmi;
        float32 m_values[4];
        struct {
            float32 x;
            float32 y;
            float32 z;
            float32 w;
        };
    };

    static const Vec4 k_zero;
};

static_assert(std::is_trivial<Vec4>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec4>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Vec4.inl" // IWYU pragma: export
