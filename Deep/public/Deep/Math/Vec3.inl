#pragma once

#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec3i.h"
#include "Deep/Math/Vec4.h"

DEEP_NAMESPACE_BEGIN

Vec3::Vec3(float32 in_x, float32 in_y, float32 in_z) :
	xmm{ in_x, in_y, in_z, 0 } {}

Vec3::Vec3(Arg_Vec4 in_vec) :
	xmm{ in_vec.xmm } {}

Vec3::Vec3(Xmm in_xmm) :
	xmm{ in_xmm } {}

Vec3& Vec3::Normalize() {
#ifdef DEEP_USE_SSE4_1
	Xmm magnitudeSqrd = _mm_dp_ps(xmm, xmm, 0x7f);
	if (_mm_cvtss_f32(magnitudeSqrd) != 0.0f) xmm = _mm_div_ps(xmm, _mm_sqrt_ps(magnitudeSqrd));
	return *this;
#else
	float32 magnitudeSqrd = sqrdMagnitude();
	if (magnitudeSqrd != 0.0f) *this /= Sqrt(magnitudeSqrd);
	return *this;
#endif
}
Vec3 Vec3::normalized() const {
	Vec3 v = *this;
	return v.Normalize();
}
bool Vec3::IsNormalized(float tolerance) const {
	return Abs(sqrdMagnitude() - 1.0f) <= tolerance;
}

float32 Vec3::sqrdMagnitude() const {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_dp_ps(xmm, xmm, 0x7f));
#else
	return x * x + y * y + z * z;
#endif
}
float32 Vec3::magnitude() const {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(xmm, xmm, 0x7f)));
#else
	return Sqrt(sqrdMagnitude());
#endif
}

float32 Vec3::Dot(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_dp_ps(in_a.xmm, in_b.xmm, 0x7f));
#else
	return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z;
#endif
}

Vec3 Vec3::Cross(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE
	Xmm t1 = _mm_shuffle_ps(in_b.xmm, in_b.xmm, _MM_SHUFFLE(0, 0, 2, 1)); // Assure Z and W are the same
	t1 = _mm_mul_ps(t1, in_a.xmm);
	Xmm t2 = _mm_shuffle_ps(in_a.xmm, in_a.xmm, _MM_SHUFFLE(0, 0, 2, 1)); // Assure Z and W are the same
	t2 = _mm_mul_ps(t2, in_b.xmm);
	Xmm t3 = _mm_sub_ps(t1, t2);
	return Vec3{ _mm_shuffle_ps(t3, t3, _MM_SHUFFLE(0, 0, 2, 1)) }; // Assure Z and W are the same
#else
	return Vec3{ in_a.m_values[1] * in_b.m_values[2] - in_a.m_values[2] * in_b.m_values[1],
		         in_a.m_values[2] * in_b.m_values[0] - in_a.m_values[0] * in_b.m_values[2],
		         in_a.m_values[0] * in_b.m_values[1] - in_a.m_values[1] * in_b.m_values[0] };
#endif
}

Vec3 Vec3::Lerp(Arg_Vec3 in_a, Arg_Vec3 in_b, float32 in_t) {
	return (in_b - in_a) * in_t + in_a;
}

Vec3::operator Vec3i() const {
#ifdef DEEP_USE_SSE
	return Vec3i{ _mm_cvtps_epi32(xmm) };
#else
	return Vec3i{ static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z) };
#endif
}

inline bool operator!=(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	return (Xmm::Equals(in_a.xmm, in_b.xmm).ToBooleanBitMask() & 0b111) != 0b111;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z;
#endif
}
inline bool operator==(Arg_Vec3 a, Arg_Vec3 b) {
	return !(a != b);
}

Vec3& Vec3::operator+=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE4_1
	xmm += in_other.xmm;
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
#endif
	return *this;
}

Vec3 operator+(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_a.xmm + in_b.xmm;
	return result;
#else
	return Vec3{ in_a.x + in_b.x, in_a.y + in_b.y, in_a.z + in_b.z };
#endif
}

Vec3& Vec3::operator-=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE4_1
	xmm -= in_other.xmm;
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
#endif
	return *this;
}

Vec3 operator-(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_a.xmm - in_b.xmm;
	return result;
#else
	return Vec3{ in_a.x - in_b.x, in_a.y - in_b.y, in_a.z - in_b.z };
#endif
}

Vec3 operator-(Arg_Vec3 in_a) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = -in_a.xmm;
	return result;
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Vec3{ 0.0f - in_a.x, 0.0f - in_a.y, 0.0f - in_a.z };
#endif
}

Vec3& Vec3::operator*=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE4_1
	xmm *= in_other.xmm;
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
#endif
	return *this;
}
Vec3 operator*(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_a.xmm * in_b.xmm;
	return result;
#else
	return Vec3{ in_a.x * in_b.x, in_a.y * in_b.y, in_a.z * in_b.z };
#endif
}

Vec3& Vec3::operator*=(float32 in_other) {
#ifdef DEEP_USE_SSE4_1
	xmm *= in_other;
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
#endif
	return *this;
}

Vec3 operator*(Arg_Vec3 in_vec, float32 in_val) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_vec.xmm * in_val;
	return result;
#else
	return Vec3{ in_vec.x * in_val, in_vec.y * in_val, in_vec.z * in_val };
#endif
}

Vec3 operator*(float32 in_val, Arg_Vec3 in_vec) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_val * in_vec.xmm;
	return result;
#else
	return Vec3{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z };
#endif
}

Vec3& Vec3::operator/=(Arg_Vec3 in_other) {
#ifdef DEEP_USE_SSE4_1
	xmm /= in_other.xmm;
#else
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
#endif
	return *this;
}
Vec3 operator/(Arg_Vec3 in_a, Arg_Vec3 in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_a.xmm / in_b.xmm;
	return result;
#else
	return Vec3{ in_a.x / in_b.x, in_a.y / in_b.y, in_a.z / in_b.z };
#endif
}

Vec3& Vec3::operator/=(float32 in_other) {
#ifdef DEEP_USE_SSE4_1
	xmm /= in_other;
#else
	x /= in_other;
	y /= in_other;
	z /= in_other;
#endif
	return *this;
}

Vec3 operator/(Arg_Vec3 in_vec, float32 in_val) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_vec.xmm / in_val;
	return result;
#else
	return Vec3{ in_vec.x / in_val, in_vec.y / in_val, in_vec.z / in_val };
#endif
}

Vec3 operator/(float32 in_val, Arg_Vec3 in_vec) {
#ifdef DEEP_USE_SSE4_1
	Vec3 result;
	result.xmm = in_val / in_vec.xmm;
	return result;
#else
	return Vec3{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z };
#endif
}

std::ostream& operator<<(std::ostream& in_os, const Vec3& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << ", " << in_vec.z << " }";
}

DEEP_NAMESPACE_END
