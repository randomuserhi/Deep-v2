#pragma once

#include "./UInt32x2.h"

#include "Deep/Simd/Int32x2.h"
#include "Deep/Simd/UInt32x4.h"
#include "Deep/Simd/Float32x2.h"
#include "Deep/Bit.h"

#include "Deep/Math/Ops.h"

DEEP_NAMESPACE_BEGIN

UInt32x2::UInt32x2(uint32 in_x, uint32 in_y) :
	x{ in_x }, y{ in_y } {}

UInt32x2::UInt32x2(Type in_internal) :
	m_internal(in_internal) {}

UInt32x2::UInt32x2(Int32x2 in_signed) :
	m_swar64(in_signed.m_swar64) {}

UInt32x2::UInt32x2(UInt32x4 in_uint32x4) :
	x{ in_uint32x4.x }, y{ in_uint32x4.y } {}

constexpr UInt32x2 UInt32x2::Constexpr(uint32 in_x, uint32 in_y) {
	UInt32x2 xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	return xmmi;
}

UInt32x2::operator Type() const {
	return m_internal;
}

Float32x2 UInt32x2::ToFloat() const {
	return Float32x2{ static_cast<float32>(x), static_cast<float32>(y) };
}

Float32x2 UInt32x2::ReinterpretAsFloat() const {
	return BitCast<Float32x2>(*this);
}

UInt32x2 UInt32x2::s_Replicate(uint32 in_value) {
	return UInt32x2{ in_value, in_value };
}

uint32 UInt32x2::ToBooleanBitMask() const {
	return (x >> 31) | ((y >> 31) << 1);
}

UInt32x2 UInt32x2::s_Min(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return UInt32x2{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y)  //
	};
}

UInt32x2 UInt32x2::s_Max(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return UInt32x2{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y)  //
	};
}

UInt32x2 UInt32x2::s_Equals(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return UInt32x2{ in_a.x == in_b.x ? 0xffffffffu : 0u, in_a.y == in_b.y ? 0xffffffffu : 0u };
}

UInt32x2 UInt32x2::s_Select(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b, Arg_Int32x2 in_control) {
	return UInt32x2{ in_control.x < 0 ? in_b.x : in_a.x, in_control.y < 0 ? in_b.y : in_a.y };
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
	m_internal = UInt32x2{ x << in_count, y << in_count };
	return *this;
}
UInt32x2 operator<<(UInt32x2 in_a, int32 in_count) {
	return in_a <<= in_count;
}

UInt32x2& UInt32x2::operator>>=(int32 in_count) {
	m_internal = UInt32x2{ x >> in_count, y >> in_count };
	return *this;
}
UInt32x2 operator>>(UInt32x2 in_a, int32 in_count) {
	return in_a >>= in_count;
}

UInt32x2& UInt32x2::operator|=(Arg_UInt32x2 in_other) {
	m_swar64 |= in_other.m_swar64;
	return *this;
}
UInt32x2 operator|(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a |= in_b;
}

UInt32x2& UInt32x2::operator&=(Arg_UInt32x2 in_other) {
	m_swar64 &= in_other.m_swar64;
	return *this;
}
UInt32x2 operator&(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a &= in_b;
}

UInt32x2& UInt32x2::operator^=(Arg_UInt32x2 in_other) {
	m_swar64 ^= in_other.m_swar64;
	return *this;
}
UInt32x2 operator^(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a ^= in_b;
}

UInt32x2 operator~(UInt32x2 in_value) {
	in_value.m_swar64 = ~in_value.m_swar64;
	return in_value;
}

UInt32x2& UInt32x2::operator+=(Arg_UInt32x2 in_other) {
	x += in_other.x;
	y += in_other.y;
	return *this;
}

UInt32x2 operator+(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a += in_b;
}

UInt32x2& UInt32x2::operator-=(Arg_UInt32x2 in_other) {
	x -= in_other.x;
	y -= in_other.y;
	return *this;
}

UInt32x2 operator-(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a -= in_b;
}

UInt32x2 operator-(Arg_UInt32x2 in_a) {
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return UInt32x2{ 0 - in_a.x, 0 - in_a.y };
}

UInt32x2& UInt32x2::operator*=(Arg_UInt32x2 in_other) {
	x *= in_other.x;
	y *= in_other.y;
	return *this;
}
UInt32x2 operator*(UInt32x2 in_a, Arg_UInt32x2 in_b) {
	return in_a *= in_b;
}

UInt32x2& UInt32x2::operator*=(uint32 in_other) {
	x *= in_other;
	y *= in_other;
	return *this;
}

UInt32x2 operator*(UInt32x2 in_vec, uint32 in_val) {
	return in_vec *= in_val;
}

UInt32x2 operator*(uint32 in_val, Arg_UInt32x2 in_vec) {
	return UInt32x2{ in_val * in_vec.x, in_val * in_vec.y };
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
