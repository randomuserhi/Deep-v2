/**
 * Quaternion
 */

#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Simd.h"
#include "Deep/Math/Vec4.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct [[nodiscard]] alignas(Float32x4) Quat {
	//

	Quat() = default;
	Quat(const Quat&) = default;
	Quat& operator=(const Quat&) = default;
	inline Quat(Vec4 in_vec);
	explicit inline Quat(Float32x4 in_xmm);
	explicit inline Quat(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
	explicit inline Quat(Vec3 in_axis, float32 in_angle);

	//

	inline Quat& Normalize();
	[[nodiscard]] inline Quat normalized() const;
	[[nodiscard]] inline bool IsNormalized(float roTolerance = 1.0e-6f) const;

	inline Quat& Conjugate();
	[[nodiscard]] inline Quat conjugated() const;

	inline Quat& Inverse();
	[[nodiscard]] inline Quat inversed() const;

	inline Mat4 ToMat4() const;
	static inline Quat s_FromMat4(Arg_Mat4 in_mat);

	//

	Deep_ForceInline constexpr float32& operator[](size_t);
	Deep_ForceInline constexpr const float32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Quat in_a, Arg_Quat in_b);
	friend inline bool operator==(Arg_Quat in_a, Arg_Quat in_b);

	// Add quaternions
	inline Quat& operator+=(Arg_Quat in_other);
	friend inline Quat operator+(Arg_Quat in_a, Arg_Quat in_b);

	// Sub quaternions
	inline Quat& operator-=(Arg_Quat in_other);
	friend inline Quat operator-(Arg_Quat in_a, Arg_Quat in_b);

	// Mul quaternions
	inline Quat& operator*=(Arg_Quat in_other);
	friend inline Quat operator*(Quat in_a, Arg_Quat in_b);

	// Mul quaternion and float
	inline Quat& operator*=(float32 in_other);
	friend inline Quat operator*(Arg_Quat in_quat, float32 in_val);
	friend inline Quat operator*(float32 in_val, Arg_Quat in_quat);

	// Div quaternion and float
	inline Quat& operator/=(float32 in_other);
	friend inline Quat operator/(Arg_Quat in_quat, float32 in_val);
	friend inline Quat operator/(float32 in_val, Arg_Quat in_quat);

	// Rotate a vector by a quaternion
	friend inline Vec3 operator*(Arg_Quat in_quat, Arg_Vec3 in_val);

	// Rotate a vector by the inverse of this quaternion
	static inline Vec3 s_InverseRotate(Arg_Quat in_quat, Arg_Vec3 in_val);

	//

	union {
		Float32x4 m_float32x4;
		Int32x4 m_int32x4;
		float32 m_values[4];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
		Vec4 vec;
	};

	DEEP_EXPORT static const Quat k_identity;
};

static_assert(std::is_trivial<Quat>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Quat>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Quat.inl" // IWYU pragma: export
