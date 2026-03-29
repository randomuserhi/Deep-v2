#pragma once

#include "Deep.h"
#include "Deep/Math.h"

#if !defined(DEEP_RAYCAST_DEFAULT_TYPE)
#define DEEP_RAYCAST_DEFAULT_TYPE e_startsOutside
#endif

DEEP_NAMESPACE_BEGIN

struct Ray3D {
	Vec3 m_origin;
	Vec3 m_direction;
};

struct RayHit3D {
	Vec3 m_point;
	Vec3 m_normal;
	float32 m_distance;
};

enum class RaycastType {
	e_startsInside,  // Rays will collide with colliders they start inside of
	e_startsOutside, // Rays will not collide with colliders they start inside of
	e_default = DEEP_RAYCAST_DEFAULT_TYPE
};

DEEP_NAMESPACE_END
