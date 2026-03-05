#pragma once

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Vec3i::Vec3i(int32 in_x, int32 in_y, int32 in_z) :
    sse_m128i(in_x, in_y, in_z, in_z) {}
#else
Vec3i::Vec3i(int32 in_x, int32 in_y, int32 in_z) {
    x = in_x;
    y = in_y;
    z = in_z;
}
#endif

#ifdef DEEP_USE_SSE4_1
Vec3i::Vec3i(Vec4iArg in_vec) :
    sse_m128i(in_vec.sse_m128i) {}
#else
Vec3i::Vec3i(Vec4iArg in_vec) {
    x = in_vec.x;
    y = in_vec.y;
    z = in_vec.z;
}
#endif

#ifdef DEEP_USE_SSE4_1
Vec3i::Vec3i(SSE_m128i in_sse_m128i) :
    sse_m128i(in_sse_m128i) {}
#else
Vec3i::Vec3i(SSE_m128i in_sse_m128i) {
    x = in_sse_m128i.x;
    y = in_sse_m128i.y;
    z = in_sse_m128i.z;
}
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
    SSE_m128i mul = _mm_mullo_epi32(in_a.sse_m128i, in_b.sse_m128i);
    return mul.x + mul.y + mul.z;
#else
    return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z;
#endif
}

Vec3i::operator Vec3() const {
    return Vec3{ _mm_cvtepi32_ps(sse_m128i) };
}

Deep_Inline bool operator!=(Vec3iArg in_a, Vec3iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    return (SSE_m128i::Equals(in_a.sse_m128i, in_b.sse_m128i).ToBooleanBitMask() & 0b111) != 0b111;
#else
    return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z;
#endif
}
Deep_Inline bool operator==(Vec3iArg a, Vec3iArg b) {
    return !(a != b);
}

Vec3i& Vec3i::operator+=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i += in_other.sse_m128i;
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
    result.sse_m128i = in_a.sse_m128i + in_b.sse_m128i;
    return result;
#else
    return Vec3{ in_a.x + in_b.x, in_a.y + in_b.y, in_a.z + in_b.z };
#endif
}

Vec3i& Vec3i::operator-=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i -= in_other.sse_m128i;
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
    result.sse_m128i = in_a.sse_m128i - in_b.sse_m128i;
    return result;
#else
    return Vec3i{ in_a.x - in_b.x, in_a.y - in_b.y, in_a.z - in_b.z };
#endif
}

Vec3i operator-(Vec3iArg in_a) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.sse_m128i = -in_a.sse_m128i;
    return result;
#else
    // NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
    return Vec3i{ 0.0f - in_a.x, 0.0f - in_a.y, 0.0f - in_a.z };
#endif
}

Vec3i& Vec3i::operator*=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i *= in_other.sse_m128i;
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
    result.sse_m128i = in_a.sse_m128i * in_b.sse_m128i;
    return result;
#else
    return Vec3i{ in_a.x * in_b.x, in_a.y * in_b.y, in_a.z * in_b.z };
#endif
}

Vec3i& Vec3i::operator*=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i *= in_other;
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
    result.sse_m128i = in_vec.sse_m128i * in_val;
    return result;
#else
    return Vec3i{ in_vec.x * in_val, in_vec.y * in_val, in_vec.z * in_val };
#endif
}

Vec3i operator*(int32 in_val, Vec3iArg in_vec) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.sse_m128i = in_val * in_vec.sse_m128i;
    return result;
#else
    return Vec3i{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z };
#endif
}

Vec3i& Vec3i::operator/=(Vec3iArg in_other) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i /= in_other.sse_m128i;
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
    result.sse_m128i = in_a.sse_m128i / in_b.sse_m128i;
    return result;
#else
    return Vec3{ in_a.x / in_b.x, in_a.y / in_b.y, in_a.z / in_b.z };
#endif
}

Vec3i& Vec3i::operator/=(int32 in_other) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i /= in_other;
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
    result.sse_m128i = in_vec.sse_m128i / in_val;
    return result;
#else
    return Vec3i{ in_vec.x / in_val, in_vec.y / in_val, in_vec.z / in_val };
#endif
}

Vec3i operator/(int32 in_val, Vec3iArg in_vec) {
#ifdef DEEP_USE_SSE4_1
    Vec3i result;
    result.sse_m128i = in_val / in_vec.sse_m128i;
    return result;
#else
    return Vec3i{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z };
#endif
}

DEEP_NAMESPACE_END