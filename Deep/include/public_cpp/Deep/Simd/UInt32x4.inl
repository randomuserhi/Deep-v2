#pragma once

#include "./UInt32x4.h"

#include "Deep/Simd/Int32x4.h"
#include "Deep/Simd/UInt32x2.h"
#include "Deep/Simd/Float32x4.h"
#include "Deep/Bit.h"

#if !defined(DEEP_USE_SSE2)
	#include "Deep/Math/Ops.h"
#endif

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE2
UInt32x4::UInt32x4(uint32 in_x, uint32 in_y, uint32 in_z, uint32 in_w) :
	m_internal{ _mm_set_epi32(static_cast<int32>(in_w), static_cast<int32>(in_z), static_cast<int32>(in_y),
	                          static_cast<int32>(in_x)) } {}
UInt32x4::UInt32x4(UInt32x2 in_low) :
	m_internal{ _mm_set_epi32(0, 0, static_cast<int32>(in_low.y), static_cast<int32>(in_low.x)) } {}
UInt32x4::UInt32x4(UInt32x2 in_low, UInt32x2 in_high) :
	m_internal{ _mm_set_epi32(static_cast<int32>(in_high.y), static_cast<int32>(in_high.x), static_cast<int32>(in_low.y),
	                          static_cast<int32>(in_low.x)) } {}
#elif defined(DEEP_USE_WASM_SIMD128)
UInt32x4::UInt32x4(uint32 in_x, uint32 in_y, uint32 in_z, uint32 in_w) :
	m_internal{ wasm_u32x4_make(in_x, in_y, in_z, in_w) } {}
UInt32x4::UInt32x4(UInt32x2 in_low) :
	m_internal{ wasm_u32x4_make(in_low.x, in_low.y, 0, 0) } {}
UInt32x4::UInt32x4(UInt32x2 in_low, UInt32x2 in_high) :
	m_internal{ wasm_u32x4_make(in_low.x, in_low.y, in_high.x, in_high.y) } {}
#else
UInt32x4::UInt32x4(uint32 in_x, uint32 in_y, uint32 in_z, uint32 in_w) :
	x{ in_x }, y{ in_y }, z{ in_z }, w{ in_w } {}
UInt32x4::UInt32x4(UInt32x2 in_low) :
	x{ in_low.x }, y{ in_low.y }, z{ 0 }, w{ 0 } {}
UInt32x4::UInt32x4(UInt32x2 in_low, UInt32x2 in_high) :
	x{ in_low.x }, y{ in_low.y }, z{ in_high.x }, w{ in_high.y } {}
#endif

UInt32x4::UInt32x4(Type in_internal) :
	m_internal(in_internal) {}

UInt32x4::UInt32x4(Int32x4 in_signed) :
	m_internal(in_signed.m_internal) {}

constexpr UInt32x4 UInt32x4::Constexpr(uint32 in_x, uint32 in_y, uint32 in_z, uint32 in_w) {
	UInt32x4 xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	xmmi.z = in_z;
	xmmi.w = in_w;
	return xmmi;
}

UInt32x4::operator Type() const {
	return m_internal;
}

Float32x4 UInt32x4::ToFloat() const {
#ifdef DEEP_USE_SSE2
	return _mm_cvtepu32_ps(m_internal);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_f32x4_convert_u32x4(m_internal);
#else
	return Float32x4{ static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z), static_cast<float32>(w) };
#endif
}
constexpr Float32x4 UInt32x4::Constexpr_ToFloat() const {
	Float32x4 xmm;
	xmm.x = static_cast<float32>(x);
	xmm.y = static_cast<float32>(y);
	xmm.z = static_cast<float32>(z);
	xmm.w = static_cast<float32>(w);
	return xmm;
}

Float32x4 UInt32x4::ReinterpretAsFloat() const {
#ifdef DEEP_USE_SSE2
	return _mm_castsi128_ps(m_internal);
#elif defined(DEEP_USE_WASM_SIMD128)
	return m_internal;
#else
	return BitCast<Float32x4>(*this);
#endif
}
constexpr Float32x4 UInt32x4::Constexpr_ReinterpretAsFloat() const {
	return BitCast<Float32x4>(*this);
}

UInt32x4 UInt32x4::s_Replicate(uint32 in_value) {
#ifdef DEEP_USE_SSE2
	return _mm_set1_epi32(static_cast<int32>(in_value));
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_u32x4_splat(in_value);
#else
	return UInt32x4{ in_value, in_value, in_value, in_value };
#endif
}

uint32 UInt32x4::ToBooleanBitMask() const {
#ifdef DEEP_USE_SSE2
	return _mm_movemask_ps(_mm_castsi128_ps(m_internal));
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i32x4_bitmask(m_internal);
#else
	return (static_cast<uint32>(x) >> 31) | ((static_cast<uint32>(y) >> 31) << 1) | ((static_cast<uint32>(z) >> 31) << 2)
	       | ((static_cast<uint32>(w) >> 31) << 3);
#endif
}

UInt32x4 UInt32x4::s_Min(Arg_UInt32x4 in_a, Arg_UInt32x4 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_min_epu32(in_a, in_b);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_u32x4_min(in_a, in_b);
#else
	return UInt32x4{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y), //
		Deep::Min(in_a.z, in_b.z), //
		Deep::Min(in_a.w, in_b.w)  //
	};
#endif
}

UInt32x4 UInt32x4::s_Max(Arg_UInt32x4 in_a, Arg_UInt32x4 in_b) {
#ifdef DEEP_USE_SSE4_1
	return _mm_max_epu32(in_a, in_b);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_u32x4_max(in_a, in_b);
#else
	return UInt32x4{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y), //
		Deep::Max(in_a.z, in_b.z), //
		Deep::Max(in_a.w, in_b.w)  //
	};
#endif
}

UInt32x4 UInt32x4::s_Equals(Arg_UInt32x4 in_a, Arg_UInt32x4 in_b) {
#ifdef DEEP_USE_SSE2
	return _mm_cmpeq_epi32(in_a, in_b);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i32x4_eq(in_a, in_b);
#else
	return UInt32x4{ in_a.x == in_b.x ? 0xffffffffu : 0u, in_a.y == in_b.y ? 0xffffffffu : 0u,
		             in_a.z == in_b.z ? 0xffffffffu : 0u, in_a.w == in_b.w ? 0xffffffffu : 0u };
#endif
}

UInt32x4 UInt32x4::s_Select(Arg_UInt32x4 in_a, Arg_UInt32x4 in_b, Arg_Int32x4 in_control) {
#ifdef DEEP_USE_SSE4_1
	return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(in_a), _mm_castsi128_ps(in_b), _mm_castsi128_ps(in_control)));
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_v128_bitselect(in_b, in_a, wasm_i32x4_shr(in_control, 31));
#else
	return UInt32x4{ in_control.x < 0 ? in_b.x : in_a.x, in_control.y < 0 ? in_b.y : in_a.y,
		             in_control.z < 0 ? in_b.z : in_a.z, in_control.w < 0 ? in_b.w : in_a.w };
#endif
}

constexpr uint32& UInt32x4::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const uint32& UInt32x4::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_UInt32x4 in_a, Arg_UInt32x4 in_b) {
#if defined(DEEP_USE_SSE4_1) || defined(DEEP_USE_WASM_SIMD128)
	return UInt32x4::s_Equals(in_a, in_b).ToBooleanBitMask() != 0b1111u;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z || in_a.w != in_b.w;
#endif
}
bool operator==(Arg_UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return !(in_a != in_b);
}

UInt32x4& UInt32x4::operator<<=(int32 in_count) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_slli_epi32(m_internal, in_count);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i32x4_shl(m_internal, in_count);
#else
	m_internal = UInt32x4{ x << in_count, y << in_count, z << in_count, w << in_count };
#endif
	return *this;
}
UInt32x4 operator<<(UInt32x4 in_a, int32 in_count) {
	return in_a <<= in_count;
}

UInt32x4& UInt32x4::operator>>=(int32 in_count) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_srli_epi32(m_internal, in_count);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_u32x4_shr(m_internal, in_count);
#else
	m_internal = UInt32x4{ x >> in_count, y >> in_count, z >> in_count, w >> in_count };
#endif
	return *this;
}
UInt32x4 operator>>(UInt32x4 in_a, int32 in_count) {
	return in_a >>= in_count;
}

UInt32x4& UInt32x4::operator|=(Arg_UInt32x4 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_or_si128(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_v128_or(m_internal, in_other);
#else
	m_internal = UInt32x4{ x | in_other.x, y | in_other.y, z | in_other.z, w | in_other.w };
#endif
	return *this;
}
UInt32x4 operator|(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a |= in_b;
}

UInt32x4& UInt32x4::operator&=(Arg_UInt32x4 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_and_si128(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_v128_and(m_internal, in_other);
#else
	m_internal = UInt32x4{ x & in_other.x, y & in_other.y, z & in_other.z, w & in_other.w };
#endif
	return *this;
}
UInt32x4 operator&(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a &= in_b;
}

UInt32x4& UInt32x4::operator^=(Arg_UInt32x4 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_xor_si128(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_v128_xor(m_internal, in_other);
#else
	m_internal = UInt32x4{ x ^ in_other.x, y ^ in_other.y, z ^ in_other.z, w ^ in_other.w };
#endif
	return *this;
}
UInt32x4 operator^(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a ^= in_b;
}

UInt32x4 operator~(UInt32x4 in_value) {
#ifdef DEEP_USE_SSE2
	in_value.m_internal = _mm_xor_si128(in_value.m_internal, _mm_set1_epi32(int32(0xffffffff)));
#elif defined(DEEP_USE_WASM_SIMD128)
	in_value.m_internal = wasm_v128_not(in_value.m_internal);
#else
	in_value.x = ~in_value.x;
	in_value.y = ~in_value.y;
	in_value.z = ~in_value.z;
	in_value.w = ~in_value.w;
#endif
	return in_value;
}

UInt32x4& UInt32x4::operator+=(Arg_UInt32x4 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_add_epi32(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i32x4_add(m_internal, in_other);
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
	w += in_other.w;
#endif
	return *this;
}

UInt32x4 operator+(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a += in_b;
}

UInt32x4& UInt32x4::operator-=(Arg_UInt32x4 in_other) {
#ifdef DEEP_USE_SSE2
	m_internal = _mm_sub_epi32(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i32x4_sub(m_internal, in_other);
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
	w -= in_other.w;
#endif
	return *this;
}

UInt32x4 operator-(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a -= in_b;
}

UInt32x4 operator-(Arg_UInt32x4 in_a) {
#ifdef DEEP_USE_SSE4_1
	return _mm_sub_epi32(_mm_setzero_si128(), in_a);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i32x4_sub(wasm_i32x4_splat(0), in_a);
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return UInt32x4{ 0 - in_a.x, 0 - in_a.y, 0 - in_a.z, 0 - in_a.w };
#endif
}

UInt32x4& UInt32x4::operator*=(Arg_UInt32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_internal = _mm_mullo_epi32(m_internal, in_other);
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i32x4_mul(m_internal, in_other);
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
	w *= in_other.w;
#endif
	return *this;
}
UInt32x4 operator*(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a *= in_b;
}

UInt32x4& UInt32x4::operator*=(uint32 in_other) {
#ifdef DEEP_USE_SSE4_1
	m_internal = _mm_mullo_epi32(m_internal, _mm_set1_epi32(in_other));
#elif defined(DEEP_USE_WASM_SIMD128)
	m_internal = wasm_i32x4_mul(m_internal, wasm_i32x4_splat(in_other));
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
	w *= in_other;
#endif
	return *this;
}

UInt32x4 operator*(UInt32x4 in_vec, uint32 in_val) {
	return in_vec *= in_val;
}

UInt32x4 operator*(uint32 in_val, Arg_UInt32x4 in_vec) {
#ifdef DEEP_USE_SSE4_1
	return _mm_mullo_epi32(_mm_set1_epi32(in_val), in_vec);
#elif defined(DEEP_USE_WASM_SIMD128)
	return wasm_i32x4_mul(wasm_i32x4_splat(in_val), in_vec);
#else
	return UInt32x4{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z, in_val * in_vec.w };
#endif
}

UInt32x4& UInt32x4::operator/=(Arg_UInt32x4 in_other) {
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
	w /= in_other.w;
	return *this;
}
UInt32x4 operator/(UInt32x4 in_a, Arg_UInt32x4 in_b) {
	return in_a /= in_b;
}

UInt32x4& UInt32x4::operator/=(uint32 in_other) {
	x /= in_other;
	y /= in_other;
	z /= in_other;
	w /= in_other;
	return *this;
}

UInt32x4 operator/(UInt32x4 in_vec, uint32 in_val) {
	return in_vec /= in_val;
}

UInt32x4 operator/(uint32 in_val, Arg_UInt32x4 in_vec) {
	return UInt32x4{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z, in_val / in_vec.w };
}

DEEP_NAMESPACE_END
