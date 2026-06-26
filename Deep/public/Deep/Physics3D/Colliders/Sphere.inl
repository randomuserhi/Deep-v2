#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Colliders/Sphere.h"

DEEP_NAMESPACE_BEGIN

Vec3& Sphere::m_Center4() {
	return *reinterpret_cast<Vec3*>(&m_center);
}

const Vec3& Sphere::m_Center4() const {
	return *reinterpret_cast<const Vec3*>(&m_center);
}

bool IsOverlapping(Arg_Vec3 in_point, Arg_Sphere in_sphere) {
	return (in_point - in_sphere.m_Center4()).sqrdMagnitude() < in_sphere.m_radius * in_sphere.m_radius;
}

bool IsOverlapping(Arg_Sphere in_sphereA, Arg_Sphere in_sphereB) {
	float32 distance = in_sphereA.m_radius + in_sphereB.m_radius;
	return (in_sphereA.m_Center4() - in_sphereB.m_Center4()).sqrdMagnitude() < distance * distance;
}

DEEP_NAMESPACE_END
