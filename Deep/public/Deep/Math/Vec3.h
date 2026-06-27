#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Simd.h"

#include <type_traits>
#include <ostream>

DEEP_NAMESPACE_BEGIN

// Actually represents storage of 4 values (x, y, z, _w)
// On initialization, `_w = z`
// On division, `_w` must match `z` to prevent divide by zero errors when floating point exceptions are enabled. The method
// `s_FixW` can be used to enforce this. Apart from division or initialization, the state of `_w` is undefined.
struct [[nodiscard]] alignas(Float32x4) Vec3 {
	//

	Vec3() = default;
	Vec3(const Vec3&) = default;
	Vec3& operator=(const Vec3&) = default;
	inline Vec3(float32 in_x, float32 in_y, float32 in_z);
	explicit inline Vec3(Float32x4 in_xmm);
	explicit inline Vec3(Arg_Vec4 in_vec);

	//

	inline operator Float32x4() const;
	explicit inline operator Vec3i() const;

	//

	inline Vec3& Normalize();
	[[nodiscard]] inline Vec3 m_Normalized() const;
	[[nodiscard]] inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] inline float32 m_SqrdMagnitude() const;
	[[nodiscard]] inline float32 m_Magnitude() const;

	[[nodiscard]] static inline float32 s_Dot(Arg_Vec3 in_a, Arg_Vec3 in_b);
	[[nodiscard]] static inline Vec3 s_Cross(Arg_Vec3 in_a, Arg_Vec3 in_b);

	[[nodiscard]] static inline Vec3 s_Lerp(Arg_Vec3 in_a, Arg_Vec3 in_b, float32 in_t);

	static inline Vec3 s_FixW(Arg_Vec3 in_vec);
	static Deep_ForceInline void s_CheckW(Arg_Vec3 in_vec);

	//

	Deep_ForceInline constexpr float32& operator[](size_t);
	Deep_ForceInline constexpr const float32& operator[](size_t) const;

	// Copy only the xyz components, retaining the current _w value
	inline void XYZ(Arg_Vec3 in_vec);

	// Equality
	friend inline bool operator!=(Arg_Vec3 in_a, Arg_Vec3 in_b);
	friend inline bool operator==(Arg_Vec3 in_a, Arg_Vec3 in_b);

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
		Float32x4 m_float32x4;
		Int32x4 m_int32x4;
		float32 m_values[4];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 _w;
		};
	};

	DEEP_EXPORT static const Vec3 k_zero;
	DEEP_EXPORT static const Vec3 k_one;

	DEEP_EXPORT static const Vec3 k_up;
	DEEP_EXPORT static const Vec3 k_down;
	DEEP_EXPORT static const Vec3 k_left;
	DEEP_EXPORT static const Vec3 k_right;
	DEEP_EXPORT static const Vec3 k_forwards;
	DEEP_EXPORT static const Vec3 k_backwards;

	static constexpr int k_numAxis = 3;
	DEEP_EXPORT static const Vec3 k_axis[k_numAxis];
};

static_assert(std::is_trivial<Vec3>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec3>(), "Is supposed to be standard layout!");

inline std::ostream& operator<<(std::ostream& in_os, const Vec3& in_vec);

DEEP_NAMESPACE_END

#include "Deep/Math/Vec3.inl" // IWYU pragma: export
