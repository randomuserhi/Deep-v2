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
	inline Vec2(float32 in_x, float32 in_y);
	explicit inline Vec2(Arg_Vec3 in_vec);
	explicit inline Vec2(Arg_Vec4 in_vec);

	//

	explicit inline operator Vec2i() const;

	//

	inline Vec2& Normalize();
	[[nodiscard]] inline Vec2 normalized() const;
	[[nodiscard]] inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] inline float32 sqrdMagnitude() const;
	[[nodiscard]] inline float32 magnitude() const;

	[[nodiscard]] static inline float32 Dot(Arg_Vec2 in_a, Arg_Vec2 in_b);
	[[nodiscard]] static inline float32 Cross(Arg_Vec2 in_a, Arg_Vec2 in_b);

	[[nodiscard]] static inline Vec2 Rotate(Arg_Vec2 in_a, float in_radians);
	[[nodiscard]] static inline float32 Angle(Arg_Vec2 in_a, Arg_Vec2 in_b);

	[[nodiscard]] static inline Vec2 Lerp(Arg_Vec2 in_a, Arg_Vec2 in_b, float32 in_t);

	//

	// Equality
	friend bool operator!=(Arg_Vec2 in_a, Arg_Vec2 in_b);
	friend bool operator==(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Add vectors
	inline Vec2& operator+=(Arg_Vec2 in_other);
	friend inline Vec2 operator+(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Sub vectors
	inline Vec2& operator-=(Arg_Vec2 in_other);
	friend inline Vec2 operator-(Arg_Vec2 in_a, Arg_Vec2 in_b);
	friend inline Vec2 operator-(Arg_Vec2 in_a);

	// Mul vectors
	inline Vec2& operator*=(Arg_Vec2 in_other);
	friend inline Vec2 operator*(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Div vectors
	inline Vec2& operator/=(Arg_Vec2 in_other);
	friend inline Vec2 operator/(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Mul vector and float
	inline Vec2& operator*=(float32 in_other);
	friend inline Vec2 operator*(Arg_Vec2 in_vec, float32 in_val);
	friend inline Vec2 operator*(float32 in_val, Arg_Vec2 in_vec);

	// Div vector and float
	inline Vec2& operator/=(float32 other);
	friend inline Vec2 operator/(Arg_Vec2 in_vec, float32 in_val);
	friend inline Vec2 operator/(float32 in_val, Arg_Vec2 in_vec);

	// Apply a quaternion to a vector
	friend inline Vec2 operator*(Arg_Quat in_quat, Arg_Vec2 in_vec);

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
