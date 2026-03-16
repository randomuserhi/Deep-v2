#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"

DEEP_NAMESPACE_BEGIN

struct ContactInfo {
	Vec3 m_normal;
	float32 m_penetrationDistance;
};

DEEP_NAMESPACE_END
