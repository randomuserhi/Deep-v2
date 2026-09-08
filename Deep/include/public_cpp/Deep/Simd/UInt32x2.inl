#pragma once

#include "./UInt32x2.h"

#include "Deep/Simd/Int32x2.h"
#include "Deep/Simd/UInt32x4.h"
#include "Deep/Simd/Float32x2.h"
#include "Deep/Bit.h"

#include "Deep/Math/Ops.h"

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_NEON
UInt32x2::UInt32x2(uint32 in_x, uint32 in_y) :
	m_internal{ in_x, in_y } {}
#else
UInt32x2::UInt32x2(uint32 in_x, uint32 in_y) :
	x{ in_x }, y{ in_y } {}
#endif

UInt32x2::UInt32x2(Type in_internal) :
	m_internal(in_internal) {}

#ifdef DEEP_USE_NEON
UInt32x2::UInt32x2(Int32x2 in_signed) :
	m_internal{ vreinterpret_u32_s32(in_signed) } {}
#else
UInt32x2::UInt32x2(Int32x2 in_signed) :
	m_swar64(in_signed.m_swar64) {}
#endif

#ifdef DEEP_USE_NEON
UInt32x2::UInt32x2(UInt32x4 in_uint32x4) :
	m_internal{ vget_low_u32(in_uint32x4) } {}
#else
UInt32x2::UInt32x2(UInt32x4 in_uint32x4) :
	x{ in_uint32x4.x }, y{ in_uint32x4.y } {}
#endif

constexpr UInt32x2 UInt32x2::Constexpr(uint32 in_x, uint32 in_y) {
	UInt32x2 xmmi;
	xmmi.m_values[0] = in_x;
	xmmi.m_values[1] = in_y;
	return xmmi;
}

UInt32x2::operator Type() const {
	return m_internal;
}

Float32x2 UInt32x2::ToFloat() const {
#if defined(DEEP_USE_NEON)
	return vcvt_f32_u32(m_internal);
#else
	return Float32x2{ static_cast<float32>(x), static_cast<float32>(y) };
#endif
}

Float32x2 UInt32x2::ReinterpretAsFloat() const {
#if defined(DEEP_USE_NEON)
	return vreinterpret_f32_u32(m_internal);
#else
	return BitCast<Float32x2>(*this);
#endif
}

UInt32x2 UInt32x2::s_Replicate(uint32 in_value) {
#if defined(DEEP_USE_NEON)
	return vdup_n_u32(in_value);
#else
	return UInt32x2{ in_value, in_value };
#endif
}

uint32 UInt32x2::ToBooleanBitMask() const {
#ifdef DEEP_USE_NEON
	uint32x2_t signs = vshr_n_u32(m_internal, 31);
	return vget_lane_u32(signs, 0) | (vget_lane_u32(signs, 1) << 1);
#else
	return (x >> 31) | ((y >> 31) << 1);
#endif
}

UInt32x2 UInt32x2::s_Min(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
#if defined(DEEP_USE_NEON)
	return vmin_u32(in_a, in_b);
#else
	return UInt32x2{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y)  //
	};
#endif
}

UInt32x2 UInt32x2::s_Max(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
#if defined(DEEP_USE_NEON)
	return vmax_u32(in_a, in_b);
#else
	return UInt32x2{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y)  //
	};
#endif
}

Int32x2 UInt32x2::s_Equals(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
#if defined(DEEP_USE_NEON)
	return vreinterpret_s32_u32(vceq_u32(in_a, in_b));
#else
	return Int32x2{ in_a.x == in_b.x ? int32(0xffffffff) : 0, in_a.y == in_b.y ? int32(0xffffffff) : 0 };
#endif
}

UInt32x2 UInt32x2::s_Select(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b, Arg_Int32x2 in_control) {
#if defined(DEEP_USE_NEON)
	return vbsl_u32(vreinterpret_u32_s32(vshr_n_s32(in_control, 31)), in_b, in_a);
#else
	return UInt32x2{ in_control.x < 0 ? in_b.x : in_a.x, in_control.y < 0 ? in_b.y : in_a.y };
#endif
}

constexpr uint32& UInt32x2::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const uint32& UInt32x2::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a.m_swar64 != in_b.m_swar64;
}
bool operator==(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return !(in_a != in_b);
}

UInt32x2& UInt32x2::operator<<=(int32 in_count) {
#if defined(DEEP_USE_NEON)
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = vshl_u32(m_internal, vdup_n_s32(in_count));
#else
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = UInt32x2{ x << in_count, y << in_count };
#endif
	return *this;
}
UInt32x2 operator<<(UInt32x2 in_a, int32 in_count) {
	return in_a <<= in_count;
}

UInt32x2& UInt32x2::operator>>=(int32 in_count) {
#if defined(DEEP_USE_NEON)
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = vshl_u32(m_internal, vdup_n_s32(-in_count));
#else
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = UInt32x2{ x >> in_count, y >> in_count };
#endif
	return *this;
}
UInt32x2 operator>>(UInt32x2 in_a, int32 in_count) {
	return in_a >>= in_count;
}

UInt32x2& UInt32x2::operator|=(Arg_UInt32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vorr_u32(m_internal, in_other);
#else
	m_swar64 |= in_other.m_swar64;
#endif
	return *this;
}
UInt32x2 operator|(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a |= in_b;
}

UInt32x2& UInt32x2::operator&=(Arg_UInt32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vand_u32(m_internal, in_other);
#else
	m_swar64 &= in_other.m_swar64;
#endif
	return *this;
}
UInt32x2 operator&(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a &= in_b;
}

UInt32x2& UInt32x2::operator^=(Arg_UInt32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = veor_u32(m_internal, in_other);
#else
	m_swar64 ^= in_other.m_swar64;
#endif
	return *this;
}
UInt32x2 operator^(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a ^= in_b;
}

UInt32x2 operator~(UInt32x2 in_value) {
#if defined(DEEP_USE_NEON)
	in_value.m_internal = vmvn_u32(in_value.m_internal);
	return in_value;
#else
	in_value.m_swar64 = ~in_value.m_swar64;
	return in_value;
#endif
}

UInt32x2& UInt32x2::operator+=(Arg_UInt32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vadd_u32(m_internal, in_other);
#else
	x += in_other.x;
	y += in_other.y;
#endif
	return *this;
}

UInt32x2 operator+(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a += in_b;
}

UInt32x2& UInt32x2::operator-=(Arg_UInt32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vsub_u32(m_internal, in_other);
#else
	x -= in_other.x;
	y -= in_other.y;
#endif
	return *this;
}

UInt32x2 operator-(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a -= in_b;
}

UInt32x2 operator-(Arg_UInt32x2 in_a) {
#if defined(DEEP_USE_NEON)
	return vsub_u32(vdup_n_u32(0), in_a);
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return UInt32x2{ 0 - in_a.x, 0 - in_a.y };
#endif
}

UInt32x2& UInt32x2::operator*=(Arg_UInt32x2 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vmul_u32(m_internal, in_other);
#else
	x *= in_other.x;
	y *= in_other.y;
#endif
	return *this;
}
UInt32x2 operator*(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a *= in_b;
}

UInt32x2& UInt32x2::operator*=(uint32 in_other) {
#if defined(DEEP_USE_NEON)
	m_internal = vmul_u32(m_internal, vdup_n_u32(in_other));
#else
	x *= in_other;
	y *= in_other;
#endif
	return *this;
}

UInt32x2 operator*(UInt32x2 in_vec, uint32 in_val) {
	return in_vec *= in_val;
}

UInt32x2 operator*(uint32 in_val, Arg_UInt32x2 in_vec) {
#if defined(DEEP_USE_NEON)
	return vmul_u32(vdup_n_u32(in_val), in_vec);
#else
	return UInt32x2{ in_val * in_vec.x, in_val * in_vec.y };
#endif
}

UInt32x2& UInt32x2::operator/=(Arg_UInt32x2 in_other) {
	x /= in_other.x;
	y /= in_other.y;
	return *this;
}
UInt32x2 operator/(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a /= in_b;
}

UInt32x2& UInt32x2::operator/=(uint32 in_other) {
	x /= in_other;
	y /= in_other;
	return *this;
}

UInt32x2 operator/(UInt32x2 in_vec, uint32 in_val) {
	return in_vec /= in_val;
}

UInt32x2 operator/(uint32 in_val, Arg_UInt32x2 in_vec) {
	return UInt32x2{ in_val / in_vec.x, in_val / in_vec.y };
}

DEEP_NAMESPACE_END
