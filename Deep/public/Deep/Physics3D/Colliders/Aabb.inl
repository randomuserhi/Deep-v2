#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Colliders/Aabb.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Arg_Vec3 in_point, Arg_Aabb3D in_box) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Vec3 minA = in_box.m_center - in_box.m_extents;
	Vec3 maxA = in_box.m_center + in_box.m_extents;
	return in_point.x > minA.x && in_point.x < maxA.x && //
	       in_point.y > minA.y && in_point.y < maxA.y && //
	       in_point.z > minA.z && in_point.z < maxA.z;
}

bool IsOverlapping(Arg_Aabb3D in_boxA, Arg_Aabb3D in_boxB) {
	Deep_Assert(in_boxA.m_extents.x > 0 && in_boxA.m_extents.y > 0 && in_boxA.m_extents.z > 0,
	            "Extents of box must be > 0.");
	Deep_Assert(in_boxB.m_extents.x > 0 && in_boxB.m_extents.y > 0 && in_boxB.m_extents.z > 0,
	            "Extents of box must be > 0.");

	Vec3 minA = in_boxA.m_center - in_boxA.m_extents;
	Vec3 maxA = in_boxA.m_center + in_boxA.m_extents;
	Vec3 minB = in_boxB.m_center - in_boxB.m_extents;
	Vec3 maxB = in_boxB.m_center + in_boxB.m_extents;

	return minA.x < maxB.x && maxA.x > minB.x && //
	       minA.y < maxB.y && maxA.y > minB.y && //
	       minA.z < maxB.z && maxA.z > minB.z;
}

DEEP_NAMESPACE_END
