#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Math/Xmmi.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmmi) Vec3i {
	//

	Vec3i() = default;
	Vec3i(const Vec3i&) = default;
	Vec3i& operator=(const Vec3i&) = default;
	inline Vec3i(int32 in_x, int32 in_y, int32 in_z);
	explicit inline Vec3i(Xmmi in_xmmi);
	explicit inline Vec3i(Arg_Vec4i in_vec);

	//

	explicit inline operator Vec3() const;

	//

	[[nodiscard]] inline int32 manhattanDistance() const;

	[[nodiscard]] inline float32 sqrdMagnitude() const;
	[[nodiscard]] inline float32 magnitude() const;

	[[nodiscard]] static inline int32 Dot(Arg_Vec3i in_a, Arg_Vec3i in_b);

	//

	// Equality
	friend bool operator!=(Arg_Vec3i in_a, Arg_Vec3i in_b);
	friend bool operator==(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Add vectors
	inline Vec3i& operator+=(Arg_Vec3i in_other);
	friend inline Vec3i operator+(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Sub vectors
	inline Vec3i& operator-=(Arg_Vec3i in_other);
	friend inline Vec3i operator-(Arg_Vec3i in_a, Arg_Vec3i in_b);
	friend inline Vec3i operator-(Arg_Vec3i in_a);

	// Mul vectors
	inline Vec3i& operator*=(Arg_Vec3i in_other);
	friend inline Vec3i operator*(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Div vectors
	inline Vec3i& operator/=(Arg_Vec3i in_other);
	friend inline Vec3i operator/(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Mul vector and float
	inline Vec3i& operator*=(int32 in_other);
	friend inline Vec3i operator*(Arg_Vec3i in_vec, int32 in_val);
	friend inline Vec3i operator*(int32 in_val, Arg_Vec3i in_vec);

	// Div vector and float
	inline Vec3i& operator/=(int32 other);
	friend inline Vec3i operator/(Arg_Vec3i in_vec, int32 in_val);
	friend inline Vec3i operator/(int32 in_val, Arg_Vec3i in_vec);

	//

	// NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
	union {
		Xmm xmm;
		Xmmi xmmi;
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

#include "Deep/Math/Vec3i.inl" // IWYU pragma: export
