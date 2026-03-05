/**
 * Quaternion
 */

#pragma once

#include "_MathTypes.h"
#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Quat {
    //

    Quat() = default;
    Quat(const Quat&) = default;
    Quat& operator=(const Quat&) = default;
    Deep_Inline Quat(Vec4 in_vec);
    explicit Deep_Inline Quat(SSE_m128 in_sse_m128);
    explicit Deep_Inline Quat(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
    explicit Deep_Inline Quat(Vec3 in_axis, float32 in_angle);

    //

    Deep_Inline Quat& Normalize();
    [[nodiscard]] Deep_Inline Quat normalized() const;
    [[nodiscard]] Deep_Inline bool IsNormalized(float roTolerance = 1.0e-6f) const;

    Deep_Inline Quat& Conjugate();
    [[nodiscard]] Deep_Inline Quat conjugated() const;

    Deep_Inline Quat& Inverse();
    [[nodiscard]] Deep_Inline Quat inversed() const;

    Deep_Inline Mat4 ToMat4() const;
    static Deep_Inline Quat FromMat4(Mat4Arg in_mat);

    //

    // Equality
    friend bool operator!=(QuatArg in_a, QuatArg in_b);
    friend bool operator==(QuatArg in_a, QuatArg in_b);

    // Add quaternions
    Deep_Inline Quat& operator+=(QuatArg in_other);
    friend Deep_Inline Quat operator+(QuatArg in_a, QuatArg in_b);

    // Sub quaternions
    Deep_Inline Quat& operator-=(QuatArg in_other);
    friend Deep_Inline Quat operator-(QuatArg in_a, QuatArg in_b);

    // Mul quaternions
    Deep_Inline Quat& operator*=(QuatArg in_other);
    friend Deep_Inline Quat operator*(Quat in_a, QuatArg in_b);

    // Mul quaternion and float
    Deep_Inline Quat& operator*=(float32 in_other);
    friend Deep_Inline Quat operator*(QuatArg in_quat, float32 in_val);
    friend Deep_Inline Quat operator*(float32 in_val, QuatArg in_quat);

    // Div quaternion and float
    Deep_Inline Quat& operator/=(float32 in_other);
    friend Deep_Inline Quat operator/(QuatArg in_quat, float32 in_val);
    friend Deep_Inline Quat operator/(float32 in_val, QuatArg in_quat);

    // Rotate a vector by a quaternion
    friend Deep_Inline Vec3 operator*(QuatArg in_quat, Vec3Arg in_val);

    // Rotate a vector by the inverse of this quaternion
    static Deep_Inline Vec3 InverseRotate(QuatArg in_quat, Vec3Arg in_val);

    //

    union {
        SSE_m128 sse_m128;
        SSE_m128i sse_m128i;
        float32 val[4];
        struct {
            float32 x;
            float32 y;
            float32 z;
            float32 w;
        };
        Vec4 vec;
    };

    static const Quat k_identity;
};

static_assert(std::is_trivial<Quat>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Quat>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END