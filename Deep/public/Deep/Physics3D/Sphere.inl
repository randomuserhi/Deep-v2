#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Sphere.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Arg_Vec3 in_point, Arg_Sphere in_a) {
	return (in_point - in_a.m_center).sqrdMagnitude() < in_a.m_radius * in_a.m_radius;
}

bool IsOverlapping(Arg_Sphere in_a, Arg_Sphere in_b) {
	float32 distance = in_a.m_radius + in_b.m_radius;
	return (in_a.m_center - in_b.m_center).sqrdMagnitude() < distance * distance;
}

DEEP_NAMESPACE_END
