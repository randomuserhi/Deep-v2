#include <Deep/Math.h>

DEEP_NAMESPACE_BEGIN

// TODO(randomuserhi): SIMD implementations + consider inlining?
Mat4& Mat4::Compose(Vec3Arg in_position, QuatArg in_rotation, Vec3Arg in_scale) {
    float32 x = in_rotation.x;
    float32 y = in_rotation.y;
    float32 z = in_rotation.z;
    float32 w = in_rotation.w;

    float32 x2 = x + x;
    float32 y2 = y + y;
    float32 z2 = z + z;

    float32 xx = x * x2;
    float32 xy = x * y2;
    float32 xz = x * z2;

    float32 yy = y * y2;
    float32 yz = y * z2;

    float32 zz = z * z2;

    float32 wx = w * x2;
    float32 wy = w * y2;
    float32 wz = w * z2;

    float32 sx = in_scale.x;
    float32 sy = in_scale.y;
    float32 sz = in_scale.z;

    m_values[0] = (1 - (yy + zz)) * sx;
    m_values[1] = (xy + wz) * sx;
    m_values[2] = (xz - wy) * sx;
    m_values[3] = 0;

    m_values[4] = (xy - wz) * sy;
    m_values[5] = (1 - (xx + zz)) * sy;
    m_values[6] = (yz + wx) * sy;
    m_values[7] = 0;

    m_values[8] = (xz + wy) * sz;
    m_values[9] = (yz - wx) * sz;
    m_values[10] = (1 - (xx + yy)) * sz;
    m_values[11] = 0;

    m_values[12] = in_position.x;
    m_values[13] = in_position.y;
    m_values[14] = in_position.z;
    m_values[15] = 1;

    return *this;
}

const Mat4 Mat4::k_identity = Mat4{
    1.0f, 0.0f, 0.0f, 0.0f, //
    0.0f, 1.0f, 0.0f, 0.0f, //
    0.0f, 0.0f, 1.0f, 0.0f, //
    0.0f, 0.0f, 0.0f, 1.0f  //
};

DEEP_NAMESPACE_END