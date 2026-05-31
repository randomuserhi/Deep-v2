#pragma once

#include "Deep/Math/Simd/Float32x4.h"
#include "Deep/Math/Simd/Int32x4.h"
#include "Deep/Bit.h"

#if !defined(DEEP_USE_SSE)
	#include "Deep/Math/Ops.h"
#endif

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Float32x4::Float32x4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	_internal{ _mm_set_ps(in_w, in_z, in_y, in_x) } {}
#else
Float32x4::Float32x4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	x{ in_x }, y{ in_y }, z{ in_z }, w{ in_w } {}
#endif

constexpr Float32x4::Float32x4(Type in_internal) :
	_internal(in_internal) {}

constexpr Float32x4 Float32x4::Constexpr(float32 in_x, float32 in_y, float32 in_z, float32 in_w) {
	Float32x4 xmm;
	xmm.x = in_x;
	xmm.y = in_y;
	xmm.z = in_z;
	xmm.w = in_w;
	return xmm;
}

constexpr Float32x4::operator Type() const {
	return _internal;
}

Int32x4 Float32x4::ToInt() const {
#ifdef DEEP_USE_SSE
	return _mm_cvttps_epi32(_internal);
#else
	return Int32x4{ static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z), static_cast<int32>(w) };
#endif
}
constexpr Int32x4 Float32x4::Constexpr_ToInt() const {
	return Int32x4::Constexpr(static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z), static_cast<int32>(w));
}

Int32x4 Float32x4::ReinterpretAsInt() const {
#ifdef DEEP_USE_SSE
	return _mm_castps_si128(_internal);
#else
	return BitCast<Int32x4>(*this);
#endif
}
constexpr Int32x4 Float32x4::Constexpr_ReinterpretAsInt() const {
	return BitCast<Int32x4>(*this);
}

Float32x4 Float32x4::Replicate(float32 in_value) {
#ifdef DEEP_USE_SSE
	return _mm_set1_ps(in_value);
#else
	return Xmm{ in_value, in_value, in_value, in_value };
#endif
}

Float32x4 Float32x4::Min(Arg_Float32x4 in_a, Arg_Float32x4 in_b) {
#ifdef DEEP_USE_SSE
	return _mm_min_ps(in_a, in_b);
#else
	return Xmm{
		Deep::Min(in_a.x, in_b.x), //
		Deep::Min(in_a.y, in_b.y), //
		Deep::Min(in_a.z, in_b.z), //
		Deep::Min(in_a.w, in_b.w)  //
	};
#endif
}

Float32x4 Float32x4::Max(Arg_Float32x4 in_a, Arg_Float32x4 in_b) {
#ifdef DEEP_USE_SSE
	return _mm_max_ps(in_a, in_b);
#else
	return Xmm{
		Deep::Max(in_a.x, in_b.x), //
		Deep::Max(in_a.y, in_b.y), //
		Deep::Max(in_a.z, in_b.z), //
		Deep::Max(in_a.w, in_b.w)  //
	};
#endif
}

Int32x4 Float32x4::Equals(Arg_Float32x4 in_a, Arg_Float32x4 in_b) {
#ifdef DEEP_USE_SSE
	return _mm_castps_si128(_mm_cmpeq_ps(in_a, in_b));
#else
	return Int32x4{ in_a.x == in_b.x ? int32(0xffffffff) : 0, //
		            in_a.y == in_b.y ? int32(0xffffffff) : 0, //
		            in_a.z == in_b.z ? int32(0xffffffff) : 0, //
		            in_a.w == in_b.w ? int32(0xffffffff) : 0 };
#endif
}
constexpr Int32x4 Float32x4::Constexpr_Equals(Arg_Float32x4 in_a, Arg_Float32x4 in_b) {
	return Int32x4::Constexpr(in_a.x == in_b.x ? int32(0xffffffff) : 0, //
	                          in_a.y == in_b.y ? int32(0xffffffff) : 0, //
	                          in_a.z == in_b.z ? int32(0xffffffff) : 0, //
	                          in_a.w == in_b.w ? int32(0xffffffff) : 0);
}

bool operator!=(Arg_Float32x4 in_a, Arg_Float32x4 in_b) {
#ifdef DEEP_USE_SSE4_1
	return Float32x4::Equals(in_a, in_b).ToBooleanBitMask() != 0b1111;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z || in_a.w != in_b.w;
#endif
}
bool operator==(Arg_Float32x4 in_a, Arg_Float32x4 in_b) {
	return !(in_a != in_b);
}

Float32x4& Float32x4::operator|=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE
	_internal = _mm_or_ps(_internal, in_other);
#else
	_internal = (ReinterpretAsInt() | in_other.ReinterpretAsInt()).ReinterpretAsFloat();
#endif
	return *this;
}
Float32x4 operator|(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a |= in_b;
}

Float32x4& Float32x4::operator&=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE
	_internal = _mm_and_ps(_internal, in_other);
#else
	_internal = (ReinterpretAsInt() & in_other.ReinterpretAsInt()).ReinterpretAsFloat();
#endif
	return *this;
}
Float32x4 operator&(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a &= in_b;
}

Float32x4& Float32x4::operator^=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE
	_internal = _mm_xor_ps(_internal, in_other);
#else
	_internal = (ReinterpretAsInt() ^ in_other.ReinterpretAsInt()).ReinterpretAsFloat();
#endif
	return *this;
}
Float32x4 operator^(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a ^= in_b;
}

Float32x4& Float32x4::operator+=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_add_ps(_internal, in_other);
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
	w += in_other.w;
#endif
	return *this;
}

Float32x4 operator+(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a += in_b;
}

Float32x4& Float32x4::operator-=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_sub_ps(_internal, in_other);
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
	w -= in_other.w;
#endif
	return *this;
}

Float32x4 operator-(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a -= in_b;
}

Float32x4 operator-(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	return _mm_sub_ps(_mm_setzero_ps(), in_other);
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Xmm{ 0.0f - in_other.x, 0.0f - in_other.y, 0.0f - in_other.z, 0.0f - in_other.w };
#endif
}

Float32x4& Float32x4::operator*=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_mul_ps(_internal, in_other);
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
	w *= in_other.w;
#endif
	return *this;
}
Float32x4 operator*(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a *= in_b;
}

Float32x4& Float32x4::operator*=(float32 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_mul_ps(_internal, _mm_set1_ps(in_other));
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
	w *= in_other;
#endif
	return *this;
}

Float32x4 operator*(Float32x4 in_vec, float32 in_val) {
	return in_vec *= in_val;
}

Float32x4 operator*(float32 in_val, Arg_Float32x4 in_vec) {
#ifdef DEEP_USE_SSE4_1
	return _mm_mul_ps(_mm_set1_ps(in_val), in_vec);
#else
	return Xmm{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z, in_val * in_vec.w };
#endif
}

Float32x4& Float32x4::operator/=(Arg_Float32x4 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_div_ps(_internal, in_other);
#else
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
	w /= in_other.w;
#endif
	return *this;
}
Float32x4 operator/(Float32x4 in_a, Arg_Float32x4 in_b) {
	return in_a /= in_b;
}

Float32x4& Float32x4::operator/=(float32 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_div_ps(_internal, _mm_set1_ps(in_other));
#else
	x /= in_other;
	y /= in_other;
	z /= in_other;
	w /= in_other;
#endif
	return *this;
}

Float32x4 operator/(Float32x4 in_vec, float32 in_val) {
	return in_vec /= in_val;
}

Float32x4 operator/(float32 in_val, Arg_Float32x4 in_vec) {
#ifdef DEEP_USE_SSE4_1
	return _mm_div_ps(_mm_set1_ps(in_val), in_vec);
#else
	return Xmm{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z, in_val / in_vec.w };
#endif
}

Int32x4 Float32x4::IsNegative(Arg_Float32x4 in_value) {
#ifdef DEEP_USE_SSE4_1
	return _mm_castps_si128(_mm_cmplt_ps(in_value, _mm_setzero_ps()));
#else
	return Int32x4{ in_value.x < 0 ? int32(0xffffffff) : 0, in_value.y < 0 ? int32(0xffffffff) : 0,
		            in_value.z < 0 ? int32(0xffffffff) : 0, in_value.w < 0 ? int32(0xffffffff) : 0 };
#endif
}

Float32x4 Float32x4::Select(Arg_Float32x4 in_a, Arg_Float32x4 in_b, Arg_Int32x4 in_control) {
#ifdef DEEP_USE_SSE4_1
	return _mm_blendv_ps(in_a, in_b, _mm_castsi128_ps(in_control));
#else
	return Xmm{ in_control.x ? in_b.x : in_a.x, in_control.y ? in_b.y : in_a.y, in_control.z ? in_b.z : in_a.z,
		        in_control.w ? in_b.w : in_a.w };
#endif
}

void Float32x4::SinCos(Float32x4& out_sin, Float32x4& out_cos) {
	// Implementation based on sinf.c from the cephes library, combines sinf and cosf in a single function, changes
	// octants to quadrants and vectorizes it Original implementation by Stephen L. Moshier (See:
	// http://www.moshier.net/)

	// Make argument positive and remember sign for sin only since cos is symmetric around x (highest bit of a float is
	// the sign bit)
	Int32x4 sinSign = ReinterpretAsInt() & Int32x4::Replicate(int32(0x80000000));
	Float32x4 _x = *this ^ sinSign.ReinterpretAsFloat();

	// x / (PI / 2) rounded to nearest int gives us the quadrant closest to x
	Int32x4 quadrant = (0.6366197723675814f * _x + Float32x4::Replicate(0.5f)).ToInt();

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
	Float32x4 floatQuadrant = quadrant.ToFloat();
	_x = ((_x - floatQuadrant * 1.5703125f) - floatQuadrant * 0.0004837512969970703125f)
	     - floatQuadrant * 7.549789948768648e-8f;

	// Calculate x2 = x^2
	Float32x4 x2 = _x * _x;

	// Taylor expansion:
	// Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + x^8/8! + ... = (((x2/8!- 1/6!) * x2 + 1/4!) * x2 - 1/2!) * x2 + 1
	Float32x4 taylorCos = ((2.443315711809948e-5f * x2 - Float32x4::Replicate(1.388731625493765e-3f)) * x2
	                       + Float32x4::Replicate(4.166664568298827e-2f))
	                          * x2 * x2
	                      - 0.5f * x2 + Float32x4::Replicate(1.0f);
	// Sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ... = ((-x2/7! + 1/5!) * x2 - 1/3!) * x2 * x + x
	Float32x4 taylorSin =
		((-1.9515295891e-4f * x2 + Float32x4::Replicate(8.3321608736e-3f)) * x2 - Float32x4::Replicate(1.6666654611e-1f))
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
	Int32x4 bit1 = quadrant.LogicalShiftLeft<31>();
	Int32x4 bit2 = quadrant.LogicalShiftLeft<30>() & Int32x4::Replicate(int32(0x80000000));

	// Select which one of the results is sin and which one is cos
	Float32x4 s = Float32x4::Select(taylorSin, taylorCos, bit1);
	Float32x4 c = Float32x4::Select(taylorCos, taylorSin, bit1);

	// Update the signs
	sinSign = sinSign ^ bit2;
	Int32x4 cos_sign = bit1 ^ bit2;

	// Correct the signs
	out_sin = s ^ sinSign.ReinterpretAsFloat();
	out_cos = c ^ cos_sign.ReinterpretAsFloat();
}

DEEP_NAMESPACE_END
