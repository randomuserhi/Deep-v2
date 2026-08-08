#pragma once

#include "Deep.h"

DEEP_NAMESPACE_BEGIN

constexpr const float32 k_pi = 3.141592653589793f;

constexpr const float32 k_deg2Rad = k_pi / 180.0f;
constexpr const float32 k_rad2Deg = 180.0f / k_pi;

// Absolute and relative tolerances that floating point math uses for comparisons
constexpr const float32 k_absTolerance = 1e-06f;
constexpr const float32 k_relTolerance = 1e-05f;

DEEP_NAMESPACE_END
