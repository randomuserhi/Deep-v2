#pragma once

#include "./Sphere.h"

DEEP_NAMESPACE_BEGIN

Vec3& Sphere::m_Center4() {
	return *reinterpret_cast<Vec3*>(&m_center);
}

const Vec3& Sphere::m_Center4() const {
	return *reinterpret_cast<const Vec3*>(&m_center);
}

Vec3 Sphere::m_Center() const {
	return Vec3::s_FixW(m_Center4());
}

void Sphere::m_Center(Arg_Vec3 in_center) {
	m_Center4().XYZ(in_center);
}

bool IsOverlapping(Arg_Vec3 in_point, Arg_Sphere in_sphere) {
	return (in_point - in_sphere.m_Center4()).m_SqrdMagnitude() < in_sphere.m_radius * in_sphere.m_radius;
}

bool IsOverlapping(Arg_Sphere in_sphereA, Arg_Sphere in_sphereB) {
	float32 distance = in_sphereA.m_radius + in_sphereB.m_radius;
	return (in_sphereA.m_Center4() - in_sphereB.m_Center4()).m_SqrdMagnitude() < distance * distance;
}

DEEP_NAMESPACE_END
