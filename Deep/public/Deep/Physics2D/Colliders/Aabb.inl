#pragma once

#include "Deep.h"
#include "Deep/Math/Vec2.h"
#include "Deep/Physics2D/Colliders/Aabb.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Arg_Vec2 in_point, Arg_Aabb2D in_box) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Vec2 minA = in_box.m_center - in_box.m_extents;
	Vec2 maxA = in_box.m_center + in_box.m_extents;
	return in_point.x > minA.x && in_point.x < maxA.x && //
	       in_point.y > minA.y && in_point.y < maxA.y;
}

bool IsOverlapping(Arg_Aabb2D in_boxA, Arg_Aabb2D in_boxB) {
	Deep_Assert(in_boxA.m_extents.x > 0 && in_boxA.m_extents.y > 0, "Extents of box must be > 0.");
	Deep_Assert(in_boxB.m_extents.x > 0 && in_boxB.m_extents.y > 0, "Extents of box must be > 0.");

	Vec2 minA = in_boxA.m_center - in_boxA.m_extents;
	Vec2 maxA = in_boxA.m_center + in_boxA.m_extents;
	Vec2 minB = in_boxB.m_center - in_boxB.m_extents;
	Vec2 maxB = in_boxB.m_center + in_boxB.m_extents;

	return minA.x < maxB.x && maxA.x > minB.x && //
	       minA.y < maxB.y && maxA.y > minB.y;
}

DEEP_NAMESPACE_END
