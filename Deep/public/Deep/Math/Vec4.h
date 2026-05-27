#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Math/Xmm.h"

#include <type_traits>
#include <ostream>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct [[nodiscard]] alignas(Xmm) Vec4 {
	//

	Vec4() = default;
	Vec4(const Vec4&) = default;
	Vec4& operator=(const Vec4&) = default;
	inline Vec4(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
	explicit inline Vec4(Xmm in_xmm);
	explicit inline Vec4(Arg_Vec3 in_xyz, float32 in_w);

	//

	explicit inline operator Vec4i() const;

	//

	inline Vec4& Normalize();
	[[nodiscard]] inline Vec4 normalized() const;
	[[nodiscard]] inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] inline float32 sqrdMagnitude() const;
	[[nodiscard]] inline float32 magnitude() const;

	[[nodiscard]] static inline float32 Dot(Arg_Vec4 in_a, Arg_Vec4 in_b);

	[[nodiscard]] static inline Vec4 Lerp(Arg_Vec4 in_a, Arg_Vec4 in_b, float32 in_t);

	//

	// Equality
	friend inline bool operator!=(Arg_Vec4 in_a, Arg_Vec4 in_b);
	friend inline bool operator==(Arg_Vec4 in_a, Arg_Vec4 in_b);

	// Add vectors
	inline Vec4& operator+=(Arg_Vec4 in_other);
	friend inline Vec4 operator+(Arg_Vec4 in_a, Arg_Vec4 in_b);

	// Sub vectors
	inline Vec4& operator-=(Arg_Vec4 in_other);
	friend inline Vec4 operator-(Arg_Vec4 in_a, Arg_Vec4 in_b);
	friend inline Vec4 operator-(Arg_Vec4 in_a);

	// Mul vectors
	inline Vec4& operator*=(Arg_Vec4 in_other);
	friend inline Vec4 operator*(Arg_Vec4 in_a, Arg_Vec4 in_b);

	// Div vectors
	inline Vec4& operator/=(Arg_Vec4 in_other);
	friend inline Vec4 operator/(Arg_Vec4 in_a, Arg_Vec4 in_b);

	// Mul vector and float
	inline Vec4& operator*=(float32 in_other);
	friend inline Vec4 operator*(Arg_Vec4 in_vec, float32 in_val);
	friend inline Vec4 operator*(float32 in_val, Arg_Vec4 in_vec);

	// Div vector and float
	inline Vec4& operator/=(float32 in_other);
	friend inline Vec4 operator/(Arg_Vec4 in_vec, float32 in_val);
	friend inline Vec4 operator/(float32 in_val, Arg_Vec4 in_vec);

	// Multiply a Matrix4x4 and Vector
	friend inline Vec4 operator*(Arg_Mat4 in_mat, Arg_Vec4 in_vec);

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

	DEEP_EXPORT static const Vec4 k_zero;
};

static_assert(std::is_trivial<Vec4>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec4>(), "Is supposed to be standard layout!");

inline std::ostream& operator<<(std::ostream& in_os, const Vec4& in_vec);

DEEP_NAMESPACE_END

#include "Deep/Math/Vec4.inl" // IWYU pragma: export
