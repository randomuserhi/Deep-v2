#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(int64) Vec2 {
	//

	Vec2() = default;
	Vec2(const Vec2&) = default;
	Vec2& operator=(const Vec2&) = default;
	Deep_Inline Vec2(float32 in_x, float32 in_y);
	explicit Deep_Inline Vec2(Arg_Vec3 in_vec);
	explicit Deep_Inline Vec2(Arg_Vec4 in_vec);

	//

	explicit Deep_Inline operator Vec2i() const;

	//

	Deep_Inline Vec2& Normalize();
	[[nodiscard]] Deep_Inline Vec2 normalized() const;
	[[nodiscard]] Deep_Inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
	[[nodiscard]] Deep_Inline float32 magnitude() const;

	[[nodiscard]] static Deep_Inline float32 Dot(Arg_Vec2 in_a, Arg_Vec2 in_b);
	[[nodiscard]] static Deep_Inline float32 Cross(Arg_Vec2 in_a, Arg_Vec2 in_b);

	[[nodiscard]] static Deep_Inline Vec2 Rotate(Arg_Vec2 in_a, float in_radians);
	[[nodiscard]] static Deep_Inline float32 Angle(Arg_Vec2 in_a, Arg_Vec2 in_b);

	[[nodiscard]] static Deep_Inline Vec2 Lerp(Arg_Vec2 in_a, Arg_Vec2 in_b, float32 in_t);

	//

	// Equality
	friend bool operator!=(Arg_Vec2 in_a, Arg_Vec2 in_b);
	friend bool operator==(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Add vectors
	Deep_Inline Vec2& operator+=(Arg_Vec2 in_other);
	friend Deep_Inline Vec2 operator+(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Sub vectors
	Deep_Inline Vec2& operator-=(Arg_Vec2 in_other);
	friend Deep_Inline Vec2 operator-(Arg_Vec2 in_a, Arg_Vec2 in_b);
	friend Deep_Inline Vec2 operator-(Arg_Vec2 in_a);

	// Mul vectors
	Deep_Inline Vec2& operator*=(Arg_Vec2 in_other);
	friend Deep_Inline Vec2 operator*(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Div vectors
	Deep_Inline Vec2& operator/=(Arg_Vec2 in_other);
	friend Deep_Inline Vec2 operator/(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Mul vector and float
	Deep_Inline Vec2& operator*=(float32 in_other);
	friend Deep_Inline Vec2 operator*(Arg_Vec2 in_vec, float32 in_val);
	friend Deep_Inline Vec2 operator*(float32 in_val, Arg_Vec2 in_vec);

	// Div vector and float
	Deep_Inline Vec2& operator/=(float32 other);
	friend Deep_Inline Vec2 operator/(Arg_Vec2 in_vec, float32 in_val);
	friend Deep_Inline Vec2 operator/(float32 in_val, Arg_Vec2 in_vec);

	// Apply a quaternion to a vector
	friend Deep_Inline Vec2 operator*(Arg_Quat in_quat, Arg_Vec2 in_vec);

	//

	// NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
	union {
		float32 m_values[2];
		struct {
			float32 x;
			float32 y;
		};
	};

	static const Vec2 k_zero;
	static const Vec2 k_one;

	static const Vec2 k_up;
	static const Vec2 k_down;
	static const Vec2 k_left;
	static const Vec2 k_right;
};

static_assert(std::is_trivial<Vec2>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec2>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Vec2.inl" // IWYU pragma: export
