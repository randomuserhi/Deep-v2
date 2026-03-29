#include "Deep/Math/Vec3i.h"

DEEP_NAMESPACE_BEGIN

const Vec3i Vec3i::k_zero = Vec3i{ 0, 0, 0 };

const Vec3i Vec3i::k_up = Vec3i{ 0, 1, 0 };
const Vec3i Vec3i::k_down = Vec3i{ 0, -1, 0 };
const Vec3i Vec3i::k_left = Vec3i{ -1, 0, 0 };
const Vec3i Vec3i::k_right = Vec3i{ 1, 0, 0 };
const Vec3i Vec3i::k_forwards = Vec3i{ 0, 0, 1 };
const Vec3i Vec3i::k_backwards = Vec3i{ 0, 0, -1 };

DEEP_NAMESPACE_END
