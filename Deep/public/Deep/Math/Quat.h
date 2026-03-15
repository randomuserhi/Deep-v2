/**
 * Quaternion
 */

#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Math/Xmm.h"
#include "Deep/Math/Vec4.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmm) Quat {
	//

	Quat() = default;
	Quat(const Quat&) = default;
	Quat& operator=(const Quat&) = default;
	Deep_Inline Quat(Vec4 in_vec);
	explicit Deep_Inline Quat(Xmm in_Xmm);
	explicit Deep_Inline Quat(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
	explicit Deep_Inline Quat(Vec3 in_axis, float32 in_angle);

	//

	Deep_Inline Quat& Normalize();
	[[nodiscard]] Deep_Inline Quat normalized() const;
	[[nodiscard]] Deep_Inline bool IsNormalized(float roTolerance = 1.0e-6f) const;

	Deep_Inline Quat& Conjugate();
	[[nodiscard]] Deep_Inline Quat conjugated() const;

	Deep_Inline Quat& Inverse();
	[[nodiscard]] Deep_Inline Quat inversed() const;

	Deep_Inline Mat4 ToMat4() const;
	static Deep_Inline Quat FromMat4(Arg_Mat4 in_mat);

	//

	// Equality
	friend bool operator!=(Arg_Quat in_a, Arg_Quat in_b);
	friend bool operator==(Arg_Quat in_a, Arg_Quat in_b);

	// Add quaternions
	Deep_Inline Quat& operator+=(Arg_Quat in_other);
	friend Deep_Inline Quat operator+(Arg_Quat in_a, Arg_Quat in_b);

	// Sub quaternions
	Deep_Inline Quat& operator-=(Arg_Quat in_other);
	friend Deep_Inline Quat operator-(Arg_Quat in_a, Arg_Quat in_b);

	// Mul quaternions
	Deep_Inline Quat& operator*=(Arg_Quat in_other);
	friend Deep_Inline Quat operator*(Quat in_a, Arg_Quat in_b);

	// Mul quaternion and float
	Deep_Inline Quat& operator*=(float32 in_other);
	friend Deep_Inline Quat operator*(Arg_Quat in_quat, float32 in_val);
	friend Deep_Inline Quat operator*(float32 in_val, Arg_Quat in_quat);

	// Div quaternion and float
	Deep_Inline Quat& operator/=(float32 in_other);
	friend Deep_Inline Quat operator/(Arg_Quat in_quat, float32 in_val);
	friend Deep_Inline Quat operator/(float32 in_val, Arg_Quat in_quat);

	// Rotate a vector by a quaternion
	friend Deep_Inline Vec3 operator*(Arg_Quat in_quat, Arg_Vec3 in_val);

	// Rotate a vector by the inverse of this quaternion
	static Deep_Inline Vec3 InverseRotate(Arg_Quat in_quat, Arg_Vec3 in_val);

	//

	union {
		Xmm xmm;
		Xmmi xmmi;
		float32 val[4];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
		Vec4 vec;
	};

	static const Quat k_identity;
};

static_assert(std::is_trivial<Quat>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Quat>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Quat.inl" // IWYU pragma: export
