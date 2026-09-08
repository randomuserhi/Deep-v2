#pragma once

#include "./Int32x2.h"

#include "Deep/Simd/UInt32x2.h"
#include "Deep/Simd/Float32x2.h"
#include "Deep/Bit.h"

#include "Deep/Math/Ops.h"

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_NEON
Int32x2::Int32x2(int32 in_x, int32 in_y) :
	m_internal{ in_x, in_y } {}
#else
Int32x2::Int32x2(int32 in_x, int32 in_y) :
	x{ in_x }, y{ in_y } {}
#endif

Int32x2::Int32x2(Type in_internal) :
	m_internal(in_internal) {}

#ifdef DEEP_USE_NEON
Int32x2::Int32x2(UInt32x2 in_unsigned) :
	m_internal{ vreinterpret_s32_u32(in_unsigned) } {}
#else
Int32x2::Int32x2(UInt32x2 in_unsigned) :
	m_swar64(in_unsigned.m_swar64) {}
#endif

#ifdef DEEP_USE_NEON
Int32x2::Int32x2(Int32x4 in_int32x4) :
	m_internal{ vget_low_s32(in_int32x4) } {}
#else
Int32x2::Int32x2(Int32x4 in_int32x4) :
	x{ in_int32x4.x }, y{ in_int32x4.y } {}
#endif

constexpr Int32x2 Int32x2::Constexpr(int32 in_x, int32 in_y) {
	Int32x2 xmmi;
	xmmi.m_values[0] = in_x;
	xmmi.m_values[1] = in_y;
	return xmmi;
}

Int32x2::operator Type() const {
	return m_internal;
}

Float32x2 Int32x2::ToFloat() const {
#if defined(DEEP_USE_NEON)
	return vcvt_f32_s32(m_internal);
#else
	return Float32x2{ static_cast<float32>(x), static_cast<float32>(y) };
#endif
}

Float32x2 Int32x2::ReinterpretAsFloat() const {
#if defined(DEEP_USE_NEON)
	return vreinterpret_f32_s32(m_internal);
#else
	return BitCast<Float32x2>(*this);
#endif
}

Int32x2 Int32x2::s_Replicate(int32 in_value) {
#if defined(DEEP_USE_NEON)
	return vdup_n_s32(in_value);
#else
	return Int32x2{ in_value, in_value };
#endif
}

uint32 Int32x2::ToBooleanBitMask() const {
#ifdef DEEP_USE_NEON
	uint32x2_t signs = vshr_n_u32(vreinterpret_u32_s32(m_internal), 31);
	return vget_lane_u32(signs, 0) | (vget_lane_u32(signs, 1) << 1);
#else
	return (static_cast<uint32>(x) >> 31) | ((static_cast<uint32>(y) >> 31) << 1);
#endif
}

Int32x2 Int32x2::s_Min(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
#if defined(DEEP_USE_NEON)
	return vmin_s32(in_a, in_b);
#else
	return Int32x2{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y)  //
	};
#endif
}

Int32x2 Int32x2::s_Max(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
#if defined(DEEP_USE_NEON)
	return vmax_s32(in_a, in_b);
#else
	return Int32x2{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y)  //
	};
#endif
}

Int32x2 Int32x2::s_Equals(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
#if defined(DEEP_USE_NEON)
	return vreinterpret_s32_u32(vceq_s32(in_a, in_b));
#else
	return Int32x2{ in_a.x == in_b.x ? int32(0xffffffff) : 0, in_a.y == in_b.y ? int32(0xffffffff) : 0 };
#endif
}

Int32x2 Int32x2::s_Select(Arg_Int32x2 in_a, Arg_Int32x2 in_b, Arg_Int32x2 in_control) {
#if defined(DEEP_USE_NEON)
	return vbsl_s32(vreinterpret_u32_s32(vshr_n_s32(in_control, 31)), in_b, in_a);
#else
	return Int32x2{ in_control.x < 0 ? in_b.x : in_a.x, in_control.y < 0 ? in_b.y : in_a.y };
#endif
}

constexpr int32& Int32x2::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int32& Int32x2::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a.m_swar64 != in_b.m_swar64;
}
bool operator==(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
	return !(in_a != in_b);
}

Int32x2& Int32x2::operator<<=(int32 in_count) {
#if defined(DEEP_USE_NEON)
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = vshl_s32(m_internal, vdup_n_s32(in_count));
#else
	m_internal = Int32x2{ x << in_count, y << in_count };
#endif
	return *this;
}
Int32x2 operator<<(Int32x2 in_a, int32 in_count) {
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	return in_a <<= in_count;
}

Int32x2& Int32x2::operator>>=(int32 in_count) {
#if defined(DEEP_USE_NEON)
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = vshl_s32(m_internal, vdup_n_s32(-in_count));
#else
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = Int32x2{ x >> in_count, y >> in_count };
#endif
	return *this;
}
Int32x2 operator>>(Int32x2 in_a, int32 in_count) {
	return in_a >>= in_count;
}

Int32x2& Int32x2::operator|=(Arg_Int32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vorr_s32(m_internal, in_other);
#else
	m_swar64 |= in_other.m_swar64;
#endif
	return *this;
}
Int32x2 operator|(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a |= in_b;
}

Int32x2& Int32x2::operator&=(Arg_Int32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vand_s32(m_internal, in_other);
#else
	m_swar64 &= in_other.m_swar64;
#endif
	return *this;
}
Int32x2 operator&(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a &= in_b;
}

Int32x2& Int32x2::operator^=(Arg_Int32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = veor_s32(m_internal, in_other);
#else
	m_swar64 ^= in_other.m_swar64;
#endif
	return *this;
}
Int32x2 operator^(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a ^= in_b;
}

Int32x2 operator~(Int32x2 in_value) {
#if defined(DEEP_USE_NEON)
	in_value.m_internal = vmvn_s32(in_value.m_internal);
	return in_value;
#else
	in_value.m_swar64 = ~in_value.m_swar64;
	return in_value;
#endif
}

Int32x2& Int32x2::operator+=(Arg_Int32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vadd_s32(m_internal, in_other);
#else
	x += in_other.x;
	y += in_other.y;
#endif
	return *this;
}

Int32x2 operator+(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a += in_b;
}

Int32x2& Int32x2::operator-=(Arg_Int32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vsub_s32(m_internal, in_other);
#else
	x -= in_other.x;
	y -= in_other.y;
#endif
	return *this;
}

Int32x2 operator-(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a -= in_b;
}

Int32x2 operator-(Arg_Int32x2 in_a) {
#if defined(DEEP_USE_NEON)
	return vsub_s32(vdup_n_s32(0), in_a);
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Int32x2{ 0 - in_a.x, 0 - in_a.y };
#endif
}

Int32x2& Int32x2::operator*=(Arg_Int32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vmul_s32(m_internal, in_other);
#else
	x *= in_other.x;
	y *= in_other.y;
#endif
	return *this;
}
Int32x2 operator*(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a *= in_b;
}

Int32x2& Int32x2::operator*=(int32 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vmul_s32(m_internal, vdup_n_s32(in_other));
#else
	x *= in_other;
	y *= in_other;
#endif
	return *this;
}

Int32x2 operator*(Int32x2 in_vec, int32 in_val) {
	return in_vec *= in_val;
}

Int32x2 operator*(int32 in_val, Arg_Int32x2 in_vec) {
#if defined(DEEP_USE_NEON)
	return vmul_s32(vdup_n_s32(in_val), in_vec);
#else
	return Int32x2{ in_val * in_vec.x, in_val * in_vec.y };
#endif
}

Int32x2& Int32x2::operator/=(Arg_Int32x2 in_other) {
	x /= in_other.x;
	y /= in_other.y;
	return *this;
}
Int32x2 operator/(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a /= in_b;
}

Int32x2& Int32x2::operator/=(int32 in_other) {
	x /= in_other;
	y /= in_other;
	return *this;
}

Int32x2 operator/(Int32x2 in_vec, int32 in_val) {
	return in_vec /= in_val;
}

Int32x2 operator/(int32 in_val, Arg_Int32x2 in_vec) {
	return Int32x2{ in_val / in_vec.x, in_val / in_vec.y };
}

DEEP_NAMESPACE_END
