#pragma once

#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec4i.h"
#include "Deep/Math/Vec4.h"

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Vec4::Vec4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	xmm(in_x, in_y, in_z, in_w) {}
#else
Vec4::Vec4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	x(in_x), y(in_y), z(in_z), w(in_w) {}
#endif

Vec4::Vec4(Xmm in_xmm) :
	xmm(in_xmm) {}

#ifdef DEEP_USE_SSE4_1
Vec4::Vec4(Arg_Vec3 in_xyz, float32 in_w) :
	xmm(_mm_blend_ps(in_xyz.xmm, _mm_set1_ps(in_w), 8)) {}
#else
Vec4::Vec4(Arg_Vec3 in_xyz, float32 in_w) :
	x(in_xyz.x), y(in_xyz.y), z(in_xyz.z), w(in_w) {}
#endif

Vec4& Vec4::Normalize() {
#ifdef DEEP_USE_SSE4_1
	Xmm magnitudeSqrd = _mm_dp_ps(xmm, xmm, 0xff);
	if (_mm_cvtss_f32(magnitudeSqrd) != 0.0f) xmm = _mm_div_ps(xmm, _mm_sqrt_ps(magnitudeSqrd));
	return *this;
#else
	float32 magnitudeSqrd = sqrdMagnitude();
	if (magnitudeSqrd != 0.0) *this /= Deep::Sqrt(magnitudeSqrd);
	return *this;
#endif
}
Vec4 Vec4::normalized() const {
	Vec4 v = *this;
	return v.Normalize();
}
bool Vec4::IsNormalized(float tolerance) const {
	return Deep::Abs(sqrdMagnitude() - 1.0f) <= tolerance;
}

float32 Vec4::sqrdMagnitude() const {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_dp_ps(xmm, xmm, 0xff));
#else
	// NOTE(randomuserhi): brackets to keep consistent with vectorised version
	return (x * x + y * y) + (z * z + w * w);
#endif
}
float32 Vec4::magnitude() const {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(xmm, xmm, 0xff)));
#else
	return Deep::Sqrt(sqrdMagnitude());
#endif
}

float32 Vec4::Dot(Arg_Vec4 in_a, Arg_Vec4 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_cvtss_f32(_mm_dp_ps(in_a.xmm, in_b.xmm, 0xff));
#else
	return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w;
#endif
}

Vec4 Vec4::Lerp(Arg_Vec4 in_a, Arg_Vec4 in_b, float32 in_t) {
	return (in_b - in_a) * in_t + in_a;
}

Vec4::operator Vec4i() const {
#ifdef DEEP_USE_SSE
	return Vec4i{ _mm_cvtps_epi32(xmm) };
#else
	return Vec4i{ static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z), static_cast<int32>(w) };
#endif
}

Deep_Inline bool operator!=(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	return in_a.xmm != in_b.xmm;
}
Deep_Inline bool operator==(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	return !(in_a != in_b);
}

Vec4& Vec4::operator+=(Arg_Vec4 in_other) {
	xmm += in_other.xmm;
	return *this;
}

Vec4 operator+(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.xmm = in_a.xmm + in_b.xmm;
	return result;
}

Vec4& Vec4::operator-=(Arg_Vec4 in_other) {
	xmm -= in_other.xmm;
	return *this;
}

Vec4 operator-(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.xmm = in_a.xmm - in_b.xmm;
	return result;
}

Vec4 operator-(Arg_Vec4 in_a) {
	Vec4 result;
	result.xmm = -in_a.xmm;
	return result;
}

Vec4& Vec4::operator*=(Arg_Vec4 in_other) {
	xmm *= in_other.xmm;
	return *this;
}
Vec4 operator*(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.xmm = in_a.xmm * in_b.xmm;
	return result;
}

Vec4& Vec4::operator*=(float32 in_other) {
	xmm *= in_other;
	return *this;
}

Vec4 operator*(Arg_Vec4 in_vec, float32 in_val) {
	Vec4 result;
	result.xmm = in_vec.xmm * in_val;
	return result;
}

Vec4 operator*(float32 in_val, Arg_Vec4 in_vec) {
	Vec4 result;
	result.xmm = in_val * in_vec.xmm;
	return result;
}

Vec4& Vec4::operator/=(Arg_Vec4 in_other) {
	xmm /= in_other.xmm;
	return *this;
}
Vec4 operator/(Arg_Vec4 in_a, Arg_Vec4 in_b) {
	Vec4 result;
	result.xmm = in_a.xmm / in_b.xmm;
	return result;
}

Vec4& Vec4::operator/=(float32 in_other) {
	xmm /= in_other;
	return *this;
}

Vec4 operator/(Arg_Vec4 in_vec, float32 in_val) {
	Vec4 result;
	result.xmm = in_vec.xmm / in_val;
	return result;
}

Vec4 operator/(float32 in_val, Arg_Vec4 in_vec) {
	Vec4 result;
	result.xmm = in_val / in_vec.xmm;
	return result;
}

DEEP_NAMESPACE_END