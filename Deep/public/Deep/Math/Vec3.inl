#pragma once

#include "Deep.h"
#include "Deep/Math/Ops.h"
#include "Deep/Math/VecArgs.h"
#include "Deep/Simd/Float32x4.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec3i.h"
#include "Deep/Math/Vec4.h"

DEEP_NAMESPACE_BEGIN

Vec3::Vec3(float32 in_x, float32 in_y, float32 in_z) :
	m_float32x4{ in_x, in_y, in_z, in_z } {}

Vec3::Vec3(Arg_Vec4 in_vec) :
	m_float32x4{ in_vec.m_float32x4 } {}

Vec3::Vec3(Float32x4 in_float32x4) :
	m_float32x4{ in_float32x4 } {}

Vec3& Vec3::Normalize() {
#ifdef DEEP_USE_SSE4_1
	Float32x4 magnitudeSqrd = _mm_dp_ps(m_float32x4, m_float32x4, 0x7f);
	if (_mm_cvtss_f32(magnitudeSqrd) != 0.0f) m_float32x4 = _mm_div_ps(m_float32x4, _mm_sqrt_ps(magnitudeSqrd));
	return *this;
#else
	float32 magnitudeSqrd = m_SqrdMagnitude();
	if (magnitudeSqrd != 0.0f) *this /= Sqrt(magnitudeSqrd);
	return *this;
#endif
}
Vec3 Vec3::m_Normalized() const {
	Vec3 v = *this;
	return v.Normalize();
}
bool Vec3::IsNormalized(float tolerance) const {
	return Abs(m_SqrdMagnitude() - 1.0f) <= tolerance;
}

float32 Vec3::m_SqrdMagnitude() const {
	return s_Dot(*this, *this);
}
float32 Vec3::m_Magnitude() const {
#ifdef DEEP_USE_SSE
	Float32x4 t = _mm_mul_ps(m_float32x4, m_float32x4);
	Float32x4 y = _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1));
	Float32x4 z = _mm_shuffle_ps(t, t, _MM_SHUFFLE(2, 2, 2, 2));
	t = _mm_add_ss(t, y);
	t = _mm_add_ss(t, z);
	return _mm_cvtss_f32(_mm_sqrt_ss(t));
#else
	return Sqrt(m_SqrdMagnitude());
#endif
}

float32 Vec3::s_Dot(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	Float32x4 t = _mm_mul_ps(in_a.m_float32x4, in_b.m_float32x4);
	Float32x4 y = _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1));
	Float32x4 z = _mm_shuffle_ps(t, t, _MM_SHUFFLE(2, 2, 2, 2));
	t = _mm_add_ss(t, y);
	t = _mm_add_ss(t, z);
	return _mm_cvtss_f32(t);
#else
	return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z;
#endif
}

Vec3 Vec3::s_Cross(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	Float32x4 t1 =
		_mm_shuffle_ps(in_b.m_float32x4, in_b.m_float32x4, _MM_SHUFFLE(0, 0, 2, 1)); // Assure Z and W are the same
	t1 = _mm_mul_ps(t1, in_a.m_float32x4);
	Float32x4 t2 =
		_mm_shuffle_ps(in_a.m_float32x4, in_a.m_float32x4, _MM_SHUFFLE(0, 0, 2, 1)); // Assure Z and W are the same
	t2 = _mm_mul_ps(t2, in_b.m_float32x4);
	Float32x4 t3 = _mm_sub_ps(t1, t2);
	return Vec3{ _mm_shuffle_ps(t3, t3, _MM_SHUFFLE(0, 0, 2, 1)) }; // Assure Z and W are the same
#else
	return Vec3{ in_a.m_values[1] * in_b.m_values[2] - in_a.m_values[2] * in_b.m_values[1],
		         in_a.m_values[2] * in_b.m_values[0] - in_a.m_values[0] * in_b.m_values[2],
		         in_a.m_values[0] * in_b.m_values[1] - in_a.m_values[1] * in_b.m_values[0] };
#endif
}

Vec3 Vec3::s_Lerp(Arg_Vec3 in_a, Arg_Vec3 in_b, float32 in_t) {
	return (in_b - in_a) * in_t + in_a;
}

void Vec3::s_CheckW(Arg_Vec3 in_vec) {
#ifdef DEEP_USE_FLOATING_POINT_EXCEPTIONS
	// reinterpret_cast to avoid asserts when both components are NaN
	Deep_Assert(reinterpret_cast<const uint32*>(in_vec.m_values)[2] == reinterpret_cast<const uint32*>(in_vec.m_values)[3],
	            "W component does not match.");
#else
	(void)in_vec;
#endif
}

Vec3 Vec3::s_FixW(Arg_Vec3 in_vec) {
#ifdef DEEP_USE_FLOATING_POINT_EXCEPTIONS
	#ifdef DEEP_USE_SSE
	return Vec3{ _mm_shuffle_ps(in_vec.m_float32x4, in_vec.m_float32x4, _MM_SHUFFLE(2, 2, 1, 0)) };
	#else
	return Vec3{ in_vec.x, in_vec.y, in_vec.z, in_vec.z };
	#endif
#else
	return in_vec;
#endif
}

Vec3::operator Float32x4() const {
	return m_float32x4;
}

Vec3::operator Vec3i() const {
#ifdef DEEP_USE_SSE
	return Vec3i{ _mm_cvtps_epi32(m_float32x4) };
#else
	return Vec3i{ static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z) };
#endif
}

constexpr float32& Vec3::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Vec3::operator[](size_t in_index) const {
	return m_values[in_index];
}

void Vec3::XYZ(Arg_Vec3 in_vec) {
#ifdef DEEP_USE_SSE4_1
	m_float32x4 = _mm_blend_ps(in_vec.m_float32x4, m_float32x4, 0x08);
#else
	x = in_vec.x;
	y = in_vec.y;
	z = in_vec.z;
#endif
}

bool operator!=(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	return (Float32x4::s_Equals(in_a.m_float32x4, in_b.m_float32x4).ToBooleanBitMask() & 0b111) != 0b111;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z;
#endif
}
bool operator==(Arg_Vec3 a, Arg_Vec3 b) {
	return !(a != b);
}

Vec3& Vec3::operator+=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE
	m_float32x4 += in_other.m_float32x4;
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
#endif
	return *this;
}

Vec3 operator+(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	return Vec3{ in_a.m_float32x4 + in_b.m_float32x4 };
#else
	return Vec3{ in_a.x + in_b.x, in_a.y + in_b.y, in_a.z + in_b.z };
#endif
}

Vec3& Vec3::operator-=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE
	m_float32x4 -= in_other.m_float32x4;
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
#endif
	return *this;
}

Vec3 operator-(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	return Vec3{ in_a.m_float32x4 - in_b.m_float32x4 };
#else
	return Vec3{ in_a.x - in_b.x, in_a.y - in_b.y, in_a.z - in_b.z };
#endif
}

Vec3 operator-(Arg_Vec3 in_a) {
#ifdef DEEP_USE_SSE
	return Vec3{ -in_a.m_float32x4 };
#else
	// 0-x to mimic SIMD version
	return Vec3{ 0.0f - in_a.x, 0.0f - in_a.y, 0.0f - in_a.z };
#endif
}

Vec3& Vec3::operator*=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE
	m_float32x4 *= in_other.m_float32x4;
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
#endif
	return *this;
}
Vec3 operator*(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	return Vec3{ in_a.m_float32x4 * in_b.m_float32x4 };
#else
	return Vec3{ in_a.x * in_b.x, in_a.y * in_b.y, in_a.z * in_b.z };
#endif
}

Vec3& Vec3::operator*=(float32 in_other) {
#ifdef DEEP_USE_SSE
	m_float32x4 *= in_other;
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
#endif
	return *this;
}

Vec3 operator*(Arg_Vec3 in_vec, float32 in_val) {
#ifdef DEEP_USE_SSE
	return Vec3{ in_vec.m_float32x4 * in_val };
#else
	return Vec3{ in_vec.x * in_val, in_vec.y * in_val, in_vec.z * in_val };
#endif
}

Vec3 operator*(float32 in_val, Arg_Vec3 in_vec) {
#ifdef DEEP_USE_SSE
	return Vec3{ in_val * in_vec.m_float32x4 };
#else
	return Vec3{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z };
#endif
}

Vec3& Vec3::operator/=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE
	Vec3::s_CheckW(in_other);
	m_float32x4 /= in_other.m_float32x4;
#else
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
#endif
	return *this;
}
Vec3 operator/(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	Vec3::s_CheckW(in_b);
	return Vec3{ in_a.m_float32x4 / in_b.m_float32x4 };
#else
	return Vec3{ in_a.x / in_b.x, in_a.y / in_b.y, in_a.z / in_b.z };
#endif
}

Vec3& Vec3::operator/=(float32 in_other) {
#ifdef DEEP_USE_SSE
	m_float32x4 /= in_other;
#else
	x /= in_other;
	y /= in_other;
	z /= in_other;
#endif
	return *this;
}

Vec3 operator/(Arg_Vec3 in_vec, float32 in_val) {
#ifdef DEEP_USE_SSE
	return Vec3{ in_vec.m_float32x4 / in_val };
#else
	return Vec3{ in_vec.x / in_val, in_vec.y / in_val, in_vec.z / in_val };
#endif
}

Vec3 operator/(float32 in_val, Arg_Vec3 in_vec) {
#ifdef DEEP_USE_SSE
	Vec3::s_CheckW(in_vec);
	return Vec3{ in_val / in_vec.m_float32x4 };
#else
	return Vec3{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z };
#endif
}

std::ostream& operator<<(std::ostream& in_os, const Vec3& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << ", " << in_vec.z << " }";
}

DEEP_NAMESPACE_END
