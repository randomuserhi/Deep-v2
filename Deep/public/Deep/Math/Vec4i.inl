#pragma once

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Vec4i::Vec4i(int32 in_x, int32 in_y, int32 in_z, int32 in_w) :
    sse_m128i(in_x, in_y, in_z, in_w) {}
#else
Vec4i::Vec4i(int32 roX, int32 in_y, int32 in_z, int32 in_w) {
    x = in_x;
    y = in_y;
    z = in_z;
    w = in_w;
}
#endif

Vec4i::Vec4i(SSE_m128i in_sse_m128i) :
    sse_m128i(in_sse_m128i) {}

Vec4i::Vec4i(Vec3iArg in_xyz, int32 in_w) {
#ifdef DEEP_USE_SSE4_1
    sse_m128i = _mm_blend_epi32(in_xyz.sse_m128i, _mm_set1_epi32(in_w), 8);
#else
    x = in_xyz.x;
    y = in_xyz.y;
    z = in_xyz.z;
    w = in_w;
#endif
}

float32 Vec4i::sqrdMagnitude() const {
    return static_cast<Vec4>(*this).sqrdMagnitude();
}

float32 Vec4i::magnitude() const {
    return static_cast<Vec4>(*this).magnitude();
}

int32 Vec4i::manhattanDistance() const {
#ifdef DEEP_USE_SSE4_1
    // NOTE(randomuserhi): Not even sure if SIMD horizontal sum here is worth. Might be better off without
    //                     Profiling required.
    SSE_m128i sum = _mm_add_epi32(sse_m128i, _mm_shuffle_epi32(sse_m128i, _MM_SHUFFLE(2, 3, 0, 1)));
    sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(1, 0, 3, 2)));
    return _mm_cvtsi128_si32(sum);
#else
    return x + y + z + w;
#endif
}

int32 Vec4i::Dot(Vec4iArg in_a, Vec4iArg in_b) {
#ifdef DEEP_USE_SSE4_1
    SSE_m128i sum = _mm_mullo_epi32(in_a.sse_m128i, in_b.sse_m128i);

    // NOTE(randomuserhi): Not even sure if SIMD horizontal sum here is worth. Might be better off without
    //                     Profiling required.
    sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(2, 3, 0, 1)));
    sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(1, 0, 3, 2)));
    return _mm_cvtsi128_si32(sum);
#else
    return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w;
#endif
}

Vec4i::operator Vec4() const {
    return Vec4{ _mm_cvtepi32_ps(sse_m128i) };
}

Deep_Inline bool operator!=(Vec4iArg in_a, Vec4iArg in_b) {
    return in_a.sse_m128i != in_b.sse_m128i;
}
Deep_Inline bool operator==(Vec4iArg in_a, Vec4iArg in_b) {
    return !(in_a != in_b);
}

Vec4i& Vec4i::operator+=(Vec4iArg in_other) {
    sse_m128i += in_other.sse_m128i;
    return *this;
}

Vec4i operator+(Vec4iArg in_a, Vec4iArg in_b) {
    Vec4i result;
    result.sse_m128i = in_a.sse_m128i + in_b.sse_m128i;
    return result;
}

Vec4i& Vec4i::operator-=(Vec4iArg in_other) {
    sse_m128i -= in_other.sse_m128i;
    return *this;
}

Vec4i operator-(Vec4iArg in_a, Vec4iArg in_b) {
    Vec4i result;
    result.sse_m128i = in_a.sse_m128i - in_b.sse_m128i;
    return result;
}

Vec4i operator-(Vec4iArg in_a) {
    Vec4i result;
    result.sse_m128i = -in_a.sse_m128i;
    return result;
}

Vec4i& Vec4i::operator*=(Vec4iArg in_other) {
    sse_m128i *= in_other.sse_m128i;
    return *this;
}
Vec4i operator*(Vec4iArg in_a, Vec4iArg in_b) {
    Vec4i result;
    result.sse_m128i = in_a.sse_m128i * in_b.sse_m128i;
    return result;
}

Vec4i& Vec4i::operator*=(int32 in_other) {
    sse_m128i *= in_other;
    return *this;
}

Vec4i operator*(Vec4iArg in_vec, int32 in_val) {
    Vec4i result;
    result.sse_m128i = in_vec.sse_m128i * in_val;
    return result;
}

Vec4i operator*(int32 in_val, Vec4iArg in_vec) {
    Vec4i result;
    result.sse_m128i = in_val * in_vec.sse_m128i;
    return result;
}

Vec4i& Vec4i::operator/=(Vec4iArg in_other) {
    sse_m128i /= in_other.sse_m128i;
    return *this;
}
Vec4i operator/(Vec4iArg in_a, Vec4iArg in_b) {
    Vec4i result;
    result.sse_m128i = in_a.sse_m128i / in_b.sse_m128i;
    return result;
}

Vec4i& Vec4i::operator/=(int32 in_other) {
    sse_m128i /= in_other;
    return *this;
}

Vec4i operator/(Vec4iArg in_vec, int32 in_val) {
    Vec4i result;
    result.sse_m128i = in_vec.sse_m128i / in_val;
    return result;
}

Vec4i operator/(int32 in_val, Vec4iArg in_vec) {
    Vec4i result;
    result.sse_m128i = in_val / in_vec.sse_m128i;
    return result;
}

DEEP_NAMESPACE_END