#pragma once

DEEP_NAMESPACE_BEGIN

SSE_m128::SSE_m128(float32 in_x, float32 in_y, float32 in_z, float32 in_w) {
#ifdef DEEP_USE_SSE4_1
    _internal = _mm_set_ps(in_w, in_z, in_y, in_x);
#else
    x = in_x;
    y = in_y;
    z = in_z;
    w = in_w;
#endif
}
SSE_m128::SSE_m128(Type in_internal) :
    _internal(in_internal) {}

SSE_m128::operator Type() const {
    return _internal;
}

SSE_m128i SSE_m128::ToInt() const {
#ifdef DEEP_USE_SSE
    return _mm_cvttps_epi32(_internal);
#else
    return SSE_m128i{ static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z), static_cast<int32>(w) };
#endif
}
SSE_m128i SSE_m128::ReinterpretAsInt() const {
#ifdef DEEP_USE_SSE
    return _mm_castps_si128(_internal);
#else
    return Deep::BitCast<SSE_m128i>(*this);
#endif
}

SSE_m128 SSE_m128::Replicate(float32 in_value) {
#ifdef DEEP_USE_SSE
    return _mm_set1_ps(in_value);
#else
    return SSE_m128{ in_value, in_value, in_value, in_value };
#endif
}
SSE_m128 SSE_m128::And(SSE_m128Arg in_a, SSE_m128Arg in_b) {
#ifdef DEEP_USE_SSE
    return _mm_and_ps(in_a, in_b);
#else
    return SSE_m128i::And(in_a.ReinterpretAsInt(), in_b.ReinterpretAsInt()).ReinterpretAsFloat();
#endif
}
SSE_m128 SSE_m128::Xor(SSE_m128Arg in_a, SSE_m128Arg in_b) {
#ifdef DEEP_USE_SSE
    return _mm_xor_ps(in_a, in_b);
#else
    return SSE_m128i::Xor(in_a.ReinterpretAsInt(), in_b.ReinterpretAsInt()).ReinterpretAsFloat();
#endif
}
SSE_m128i SSE_m128::Equals(SSE_m128Arg in_a, SSE_m128Arg in_b) {
#ifdef DEEP_USE_SSE
    return _mm_castps_si128(_mm_cmpeq_ps(in_a, in_b));
#else
    return SSE_m128i{ in_a.x == in_b.x ? (int32)0xffffffff : 0, in_a.y == in_b.y ? (int32)0xffffffff : 0,
                      in_a.z == in_b.z ? (int32)0xffffffff : 0, in_a.w == in_b.w ? (int32)0xffffffff : 0 };
#endif
}

Deep_Inline bool operator!=(SSE_m128Arg in_a, SSE_m128Arg in_b) {
#ifdef DEEP_USE_SSE4_1
    return SSE_m128::Equals(in_a, in_b).ToBooleanBitMask() != 0b1111;
#else
    return in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z || in_a.w != in_b.w;
#endif
}
Deep_Inline bool operator==(SSE_m128Arg in_a, SSE_m128Arg in_b) {
    return !(in_a != in_b);
}

SSE_m128& SSE_m128::operator+=(SSE_m128Arg in_other) {
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

SSE_m128 operator+(SSE_m128 in_a, SSE_m128Arg in_b) {
    return in_a += in_b;
}

SSE_m128& SSE_m128::operator-=(SSE_m128Arg in_other) {
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

SSE_m128 operator-(SSE_m128 in_a, SSE_m128Arg in_b) {
    return in_a -= in_b;
}

SSE_m128 operator-(SSE_m128Arg in_other) {
#ifdef DEEP_USE_SSE4_1
    return _mm_sub_ps(_mm_setzero_ps(), in_other);
#else
    // NOTE(randomuserhi): 0.0f - x to stay consistent with vectorised version
    return SSE_m128{ 0.0f - in_other.x, 0.0f - in_other.y, 0.0f - in_other.z, 0.0f - in_other.w };
#endif
}

SSE_m128& SSE_m128::operator*=(SSE_m128Arg in_other) {
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
SSE_m128 operator*(SSE_m128 in_a, SSE_m128Arg in_b) {
    return in_a *= in_b;
}

SSE_m128& SSE_m128::operator*=(float32 in_other) {
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

SSE_m128 operator*(SSE_m128 in_vec, float32 in_val) {
    return in_vec *= in_val;
}

SSE_m128 operator*(float32 in_val, SSE_m128Arg in_vec) {
#ifdef DEEP_USE_SSE4_1
    return _mm_mul_ps(_mm_set1_ps(in_val), in_vec);
#else
    return SSE_m128{ in_val * in_vec.x, in_val * in_vec.y, in_val * in_vec.z, in_val * in_vec.w };
#endif
}

SSE_m128& SSE_m128::operator/=(SSE_m128Arg in_other) {
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
SSE_m128 operator/(SSE_m128 in_a, SSE_m128Arg in_b) {
    return in_a /= in_b;
}

SSE_m128& SSE_m128::operator/=(float32 in_other) {
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

SSE_m128 operator/(SSE_m128 in_vec, float32 in_val) {
    return in_vec /= in_val;
}

SSE_m128 operator/(float32 in_val, SSE_m128Arg in_vec) {
#ifdef DEEP_USE_SSE4_1
    return _mm_div_ps(_mm_set1_ps(in_val), in_vec);
#else
    return SSE_m128{ in_val / in_vec.x, in_val / in_vec.y, in_val / in_vec.z, in_val / in_vec.w };
#endif
}

SSE_m128 SSE_m128::Select(SSE_m128Arg in_a, SSE_m128Arg in_b, SSE_m128iArg in_control) {
#ifdef DEEP_USE_SSE4_1
    return _mm_blendv_ps(in_a, in_b, _mm_castsi128_ps(in_control));
#else
    return SSE_m128{ in_control.x ? in_b.x : in_a.x, in_control.y ? in_b.y : in_a.y, in_control.z ? in_b.z : in_a.z,
                     in_control.w ? in_b.w : in_a.w };
#endif
}

void SSE_m128::SinCos(SSE_m128& wo_sin, SSE_m128& wo_cos) {
    // Implementation based on sinf.c from the cephes library, combines sinf and cosf in a single function, changes
    // octants to quadrants and vectorizes it Original implementation by Stephen L. Moshier (See:
    // http://www.moshier.net/)

    // Make argument positive and remember sign for sin only since cos is symmetric around x (highest bit of a float is
    // the sign bit)
    SSE_m128i sinSign = SSE_m128i::And(ReinterpretAsInt(), SSE_m128i::Replicate(0x80000000U));
    SSE_m128 _x = SSE_m128::Xor(*this, sinSign.ReinterpretAsFloat());

    // x / (PI / 2) rounded to nearest int gives us the quadrant closest to x
    SSE_m128i quadrant = (0.6366197723675814f * _x + SSE_m128::Replicate(0.5f)).ToInt();

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
    SSE_m128 floatQuadrant = quadrant.ToFloat();
    _x = ((_x - floatQuadrant * 1.5703125f) - floatQuadrant * 0.0004837512969970703125f)
         - floatQuadrant * 7.549789948768648e-8f;

    // Calculate x2 = x^2
    SSE_m128 x2 = _x * _x;

    // Taylor expansion:
    // Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + x^8/8! + ... = (((x2/8!- 1/6!) * x2 + 1/4!) * x2 - 1/2!) * x2 + 1
    SSE_m128 taylorCos = ((2.443315711809948e-5f * x2 - SSE_m128::Replicate(1.388731625493765e-3f)) * x2
                          + SSE_m128::Replicate(4.166664568298827e-2f))
                             * x2 * x2
                         - 0.5f * x2 + SSE_m128::Replicate(1.0f);
    // Sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ... = ((-x2/7! + 1/5!) * x2 - 1/3!) * x2 * x + x
    SSE_m128 taylorSin =
        ((-1.9515295891e-4f * x2 + SSE_m128::Replicate(8.3321608736e-3f)) * x2 - SSE_m128::Replicate(1.6666654611e-1f)) * x2
            * _x
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
    SSE_m128i bit1 = quadrant.LogicalShiftLeft<31>();
    SSE_m128i bit2 = SSE_m128i::And(quadrant.LogicalShiftLeft<30>(), SSE_m128i::Replicate(0x80000000U));

    // Select which one of the results is sin and which one is cos
    SSE_m128 s = SSE_m128::Select(taylorSin, taylorCos, bit1);
    SSE_m128 c = SSE_m128::Select(taylorCos, taylorSin, bit1);

    // Update the signs
    sinSign = SSE_m128i::Xor(sinSign, bit2);
    SSE_m128i cos_sign = SSE_m128i::Xor(bit1, bit2);

    // Correct the signs
    wo_sin = SSE_m128::Xor(s, sinSign.ReinterpretAsFloat());
    wo_cos = SSE_m128::Xor(c, cos_sign.ReinterpretAsFloat());
}

DEEP_NAMESPACE_END
