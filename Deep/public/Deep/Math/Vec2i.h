#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(int64) Vec2i {
	//

	Vec2i() = default;
	Vec2i(const Vec2i&) = default;
	Vec2i& operator=(const Vec2i&) = default;
	inline Vec2i(int32 in_x, int32 in_y);
	explicit inline Vec2i(Arg_Vec3i in_vec);
	explicit inline Vec2i(Arg_Vec4i in_vec);

	//

	explicit inline operator Vec2() const;

	//

	[[nodiscard]] inline int32 manhattanDistance() const;

	[[nodiscard]] inline float32 sqrdMagnitude() const;
	[[nodiscard]] inline float32 magnitude() const;

	[[nodiscard]] static inline int32 Dot(Arg_Vec2i in_a, Arg_Vec2i in_b);

	//

	// Equality
	friend bool operator!=(Arg_Vec2i in_a, Arg_Vec2i in_b);
	friend bool operator==(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Add vectors
	inline Vec2i& operator+=(Arg_Vec2i in_other);
	friend inline Vec2i operator+(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Sub vectors
	inline Vec2i& operator-=(Arg_Vec2i in_other);
	friend inline Vec2i operator-(Arg_Vec2i in_a, Arg_Vec2i in_b);
	friend inline Vec2i operator-(Arg_Vec2i in_a);

	// Mul vectors
	inline Vec2i& operator*=(Arg_Vec2i in_other);
	friend inline Vec2i operator*(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Div vectors
	inline Vec2i& operator/=(Arg_Vec2i in_other);
	friend inline Vec2i operator/(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Mul vector and float
	inline Vec2i& operator*=(int32 in_other);
	friend inline Vec2i operator*(Arg_Vec2i in_vec, int32 in_val);
	friend inline Vec2i operator*(int32 in_val, Arg_Vec2i in_vec);

	// Div vector and float
	inline Vec2i& operator/=(int32 other);
	friend inline Vec2i operator/(Arg_Vec2i in_vec, int32 in_val);
	friend inline Vec2i operator/(int32 in_val, Arg_Vec2i in_vec);

	//

	// NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
	union {
		int32 m_values[2];
		struct {
			int32 x;
			int32 y;
		};
	};

	static const Vec2i k_zero;

	static const Vec2i k_up;
	static const Vec2i k_down;
	static const Vec2i k_left;
	static const Vec2i k_right;
};

static_assert(std::is_trivial<Vec2i>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec2i>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Vec2i.inl" // IWYU pragma: export
