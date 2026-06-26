#pragma once

#include "Deep.h"
#include "Deep/Simd/Int64x2.h"
#include <smmintrin.h>

#if !defined(DEEP_USE_SSE)
	#include "Deep/Math/Ops.h"
#endif

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Int64x2::Int64x2(int64 in_x, int64 in_y) :
	m_internal{ _mm_set_epi64x(in_y, in_x) } {}
#else
Int64x2::Int64x2(int64 in_x, int64 in_y) :
	x{ in_x }, y{ in_y } {}
#endif

Int64x2::Int64x2(Type in_internal) :
	m_internal(in_internal) {}

constexpr Int64x2 Int64x2::Constexpr(int64 in_x, int64 in_y) {
	Int64x2 xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	return xmmi;
}

Int64x2::operator Type() const {
	return m_internal;
}

Int64x2 Int64x2::Replicate(int64 in_value) {
#ifdef DEEP_USE_SSE
	return _mm_set1_epi64x(in_value);
#else
	return Int64x2{ in_value, in_value };
#endif
}

int32 Int64x2::ToBooleanBitMask() const {
#ifdef DEEP_USE_SSE
	return _mm_movemask_pd(_mm_castsi128_pd(m_internal));
#else
	return (x >> 63) | ((y >> 63) << 1);
#endif
}

Int64x2 Int64x2::Equals(Arg_Int64x2 in_a, Arg_Int64x2 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_cmpeq_epi64(in_a, in_b);
#else
	return Int64x2{ in_a.x == in_b.x ? int64(0xffffffffffffffffull) : 0,
		            in_a.y == in_b.y ? int64(0xffffffffffffffffull) : 0 };
#endif
}

constexpr int64& Int64x2::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int64& Int64x2::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Int64x2 in_a, Arg_Int64x2 in_b) {
#ifdef DEEP_USE_SSE4_1
	return Int64x2::Equals(in_a, in_b).ToBooleanBitMask() != 0b11;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y;
#endif
}
bool operator==(Arg_Int64x2 in_a, Arg_Int64x2 in_b) {
	return !(in_a != in_b);
}

Int64x2& Int64x2::operator<<=(int32 in_count) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_slli_epi64(m_internal, in_count);
#else
	m_internal = Int64x2{ x << in_count, y << in_count };
#endif
	return *this;
}
Int64x2 operator<<(Int64x2 in_a, int32 in_count) {
	return in_a <<= in_count;
}

Int64x2& Int64x2::operator>>=(int32 in_count) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_srli_epi64(m_internal, in_count);
#else
	m_internal = Int64x2{ x >> in_count, y >> in_count };
#endif
	return *this;
}
Int64x2 operator>>(Int64x2 in_a, int32 in_count) {
	return in_a >>= in_count;
}

Int64x2& Int64x2::operator|=(Arg_Int64x2 in_other) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_or_si128(m_internal, in_other);
#else
	m_internal = Int64x2{ x | in_other.x, y | in_other.y };
#endif
	return *this;
}
Int64x2 operator|(Int64x2 in_a, Arg_Int64x2 in_b) {
	return in_a |= in_b;
}

Int64x2& Int64x2::operator&=(Arg_Int64x2 in_other) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_and_si128(m_internal, in_other);
#else
	m_internal = Int64x2{ x & in_other.x, y & in_other.y };
#endif
	return *this;
}
Int64x2 operator&(Int64x2 in_a, Arg_Int64x2 in_b) {
	return in_a &= in_b;
}

Int64x2& Int64x2::operator^=(Arg_Int64x2 in_other) {
#ifdef DEEP_USE_SSE
	m_internal = _mm_xor_si128(m_internal, in_other);
#else
	m_internal = Int64x2{ x ^ in_other.x, y ^ in_other.y };
#endif
	return *this;
}
Int64x2 operator^(Int64x2 in_a, Arg_Int64x2 in_b) {
	return in_a ^= in_b;
}

DEEP_NAMESPACE_END
