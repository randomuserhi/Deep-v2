#pragma once

DEEP_NAMESPACE_BEGIN

#ifdef DEEP_USE_SSE4_1
Vec4::Vec4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
    sse_m128(in_x, in_y, in_z, in_w) {}
#else
Vec4::Vec4(float32 roX, float32 in_y, float32 in_z, float32 in_w) {
    x = in_x;
    y = in_y;
    z = in_z;
    w = in_w;
}
#endif

Vec4::Vec4(SSE_m128 in_sse_m128) :
    sse_m128(in_sse_m128) {}

Vec4::Vec4(Vec3Arg in_xyz, float32 in_w) {
#ifdef DEEP_USE_SSE4_1
    sse_m128 = _mm_blend_ps(in_xyz.sse_m128, _mm_set1_ps(in_w), 8);
#else
    x = in_xyz.x;
    y = in_xyz.y;
    z = in_xyz.z;
    w = in_w;
#endif
}

Vec4& Vec4::Normalize() {
#ifdef DEEP_USE_SSE4_1
    SSE_m128 magnitudeSqrd = _mm_dp_ps(sse_m128, sse_m128, 0xff);
    if (_mm_cvtss_f32(magnitudeSqrd) != 0.0f) sse_m128 = _mm_div_ps(sse_m128, _mm_sqrt_ps(magnitudeSqrd));
    return *this;
#else
    float32 magnitudeSqrd = sqrdMagnitude();
    if (magnitudeSqrd != 0.0) *this /= Deep::Sqrt(magnitudeSqrd);
    return *this;
#endif
}
Vec4 Vec4::normalized() const {
    Vec4 v = *this;
    return v.Normalize();
}
bool Vec4::IsNormalized(float tolerance) const {
    return Deep::Abs(sqrdMagnitude() - 1.0f) <= tolerance;
}

float32 Vec4::sqrdMagnitude() const {
#ifdef DEEP_USE_SSE4_1
    return _mm_cvtss_f32(_mm_dp_ps(sse_m128, sse_m128, 0xff));
#else
    // NOTE(randomuserhi): brackets to keep consistent with vectorised version
    return (x * x + y * y) + (z * z + w * w);
#endif
}
float32 Vec4::magnitude() const {
#ifdef DEEP_USE_SSE4_1
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(sse_m128, sse_m128, 0xff)));
#else
    return Deep::Sqrt(sqrdMagnitude());
#endif
}

float32 Vec4::Dot(Vec4Arg in_a, Vec4Arg in_b) {
#ifdef DEEP_USE_SSE4_1
    return _mm_cvtss_f32(_mm_dp_ps(in_a.sse_m128, in_b.sse_m128, 0xff));
#else
    return in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w;
#endif
}

Vec4::operator Vec4i() const {
    return Vec4i{ _mm_cvtps_epi32(sse_m128) };
}

Deep_Inline bool operator!=(Vec4Arg in_a, Vec4Arg in_b) {
    return in_a.sse_m128 != in_b.sse_m128;
}
Deep_Inline bool operator==(Vec4Arg in_a, Vec4Arg in_b) {
    return !(in_a != in_b);
}

Vec4& Vec4::operator+=(Vec4Arg in_other) {
    sse_m128 += in_other.sse_m128;
    return *this;
}

Vec4 operator+(Vec4Arg in_a, Vec4Arg in_b) {
    Vec4 result;
    result.sse_m128 = in_a.sse_m128 + in_b.sse_m128;
    return result;
}

Vec4& Vec4::operator-=(Vec4Arg in_other) {
    sse_m128 -= in_other.sse_m128;
    return *this;
}

Vec4 operator-(Vec4Arg in_a, Vec4Arg in_b) {
    Vec4 result;
    result.sse_m128 = in_a.sse_m128 - in_b.sse_m128;
    return result;
}

Vec4 operator-(Vec4Arg in_a) {
    Vec4 result;
    result.sse_m128 = -in_a.sse_m128;
    return result;
}

Vec4& Vec4::operator*=(Vec4Arg in_other) {
    sse_m128 *= in_other.sse_m128;
    return *this;
}
Vec4 operator*(Vec4Arg in_a, Vec4Arg in_b) {
    Vec4 result;
    result.sse_m128 = in_a.sse_m128 * in_b.sse_m128;
    return result;
}

Vec4& Vec4::operator*=(float32 in_other) {
    sse_m128 *= in_other;
    return *this;
}

Vec4 operator*(Vec4Arg in_vec, float32 in_val) {
    Vec4 result;
    result.sse_m128 = in_vec.sse_m128 * in_val;
    return result;
}

Vec4 operator*(float32 in_val, Vec4Arg in_vec) {
    Vec4 result;
    result.sse_m128 = in_val * in_vec.sse_m128;
    return result;
}

Vec4& Vec4::operator/=(Vec4Arg in_other) {
    sse_m128 /= in_other.sse_m128;
    return *this;
}
Vec4 operator/(Vec4Arg in_a, Vec4Arg in_b) {
    Vec4 result;
    result.sse_m128 = in_a.sse_m128 / in_b.sse_m128;
    return result;
}

Vec4& Vec4::operator/=(float32 in_other) {
    sse_m128 /= in_other;
    return *this;
}

Vec4 operator/(Vec4Arg in_vec, float32 in_val) {
    Vec4 result;
    result.sse_m128 = in_vec.sse_m128 / in_val;
    return result;
}

Vec4 operator/(float32 in_val, Vec4Arg in_vec) {
    Vec4 result;
    result.sse_m128 = in_val / in_vec.sse_m128;
    return result;
}

DEEP_NAMESPACE_END