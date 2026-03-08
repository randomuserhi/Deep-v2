#pragma once

#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec3i.h"
#include "Deep/Math/Vec4i.h"

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Vec3i::Vec3i(int32 in_x, int32 in_y, int32 in_z) :
    xmmi(in_x, in_y, in_z, in_z) {}
#else
Vec3i::Vec3i(int32 in_x, int32 in_y, int32 in_z) :
    x(in_x), y(in_y), z(in_z) {}
#endif

#ifdef DEEP_USE_SSE4_1
Vec3i::Vec3i(Vec4iArg in_vec) :
    xmmi(in_vec.xmmi) {}
#else
Vec3i::Vec3i(Vec4iArg in_vec) :
    x(in_vec.x), y(in_vec.y), z(in_vec.z) {}
#endif

#ifdef DEEP_USE_SSE4_1
Vec3i::Vec3i(Xmmi in_xmmi) :
    xmmi(in_xmmi) {}
#else
Vec3i::Vec3i(Xmmi in_xmmi) :
    x(in_xmmi.x), y(in_xmmi.y), z(in_xmmi.z) {}
#endif

float32 Vec3i::sqrdMagnitude() const {
    return static_cast<Vec3>(*this).sqrdMagnitude();
}

float32 Vec3i::magnitude() const {
    return static_cast<Vec3>(*this).magnitude();
}

int32 Vec3i::manhattanDistance() const {
    return x + y + z;
}

int32 Vec3i::Dot(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    Xmmi mul = _mm_mullo_epi32(in_a.xmmi, in_b.xmmi);
    return mul.x + mul.y + mul.z;
#else
    return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z;
#endif
}

Vec3i::operator Vec3() const {
    return Vec3{ _mm_cvtepi32_ps(xmmi) };
}

Deep_Inline bool operator!=(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    return (Xmmi::Equals(in_a.xmmi, in_b.xmmi).ToBooleanBitMask() & 0b111) != 0b111;
#else
    return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z;
#endif
}
Deep_Inline bool operator==(Vec3iArg a, Vec3iArg b) {
    return !(a != b);
}

Vec3i& Vec3i::operator+=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    xmmi += in_other.xmmi;
#else
    x += in_other.x;
    y += in_other.y;
    z += in_other.z;
#endif
    return *this;
}

Vec3i operator+(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_a.xmmi + in_b.xmmi;
    return result;
#else
    return Vec3{ in_a.x + in_b.x, in_a.y + in_b.y, in_a.z + in_b.z };
#endif
}

Vec3i& Vec3i::operator-=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    xmmi -= in_other.xmmi;
#else
    x -= in_other.x;
    y -= in_other.y;
    z -= in_other.z;
#endif
    return *this;
}

Vec3i operator-(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_a.xmmi - in_b.xmmi;
    return result;
#else
    return Vec3i{ in_a.x - in_b.x, in_a.y - in_b.y, in_a.z - in_b.z };
#endif
}

Vec3i operator-(Vec3iArg in_a) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = -in_a.xmmi;
    return result;
#else
    // NOTE(randomuserhi): 0 - x to stay consistent with vectorised version
    return Vec3i{ 0 - in_a.x, 0 - in_a.y, 0 - in_a.z };
#endif
}

Vec3i& Vec3i::operator*=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    xmmi *= in_other.xmmi;
#else
    x *= in_other.x;
    y *= in_other.y;
    z *= in_other.z;
#endif
    return *this;
}
Vec3i operator*(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_a.xmmi * in_b.xmmi;
    return result;
#else
    return Vec3i{ in_a.x * in_b.x, in_a.y * in_b.y, in_a.z * in_b.z };
#endif
}

Vec3i& Vec3i::operator*=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
    xmmi *= in_other;
#else
    x *= in_other;
    y *= in_other;
    z *= in_other;
#endif
    return *this;
}

Vec3i operator*(Vec3iArg in_vec, int32 in_val) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_vec.xmmi * in_val;
    return result;
#else
    return Vec3i{ in_vec.x * in_val, in_vec.y * in_val, in_vec.z * in_val };
#endif
}

Vec3i operator*(int32 in_val, Vec3iArg in_vec) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_val * in_vec.xmmi;
    return result;
#else
    return Vec3i{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z };
#endif
}

Vec3i& Vec3i::operator/=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    xmmi /= in_other.xmmi;
#else
    x /= in_other.x;
    y /= in_other.y;
    z /= in_other.z;
#endif
    return *this;
}
Vec3i operator/(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_a.xmmi / in_b.xmmi;
    return result;
#else
    return Vec3{ in_a.x / in_b.x, in_a.y / in_b.y, in_a.z / in_b.z };
#endif
}

Vec3i& Vec3i::operator/=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
    xmmi /= in_other;
#else
    x /= in_other;
    y /= in_other;
    z /= in_other;
#endif
    return *this;
}

Vec3i operator/(Vec3iArg in_vec, int32 in_val) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_vec.xmmi / in_val;
    return result;
#else
    return Vec3i{ in_vec.x / in_val, in_vec.y / in_val, in_vec.z / in_val };
#endif
}

Vec3i operator/(int32 in_val, Vec3iArg in_vec) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.xmmi = in_val / in_vec.xmmi;
    return result;
#else
    return Vec3i{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z };
#endif
}

DEEP_NAMESPACE_END