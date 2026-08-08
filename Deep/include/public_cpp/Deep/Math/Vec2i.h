#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Simd.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct [[nodiscard]] alignas(Int32x2) Vec2i {
	//

	Vec2i() = default;
	Vec2i(const Vec2i&) = default;
	Vec2i& operator=(const Vec2i&) = default;
	inline Vec2i(int32 in_x, int32 in_y);
	explicit inline Vec2i(Int32x2 in_xmmi);
	explicit inline Vec2i(Arg_Vec3i in_vec);
	explicit inline Vec2i(Arg_Vec4i in_vec);

	//

	inline operator Int32x2() const;
	explicit inline operator Vec2() const;

	//

	[[nodiscard]] inline int32 m_ManhattanDistance() const;

	[[nodiscard]] inline float32 m_SqrdMagnitude() const;
	[[nodiscard]] inline float32 m_Magnitude() const;

	[[nodiscard]] static inline int32 s_Dot(Arg_Vec2i in_a, Arg_Vec2i in_b);

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Vec2i in_a, Arg_Vec2i in_b);
	friend inline bool operator==(Arg_Vec2i in_a, Arg_Vec2i in_b);

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
		Int32x2 m_int32x2;
		int32 m_values[2];
		struct {
			int32 x;
			int32 y;
		};
	};

	DEEP_EXPORT static const Vec2i k_zero;

	DEEP_EXPORT static const Vec2i k_up;
	DEEP_EXPORT static const Vec2i k_down;
	DEEP_EXPORT static const Vec2i k_left;
	DEEP_EXPORT static const Vec2i k_right;
};

static_assert(std::is_trivial_v<Vec2i>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<Vec2i>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "./Vec2i.inl" // IWYU pragma: export
