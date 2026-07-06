#include "Deep/Math/Vec2.h"

DEEP_NAMESPACE_BEGIN

const Vec2 Vec2::k_zero = Vec2{ 0.0f, 0.0f };
const Vec2 Vec2::k_one = Vec2{ 1.0f, 1.0f };

const Vec2 Vec2::k_up = Vec2{ 0.0f, 1.0f };
const Vec2 Vec2::k_down = Vec2{ 0.0f, -1.0f };
const Vec2 Vec2::k_left = Vec2{ -1.0f, 0.0f };
const Vec2 Vec2::k_right = Vec2{ 1.0f, 0.0f };

const Vec2 Vec2::k_axis[Vec2::k_numAxis] = { Vec2::k_right, Vec2::k_up };

DEEP_NAMESPACE_END
