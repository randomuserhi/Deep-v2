#include "Deep/Math/Vec3.h"

DEEP_NAMESPACE_BEGIN

const Vec3 Vec3::k_zero = Vec3{ 0.0f, 0.0f, 0.0f };
const Vec3 Vec3::k_one = Vec3{ 1.0f, 1.0f, 1.0f };

const Vec3 Vec3::k_up = Vec3{ 0.0f, 1.0f, 0.0f };
const Vec3 Vec3::k_down = Vec3{ 0.0f, -1.0f, 0.0f };
const Vec3 Vec3::k_left = Vec3{ -1.0f, 0.0f, 0.0f };
const Vec3 Vec3::k_right = Vec3{ 1.0f, 0.0f, 0.0f };
const Vec3 Vec3::k_forwards = Vec3{ 0.0f, 0.0f, 1.0f };
const Vec3 Vec3::k_backwards = Vec3{ 0.0f, 0.0f, -1.0f };

DEEP_NAMESPACE_END