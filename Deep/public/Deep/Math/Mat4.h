#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Math/Xmm.h"
#include "Deep/Math/Vec4.h"
#include "Deep/Math/XmmArgs.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Matrix4x4
// Notation is column followed by row, so m10 is column 1, row 0.
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(Xmm) Mat4 {
	//

	Mat4() = default;
	Mat4(const Mat4&) = default;
	Mat4& operator=(const Mat4&) = default;
	Deep_Inline Mat4(const XmmArg in_col0, const XmmArg in_col1, const XmmArg in_col2, const XmmArg in_col3);
	Deep_Inline Mat4(                                                   //
		float32 in_m00, float32 in_m01, float32 in_m02, float32 in_m03, //
		float32 in_m10, float32 in_m11, float32 in_m12, float32 in_m13, //
		float32 in_m20, float32 in_m21, float32 in_m22, float32 in_m23, //
		float32 in_m30, float32 in_m31, float32 in_m32, float32 in_m33  //
	);                                                                  //
	Deep_Inline Mat4(const Arg_Vec4 in_col0, const Arg_Vec4 in_col1, const Arg_Vec4 in_col2, const Arg_Vec4 in_col3);

	//

	[[nodiscard]] Deep_Inline float32 determinant() const;

	Deep_Inline Mat4& Transpose();
	[[nodiscard]] Deep_Inline Mat4 transposed() const;

	Deep_Inline Mat4& Inverse();
	[[nodiscard]] Deep_Inline Mat4 inversed() const;

	Mat4& Compose(Arg_Vec3 in_position, Arg_Quat in_rotation, Arg_Vec3 in_scale);

	//

	// Equality
	friend bool operator!=(Arg_Mat4 in_a, Arg_Mat4 in_b);
	friend bool operator==(Arg_Mat4 in_a, Arg_Mat4 in_b);

	// Mul Matrix4x4s
	friend Deep_Inline Mat4 operator*(Arg_Mat4 in_a, Arg_Mat4 in_b);

	// Mul Matrix4x4s and float
	Deep_Inline Mat4& operator*=(float32 in_other);
	friend Deep_Inline Mat4 operator*(Arg_Mat4 in_mat, float32 in_val);
	friend Deep_Inline Mat4 operator*(float32 in_val, Arg_Mat4 in_mat);

	Deep_Inline Mat4& operator/=(float32 in_other);
	friend Deep_Inline Mat4 operator/(Arg_Mat4 in_mat, float32 in_val);
	friend Deep_Inline Mat4 operator/(float32 in_val, Arg_Mat4 in_mat);

	// Multiply Matrix4x4 and Vectors
	friend Deep_Inline Vec3 operator*(Arg_Mat4 in_mat, Arg_Vec3 in_vec); // NOTE(randomuserhi): Assumes Vec4 with w = 1
	friend Deep_Inline Vec4 operator*(Arg_Mat4 in_mat, Arg_Vec4 in_vec);

	[[nodiscard]] static Deep_Inline Mat4 FromQuaternion(const Quat& in_quat);

	//

	/**
	 * (00, 01, 02, 03)
	 * (10, 11, 12, 13)
	 * (20, 21, 22, 23)
	 * (30, 31, 32, 33)
	 */
	union {
		Xmm m_cols[4];
		Vec4 m_vcols[4];
		float32 m_values[16];
		// NOTE(randomuserhi): order of values matter for specific memory access patterns
		//                     column-major ordering is used
		struct {
			float32 m00;
			float32 m10;
			float32 m20;
			float32 m30;
			float32 m01;
			float32 m11;
			float32 m21;
			float32 m31;
			float32 m02;
			float32 m12;
			float32 m22;
			float32 m32;
			float32 m03;
			float32 m13;
			float32 m23;
			float32 m33;
		};
	};

	static const Mat4 k_identity;
};

static_assert(std::is_trivial<Mat4>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Mat4>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Mat4.inl" // IWYU pragma: export
