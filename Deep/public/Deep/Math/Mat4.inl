#pragma once

#include "Deep/Math/Mat4.h"
#include "Deep/Math/Quat.h"

DEEP_NAMESPACE_BEGIN

Mat4::Mat4(Xmm in_col0, Xmm in_col1, Xmm in_col2, Xmm in_col3) :
    m_cols{ in_col0, in_col1, in_col2, in_col3 } {}
Mat4::Mat4(                                                         //
    float32 in_m00, float32 in_m01, float32 in_m02, float32 in_m03, //
    float32 in_m10, float32 in_m11, float32 in_m12, float32 in_m13, //
    float32 in_m20, float32 in_m21, float32 in_m22, float32 in_m23, //
    float32 in_m30, float32 in_m31, float32 in_m32, float32 in_m33  //
    ) :
    m_cols{
        Xmm(in_m00, in_m10, in_m20, in_m30), //
        Xmm(in_m01, in_m11, in_m21, in_m31), //
        Xmm(in_m02, in_m12, in_m22, in_m32), //
        Xmm(in_m03, in_m13, in_m23, in_m33)  //
    } {}
Mat4::Mat4(Vec4 in_col0, Vec4 in_col1, Vec4 in_col2, Vec4 in_col3) :
    m_vcols{ in_col0, in_col1, in_col2, in_col3 } {}

Mat4& Mat4::Transpose() {
    *this = transposed();
    return *this;
}
Mat4 Mat4::transposed() const {
#ifdef DEEP_USE_SSE
    __m128 tmp1 = _mm_shuffle_ps(m_cols[0], m_cols[1], _MM_SHUFFLE(1, 0, 1, 0));
    __m128 tmp3 = _mm_shuffle_ps(m_cols[0], m_cols[1], _MM_SHUFFLE(3, 2, 3, 2));
    __m128 tmp2 = _mm_shuffle_ps(m_cols[2], m_cols[3], _MM_SHUFFLE(1, 0, 1, 0));
    __m128 tmp4 = _mm_shuffle_ps(m_cols[2], m_cols[3], _MM_SHUFFLE(3, 2, 3, 2));

    return Mat4{
        _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(2, 0, 2, 0)), //
        _mm_shuffle_ps(tmp1, tmp2, _MM_SHUFFLE(3, 1, 3, 1)), //
        _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(2, 0, 2, 0)), //
        _mm_shuffle_ps(tmp3, tmp4, _MM_SHUFFLE(3, 1, 3, 1))  //
    };
#else
    return Mat4{
        m00, m10, m20, m30, //
        m01, m11, m21, m31, //
        m02, m12, m22, m32, //
        m03, m13, m23, m33, //
    };
#endif
}

Mat4 Mat4::FromQuaternion(const Quat& in_quat) {
    Deep_Assert(in_quat.IsNormalized(), "Quaternion should be normalized");

    // See: https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation section 'Quaternion-derived rotation matrix'

#ifdef DEEP_USE_SSE4_1
    __m128 xyzw = in_quat.xmm;
    __m128 two_xyzw = _mm_add_ps(xyzw, xyzw);
    __m128 yzxw = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 two_yzxw = _mm_add_ps(yzxw, yzxw);
    __m128 zxyw = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 two_zxyw = _mm_add_ps(zxyw, zxyw);
    __m128 wwww = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3, 3, 3, 3));
    __m128 diagonal =
        _mm_sub_ps(_mm_sub_ps(_mm_set1_ps(1.0f), _mm_mul_ps(two_yzxw, yzxw)),
                   _mm_mul_ps(two_zxyw, zxyw)); // (1 - 2 y^2 - 2 z^2, 1 - 2 x^2 - 2 z^2, 1 - 2 x^2 - 2 y^2, 1 - 4 w^2)
    __m128 plus = _mm_add_ps(_mm_mul_ps(two_xyzw, zxyw), _mm_mul_ps(two_yzxw, wwww));  // 2 * (xz + yw, xy + zw, yz + xw, ww)
    __m128 minus = _mm_sub_ps(_mm_mul_ps(two_yzxw, xyzw), _mm_mul_ps(two_zxyw, wwww)); // 2 * (xy - zw, yz - xw, xz - yw, 0)

    // Workaround for compiler changing _mm_sub_ps(_mm_mul_ps(...), ...) into a fused multiply sub instruction, resulting
    // in w not being 0 There doesn't appear to be a reliable way to turn this off in Clang
    minus = _mm_insert_ps(minus, minus, 0b1000);

    return Mat4{ _mm_blend_ps(_mm_blend_ps(plus, diagonal, 0b0001), minus,
                              0b1100), // (1 - 2 y^2 - 2 z^2, 2 xy + 2 zw, 2 xz - 2 yw, 0)
                 _mm_blend_ps(_mm_blend_ps(diagonal, minus, 0b1001), plus,
                              0b0100), // (2 xy - 2 zw, 1 - 2 x^2 - 2 z^2, 2 yz + 2 xw, 0)
                 _mm_blend_ps(_mm_blend_ps(minus, plus, 0b0001), diagonal,
                              0b0100), // (2 xz + 2 yw, 2 yz - 2 xw, 1 - 2 x^2 - 2 y^2, 0)
                 _mm_set_ps(1, 0, 0, 0) };
#else
    float32 x = in_quat.x;
    float32 y = in_quat.y;
    float32 z = in_quat.z;
    float32 w = in_quat.w;

    // Note: Using x + x instead of 2.0f * x to to stay consistent with vectorised version
    float32 tx = x + x;
    float32 ty = y + y;
    float32 tz = z + z;

    float32 xx = tx * x;
    float32 yy = ty * y;
    float32 zz = tz * z;
    float32 xy = tx * y;
    float32 xz = tx * z;
    float32 xw = tx * w;
    float32 yz = ty * z;
    float32 yw = ty * w;
    float32 zw = tz * w;

    // NOTE(randomuserhi): Brackets to stay consistent with vectorised version
    return Mat4{
        Xmm((1.0f - yy) - zz, xy + zw, xz - yw, 0.0f), //
        Xmm(xy - zw, (1.0f - zz) - xx, yz + xw, 0.0f), //
        Xmm(xz + yw, yz - xw, (1.0f - xx) - yy, 0.0f), //
        Xmm(0.0f, 0.0f, 0.0f, 1.0f)                    //
    };
#endif
}

Mat4& Mat4::Inverse() {
    *this = inversed();
    return *this;
}

Mat4 Mat4::inversed() const {
#ifdef DEEP_USE_SSE
    // Algorithm from: http://download.intel.com/design/PentiumIII/sml/24504301.pdf
    // Mirror: https://peertje.daanberg.net/drivers/intel/download.intel.com/design/PentiumIII/sml/24504301.pdf
    // Streaming SIMD Extensions - Inverse of 4x4 Matrix
    // Adapted to load data using _mm_shuffle_ps instead of loading from memory
    // Replaced _mm_rcp_ps with _mm_div_ps for better accuracy
    __m128 tmp1 = _mm_shuffle_ps(m_cols[0], m_cols[1], _MM_SHUFFLE(1, 0, 1, 0));
    __m128 row1 = _mm_shuffle_ps(m_cols[2], m_cols[3], _MM_SHUFFLE(1, 0, 1, 0));
    __m128 row0 = _mm_shuffle_ps(tmp1, row1, _MM_SHUFFLE(2, 0, 2, 0));
    row1 = _mm_shuffle_ps(row1, tmp1, _MM_SHUFFLE(3, 1, 3, 1));
    tmp1 = _mm_shuffle_ps(m_cols[0], m_cols[1], _MM_SHUFFLE(3, 2, 3, 2));
    __m128 row3 = _mm_shuffle_ps(m_cols[2], m_cols[3], _MM_SHUFFLE(3, 2, 3, 2));
    __m128 row2 = _mm_shuffle_ps(tmp1, row3, _MM_SHUFFLE(2, 0, 2, 0));
    row3 = _mm_shuffle_ps(row3, tmp1, _MM_SHUFFLE(3, 1, 3, 1));

    tmp1 = _mm_mul_ps(row2, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 minor0 = _mm_mul_ps(row1, tmp1);
    __m128 minor1 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 3, 2));
    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, _MM_SHUFFLE(1, 0, 3, 2));

    tmp1 = _mm_mul_ps(row1, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(2, 3, 0, 1));
    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
    __m128 minor3 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 3, 2));
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
    minor3 = _mm_shuffle_ps(minor3, minor3, _MM_SHUFFLE(1, 0, 3, 2));

    tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, _MM_SHUFFLE(1, 0, 3, 2)), row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(2, 3, 0, 1));
    row2 = _mm_shuffle_ps(row2, row2, _MM_SHUFFLE(1, 0, 3, 2));
    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
    __m128 minor2 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 3, 2));
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, _MM_SHUFFLE(1, 0, 3, 2));

    tmp1 = _mm_mul_ps(row0, row1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(2, 3, 0, 1));
    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 3, 2));
    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

    tmp1 = _mm_mul_ps(row0, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(2, 3, 0, 1));
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 3, 2));
    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

    tmp1 = _mm_mul_ps(row0, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(2, 3, 0, 1));
    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, _MM_SHUFFLE(1, 0, 3, 2));
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

    __m128 det = _mm_mul_ps(row0, minor0);
    // NOTE(randomuserhi): Original code did (x + z) + (y + w), changed to (x + y) + (z + w) to match the non vectorised
    // code
    det = _mm_add_ps(_mm_shuffle_ps(det, det, _MM_SHUFFLE(2, 3, 0, 1)), det);
    det = _mm_add_ss(_mm_shuffle_ps(det, det, _MM_SHUFFLE(1, 0, 3, 2)), det);
    det = _mm_div_ss(_mm_set_ss(1.0f), det);
    det = _mm_shuffle_ps(det, det, _MM_SHUFFLE(0, 0, 0, 0));

    return Mat4{
        _mm_mul_ps(det, minor0), //
        _mm_mul_ps(det, minor1), //
        _mm_mul_ps(det, minor2), //
        _mm_mul_ps(det, minor3)  //
    };
#else
    float32 m00 = this->m00, m01 = this->m01, m02 = this->m02, m03 = this->m03;
    float32 m10 = this->m10, m11 = this->m11, m12 = this->m12, m13 = this->m13;
    float32 m20 = this->m20, m21 = this->m21, m22 = this->m22, m23 = this->m23;
    float32 m30 = this->m30, m31 = this->m31, m32 = this->m32, m33 = this->m33;

    float32 n01121102 = m01 * m12 - m11 * m02;
    float32 n01222102 = m01 * m22 - m21 * m02;
    float32 n01323102 = m01 * m32 - m31 * m02;
    float32 n01131103 = m01 * m13 - m11 * m03;
    float32 n01232103 = m01 * m23 - m21 * m03;
    float32 n01333103 = m01 * m33 - m31 * m03;
    float32 n11222112 = m11 * m22 - m21 * m12;
    float32 n11323112 = m11 * m32 - m31 * m12;
    float32 n11232113 = m11 * m23 - m21 * m13;
    float32 n11333113 = m11 * m33 - m31 * m13;
    float32 n21323122 = m21 * m32 - m31 * m22;
    float32 n21333123 = m21 * m33 - m31 * m23;
    float32 n02131203 = m02 * m13 - m12 * m03;
    float32 n02232203 = m02 * m23 - m22 * m03;
    float32 n02333203 = m02 * m33 - m32 * m03;
    float32 n12232213 = m12 * m23 - m22 * m13;
    float32 n12333213 = m12 * m33 - m32 * m13;
    float32 n22333223 = m22 * m33 - m32 * m23;

    Mat4 result{
        Xmm{ m11 * n22333223 - m21 * n12333213 + m31 * n12232213, -m01 * n22333223 + m21 * n02333203 - m31 * n02232203,
             m01 * n12333213 - m11 * n02333203 + m31 * n02131203, -m01 * n12232213 + m11 * n02232203 - m21 * n02131203 },
        Xmm{ -m10 * n22333223 + m20 * n12333213 - m30 * n12232213, m00 * n22333223 - m20 * n02333203 + m30 * n02232203,
             -m00 * n12333213 + m10 * n02333203 - m30 * n02131203, m00 * n12232213 - m10 * n02232203 + m20 * n02131203 },
        Xmm{ m10 * n21333123 - m20 * n11333113 + m30 * n11232113, -m00 * n21333123 + m20 * n01333103 - m30 * n01232103,
             m00 * n11333113 - m10 * n01333103 + m30 * n01131103, -m00 * n11232113 + m10 * n01232103 - m20 * n01131103 },
        Xmm{ -m10 * n21323122 + m20 * n11323112 - m30 * n11222112, m00 * n21323122 - m20 * n01323102 + m30 * n01222102,
             -m00 * n11323112 + m10 * n01323102 - m30 * n01121102, m00 * n11222112 - m10 * n01222102 + m20 * n01121102 }
    };

    float det = m00 * result.m_cols[0].x + m10 * result.m_cols[0].y + m20 * result.m_cols[0].z + m30 * result.m_cols[0].w;

    result.m_cols[0] /= det;
    result.m_cols[1] /= det;
    result.m_cols[2] /= det;
    result.m_cols[3] /= det;

    return result;
#endif
}

Deep_Inline bool operator!=(Arg_Mat4 in_a, Arg_Mat4 in_b) {
    return Xmmi::And(Xmmi::And(Xmm::Equals(in_a.m_cols[0], in_b.m_cols[0]), Xmm::Equals(in_a.m_cols[1], in_b.m_cols[1])),
                     Xmmi::And(Xmm::Equals(in_a.m_cols[2], in_b.m_cols[2]), Xmm::Equals(in_a.m_cols[3], in_b.m_cols[3])))
               .ToBooleanBitMask()
           != 0b1111;
}
Deep_Inline bool operator==(Arg_Mat4 in_a, Arg_Mat4 in_b) {
    return !(in_a != in_b);
}

Mat4 operator*(Arg_Mat4 in_a, Arg_Mat4 in_b) {
    Mat4 c;

#ifdef DEEP_USE_SSE
    c.m_cols[0] = _mm_mul_ps(in_a.m_cols[0], _mm_shuffle_ps(in_b.m_cols[0], in_b.m_cols[0], _MM_SHUFFLE(0, 0, 0, 0)));
    c.m_cols[0] = _mm_add_ps(
        c.m_cols[0], _mm_mul_ps(in_a.m_cols[1], _mm_shuffle_ps(in_b.m_cols[0], in_b.m_cols[0], _MM_SHUFFLE(1, 1, 1, 1))));
    c.m_cols[0] = _mm_add_ps(
        c.m_cols[0], _mm_mul_ps(in_a.m_cols[2], _mm_shuffle_ps(in_b.m_cols[0], in_b.m_cols[0], _MM_SHUFFLE(2, 2, 2, 2))));
    c.m_cols[0] = _mm_add_ps(
        c.m_cols[0], _mm_mul_ps(in_a.m_cols[3], _mm_shuffle_ps(in_b.m_cols[0], in_b.m_cols[0], _MM_SHUFFLE(3, 3, 3, 3))));

    c.m_cols[1] = _mm_mul_ps(in_a.m_cols[0], _mm_shuffle_ps(in_b.m_cols[1], in_b.m_cols[1], _MM_SHUFFLE(0, 0, 0, 0)));
    c.m_cols[1] = _mm_add_ps(
        c.m_cols[1], _mm_mul_ps(in_a.m_cols[1], _mm_shuffle_ps(in_b.m_cols[1], in_b.m_cols[1], _MM_SHUFFLE(1, 1, 1, 1))));
    c.m_cols[1] = _mm_add_ps(
        c.m_cols[1], _mm_mul_ps(in_a.m_cols[2], _mm_shuffle_ps(in_b.m_cols[1], in_b.m_cols[1], _MM_SHUFFLE(2, 2, 2, 2))));
    c.m_cols[1] = _mm_add_ps(
        c.m_cols[1], _mm_mul_ps(in_a.m_cols[3], _mm_shuffle_ps(in_b.m_cols[1], in_b.m_cols[1], _MM_SHUFFLE(3, 3, 3, 3))));

    c.m_cols[2] = _mm_mul_ps(in_a.m_cols[0], _mm_shuffle_ps(in_b.m_cols[2], in_b.m_cols[2], _MM_SHUFFLE(0, 0, 0, 0)));
    c.m_cols[2] = _mm_add_ps(
        c.m_cols[2], _mm_mul_ps(in_a.m_cols[1], _mm_shuffle_ps(in_b.m_cols[2], in_b.m_cols[2], _MM_SHUFFLE(1, 1, 1, 1))));
    c.m_cols[2] = _mm_add_ps(
        c.m_cols[2], _mm_mul_ps(in_a.m_cols[2], _mm_shuffle_ps(in_b.m_cols[2], in_b.m_cols[2], _MM_SHUFFLE(2, 2, 2, 2))));
    c.m_cols[2] = _mm_add_ps(
        c.m_cols[2], _mm_mul_ps(in_a.m_cols[3], _mm_shuffle_ps(in_b.m_cols[2], in_b.m_cols[2], _MM_SHUFFLE(3, 3, 3, 3))));

    c.m_cols[3] = _mm_mul_ps(in_a.m_cols[0], _mm_shuffle_ps(in_b.m_cols[3], in_b.m_cols[3], _MM_SHUFFLE(0, 0, 0, 0)));
    c.m_cols[3] = _mm_add_ps(
        c.m_cols[3], _mm_mul_ps(in_a.m_cols[1], _mm_shuffle_ps(in_b.m_cols[3], in_b.m_cols[3], _MM_SHUFFLE(1, 1, 1, 1))));
    c.m_cols[3] = _mm_add_ps(
        c.m_cols[3], _mm_mul_ps(in_a.m_cols[2], _mm_shuffle_ps(in_b.m_cols[3], in_b.m_cols[3], _MM_SHUFFLE(2, 2, 2, 2))));
    c.m_cols[3] = _mm_add_ps(
        c.m_cols[3], _mm_mul_ps(in_a.m_cols[3], _mm_shuffle_ps(in_b.m_cols[3], in_b.m_cols[3], _MM_SHUFFLE(3, 3, 3, 3))));
#else
    c.m00 = in_a.m00 * in_b.m00 + in_a.m01 * in_b.m10 + in_a.m02 * in_b.m20 + in_a.m03 * in_b.m30;
    c.m10 = in_a.m10 * in_b.m00 + in_a.m11 * in_b.m10 + in_a.m12 * in_b.m20 + in_a.m13 * in_b.m30;
    c.m20 = in_a.m20 * in_b.m00 + in_a.m21 * in_b.m10 + in_a.m22 * in_b.m20 + in_a.m23 * in_b.m30;
    c.m30 = in_a.m30 * in_b.m00 + in_a.m31 * in_b.m10 + in_a.m32 * in_b.m20 + in_a.m33 * in_b.m30;

    c.m01 = in_a.m00 * in_b.m01 + in_a.m01 * in_b.m11 + in_a.m02 * in_b.m21 + in_a.m03 * in_b.m31;
    c.m11 = in_a.m10 * in_b.m01 + in_a.m11 * in_b.m11 + in_a.m12 * in_b.m21 + in_a.m13 * in_b.m31;
    c.m21 = in_a.m20 * in_b.m01 + in_a.m21 * in_b.m11 + in_a.m22 * in_b.m21 + in_a.m23 * in_b.m31;
    c.m31 = in_a.m30 * in_b.m01 + in_a.m31 * in_b.m11 + in_a.m32 * in_b.m21 + in_a.m33 * in_b.m31;

    c.m02 = in_a.m00 * in_b.m02 + in_a.m01 * in_b.m12 + in_a.m02 * in_b.m22 + in_a.m03 * in_b.m32;
    c.m12 = in_a.m10 * in_b.m02 + in_a.m11 * in_b.m12 + in_a.m12 * in_b.m22 + in_a.m13 * in_b.m32;
    c.m22 = in_a.m20 * in_b.m02 + in_a.m21 * in_b.m12 + in_a.m22 * in_b.m22 + in_a.m23 * in_b.m32;
    c.m32 = in_a.m30 * in_b.m02 + in_a.m31 * in_b.m12 + in_a.m32 * in_b.m22 + in_a.m33 * in_b.m32;

    c.m03 = in_a.m00 * in_b.m03 + in_a.m01 * in_b.m13 + in_a.m02 * in_b.m23 + in_a.m03 * in_b.m33;
    c.m13 = in_a.m10 * in_b.m03 + in_a.m11 * in_b.m13 + in_a.m12 * in_b.m23 + in_a.m13 * in_b.m33;
    c.m23 = in_a.m20 * in_b.m03 + in_a.m21 * in_b.m13 + in_a.m22 * in_b.m23 + in_a.m23 * in_b.m33;
    c.m33 = in_a.m30 * in_b.m03 + in_a.m31 * in_b.m13 + in_a.m32 * in_b.m23 + in_a.m33 * in_b.m33;
#endif

    return c;
}

Vec3 operator*(Arg_Mat4 in_mat, Arg_Vec3 in_vec) {
    Vec3 _v;
#ifdef DEEP_USE_SSE4_1
    _v.xmm = _mm_mul_ps(in_mat.m_cols[0], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(0, 0, 0, 0)));
    _v.xmm =
        _mm_add_ps(_v.xmm, _mm_mul_ps(in_mat.m_cols[1], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(1, 1, 1, 1))));
    _v.xmm =
        _mm_add_ps(_v.xmm, _mm_mul_ps(in_mat.m_cols[2], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(2, 2, 2, 2))));
    _v.xmm = _mm_add_ps(_v.xmm, in_mat.m_cols[3]);
#else
    float32 invW = 1.0f / (in_mat.m30 * in_vec.x + in_mat.m31 * in_vec.y + in_mat.m32 * in_vec.z + in_mat.m33);
    _v.x = (in_mat.m00 * in_vec.x + in_mat.m01 * in_vec.y + in_mat.m02 * in_vec.z + in_mat.m03) * invW;
    _v.y = (in_mat.m10 * in_vec.x + in_mat.m11 * in_vec.y + in_mat.m12 * in_vec.z + in_mat.m13) * invW;
    _v.z = (in_mat.m20 * in_vec.x + in_mat.m21 * in_vec.y + in_mat.m22 * in_vec.z + in_mat.m23) * invW;
#endif
    return _v;
}

Vec4 operator*(Arg_Mat4 in_mat, Arg_Vec4 in_vec) {
    Vec4 _v;
#ifdef DEEP_USE_SSE4_1
    _v.xmm = _mm_mul_ps(in_mat.m_cols[0], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(0, 0, 0, 0)));
    _v.xmm =
        _mm_add_ps(_v.xmm, _mm_mul_ps(in_mat.m_cols[1], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(1, 1, 1, 1))));
    _v.xmm =
        _mm_add_ps(_v.xmm, _mm_mul_ps(in_mat.m_cols[2], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(2, 2, 2, 2))));
    _v.xmm =
        _mm_add_ps(_v.xmm, _mm_mul_ps(in_mat.m_cols[3], _mm_shuffle_ps(in_vec.xmm, in_vec.xmm, _MM_SHUFFLE(3, 3, 3, 3))));
#else
    _v.x = in_mat.m00 * in_vec.x + in_mat.m01 * in_vec.y + in_mat.m02 * in_vec.z + in_mat.m03 * in_vec.w;
    _v.y = in_mat.m10 * in_vec.x + in_mat.m11 * in_vec.y + in_mat.m12 * in_vec.z + in_mat.m13 * in_vec.w;
    _v.z = in_mat.m20 * in_vec.x + in_mat.m21 * in_vec.y + in_mat.m22 * in_vec.z + in_mat.m23 * in_vec.w;
    _v.w = in_mat.m30 * in_vec.x + in_mat.m31 * in_vec.y + in_mat.m32 * in_vec.z + in_mat.m33 * in_vec.w;
#endif
    return _v;
}

Mat4& Mat4::operator*=(float32 in_other) {
    m_cols[0] *= in_other;
    m_cols[1] *= in_other;
    m_cols[2] *= in_other;
    m_cols[3] *= in_other;
    return *this;
}

Mat4 operator*(Arg_Mat4 in_vec, float32 in_val) {
    Mat4 result;
    result.m_cols[0] = in_vec.m_cols[0] * in_val;
    result.m_cols[1] = in_vec.m_cols[1] * in_val;
    result.m_cols[2] = in_vec.m_cols[2] * in_val;
    result.m_cols[3] = in_vec.m_cols[3] * in_val;
    return result;
}

Mat4 operator*(float32 in_val, Arg_Mat4 in_vec) {
    Mat4 result;
    result.m_cols[0] = in_val * in_vec.m_cols[0];
    result.m_cols[1] = in_val * in_vec.m_cols[1];
    result.m_cols[2] = in_val * in_vec.m_cols[2];
    result.m_cols[3] = in_val * in_vec.m_cols[3];
    return result;
}

Mat4& Mat4::operator/=(float32 in_other) {
    m_cols[0] /= in_other;
    m_cols[1] /= in_other;
    m_cols[2] /= in_other;
    m_cols[3] /= in_other;
    return *this;
}

Mat4 operator/(Arg_Mat4 in_vec, float32 in_val) {
    Mat4 result;
    result.m_cols[0] = in_vec.m_cols[0] / in_val;
    result.m_cols[1] = in_vec.m_cols[1] / in_val;
    result.m_cols[2] = in_vec.m_cols[2] / in_val;
    result.m_cols[3] = in_vec.m_cols[3] / in_val;
    return result;
}

Mat4 operator/(float32 in_val, Arg_Mat4 in_vec) {
    Mat4 result;
    result.m_cols[0] = in_val / in_vec.m_cols[0];
    result.m_cols[1] = in_val / in_vec.m_cols[1];
    result.m_cols[2] = in_val / in_vec.m_cols[2];
    result.m_cols[3] = in_val / in_vec.m_cols[3];
    return result;
}

DEEP_NAMESPACE_END