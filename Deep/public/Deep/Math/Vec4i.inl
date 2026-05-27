#pragma once

#include "Deep/Math/Vec4i.h"
#include "Deep/Math/Vec4.h"

DEEP_NAMESPACE_BEGIN

Vec4i::Vec4i(int32 in_x, int32 in_y, int32 in_z, int32 in_w) :
	xmmi{ in_x, in_y, in_z, in_w } {}

Vec4i::Vec4i(Xmmi in_xmmi) :
	xmmi{ in_xmmi } {}

#ifdef DEEP_USE_SSE4_1
Vec4i::Vec4i(Arg_Vec3i in_xyz, int32 in_w) :
	xmmi{ _mm_blend_epi32(in_xyz.xmmi, _mm_set1_epi32(in_w), 8) } {}
#else
Vec4i::Vec4i(Arg_Vec3i in_xyz, int32 in_w) :
	x{ in_xyz.x }, y{ in_xyz.y }, z{ in_xyz.z }, w{ in_w } {}
#endif

float32 Vec4i::sqrdMagnitude() const {
	return static_cast<Vec4>(*this).sqrdMagnitude();
}

float32 Vec4i::magnitude() const {
	return static_cast<Vec4>(*this).magnitude();
}

int32 Vec4i::manhattanDistance() const {
#ifdef DEEP_USE_SSE4_1
	// NOTE(randomuserhi): Not even sure if SIMD horizontal sum here is worth. Might be better off without
	//                     Profiling required.
	Xmmi sum = _mm_add_epi32(xmmi, _mm_shuffle_epi32(xmmi, _MM_SHUFFLE(2, 3, 0, 1)));
	sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(1, 0, 3, 2)));
	return _mm_cvtsi128_si32(sum);
#else
	return x + y + z + w;
#endif
}

int32 Vec4i::Dot(Arg_Vec4i in_a, Arg_Vec4i in_b) {
#ifdef DEEP_USE_SSE4_1
	Xmmi sum = _mm_mullo_epi32(in_a.xmmi, in_b.xmmi);

	// NOTE(randomuserhi): Not even sure if SIMD horizontal sum here is worth. Might be better off without
	//                     Profiling required.
	sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(2, 3, 0, 1)));
	sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(1, 0, 3, 2)));
	return _mm_cvtsi128_si32(sum);
#else
	return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w;
#endif
}

Vec4i::operator Vec4() const {
#ifdef DEEP_USE_SSE
	return Vec4{ _mm_cvtepi32_ps(xmmi) };
#else
	return Vec4{ static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z), static_cast<float32>(w) };
#endif
}

bool operator!=(Arg_Vec4i in_a, Arg_Vec4i in_b) {
	return in_a.xmmi != in_b.xmmi;
}
bool operator==(Arg_Vec4i in_a, Arg_Vec4i in_b) {
	return !(in_a != in_b);
}

Vec4i& Vec4i::operator+=(Arg_Vec4i in_other) {
	xmmi += in_other.xmmi;
	return *this;
}

Vec4i operator+(Arg_Vec4i in_a, Arg_Vec4i in_b) {
	Vec4i result;
	result.xmmi = in_a.xmmi + in_b.xmmi;
	return result;
}

Vec4i& Vec4i::operator-=(Arg_Vec4i in_other) {
	xmmi -= in_other.xmmi;
	return *this;
}

Vec4i operator-(Arg_Vec4i in_a, Arg_Vec4i in_b) {
	Vec4i result;
	result.xmmi = in_a.xmmi - in_b.xmmi;
	return result;
}

Vec4i operator-(Arg_Vec4i in_a) {
	Vec4i result;
	result.xmmi = -in_a.xmmi;
	return result;
}

Vec4i& Vec4i::operator*=(Arg_Vec4i in_other) {
	xmmi *= in_other.xmmi;
	return *this;
}
Vec4i operator*(Arg_Vec4i in_a, Arg_Vec4i in_b) {
	Vec4i result;
	result.xmmi = in_a.xmmi * in_b.xmmi;
	return result;
}

Vec4i& Vec4i::operator*=(int32 in_other) {
	xmmi *= in_other;
	return *this;
}

Vec4i operator*(Arg_Vec4i in_vec, int32 in_val) {
	Vec4i result;
	result.xmmi = in_vec.xmmi * in_val;
	return result;
}

Vec4i operator*(int32 in_val, Arg_Vec4i in_vec) {
	Vec4i result;
	result.xmmi = in_val * in_vec.xmmi;
	return result;
}

Vec4i& Vec4i::operator/=(Arg_Vec4i in_other) {
	xmmi /= in_other.xmmi;
	return *this;
}
Vec4i operator/(Arg_Vec4i in_a, Arg_Vec4i in_b) {
	Vec4i result;
	result.xmmi = in_a.xmmi / in_b.xmmi;
	return result;
}

Vec4i& Vec4i::operator/=(int32 in_other) {
	xmmi /= in_other;
	return *this;
}

Vec4i operator/(Arg_Vec4i in_vec, int32 in_val) {
	Vec4i result;
	result.xmmi = in_vec.xmmi / in_val;
	return result;
}

Vec4i operator/(int32 in_val, Arg_Vec4i in_vec) {
	Vec4i result;
	result.xmmi = in_val / in_vec.xmmi;
	return result;
}

DEEP_NAMESPACE_END
