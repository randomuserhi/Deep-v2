#pragma once

#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec4i.h"
#include "Deep/Math/Vec4.h"

DEEP_NAMESPACE_BEGIN

Vec4::Vec4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	m_float32x4{ in_x, in_y, in_z, in_w } {}

Vec4::Vec4(Float32x4 in_float32x4) :
	m_float32x4{ in_float32x4 } {}

#ifdef DEEP_USE_SSE4_1
Vec4::Vec4(Arg_Vec3 in_xyz, float32 in_w) :
	m_float32x4{ _mm_blend_ps(in_xyz.m_float32x4, _mm_set1_ps(in_w), 8) } {}
#else
Vec4::Vec4(Arg_Vec3 in_xyz, float32 in_w) :
	x{ in_xyz.x }, y{ in_xyz.y }, z{ in_xyz.z }, w{ in_w } {}
#endif

Vec4& Vec4::Normalize() {
#ifdef DEEP_USE_SSE4_1
	Float32x4 magnitudeSqrd = _mm_dp_ps(m_float32x4, m_float32x4, 0xff);
	if (_mm_cvtss_f32(magnitudeSqrd) != 0.0f) m_float32x4 = _mm_div_ps(m_float32x4, _mm_sqrt_ps(magnitudeSqrd));
	return *this;
#else
	float32 magnitudeSqrd = sqrdMagnitude();
	if (magnitudeSqrd != 0.0f) *this /= Sqrt(magnitudeSqrd);
	return *this;
#endif
}
Vec4 Vec4::normalized() const {
	Vec4 v = *this;
	return v.Normalize();
}
bool Vec4::IsNormalized(float tolerance) const {
	return Abs(sqrdMagnitude() - 1.0f) <= tolerance;
}

float32 Vec4::sqrdMagnitude() const {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_dp_ps(m_float32x4, m_float32x4, 0xff));
#else
	// NOTE(randomuserhi): brackets to keep consistent with vectorised version
	return (x * x + y * y) + (z * z + w * w);
#endif
}
float32 Vec4::magnitude() const {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(m_float32x4, m_float32x4, 0xff)));
#else
	return Sqrt(sqrdMagnitude());
#endif
}

float32 Vec4::Dot(Arg_Vec4 in_a, Arg_Vec4 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_dp_ps(in_a.m_float32x4, in_b.m_float32x4, 0xff));
#else
	return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w;
#endif
}

Vec4 Vec4::Lerp(Arg_Vec4 in_a, Arg_Vec4 in_b, float32 in_t) {
	return (in_b - in_a) * in_t + in_a;
}

Vec4::operator Float32x4() const {
	return m_float32x4;
}

Vec4::operator Vec4i() const {
#ifdef DEEP_USE_SSE
	return Vec4i{ _mm_cvtps_epi32(m_float32x4) };
#else
	return Vec4i{ static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z), static_cast<int32>(w) };
#endif
}

constexpr float32& Vec4::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Vec4::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	return in_a.m_float32x4 != in_b.m_float32x4;
}
bool operator==(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	return !(in_a != in_b);
}

Vec4& Vec4::operator+=(Arg_Vec4 in_other) {
	m_float32x4 += in_other.m_float32x4;
	return *this;
}

Vec4 operator+(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.m_float32x4 = in_a.m_float32x4 + in_b.m_float32x4;
	return result;
}

Vec4& Vec4::operator-=(Arg_Vec4 in_other) {
	m_float32x4 -= in_other.m_float32x4;
	return *this;
}

Vec4 operator-(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.m_float32x4 = in_a.m_float32x4 - in_b.m_float32x4;
	return result;
}

Vec4 operator-(Arg_Vec4 in_a) {
	Vec4 result;
	result.m_float32x4 = -in_a.m_float32x4;
	return result;
}

Vec4& Vec4::operator*=(Arg_Vec4 in_other) {
	m_float32x4 *= in_other.m_float32x4;
	return *this;
}
Vec4 operator*(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.m_float32x4 = in_a.m_float32x4 * in_b.m_float32x4;
	return result;
}

Vec4& Vec4::operator*=(float32 in_other) {
	m_float32x4 *= in_other;
	return *this;
}

Vec4 operator*(Arg_Vec4 in_vec, float32 in_val) {
	Vec4 result;
	result.m_float32x4 = in_vec.m_float32x4 * in_val;
	return result;
}

Vec4 operator*(float32 in_val, Arg_Vec4 in_vec) {
	Vec4 result;
	result.m_float32x4 = in_val * in_vec.m_float32x4;
	return result;
}

Vec4& Vec4::operator/=(Arg_Vec4 in_other) {
	m_float32x4 /= in_other.m_float32x4;
	return *this;
}
Vec4 operator/(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.m_float32x4 = in_a.m_float32x4 / in_b.m_float32x4;
	return result;
}

Vec4& Vec4::operator/=(float32 in_other) {
	m_float32x4 /= in_other;
	return *this;
}

Vec4 operator/(Arg_Vec4 in_vec, float32 in_val) {
	Vec4 result;
	result.m_float32x4 = in_vec.m_float32x4 / in_val;
	return result;
}

Vec4 operator/(float32 in_val, Arg_Vec4 in_vec) {
	Vec4 result;
	result.m_float32x4 = in_val / in_vec.m_float32x4;
	return result;
}

std::ostream& operator<<(std::ostream& in_os, const Vec4& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << ", " << in_vec.z << ", " << in_vec.w << " }";
}

DEEP_NAMESPACE_END
