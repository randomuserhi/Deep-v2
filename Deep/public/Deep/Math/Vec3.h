#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Math/Xmm.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmm) Vec3 {
	//

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3& operator=(const Vec3&) = default;
	Deep_Inline Vec3(float32 in_x, float32 in_y, float32 in_z);
	explicit Deep_Inline Vec3(Xmm in_xmm);
	explicit Deep_Inline Vec3(Arg_Vec4 in_vec);

	//

	explicit Deep_Inline operator Vec3i() const;

	//

	Deep_Inline Vec3& Normalize();
	[[nodiscard]] Deep_Inline Vec3 normalized() const;
	[[nodiscard]] Deep_Inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
	[[nodiscard]] Deep_Inline float32 magnitude() const;

	[[nodiscard]] static Deep_Inline float32 Dot(Arg_Vec3 in_a, Arg_Vec3 in_b);
	[[nodiscard]] static Deep_Inline Vec3 Cross(Arg_Vec3 in_a, Arg_Vec3 in_b);

	[[nodiscard]] static Deep_Inline Vec3 Lerp(Arg_Vec3 in_a, Arg_Vec3 in_b, float32 in_t);

	//

	// Equality
	friend bool operator!=(Arg_Vec3 in_a, Arg_Vec3 in_b);
	friend bool operator==(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Add vectors
	Deep_Inline Vec3& operator+=(Arg_Vec3 in_other);
	friend Deep_Inline Vec3 operator+(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Sub vectors
	Deep_Inline Vec3& operator-=(Arg_Vec3 in_other);
	friend Deep_Inline Vec3 operator-(Arg_Vec3 in_a, Arg_Vec3 in_b);
	friend Deep_Inline Vec3 operator-(Arg_Vec3 in_a);

	// Mul vectors
	Deep_Inline Vec3& operator*=(Arg_Vec3 in_other);
	friend Deep_Inline Vec3 operator*(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Div vectors
	Deep_Inline Vec3& operator/=(Arg_Vec3 in_other);
	friend Deep_Inline Vec3 operator/(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Mul vector and float
	Deep_Inline Vec3& operator*=(float32 in_other);
	friend Deep_Inline Vec3 operator*(Arg_Vec3 in_vec, float32 in_val);
	friend Deep_Inline Vec3 operator*(float32 in_val, Arg_Vec3 in_vec);

	// Div vector and float
	Deep_Inline Vec3& operator/=(float32 other);
	friend Deep_Inline Vec3 operator/(Arg_Vec3 in_vec, float32 in_val);
	friend Deep_Inline Vec3 operator/(float32 in_val, Arg_Vec3 in_vec);

	// Multiply a Matrix4x4 and Vector
	friend Deep_Inline Vec3 operator*(Arg_Mat4 in_mat, Arg_Vec3 in_vec); // NOTE(randomuserhi): Assumes Vec4 with w = 1

	// Apply a quaternion to a vector
	friend Deep_Inline Vec3 operator*(Arg_Quat in_quat, Arg_Vec3 in_vec);

	//

	// NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
	union {
		Xmm xmm;
		Xmmi xmmi;
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

#include "Deep/Math/Vec3.inl" // IWYU pragma: export
