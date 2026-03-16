#pragma once

#include "Deep.h"
#include "Deep/Math.h"

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

enum class RaycastType { e_startsInside, e_startsOutside, e_default = e_startsOutside };

DEEP_NAMESPACE_END
