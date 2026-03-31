#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Colliders/Sphere.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Arg_Vec3 in_point, Arg_Sphere in_sphere) {
	return (in_point - in_sphere.m_center).sqrdMagnitude() < in_sphere.m_radius * in_sphere.m_radius;
}

bool IsOverlapping(Arg_Sphere in_sphereA, Arg_Sphere in_sphereB) {
	float32 distance = in_sphereA.m_radius + in_sphereB.m_radius;
	return (in_sphereA.m_center - in_sphereB.m_center).sqrdMagnitude() < distance * distance;
}

DEEP_NAMESPACE_END
