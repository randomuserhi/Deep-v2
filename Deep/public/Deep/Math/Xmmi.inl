#pragma once

#include "Deep/Math/Xmmi.h"
#include "Deep/Math/Xmm.h"
#include "Deep/Bit.h"

#if !defined(DEEP_USE_SSE)
#include "Deep/Bit.h"
#endif

DEEP_NAMESPACE_BEGIN

Xmmi::Xmmi(int32 in_x, int32 in_y, int32 in_z, int32 in_w) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_set_epi32(in_w, in_z, in_y, in_x);
#else
	this->x = in_x;
	this->y = in_y;
	this->z = in_z;
	this->w = in_w;
#endif
}
constexpr Xmmi::Xmmi(Type in_internal) :
	_internal(in_internal) {}

constexpr Xmmi Xmmi::Constexpr(int32 in_x, int32 in_y, int32 in_z, int32 in_w) {
	Xmmi xmmi;
	xmmi.x = in_x;
	xmmi.y = in_y;
	xmmi.z = in_z;
	xmmi.w = in_w;
	return xmmi;
}

Xmmi::operator Type() const {
	return _internal;
}

Xmm Xmmi::ToFloat() const {
#ifdef DEEP_USE_SSE
	return _mm_cvtepi32_ps(_internal);
#else
	return Xmm{ static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z), static_cast<float32>(w) };
#endif
}
constexpr Xmm Xmmi::Constexpr_ToFloat() const {
	Xmm xmm;
	xmm.x = static_cast<float32>(x);
	xmm.y = static_cast<float32>(y);
	xmm.z = static_cast<float32>(z);
	xmm.w = static_cast<float32>(w);
	return xmm;
}

Xmm Xmmi::ReinterpretAsFloat() const {
#ifdef DEEP_USE_SSE
	return _mm_castsi128_ps(_internal);
#else
	return Deep::BitCast<Xmm>(*this);
#endif
}
constexpr Xmm Xmmi::Constexpr_ReinterpretAsFloat() const {
	return Deep::BitCast<Xmm>(*this);
}

Xmmi Xmmi::Replicate(int in_value) {
#ifdef DEEP_USE_SSE
	return _mm_set1_epi32(in_value);
#else
	return Xmmi{ in_value, in_value, in_value, in_value };
#endif
}

int32 Xmmi::ToBooleanBitMask() const {
#ifdef DEEP_USE_SSE
	return _mm_movemask_ps(_mm_castsi128_ps(_internal));
#else
	// NOTE(randomuserhi): Casting int32 to uint32 is well defined to be no-op as if implemented with twos-complement,
	// https://stackoverflow.com/a/59601196
	//                     So no need for a bitcast
	return (static_cast<uint32>(x) >> 31u) | ((static_cast<uint32>(y) >> 31u) << 1) | ((static_cast<uint32>(z) >> 31u) << 2u)
	       | ((static_cast<uint32>(w) >> 31u) << 3u);
#endif
}

Xmmi Xmmi::Xor(const XmmiArg in_a, const XmmiArg in_b) {
#ifdef DEEP_USE_SSE
	return _mm_xor_si128(in_a, in_b);
#else
	return Xmmi{ in_a.x ^ in_b.x, in_a.y ^ in_b.y, in_a.z ^ in_b.z, in_a.w ^ in_b.w };
#endif
}
constexpr Xmmi Xmmi::Constexpr_Xor(const XmmiArg in_a, const XmmiArg in_b) {
	return Xmmi::Constexpr(in_a.x ^ in_b.x, in_a.y ^ in_b.y, in_a.z ^ in_b.z, in_a.w ^ in_b.w);
}

Xmmi Xmmi::And(const XmmiArg in_a, const XmmiArg in_b) {
#ifdef DEEP_USE_SSE
	return _mm_and_si128(in_a, in_b);
#else
	return Xmmi{ in_a.x & in_b.x, in_a.y & in_b.y, in_a.z & in_b.z, in_a.w & in_b.w };
#endif
}
constexpr Xmmi Xmmi::Constexpr_And(const XmmiArg in_a, const XmmiArg in_b) {
	return Xmmi::Constexpr(in_a.x & in_b.x, in_a.y & in_b.y, in_a.z & in_b.z, in_a.w & in_b.w);
}

Xmmi Xmmi::Equals(const XmmiArg in_a, const XmmiArg in_b) {
#ifdef DEEP_USE_SSE
	return _mm_cmpeq_epi32(in_a, in_b);
#else
	return Xmmi{ in_a.x == in_b.x ? static_cast<int32>(0xffffffff) : 0,
		         in_a.y == in_b.y ? static_cast<int32>(0xffffffff) : 0,
		         in_a.z == in_b.z ? static_cast<int32>(0xffffffff) : 0,
		         in_a.w == in_b.w ? static_cast<int32>(0xffffffff) : 0 };
#endif
}
constexpr Xmmi Xmmi::Constexpr_Equals(const XmmiArg in_a, const XmmiArg in_b) {
	return Xmmi::Constexpr(in_a.x == in_b.x ? static_cast<int32>(0xffffffff) : 0, //
	                       in_a.y == in_b.y ? static_cast<int32>(0xffffffff) : 0, //
	                       in_a.z == in_b.z ? static_cast<int32>(0xffffffff) : 0, //
	                       in_a.w == in_b.w ? static_cast<int32>(0xffffffff) : 0);
}

template<const uint32 Count>
Xmmi Xmmi::LogicalShiftLeft() const {
	static_assert(Count <= 31, "Invalid shift");

#ifdef DEEP_USE_SSE
	return _mm_slli_epi32(_internal, Count);
#else
	return Xmmi{ x << Count, y << Count, z << Count, w << Count };
#endif
}

template<const uint32 Count>
Xmmi Xmmi::LogicalShiftRight() const {
	static_assert(Count <= 31, "Invalid shift");

#ifdef DEEP_USE_SSE
	return _mm_srli_epi32(_internal, Count);
#else
	return Xmmi{ x >> Count, y >> Count, z >> Count, w >> Count };
#endif
}

Xmmi Xmmi::Select(const XmmiArg in_a, const XmmiArg in_b, const XmmiArg in_control) {
#ifdef DEEP_USE_SSE4_1
	return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(in_a), _mm_castsi128_ps(in_b), _mm_castsi128_ps(in_control)));
#else
	return Xmmi{ in_control.x ? in_b.x : in_a.x, in_control.y ? in_b.y : in_a.y, in_control.z ? in_b.z : in_a.z,
		         in_control.w ? in_b.w : in_a.w };
#endif
}

Deep_Inline bool operator!=(const XmmiArg in_a, const XmmiArg in_b) {
#ifdef DEEP_USE_SSE4_1
	return Xmmi::Equals(in_a, in_b).ToBooleanBitMask() != 0b1111;
#else
	return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z || in_a.w != in_b.w;
#endif
}
Deep_Inline bool operator==(const XmmiArg in_a, const XmmiArg in_b) {
	return !(in_a != in_b);
}

Xmmi& Xmmi::operator+=(const XmmiArg in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_add_epi32(_internal, in_other);
#else
	x += in_other.x;
	y += in_other.y;
	z += in_other.z;
	w += in_other.w;
#endif
	return *this;
}

Xmmi operator+(Xmmi in_a, const XmmiArg in_b) {
	return in_a += in_b;
}

Xmmi& Xmmi::operator-=(const XmmiArg in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_sub_epi32(_internal, in_other);
#else
	x -= in_other.x;
	y -= in_other.y;
	z -= in_other.z;
	w -= in_other.w;
#endif
	return *this;
}

Xmmi operator-(Xmmi in_a, const XmmiArg in_b) {
	return in_a -= in_b;
}

Xmmi operator-(const XmmiArg in_a) {
#ifdef DEEP_USE_SSE4_1
	return _mm_sub_epi32(_mm_setzero_si128(), in_a);
#else
	// NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
	return Xmmi{ 0 - in_a.x, 0 - in_a.y, 0 - in_a.z, 0 - in_a.w };
#endif
}

Xmmi& Xmmi::operator*=(const XmmiArg in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_mullo_epi32(_internal, in_other);
#else
	x *= in_other.x;
	y *= in_other.y;
	z *= in_other.z;
	w *= in_other.w;
#endif
	return *this;
}
Xmmi operator*(Xmmi in_a, const XmmiArg in_b) {
	return in_a *= in_b;
}

Xmmi& Xmmi::operator*=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
	_internal = _mm_mullo_epi32(_internal, _mm_set1_epi32(in_other));
#else
	x *= in_other;
	y *= in_other;
	z *= in_other;
	w *= in_other;
#endif
	return *this;
}

Xmmi operator*(Xmmi in_vec, int32 in_val) {
	return in_vec *= in_val;
}

Xmmi operator*(int32 in_val, const XmmiArg in_vec) {
#ifdef DEEP_USE_SSE4_1
	return _mm_mullo_epi32(_mm_set1_epi32(in_val), in_vec);
#else
	return Xmmi{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z, in_val * in_vec.w };
#endif
}

Xmmi& Xmmi::operator/=(const XmmiArg in_other) {
	x /= in_other.x;
	y /= in_other.y;
	z /= in_other.z;
	w /= in_other.w;
	return *this;
}
Xmmi operator/(Xmmi in_a, const XmmiArg in_b) {
	return in_a /= in_b;
}

Xmmi& Xmmi::operator/=(int32 in_other) {
	x /= in_other;
	y /= in_other;
	z /= in_other;
	w /= in_other;
	return *this;
}

Xmmi operator/(Xmmi in_vec, int32 in_val) {
	return in_vec /= in_val;
}

Xmmi operator/(int32 in_val, const XmmiArg in_vec) {
	return Xmmi{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z, in_val / in_vec.w };
}

DEEP_NAMESPACE_END