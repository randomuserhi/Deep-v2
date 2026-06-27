#pragma once

#include "Deep/Simd/Int32x2.h"
#include "Deep/Simd/Int32x4.h"
#include "Deep/Simd/Float32x4.h"
#include "Deep/Bit.h"
#include <smmintrin.h>

#if !defined(DEEP_USE_SSE)
	#include "Deep/Math/Ops.h"
#endif

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Int32x4::Int32x4(int32 in_x, int32 in_y, int32 in_z, int32 in_w) :
	m_internal{ _mm_set_epi32(in_w, in_z, in_y, in_x) } {}
Int32x4::Int32x4(Int32x2 in_low) :
	m_internal{ _mm_set_epi32(0, 0, in_low.y, in_low.x) } {}
Int32x4::Int32x4(Int32x2 in_low, Int32x2 in_high) :
	m_internal{ _mm_set_epi32(in_high.y, in_high.x, in_low.y, in_low.x) } {}
#else
Int32x4::Int32x4(int32 in_x, int32 in_y, int32 in_z, int32 in_w) :
	x{ in_x }, y{ in_y }, z{ in_z }, w{ in_w } {}
Int32x4::Int32x4(Int32x2 in_low) :
	x{ in_low.x }, y{ in_low.y }, z{ 0 }, w{ 0 } {}
Int32x4::Int32x4(Int32x2 in_low, Int32x2 in_high) :
	x{ in_low.x }, y{ in_low.y }, z{ in_high.x }, w{ in_high.y } {}
#endif

Int32x4::Int32x4(Type in_internal) :
	m_internal(in_internal) {}

constexpr Int32x4 Int32x4::Constexpr(int32 in_x, int32 in_y, int32 in_z, int32 in_w) {
	Int32x4 xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	xmmi.z = in_z;
	xmmi.w = in_w;
	return xmmi;
}

Int32x4::operator Type() const {
	return m_internal;
}

Float32x4 Int32x4::ToFloat() const {
#ifdef DEEP_USE_SSE
	return _mm_cvtepi32_ps(m_internal);
#else
	return Float32x4{ static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z), static_cast<float32>(w) };
#endif
}
constexpr Float32x4 Int32x4::Constexpr_ToFloat() const {
	Float32x4 xmm;
	xmm.x = static_cast<float32>(x);
	xmm.y = static_cast<float32>(y);
	xmm.z = static_cast<float32>(z);
	xmm.w = static_cast<float32>(w);
	return xmm;
}

Float32x4 Int32x4::ReinterpretAsFloat() const {
#ifdef DEEP_USE_SSE
	return _mm_castsi128_ps(m_internal);
#else
	return BitCast<Float32x4>(*this);
#endif
}
constexpr Float32x4 Int32x4::Constexpr_ReinterpretAsFloat() const {
	return BitCast<Float32x4>(*this);
}

Int32x4 Int32x4::s_Replicate(int32 in_value) {
#ifdef DEEP_USE_SSE
	return _mm_set1_epi32(in_value);
#else
	return Int32x4{ in_value, in_value, in_value, in_value };
#endif
}

int32 Int32x4::ToBooleanBitMask() const {
#ifdef DEEP_USE_SSE
	return _mm_movemask_ps(_mm_castsi128_ps(m_internal));
#else
	return (x >> 31) | ((y >> 31) << 1) | ((z >> 31) << 2) | ((w >> 31) << 3);
#endif
}

Int32x4 Int32x4::s_Min(Arg_Int32x4 in_a, Arg_Int32x4 in_b) {
#ifdef DEEP_USE_SSE
	return _mm_min_epi32(in_a, in_b);
#else
	return Int32x4{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y), //
		Deep::Min(in_a.z, in_b.z), //
		Deep::Min(in_a.w, in_b.w)  //
	};
#endif
}

Int32x4 Int32x4::s_Max(Arg_Int32x4 in_a, Arg_Int32x4 in_b) {
#ifdef DEEP_USE_SSE
	return _mm_max_epi32(in_a, in_b);
#else
	return Int32x4{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y), //
		Deep::Max(in_a.z, in_b.z), //
		Deep::Max(in_a.w, in_b.w)  //
	};
#endif
}

Int32x4 Int32x4::s_Equals(Arg_Int32x4 in_a, Arg_Int32x4 in_b) {
#ifdef DEEP_USE_SSE
	return _mm_cmpeq_epi32(in_a, in_b);
#else
	return Int32x4{ in_a.x == in_b.x ? int32(0xffffffff) : 0, in_a.y == in_b.y ? int32(0xffffffff) : 0,
		            in_a.z == in_b.z ? int32(0xffffffff) : 0, in_a.w == in_b.w ? int32(0xffffffff) : 0 };
#endif
}

Int32x4 Int32x4::s_Select(Arg_Int32x4 in_a, Arg_Int32x4 in_b, Arg_Int32x4 in_control) {
#ifdef DEEP_USE_SSE4_1
	return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(in_a), _mm_castsi128_ps(in_b), _mm_castsi128_ps(in_control)));
#else
	return Int32x4{ in_control.x ? in_b.x : in_a.x, in_control.y ? in_b.y : in_a.y, in_control.z ? in_b.z : in_a.z,
		            in_control.w ? in_b.w : in_a.w };
#endif
}

constexpr int32& Int32x4::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int32& Int32x4::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Int32x4 in_a, Arg_Int32x4 in_b) {
#ifdef DEEP_USE_SSE4_1
	return Int32x4::s_Equals(in_a, in_b).ToBooleanBitMask() != 0b1111;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z || in_a.w != in_b.w;
#endif
}
bool operator==(Arg_Int32x4 in_a, Arg_Int32x4 in_b) {
	return !(in_a != in_b);
}

Int32x4& Int32x4::operator<<=(int32 in_count) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_slli_epi32(m_internal, in_count);
#else
	m_internal = Int32x4{ x << in_count, y << in_count, z << in_count, w << in_count };
#endif
	return *this;
}
Int32x4 operator<<(Int32x4 in_a, int32 in_count) {
	return in_a <<= in_count;
}

Int32x4& Int32x4::operator>>=(int32 in_count) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_srli_epi32(m_internal, in_count);
#else
	m_internal = Int32x4{ x >> in_count, y >> in_count, z >> in_count, w >> in_count };
#endif
	return *this;
}
Int32x4 operator>>(Int32x4 in_a, int32 in_count) {
	return in_a >>= in_count;
}

Int32x4& Int32x4::operator|=(Arg_Int32x4 in_other) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_or_si128(m_internal, in_other);
#else
	m_internal = Int32x4{ x | in_other.x, y | in_other.y, z | in_other.z, w | in_other.w };
#endif
	return *this;
}
Int32x4 operator|(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a |= in_b;
}

Int32x4& Int32x4::operator&=(Arg_Int32x4 in_other) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_and_si128(m_internal, in_other);
#else
	m_internal = Int32x4{ x & in_other.x, y & in_other.y, z & in_other.z, w & in_other.w };
#endif
	return *this;
}
Int32x4 operator&(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a &= in_b;
}

Int32x4& Int32x4::operator^=(Arg_Int32x4 in_other) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_xor_si128(m_internal, in_other);
#else
	m_internal = Int32x4{ x ^ in_other.x, y ^ in_other.y, z ^ in_other.z, w ^ in_other.w };
#endif
	return *this;
}
Int32x4 operator^(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a ^= in_b;
}

Int32x4& Int32x4::operator+=(Arg_Int32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_internal = _mm_add_epi32(m_internal, in_other);
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
	w += in_other.w;
#endif
	return *this;
}

Int32x4 operator+(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a += in_b;
}

Int32x4& Int32x4::operator-=(Arg_Int32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_internal = _mm_sub_epi32(m_internal, in_other);
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
	w -= in_other.w;
#endif
	return *this;
}

Int32x4 operator-(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a -= in_b;
}

Int32x4 operator-(Arg_Int32x4 in_a) {
#ifdef DEEP_USE_SSE4_1
	return _mm_sub_epi32(_mm_setzero_si128(), in_a);
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Int32x4{ 0 - in_a.x, 0 - in_a.y, 0 - in_a.z, 0 - in_a.w };
#endif
}

Int32x4& Int32x4::operator*=(Arg_Int32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_internal = _mm_mullo_epi32(m_internal, in_other);
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
	w *= in_other.w;
#endif
	return *this;
}
Int32x4 operator*(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a *= in_b;
}

Int32x4& Int32x4::operator*=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_internal = _mm_mullo_epi32(m_internal, _mm_set1_epi32(in_other));
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
	w *= in_other;
#endif
	return *this;
}

Int32x4 operator*(Int32x4 in_vec, int32 in_val) {
	return in_vec *= in_val;
}

Int32x4 operator*(int32 in_val, Arg_Int32x4 in_vec) {
#ifdef DEEP_USE_SSE4_1
	return _mm_mullo_epi32(_mm_set1_epi32(in_val), in_vec);
#else
	return Int32x4{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z, in_val * in_vec.w };
#endif
}

Int32x4& Int32x4::operator/=(Arg_Int32x4 in_other) {
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
	w /= in_other.w;
	return *this;
}
Int32x4 operator/(Int32x4 in_a, Arg_Int32x4 in_b) {
	return in_a /= in_b;
}

Int32x4& Int32x4::operator/=(int32 in_other) {
	x /= in_other;
	y /= in_other;
	z /= in_other;
	w /= in_other;
	return *this;
}

Int32x4 operator/(Int32x4 in_vec, int32 in_val) {
	return in_vec /= in_val;
}

Int32x4 operator/(int32 in_val, Arg_Int32x4 in_vec) {
	return Int32x4{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z, in_val / in_vec.w };
}

DEEP_NAMESPACE_END
