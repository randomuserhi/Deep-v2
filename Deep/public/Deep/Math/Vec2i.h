#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(int64) Vec2i {
	//

	Vec2i() = default;
	Vec2i(const Vec2i&) = default;
	Vec2i& operator=(const Vec2i&) = default;
	Deep_Inline Vec2i(int32 in_x, int32 in_y);
	explicit Deep_Inline Vec2i(Arg_Vec3i in_vec);
	explicit Deep_Inline Vec2i(Arg_Vec4i in_vec);

	//

	explicit Deep_Inline operator Vec2() const;

	//

	[[nodiscard]] Deep_Inline int32 manhattanDistance() const;

	[[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
	[[nodiscard]] Deep_Inline float32 magnitude() const;

	[[nodiscard]] static Deep_Inline int32 Dot(Arg_Vec2i in_a, Arg_Vec2i in_b);

	//

	// Equality
	friend bool operator!=(Arg_Vec2i in_a, Arg_Vec2i in_b);
	friend bool operator==(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Add vectors
	Deep_Inline Vec2i& operator+=(Arg_Vec2i in_other);
	friend Deep_Inline Vec2i operator+(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Sub vectors
	Deep_Inline Vec2i& operator-=(Arg_Vec2i in_other);
	friend Deep_Inline Vec2i operator-(Arg_Vec2i in_a, Arg_Vec2i in_b);
	friend Deep_Inline Vec2i operator-(Arg_Vec2i in_a);

	// Mul vectors
	Deep_Inline Vec2i& operator*=(Arg_Vec2i in_other);
	friend Deep_Inline Vec2i operator*(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Div vectors
	Deep_Inline Vec2i& operator/=(Arg_Vec2i in_other);
	friend Deep_Inline Vec2i operator/(Arg_Vec2i in_a, Arg_Vec2i in_b);

	// Mul vector and float
	Deep_Inline Vec2i& operator*=(int32 in_other);
	friend Deep_Inline Vec2i operator*(Arg_Vec2i in_vec, int32 in_val);
	friend Deep_Inline Vec2i operator*(int32 in_val, Arg_Vec2i in_vec);

	// Div vector and float
	Deep_Inline Vec2i& operator/=(int32 other);
	friend Deep_Inline Vec2i operator/(Arg_Vec2i in_vec, int32 in_val);
	friend Deep_Inline Vec2i operator/(int32 in_val, Arg_Vec2i in_vec);

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
