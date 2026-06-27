#pragma once

#include "Deep/Math/Quat.h"
#include "Deep/Math/Mat4.h" // IWYU pragma: export

DEEP_NAMESPACE_BEGIN

Quat::Quat(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	vec{ in_x, in_y, in_z, in_w } {};
Quat::Quat(Float32x4 in_float32x4) :
	m_float32x4{ in_float32x4 } {};
Quat::Quat(Vec4 in_vec) :
	vec{ in_vec } {};

Quat Quat::s_FromMat4(Arg_Mat4 in_mat) {
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm

	float trace = in_mat.m00 + in_mat.m11 + in_mat.m22;

	if (trace >= 0.0f) {
		float s = Sqrt(trace + 1.0f);
		float is = 0.5f / s;
		return Quat((in_mat.m12 - in_mat.m21) * is, (in_mat.m20 - in_mat.m02) * is, (in_mat.m01 - in_mat.m10) * is,
		            0.5f * s);
	} else {
		int i = 0;
		if (in_mat.m11 > in_mat.m00) i = 1;
		if (in_mat.m22 > in_mat.m_cols[i].m_values[i]) i = 2;

		if (i == 0) {
			float s = Sqrt(in_mat.m00 - (in_mat.m11 + in_mat.m22) + 1.0f);
			float is = 0.5f / s;
			return Quat(0.5f * s, (in_mat.m10 + in_mat.m01) * is, (in_mat.m02 + in_mat.m20) * is,
			            (in_mat.m12 - in_mat.m21) * is);
		} else if (i == 1) {
			float s = Sqrt(in_mat.m11 - (in_mat.m22 + in_mat.m00) + 1.0f);
			float is = 0.5f / s;
			return Quat((in_mat.m10 + in_mat.m01) * is, 0.5f * s, (in_mat.m21 + in_mat.m12) * is,
			            (in_mat.m20 - in_mat.m02) * is);
		} else {
			Deep_Assert(i == 2, "Index should be 2.");

			float s = Sqrt(in_mat.m22 - (in_mat.m00 + in_mat.m11) + 1.0f);
			float is = 0.5f / s;
			return Quat((in_mat.m02 + in_mat.m20) * is, (in_mat.m21 + in_mat.m12) * is, 0.5f * s,
			            (in_mat.m01 - in_mat.m10) * is);
		}
	}
}

Quat& Quat::Normalize() {
	vec.Normalize();
	return *this;
}
Quat Quat::normalized() const {
	Quat q = *this;
	return q.Normalize();
}
bool Quat::IsNormalized(float in_tolerance) const {
	return vec.IsNormalized(in_tolerance);
}

Mat4 Quat::ToMat4() const {
	Mat4 m;
	return m.s_FromQuaternion(*this);
}

Quat& Quat::Conjugate() {
	// https://stackoverflow.com/questions/56992811/is-there-a-way-to-flip-the-sign-bit-of-32-bit-float-with-xor
	m_float32x4 =
		m_float32x4 ^ Int32x4{ int32(0x80000000), int32(0x80000000), int32(0x80000000), int32(0) }.ReinterpretAsFloat();
	return *this;
}
Quat Quat::conjugated() const {
	Quat q = *this;
	return q.Conjugate();
}

Quat& Quat::Inverse() {
	return Conjugate() /= vec.m_Magnitude();
}
Quat Quat::inversed() const {
	Quat q = *this;
	return q.Inverse();
}

Quat::Quat(Vec3 in_axis, float32 in_angle) {
	Deep_Assert(in_axis.IsNormalized(), "Axis must be normalized.");

	// { x,y,z } = axis * sin(0.5f * inAngle)
	//   w       = cos(0.5f * inAngle)

	Float32x4 s, c;
	Float32x4::s_Replicate(0.5f * in_angle).SinCos(s, c);
	m_float32x4 = Float32x4::s_Select(in_axis.m_float32x4 * s, c, Int32x4{ 0, 0, 0, int32(0xffffffff) });
}

constexpr float32& Quat::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Quat::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Quat in_a, Arg_Quat in_b) {
	return in_a.vec != in_b.vec;
}

bool operator==(Arg_Quat in_a, Arg_Quat in_b) {
	return !(in_a != in_b);
}

Quat& Quat::operator+=(Arg_Quat in_other) {
	vec += in_other.vec;
	return *this;
}

Quat operator+(Arg_Quat in_a, Arg_Quat in_b) {
	return in_a.vec + in_b.vec;
}

Quat& Quat::operator-=(Arg_Quat in_other) {
	vec -= in_other.vec;
	return *this;
}

Quat operator-(Arg_Quat in_a, Arg_Quat in_b) {
	return in_a.vec - in_b.vec;
}

Quat& Quat::operator*=(float32 in_other) {
	vec *= in_other;
	return *this;
}

Quat& Quat::operator/=(float32 in_other) {
	vec /= in_other;
	return *this;
}

Quat operator*(Arg_Quat in_quat, float32 in_val) {
	return in_quat.vec * in_val;
}

Quat operator*(float32 in_val, Arg_Quat in_quat) {
	return in_val * in_quat.vec;
}

Quat operator/(Arg_Quat in_quat, float32 in_val) {
	return in_quat.vec / in_val;
}

Quat operator/(float32 in_val, Arg_Quat in_quat) {
	return in_val / in_quat.vec;
}

Quat& Quat::operator*=(Arg_Quat in_other) {
#ifdef DEEP_USE_SSE4_1
	// Taken from: http://momchil-velikov.blogspot.nl/2013/10/fast-sse-quternion-multiplication.html
	__m128 abcd = m_float32x4;
	__m128 xyzw = in_other.m_float32x4;

	__m128 t0 = _mm_shuffle_ps(abcd, abcd, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 t1 = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(2, 3, 0, 1));

	__m128 t3 = _mm_shuffle_ps(abcd, abcd, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 t4 = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(1, 0, 3, 2));

	__m128 t5 = _mm_shuffle_ps(abcd, abcd, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 t6 = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(2, 0, 3, 1));

	// [d,d,d,d] * [z,w,x,y] = [dz,dw,dx,dy]
	__m128 m0 = _mm_mul_ps(t0, t1);

	// [a,a,a,a] * [y,x,w,z] = [ay,ax,aw,az]
	__m128 m1 = _mm_mul_ps(t3, t4);

	// [b,b,b,b] * [z,x,w,y] = [bz,bx,bw,by]
	__m128 m2 = _mm_mul_ps(t5, t6);

	// [c,c,c,c] * [w,z,x,y] = [cw,cz,cx,cy]
	__m128 t7 = _mm_shuffle_ps(abcd, abcd, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 t8 = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3, 2, 0, 1));
	__m128 m3 = _mm_mul_ps(t7, t8);

	// [dz,dw,dx,dy] + -[ay,ax,aw,az] = [dz+ay,dw-ax,dx+aw,dy-az]
	__m128 e = _mm_addsub_ps(m0, m1);

	// [dx+aw,dz+ay,dy-az,dw-ax]
	e = _mm_shuffle_ps(e, e, _MM_SHUFFLE(1, 3, 0, 2));

	// [dx+aw,dz+ay,dy-az,dw-ax] + -[bz,bx,bw,by] = [dx+aw+bz,dz+ay-bx,dy-az+bw,dw-ax-by]
	e = _mm_addsub_ps(e, m2);

	// [dz+ay-bx,dw-ax-by,dy-az+bw,dx+aw+bz]
	e = _mm_shuffle_ps(e, e, _MM_SHUFFLE(2, 0, 1, 3));

	// [dz+ay-bx,dw-ax-by,dy-az+bw,dx+aw+bz] + -[cw,cz,cx,cy] = [dz+ay-bx+cw,dw-ax-by-cz,dy-az+bw+cx,dx+aw+bz-cy]
	e = _mm_addsub_ps(e, m3);

	// [dw-ax-by-cz,dz+ay-bx+cw,dy-az+bw+cx,dx+aw+bz-cy]
	m_float32x4 = _mm_shuffle_ps(e, e, _MM_SHUFFLE(2, 3, 1, 0));
#else
	float lx = x;
	float ly = y;
	float lz = z;
	float lw = w;

	float rx = in_other.x;
	float ry = in_other.y;
	float rz = in_other.z;
	float rw = in_other.w;

	x = lw * rx + lx * rw + ly * rz - lz * ry;
	y = lw * ry - lx * rz + ly * rw + lz * rx;
	z = lw * rz + lx * ry - ly * rx + lz * rw;
	w = lw * rw - lx * rx - ly * ry - lz * rz;
#endif
	return *this;
}

Quat operator*(Quat in_a, Arg_Quat in_b) {
	return in_a *= in_b;
}

Vec3 operator*(Arg_Quat in_quat, Arg_Vec3 in_vec) {
	Deep_Assert(in_quat.IsNormalized(), "Quaternion must be normalized.");

	// Rotating a vector by a quaternion is done by: p' = q * p * q^-1 (q^-1 = conjugated(q) for a unit quaternion)
	return Vec3{ (in_quat * Vec4(in_vec, 0.0f) * in_quat.conjugated()).m_float32x4 };
}

Vec3 Quat::s_InverseRotate(Arg_Quat in_quat, Arg_Vec3 in_vec) {
	Deep_Assert(in_quat.IsNormalized(), "Quaternion must be normalized.");

	// Rotating a vector by a quaternion is done by: p' = q * p * q^-1 (q^-1 = conjugated(q) for a unit quaternion)
	return Vec3{ (in_quat.conjugated() * Vec4(in_vec, 0.0f) * in_quat).m_float32x4 };
}

DEEP_NAMESPACE_END
