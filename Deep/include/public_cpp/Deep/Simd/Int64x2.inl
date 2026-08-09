#pragma once

#include "./Int64x2.h"
#include "Deep/Simd/UInt64x2.h"
#include "Deep/Simd/SimdArgs.h"

#if !defined(DEEP_USE_SSE2)
	#include "Deep/Math/Ops.h"
#endif

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE2
Int64x2::Int64x2(int64 in_x, int64 in_y) :
	m_internal{ _mm_set_epi64x(in_y, in_x) } {}
#elif defined(DEEP_USE_WASM_SIMD128)
Int64x2::Int64x2(int64 in_x, int64 in_y) :
	m_internal{ wasm_i64x2_make(in_x, in_y) } {}
#else
Int64x2::Int64x2(int64 in_x, int64 in_y) :
	x{ in_x }, y{ in_y } {}
#endif

Int64x2::Int64x2(Type in_internal) :
	m_internal(in_internal) {}

Int64x2::Int64x2(UInt64x2 in_unsigned) :
	m_internal(in_unsigned.m_internal) {}

constexpr Int64x2 Int64x2::Constexpr(int64 in_x, int64 in_y) {
	Int64x2 xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	return xmmi;
}

Int64x2::operator Type() const {
	return m_internal;
}

Int64x2 Int64x2::s_Replicate(int64 in_value) {
#ifdef DEEP_USE_SSE2
	return _mm_set1_epi64x(in_value);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i64x2_splat(in_value);
#else
	return Int64x2{ in_value, in_value };
#endif
}

uint32 Int64x2::ToBooleanBitMask() const {
#ifdef DEEP_USE_SSE2
	return _mm_movemask_pd(_mm_castsi128_pd(m_internal));
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i64x2_bitmask(m_internal);
#else
	return (static_cast<uint64>(x) >> 63) | ((static_cast<uint64>(y) >> 63) << 1);
#endif
}

Int64x2 Int64x2::s_Equals(Arg_Int64x2 in_a, Arg_Int64x2 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_cmpeq_epi64(in_a, in_b);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i64x2_eq(in_a, in_b);
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
	return Int64x2::s_Equals(in_a, in_b).ToBooleanBitMask() != 0b11u;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y;
#endif
}
bool operator==(Arg_Int64x2 in_a, Arg_Int64x2 in_b) {
	return !(in_a != in_b);
}

Int64x2& Int64x2::operator<<=(int32 in_count) {
	Deep_Assert(in_count >= 0 && in_count < 64, "Invalid shift amount.");
#ifdef DEEP_USE_SSE2
	m_internal = _mm_slli_epi64(m_internal, in_count);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i64x2_shl(m_internal, in_count);
#else
	m_internal = Int64x2{ x << in_count, y << in_count };
#endif
	return *this;
}
Int64x2 operator<<(Int64x2 in_a, int32 in_count) {
	return in_a <<= in_count;
}

Int64x2& Int64x2::operator>>=(int32 in_count) {
	Deep_Assert(in_count >= 0 && in_count < 64, "Invalid shift amount.");
#ifdef DEEP_USE_AVX512
	m_internal = _mm_srai_epi64(m_internal, in_count);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i64x2_shr(m_internal, in_count);
#else
	m_internal = Int64x2{ x >> in_count, y >> in_count };
#endif
	return *this;
}
Int64x2 operator>>(Int64x2 in_a, int32 in_count) {
	return in_a >>= in_count;
}

Int64x2& Int64x2::operator|=(Arg_Int64x2 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_or_si128(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_v128_or(m_internal, in_other);
#else
	m_internal = Int64x2{ x | in_other.x, y | in_other.y };
#endif
	return *this;
}
Int64x2 operator|(Int64x2 in_a, Arg_Int64x2 in_b) {
	return in_a |= in_b;
}

Int64x2& Int64x2::operator&=(Arg_Int64x2 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_and_si128(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_v128_and(m_internal, in_other);
#else
	m_internal = Int64x2{ x & in_other.x, y & in_other.y };
#endif
	return *this;
}
Int64x2 operator&(Int64x2 in_a, Arg_Int64x2 in_b) {
	return in_a &= in_b;
}

Int64x2& Int64x2::operator^=(Arg_Int64x2 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_xor_si128(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_v128_xor(m_internal, in_other);
#else
	m_internal = Int64x2{ x ^ in_other.x, y ^ in_other.y };
#endif
	return *this;
}
Int64x2 operator^(Int64x2 in_a, Arg_Int64x2 in_b) {
	return in_a ^= in_b;
}

Int64x2 operator~(Int64x2 in_value) {
#ifdef DEEP_USE_SSE2
	in_value.m_internal = _mm_xor_si128(in_value.m_internal, _mm_set1_epi64x(int64{ -1 }));
#elif defined(DEEP_USE_WASM_SIMD128)
	in_value.m_internal = wasm_v128_not(in_value.m_internal);
#else
	in_value.x = ~in_value.x;
	in_value.y = ~in_value.y;
#endif
	return in_value;
}

DEEP_NAMESPACE_END
