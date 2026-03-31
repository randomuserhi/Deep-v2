#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Colliders/Aabb.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Arg_Vec3 in_point, Arg_Aabb3D in_sphere) {
	Deep_Assert(in_sphere.m_extents.x > 0 && in_sphere.m_extents.y > 0 && in_sphere.m_extents.z > 0,
	            "Extents of box must be > 0.");
	Vec3 minA = in_sphere.m_center - in_sphere.m_extents;
	Vec3 maxA = in_sphere.m_center + in_sphere.m_extents;
	return in_point.x > minA.x && in_point.x < maxA.x && //
	       in_point.y > minA.y && in_point.y < maxA.y && //
	       in_point.z > minA.z && in_point.z < maxA.z;
}

bool IsOverlapping(Arg_Aabb3D in_sphereA, Arg_Aabb3D in_sphereB) {
	Deep_Assert(in_sphereA.m_extents.x > 0 && in_sphereA.m_extents.y > 0 && in_sphereA.m_extents.z > 0,
	            "Extents of box must be > 0.");
	Deep_Assert(in_sphereB.m_extents.x > 0 && in_sphereB.m_extents.y > 0 && in_sphereB.m_extents.z > 0,
	            "Extents of box must be > 0.");

	Vec3 minA = in_sphereA.m_center - in_sphereA.m_extents;
	Vec3 maxA = in_sphereA.m_center + in_sphereA.m_extents;
	Vec3 minB = in_sphereB.m_center - in_sphereB.m_extents;
	Vec3 maxB = in_sphereB.m_center + in_sphereB.m_extents;

	return minA.x < maxB.x && maxA.x > minB.x && //
	       minA.y < maxB.y && maxA.y > minB.y && //
	       minA.z < maxB.z && maxA.z > minB.z;
}

DEEP_NAMESPACE_END
