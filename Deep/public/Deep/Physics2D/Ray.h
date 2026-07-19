#pragma once

#include "Deep.h"
#include "Deep/Math.h"

#if !defined(DEEP_RAYCAST_DEFAULT_TYPE)
	#define DEEP_RAYCAST_DEFAULT_TYPE e_startsOutside
#endif

DEEP_NAMESPACE_BEGIN

struct Ray2D {
	Vec2 m_origin;
	Vec2 m_direction;
};

using Arg_Ray2D = const Ray2D&;

struct RayHit2D {
	Vec2 m_point;
	Vec2 m_normal;
	float32 m_distance;
};

using Arg_RayHit2D = const RayHit2D&;

enum class RaycastType {
	e_startsInside,  // Rays will collide with colliders they start inside of
	e_startsOutside, // Rays will not collide with colliders they start inside of
	e_default = DEEP_RAYCAST_DEFAULT_TYPE
};

DEEP_NAMESPACE_END

#include "./Ray.inl" // IWYU pragma: export
