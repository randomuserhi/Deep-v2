#pragma once

#include "Deep/Simd/Float32x4.h"
#include "Deep/Simd/Float32x2.h"
#include "Deep/Simd/Int32x2.h"
#include "Deep/Bit.h"

#include "Deep/Math/Ops.h"

DEEP_NAMESPACE_BEGIN

Float32x2::Float32x2(float32 in_x, float32 in_y) :
	x{ in_x }, y{ in_y } {}

Float32x2::Float32x2(Type in_internal) :
	m_internal(in_internal) {}

Float32x2::Float32x2(Float32x4 in_float32x4) :
	x{ in_float32x4.x }, y{ in_float32x4.y } {}

constexpr Float32x2 Float32x2::Constexpr(float32 in_x, float32 in_y) {
	Float32x2 xmm;
	xmm.x = in_x;
	xmm.y = in_y;
	return xmm;
}

constexpr Float32x2::operator Type() const {
	return m_internal;
}

Int32x2 Float32x2::ToInt() const {
	return Int32x2{ static_cast<int32>(x), static_cast<int32>(y) };
}
constexpr Int32x2 Float32x2::Constexpr_ToInt() const {
	return Int32x2::Constexpr(static_cast<int32>(x), static_cast<int32>(y));
}

Int32x2 Float32x2::ReinterpretAsInt() const {
	return BitCast<Int32x2>(*this);
}
constexpr Int32x2 Float32x2::Constexpr_ReinterpretAsInt() const {
	return BitCast<Int32x2>(*this);
}

Float32x2 Float32x2::Replicate(float32 in_value) {
	return Float32x2{ in_value, in_value };
}

Float32x2 Float32x2::Min(Arg_Float32x2 in_a, Arg_Float32x2 in_b) {
	return Float32x2{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y)  //
	};
}

Float32x2 Float32x2::Max(Arg_Float32x2 in_a, Arg_Float32x2 in_b) {
	return Float32x2{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y)  //
	};
}

Int32x2 Float32x2::Equals(Arg_Float32x2 in_a, Arg_Float32x2 in_b) {
	return Int32x2{ in_a.x == in_b.x ? int32(0xffffffff) : 0, //
		            in_a.y == in_b.y ? int32(0xffffffff) : 0 };
}
constexpr Int32x2 Float32x2::Constexpr_Equals(Arg_Float32x2 in_a, Arg_Float32x2 in_b) {
	return Int32x2::Constexpr(in_a.x == in_b.x ? int32(0xffffffff) : 0, //
	                          in_a.y == in_b.y ? int32(0xffffffff) : 0);
}

constexpr float32& Float32x2::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Float32x2::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a.x != in_b.x || in_a.y != in_b.y;
}
bool operator==(Arg_Float32x2 in_a, Arg_Float32x2 in_b) {
	return !(in_a != in_b);
}

Float32x2& Float32x2::operator|=(Arg_Float32x2 in_other) {
	m_internal = (ReinterpretAsInt() | in_other.ReinterpretAsInt()).ReinterpretAsFloat();
	return *this;
}
Float32x2 operator|(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a |= in_b;
}

Float32x2& Float32x2::operator&=(Arg_Float32x2 in_other) {
	m_internal = (ReinterpretAsInt() & in_other.ReinterpretAsInt()).ReinterpretAsFloat();
	return *this;
}
Float32x2 operator&(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a &= in_b;
}

Float32x2& Float32x2::operator^=(Arg_Float32x2 in_other) {
	m_internal = (ReinterpretAsInt() ^ in_other.ReinterpretAsInt()).ReinterpretAsFloat();
	return *this;
}
Float32x2 operator^(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a ^= in_b;
}

Float32x2& Float32x2::operator+=(Arg_Float32x2 in_other) {
	x += in_other.x;
	y += in_other.y;
	return *this;
}

Float32x2 operator+(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a += in_b;
}

Float32x2& Float32x2::operator-=(Arg_Float32x2 in_other) {
	x -= in_other.x;
	y -= in_other.y;
	return *this;
}

Float32x2 operator-(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a -= in_b;
}

Float32x2 operator-(Arg_Float32x2 in_other) {
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Float32x2{ 0.0f - in_other.x, 0.0f - in_other.y };
}

Float32x2& Float32x2::operator*=(Arg_Float32x2 in_other) {
	x *= in_other.x;
	y *= in_other.y;
	return *this;
}
Float32x2 operator*(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a *= in_b;
}

Float32x2& Float32x2::operator*=(float32 in_other) {
	x *= in_other;
	y *= in_other;
	return *this;
}

Float32x2 operator*(Float32x2 in_vec, float32 in_val) {
	return in_vec *= in_val;
}

Float32x2 operator*(float32 in_val, Arg_Float32x2 in_vec) {
	return Float32x2{ in_val * in_vec.x, in_val * in_vec.y };
}

Float32x2& Float32x2::operator/=(Arg_Float32x2 in_other) {
	x /= in_other.x;
	y /= in_other.y;
	return *this;
}
Float32x2 operator/(Float32x2 in_a, Arg_Float32x2 in_b) {
	return in_a /= in_b;
}

Float32x2& Float32x2::operator/=(float32 in_other) {
	x /= in_other;
	y /= in_other;
	return *this;
}

Float32x2 operator/(Float32x2 in_vec, float32 in_val) {
	return in_vec /= in_val;
}

Float32x2 operator/(float32 in_val, Arg_Float32x2 in_vec) {
	return Float32x2{ in_val / in_vec.x, in_val / in_vec.y };
}

Int32x2 Float32x2::IsNegative(Arg_Float32x2 in_value) {
	return Int32x2{ in_value.x < 0 ? int32(0xffffffff) : 0, in_value.y < 0 ? int32(0xffffffff) : 0 };
}

Float32x2 Float32x2::Select(Arg_Float32x2 in_a, Arg_Float32x2 in_b, Arg_Int32x2 in_control) {
	return Float32x2{ in_control.x ? in_b.x : in_a.x, in_control.y ? in_b.y : in_a.y };
}

void Float32x2::SinCos(Float32x2& out_sin, Float32x2& out_cos) {
	// Implementation based on sinf.c from the cephes library, combines sinf and cosf in a single function, changes
	// octants to quadrants and vectorizes it Original implementation by Stephen L. Moshier (See:
	// http://www.moshier.net/)

	// Make argument positive and remember sign for sin only since cos is symmetric around x (highest bit of a float is
	// the sign bit)
	Int32x2 sinSign = ReinterpretAsInt() & Int32x2::Replicate(int32(0x80000000));
	Float32x2 _x = *this ^ sinSign.ReinterpretAsFloat();

	// x / (PI / 2) rounded to nearest int gives us the quadrant closest to x
	Int32x2 quadrant = (0.6366197723675814f * _x + Float32x2::Replicate(0.5f)).ToInt();

	// Make x relative to the closest quadrant.
	// This does x = x - quadrant * PI / 2 using a two step Cody-Waite argument reduction.
	// This improves the accuracy of the result by avoiding loss of significant bits in the subtraction.
	// We start with x = x - quadrant * PI / 2, PI / 2 in hexadecimal notation is 0x3fc90fdb, we remove the lowest 16
	// bits to get 0x3fc90000 (= 1.5703125) this means we can now multiply with a number of up to 2^16 without losing any
	// bits. This leaves us with: x = (x - quadrant * 1.5703125) - quadrant * (PI / 2 - 1.5703125). PI / 2 - 1.5703125 in
	// hexadecimal is 0x39fdaa22, stripping the lowest 12 bits we get 0x39fda000 (= 0.0004837512969970703125) This leaves
	// uw with: x = ((x - quadrant * 1.5703125) - quadrant * 0.0004837512969970703125) - quadrant * (PI / 2 - 1.5703125 -
	// 0.0004837512969970703125) See:
	// https://stackoverflow.com/questions/42455143/sine-cosine-modular-extended-precision-arithmetic After this we have
	// x in the range [-PI / 4, PI / 4].
	Float32x2 floatQuadrant = quadrant.ToFloat();
	_x = ((_x - floatQuadrant * 1.5703125f) - floatQuadrant * 0.0004837512969970703125f)
	     - floatQuadrant * 7.549789948768648e-8f;

	// Calculate x2 = x^2
	Float32x2 x2 = _x * _x;

	// Taylor expansion:
	// Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + x^8/8! + ... = (((x2/8!- 1/6!) * x2 + 1/4!) * x2 - 1/2!) * x2 + 1
	Float32x2 taylorCos = ((2.443315711809948e-5f * x2 - Float32x2::Replicate(1.388731625493765e-3f)) * x2
	                       + Float32x2::Replicate(4.166664568298827e-2f))
	                          * x2 * x2
	                      - 0.5f * x2 + Float32x2::Replicate(1.0f);
	// Sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ... = ((-x2/7! + 1/5!) * x2 - 1/3!) * x2 * x + x
	Float32x2 taylorSin =
		((-1.9515295891e-4f * x2 + Float32x2::Replicate(8.3321608736e-3f)) * x2 - Float32x2::Replicate(1.6666654611e-1f))
			* x2 * _x
		+ _x;

	// The lowest 2 bits of quadrant indicate the quadrant that we are in.
	// Let x be the original input value and x' our value that has been mapped to the range [-PI / 4, PI / 4].
	// since cos(x) = sin(x - PI / 2) and since we want to use the Taylor expansion as close as possible to 0,
	// we can alternate between using the Taylor expansion for sin and cos according to the following table:
	//
	// quadrant	 sin(x)		 cos(x)
	// XXX00b	 sin(x')	 cos(x')
	// XXX01b	 cos(x')	-sin(x')
	// XXX10b	-sin(x')	-cos(x')
	// XXX11b	-cos(x')	 sin(x')
	//
	// So: sin_sign = bit2, cos_sign = bit1 ^ bit2, bit1 determines if we use sin or cos Taylor expansion
	Int32x2 bit1 = quadrant << 31;
	Int32x2 bit2 = (quadrant << 30) & Int32x2::Replicate(int32(0x80000000));

	// Select which one of the results is sin and which one is cos
	Float32x2 s = Float32x2::Select(taylorSin, taylorCos, bit1);
	Float32x2 c = Float32x2::Select(taylorCos, taylorSin, bit1);

	// Update the signs
	sinSign = sinSign ^ bit2;
	Int32x2 cos_sign = bit1 ^ bit2;

	// Correct the signs
	out_sin = s ^ sinSign.ReinterpretAsFloat();
	out_cos = c ^ cos_sign.ReinterpretAsFloat();
}

DEEP_NAMESPACE_END
