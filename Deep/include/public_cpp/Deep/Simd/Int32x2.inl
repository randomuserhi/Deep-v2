#pragma once

#include "./Int32x2.h"

#include "Deep/Simd/UInt32x2.h"
#include "Deep/Simd/Float32x2.h"
#include "Deep/Bit.h"

#include "Deep/Math/Ops.h"

DEEP_NAMESPACE_BEGIN

Int32x2::Int32x2(int32 in_x, int32 in_y) :
	x{ in_x }, y{ in_y } {}

Int32x2::Int32x2(Type in_internal) :
	m_internal(in_internal) {}

Int32x2::Int32x2(UInt32x2 in_unsigned) :
	m_swar64(in_unsigned.m_swar64) {}

Int32x2::Int32x2(Int32x4 in_int32x4) :
	x{ in_int32x4.x }, y{ in_int32x4.y } {}

constexpr Int32x2 Int32x2::Constexpr(int32 in_x, int32 in_y) {
	Int32x2 xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	return xmmi;
}

Int32x2::operator Type() const {
	return m_internal;
}

Float32x2 Int32x2::ToFloat() const {
	return Float32x2{ static_cast<float32>(x), static_cast<float32>(y) };
}

Float32x2 Int32x2::ReinterpretAsFloat() const {
	return BitCast<Float32x2>(*this);
}

Int32x2 Int32x2::s_Replicate(int32 in_value) {
	return Int32x2{ in_value, in_value };
}

uint32 Int32x2::ToBooleanBitMask() const {
	return (static_cast<uint32>(x) >> 31) | ((static_cast<uint32>(y) >> 31) << 1);
}

Int32x2 Int32x2::s_Min(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
	return Int32x2{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y)  //
	};
}

Int32x2 Int32x2::s_Max(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
	return Int32x2{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y)  //
	};
}

Int32x2 Int32x2::s_Equals(Arg_Int32x2 in_a, Arg_Int32x2 in_b) {
	return Int32x2{ in_a.x == in_b.x ? int32(0xffffffff) : 0, in_a.y == in_b.y ? int32(0xffffffff) : 0 };
}

Int32x2 Int32x2::s_Select(Arg_Int32x2 in_a, Arg_Int32x2 in_b, Arg_Int32x2 in_control) {
	return Int32x2{ in_control.x < 0 ? in_b.x : in_a.x, in_control.y < 0 ? in_b.y : in_a.y };
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
	m_internal = Int32x2{ x << in_count, y << in_count };
	return *this;
}
Int32x2 operator<<(Int32x2 in_a, int32 in_count) {
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	return in_a <<= in_count;
}

Int32x2& Int32x2::operator>>=(int32 in_count) {
	Deep_Assert(in_count >= 0 && in_count < 32, "Invalid shift amount.");
	m_internal = Int32x2{ x >> in_count, y >> in_count };
	return *this;
}
Int32x2 operator>>(Int32x2 in_a, int32 in_count) {
	return in_a >>= in_count;
}

Int32x2& Int32x2::operator|=(Arg_Int32x2 in_other) {
	m_swar64 |= in_other.m_swar64;
	return *this;
}
Int32x2 operator|(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a |= in_b;
}

Int32x2& Int32x2::operator&=(Arg_Int32x2 in_other) {
	m_swar64 &= in_other.m_swar64;
	return *this;
}
Int32x2 operator&(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a &= in_b;
}

Int32x2& Int32x2::operator^=(Arg_Int32x2 in_other) {
	m_swar64 ^= in_other.m_swar64;
	return *this;
}
Int32x2 operator^(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a ^= in_b;
}

Int32x2 operator~(Int32x2 in_value) {
	in_value.m_swar64 = ~in_value.m_swar64;
	return in_value;
}

Int32x2& Int32x2::operator+=(Arg_Int32x2 in_other) {
	x += in_other.x;
	y += in_other.y;
	return *this;
}

Int32x2 operator+(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a += in_b;
}

Int32x2& Int32x2::operator-=(Arg_Int32x2 in_other) {
	x -= in_other.x;
	y -= in_other.y;
	return *this;
}

Int32x2 operator-(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a -= in_b;
}

Int32x2 operator-(Arg_Int32x2 in_a) {
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Int32x2{ 0 - in_a.x, 0 - in_a.y };
}

Int32x2& Int32x2::operator*=(Arg_Int32x2 in_other) {
	x *= in_other.x;
	y *= in_other.y;
	return *this;
}
Int32x2 operator*(Int32x2 in_a, Arg_Int32x2 in_b) {
	return in_a *= in_b;
}

Int32x2& Int32x2::operator*=(int32 in_other) {
	x *= in_other;
	y *= in_other;
	return *this;
}

Int32x2 operator*(Int32x2 in_vec, int32 in_val) {
	return in_vec *= in_val;
}

Int32x2 operator*(int32 in_val, Arg_Int32x2 in_vec) {
	return Int32x2{ in_val * in_vec.x, in_val * in_vec.y };
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
