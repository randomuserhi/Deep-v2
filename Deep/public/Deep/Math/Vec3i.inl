#pragma once

#include "Deep/Simd/Int32x4.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec3i.h"
#include "Deep/Math/Vec4i.h"

DEEP_NAMESPACE_BEGIN

Vec3i::Vec3i(int32 in_x, int32 in_y, int32 in_z) :
	m_int32x4{ in_x, in_y, in_z, in_z } {}

Vec3i::Vec3i(Arg_Vec4i in_vec) :
	m_int32x4{ in_vec.m_int32x4 } {}

Vec3i::Vec3i(Int32x4 in_int32x4) :
	m_int32x4{ in_int32x4 } {}

float32 Vec3i::sqrdMagnitude() const {
	return static_cast<Vec3>(*this).sqrdMagnitude();
}

float32 Vec3i::magnitude() const {
	return static_cast<Vec3>(*this).magnitude();
}

int32 Vec3i::manhattanDistance() const {
	return x + y + z;
}

int32 Vec3i::Dot(Arg_Vec3i in_a, Arg_Vec3i in_b) {
#ifdef DEEP_USE_SSE4_1
	__m128i mul = _mm_mullo_epi32(in_a.m_int32x4, in_b.m_int32x4);
	__m128i sum = _mm_add_epi32(mul, _mm_srli_si128(mul, 4));
	sum = _mm_add_epi32(sum, _mm_srli_si128(mul, 8));
	return _mm_cvtsi128_si32(sum);
#else
	return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z;
#endif
}

Vec3i::operator Int32x4() const {
	return m_int32x4;
}

Vec3i::operator Vec3() const {
#ifdef DEEP_USE_SSE
	return Vec3{ _mm_cvtepi32_ps(m_int32x4) };
#else
	return Vec3{ static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z) };
#endif
}

constexpr int32& Vec3i::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int32& Vec3i::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Vec3i in_a, Arg_Vec3i in_b) {
#ifdef DEEP_USE_SSE4_1
	return (Int32x4::Equals(in_a.m_int32x4, in_b.m_int32x4).ToBooleanBitMask() & 0b111) != 0b111;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z;
#endif
}
bool operator==(Arg_Vec3i a, Arg_Vec3i b) {
	return !(a != b);
}

Vec3i& Vec3i::operator+=(Arg_Vec3i in_other) {
#ifdef DEEP_USE_SSE4_1
	m_int32x4 += in_other.m_int32x4;
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
#endif
	return *this;
}

Vec3i operator+(Arg_Vec3i in_a, Arg_Vec3i in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_a.m_int32x4 + in_b.m_int32x4;
	return result;
#else
	return Vec3i{ in_a.x + in_b.x, in_a.y + in_b.y, in_a.z + in_b.z };
#endif
}

Vec3i& Vec3i::operator-=(Arg_Vec3i in_other) {
#ifdef DEEP_USE_SSE4_1
	m_int32x4 -= in_other.m_int32x4;
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
#endif
	return *this;
}

Vec3i operator-(Arg_Vec3i in_a, Arg_Vec3i in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_a.m_int32x4 - in_b.m_int32x4;
	return result;
#else
	return Vec3i{ in_a.x - in_b.x, in_a.y - in_b.y, in_a.z - in_b.z };
#endif
}

Vec3i operator-(Arg_Vec3i in_a) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = -in_a.m_int32x4;
	return result;
#else
	// NOTE(randomuserhi): 0 - x to stay consistent with vectorised version
	return Vec3i{ 0 - in_a.x, 0 - in_a.y, 0 - in_a.z };
#endif
}

Vec3i& Vec3i::operator*=(Arg_Vec3i in_other) {
#ifdef DEEP_USE_SSE4_1
	m_int32x4 *= in_other.m_int32x4;
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
#endif
	return *this;
}
Vec3i operator*(Arg_Vec3i in_a, Arg_Vec3i in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_a.m_int32x4 * in_b.m_int32x4;
	return result;
#else
	return Vec3i{ in_a.x * in_b.x, in_a.y * in_b.y, in_a.z * in_b.z };
#endif
}

Vec3i& Vec3i::operator*=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_int32x4 *= in_other;
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
#endif
	return *this;
}

Vec3i operator*(Arg_Vec3i in_vec, int32 in_val) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_vec.m_int32x4 * in_val;
	return result;
#else
	return Vec3i{ in_vec.x * in_val, in_vec.y * in_val, in_vec.z * in_val };
#endif
}

Vec3i operator*(int32 in_val, Arg_Vec3i in_vec) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_val * in_vec.m_int32x4;
	return result;
#else
	return Vec3i{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z };
#endif
}

Vec3i& Vec3i::operator/=(Arg_Vec3i in_other) {
#ifdef DEEP_USE_SSE4_1
	m_int32x4 /= in_other.m_int32x4;
#else
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
#endif
	return *this;
}
Vec3i operator/(Arg_Vec3i in_a, Arg_Vec3i in_b) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_a.m_int32x4 / in_b.m_int32x4;
	return result;
#else
	return Vec3i{ in_a.x / in_b.x, in_a.y / in_b.y, in_a.z / in_b.z };
#endif
}

Vec3i& Vec3i::operator/=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_int32x4 /= in_other;
#else
	x /= in_other;
	y /= in_other;
	z /= in_other;
#endif
	return *this;
}

Vec3i operator/(Arg_Vec3i in_vec, int32 in_val) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_vec.m_int32x4 / in_val;
	return result;
#else
	return Vec3i{ in_vec.x / in_val, in_vec.y / in_val, in_vec.z / in_val };
#endif
}

Vec3i operator/(int32 in_val, Arg_Vec3i in_vec) {
#ifdef DEEP_USE_SSE4_1
	Vec3i result;
	result.m_int32x4 = in_val / in_vec.m_int32x4;
	return result;
#else
	return Vec3i{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z };
#endif
}

DEEP_NAMESPACE_END
