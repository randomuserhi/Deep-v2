#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Math/Xmm.h"

#include <type_traits>
#include <ostream>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmm) Vec3 {
	//

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3& operator=(const Vec3&) = default;
	inline Vec3(float32 in_x, float32 in_y, float32 in_z);
	explicit inline Vec3(Xmm in_xmm);
	explicit inline Vec3(Arg_Vec4 in_vec);

	//

	explicit inline operator Vec3i() const;

	//

	inline Vec3& Normalize();
	[[nodiscard]] inline Vec3 normalized() const;
	[[nodiscard]] inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] inline float32 sqrdMagnitude() const;
	[[nodiscard]] inline float32 magnitude() const;

	[[nodiscard]] static inline float32 Dot(Arg_Vec3 in_a, Arg_Vec3 in_b);
	[[nodiscard]] static inline Vec3 Cross(Arg_Vec3 in_a, Arg_Vec3 in_b);

	[[nodiscard]] static inline Vec3 Lerp(Arg_Vec3 in_a, Arg_Vec3 in_b, float32 in_t);

	//

	// Equality
	friend bool operator!=(Arg_Vec3 in_a, Arg_Vec3 in_b);
	friend bool operator==(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Add vectors
	inline Vec3& operator+=(Arg_Vec3 in_other);
	friend inline Vec3 operator+(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Sub vectors
	inline Vec3& operator-=(Arg_Vec3 in_other);
	friend inline Vec3 operator-(Arg_Vec3 in_a, Arg_Vec3 in_b);
	friend inline Vec3 operator-(Arg_Vec3 in_a);

	// Mul vectors
	inline Vec3& operator*=(Arg_Vec3 in_other);
	friend inline Vec3 operator*(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Div vectors
	inline Vec3& operator/=(Arg_Vec3 in_other);
	friend inline Vec3 operator/(Arg_Vec3 in_a, Arg_Vec3 in_b);

	// Mul vector and float
	inline Vec3& operator*=(float32 in_other);
	friend inline Vec3 operator*(Arg_Vec3 in_vec, float32 in_val);
	friend inline Vec3 operator*(float32 in_val, Arg_Vec3 in_vec);

	// Div vector and float
	inline Vec3& operator/=(float32 other);
	friend inline Vec3 operator/(Arg_Vec3 in_vec, float32 in_val);
	friend inline Vec3 operator/(float32 in_val, Arg_Vec3 in_vec);

	// Multiply a Matrix4x4 and Vector
	friend inline Vec3 operator*(Arg_Mat4 in_mat, Arg_Vec3 in_vec); // NOTE(randomuserhi): Assumes Vec4 with w = 1

	// Apply a quaternion to a vector
	friend inline Vec3 operator*(Arg_Quat in_quat, Arg_Vec3 in_vec);

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

	static constexpr int k_numAxis = 3;
	static const Vec3 k_axis[k_numAxis];
};

static_assert(std::is_trivial<Vec3>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec3>(), "Is supposed to be standard layout!");

inline std::ostream& operator<<(std::ostream& in_os, const Vec3& in_vec);

DEEP_NAMESPACE_END

#include "Deep/Math/Vec3.inl" // IWYU pragma: export
