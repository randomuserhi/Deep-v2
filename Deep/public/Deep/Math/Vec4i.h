#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Math/Xmmi.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmmi) Vec4i {
	//

	Vec4i() = default;
	Vec4i(const Vec4i&) = default;
	Vec4i& operator=(const Vec4i&) = default;
	Deep_Inline Vec4i(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
	explicit Deep_Inline Vec4i(Xmmi in_xmmi);
	explicit Deep_Inline Vec4i(Arg_Vec3i in_xyz, int32 in_w);

	//

	explicit Deep_Inline operator Vec4() const;

	//

	[[nodiscard]] Deep_Inline int32 manhattanDistance() const;

	[[nodiscard]] Deep_Inline float32 sqrdMagnitude() const;
	[[nodiscard]] Deep_Inline float32 magnitude() const;

	[[nodiscard]] static Deep_Inline int32 Dot(Arg_Vec4i in_a, Arg_Vec4i in_b);

	//

	// Equality
	friend bool operator!=(Arg_Vec4i in_a, Arg_Vec4i in_b);
	friend bool operator==(Arg_Vec4i in_a, Arg_Vec4i in_b);

	// Add vectors
	Deep_Inline Vec4i& operator+=(Arg_Vec4i in_other);
	friend Deep_Inline Vec4i operator+(Arg_Vec4i in_a, Arg_Vec4i in_b);

	// Sub vectors
	Deep_Inline Vec4i& operator-=(Arg_Vec4i in_other);
	friend Deep_Inline Vec4i operator-(Arg_Vec4i in_a, Arg_Vec4i in_b);
	friend Deep_Inline Vec4i operator-(Arg_Vec4i in_a);

	// Mul vectors
	Deep_Inline Vec4i& operator*=(Arg_Vec4i in_other);
	friend Deep_Inline Vec4i operator*(Arg_Vec4i in_a, Arg_Vec4i in_b);

	// Div vectors
	Deep_Inline Vec4i& operator/=(Arg_Vec4i in_other);
	friend Deep_Inline Vec4i operator/(Arg_Vec4i in_a, Arg_Vec4i in_b);

	// Mul vector and int
	Deep_Inline Vec4i& operator*=(int32 in_other);
	friend Deep_Inline Vec4i operator*(Arg_Vec4i in_vec, int32 in_val);
	friend Deep_Inline Vec4i operator*(int32 in_val, Arg_Vec4i in_vec);

	// Div vector and int
	Deep_Inline Vec4i& operator/=(int32 in_other);
	friend Deep_Inline Vec4i operator/(Arg_Vec4i in_vec, int32 in_val);
	friend Deep_Inline Vec4i operator/(int32 in_val, Arg_Vec4i in_vec);

	//

	union {
		Xmm xmm;
		Xmmi xmmi;
		int32 m_values[4];
		struct {
			int32 x;
			int32 y;
			int32 z;
			int32 w;
		};
	};

	static const Vec4i k_zero;
};

static_assert(std::is_trivial<Vec4i>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec4i>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Vec4i.inl" // IWYU pragma: export
