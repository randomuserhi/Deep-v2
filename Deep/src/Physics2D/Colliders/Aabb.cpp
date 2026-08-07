#include "Deep.h"
#include "Deep/Math/Vec2.h"
#include "Deep/Physics2D/Ray.h"
#include "Deep/Physics2D/Colliders/Aabb.h"

DEEP_NAMESPACE_BEGIN

int IsOverlapping(Arg_Aabb2D in_boxA, Arg_Aabb2D in_boxB, ContactInfo2D* out_contactInfo) {
	Deep_Assert(in_boxA.m_extents.x > 0 && in_boxA.m_extents.y > 0, "Extents of box must be > 0.");
	Deep_Assert(in_boxB.m_extents.x > 0 && in_boxB.m_extents.y > 0, "Extents of box must be > 0.");
	Deep_Assert(out_contactInfo != nullptr, "Out param must not be nullptr.");

	ContactInfo2D contactInfo;

	Vec2 minA = in_boxA.m_center - in_boxA.m_extents;
	Vec2 maxA = in_boxA.m_center + in_boxA.m_extents;
	Vec2 minB = in_boxB.m_center - in_boxB.m_extents;
	Vec2 maxB = in_boxB.m_center + in_boxB.m_extents;

	float32 dx1 = maxB.x - minA.x;
	float32 dx2 = maxA.x - minB.x;
	contactInfo.m_penetrationDistance = Min(dx1, dx2);
	contactInfo.m_normal = { dx1 < dx2 ? -1.0f : 1.0f, 0.0f };

	if (contactInfo.m_penetrationDistance <= 0) return 0;

	float32 dy1 = maxB.y - minA.y;
	float32 dy2 = maxA.y - minB.y;
	float32 py = Min(dy1, dy2);
	if (py <= 0) return 0;
	if (py < contactInfo.m_penetrationDistance) {
		contactInfo.m_normal = { 0.0f, dy1 < dy2 ? -1.0f : 1.0f };
		contactInfo.m_penetrationDistance = py;
	}

	*out_contactInfo = contactInfo;
	return 1;
}

template<RaycastType2D in_queryType>
bool Raycast(Arg_Ray2D in_ray, Arg_Aabb2D in_box) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	Vec2 min = in_box.m_center - in_box.m_extents;
	Vec2 max = in_box.m_center + in_box.m_extents;

	Vec2 invDir = 1.0f / in_ray.m_direction;

	Vec2 t1 = (min - in_ray.m_origin) * invDir;
	Vec2 t2 = (max - in_ray.m_origin) * invDir;

	Float32x2 tmin = Float32x2::s_Min(t1, t2);
	Float32x2 tmax = Float32x2::s_Max(t1, t2);

	// NOTE(randomuserhi): Order of max/min comparisons matches that of Raycast(Arg_Ray2D, Arg_Aabb2D, RayHit2D*)
	//                     This is necessary to keep behaviour of nan and inf values consistent.
	//
	//                     Refer to Deep::Max and Deep::Min, which follow Intel Instruction convention where if either
	//                     values are nan/inf, the second provided value is defaulted to as the result.
	//
	// TODO(randomuserhi): Modify the algorithm to be robust across architectures - currently relies on Intel Instruction
	//                     convention which is not reliable for ARM Neon

	float32 tEnter = Max(tmin.y, tmin.x);
	float32 tExit = Min(tmax.y, tmax.x);

	if constexpr (in_queryType == RaycastType2D::e_startsOutside) {
		if (tEnter < 0.0f) return false;
	}
	return tEnter <= tExit && tExit > 0.0f;
}
template bool Raycast<RaycastType2D::e_startsInside>(Arg_Ray2D, Arg_Aabb2D);
template bool Raycast<RaycastType2D::e_startsOutside>(Arg_Ray2D, Arg_Aabb2D);

template<RaycastType2D in_queryType>
bool Raycast(Arg_Ray2D in_ray, Arg_Aabb2D in_box, RayHit2D* out_hit) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hit != nullptr, "Out param must not be nullptr.");

	Vec2 min = in_box.m_center - in_box.m_extents;
	Vec2 max = in_box.m_center + in_box.m_extents;

	Vec2 invDir = 1.0f / in_ray.m_direction;

	Vec2 t1 = (min - in_ray.m_origin) * invDir;
	Vec2 t2 = (max - in_ray.m_origin) * invDir;

	Float32x2 tmin = Float32x2::s_Min(t1, t2);
	Float32x2 tmax = Float32x2::s_Max(t1, t2);

	// NOTE(randomuserhi): Order of max/min comparisons matches that of Raycast(Arg_Ray2D, Arg_Aabb2D)
	//                     This is necessary to keep behaviour of nan and inf values consistent.
	//
	//                     Refer to Deep::Max and Deep::Min, which follow Intel Instruction convention where if either
	//                     values are nan/inf, the second provided value is defaulted to as the result.
	//
	// TODO(randomuserhi): Modify the algorithm to be robust across architectures - currently relies on Intel Instruction
	//                     convention which is not reliable for ARM Neon

	int32 enterAxis = (tmin.y > tmin.x) ? 1 : 0;
	float32 tEnter = (tmin.y > tmin.x) ? tmin.y : tmin.x;

	int32 exitAxis = (tmax.y < tmax.x) ? 1 : 0;
	float32 tExit = (tmax.y < tmax.x) ? tmax.y : tmax.x;

	bool outside = tEnter >= 0;
	if constexpr (in_queryType == RaycastType2D::e_startsOutside) {
		if (!outside || tEnter > tExit || tExit <= 0.0f) return false;

		const int32 axis = enterAxis;
		const float32 distance = tEnter;
		const float32 multiplier = -1.0f;
		out_hit->m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hit->m_normal = Vec2::k_zero;
		out_hit->m_normal.m_values[axis] = Sign(in_ray.m_direction.m_values[axis]) * multiplier;
		out_hit->m_distance = distance;
	} else {
		if (tEnter > tExit || tExit <= 0.0f) return false;

		const int32 axis = outside ? enterAxis : exitAxis;
		const float32 distance = outside ? tEnter : tExit;
		const float32 multiplier = outside ? -1.0f : 1.0f;
		out_hit->m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hit->m_normal = Vec2::k_zero;
		out_hit->m_normal.m_values[axis] = Sign(in_ray.m_direction.m_values[axis]) * multiplier;
		out_hit->m_distance = distance;
	}

	return true;
}
template bool Raycast<RaycastType2D::e_startsInside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);
template bool Raycast<RaycastType2D::e_startsOutside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);

template<RaycastType2D in_queryType>
int32 RaycastAll(Arg_Ray2D in_ray, Arg_Aabb2D in_box, RayHit2D* out_hits) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hits != nullptr, "Out param must not be nullptr.");

	Vec2 min = in_box.m_center - in_box.m_extents;
	Vec2 max = in_box.m_center + in_box.m_extents;

	Vec2 invDir = 1.0f / in_ray.m_direction;

	Vec2 t1 = (min - in_ray.m_origin) * invDir;
	Vec2 t2 = (max - in_ray.m_origin) * invDir;

	Float32x2 tmin = Float32x2::s_Min(t1, t2);
	Float32x2 tmax = Float32x2::s_Max(t1, t2);

	// NOTE(randomuserhi): Order of max/min comparisons matches that of Raycast(Arg_Ray2D, Arg_Aabb2D)
	//                     This is necessary to keep behaviour of nan and inf values consistent.
	//
	//                     Refer to Deep::Max and Deep::Min, which follow Intel Instruction convention where if either
	//                     values are nan/inf, the second provided value is defaulted to as the result.
	//
	// TODO(randomuserhi): Modify the algorithm to be robust across architectures - currently relies on Intel Instruction
	//                     convention which is not reliable for ARM Neon

	int32 enterAxis = (tmin.y > tmin.x) ? 1 : 0;
	float32 tEnter = (tmin.y > tmin.x) ? tmin.y : tmin.x;

	int32 exitAxis = (tmax.y < tmax.x) ? 1 : 0;
	float32 tExit = (tmax.y < tmax.x) ? tmax.y : tmax.x;

	if (tEnter > tExit || tExit <= 0.0f) return 0;

	if constexpr (in_queryType == RaycastType2D::e_startsOutside) {
		if (tEnter < 0.0f) return 0;
	}

	int32 hitCount = 0;

	// Handle entry hit
	if (tEnter >= 0.0f) {
		float32 distance = tEnter;
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hits[hitCount].m_normal = Vec2::k_zero;
		out_hits[hitCount].m_normal.m_values[enterAxis] = Sign(in_ray.m_direction.m_values[enterAxis]) * -1.0f;
		out_hits[hitCount].m_distance = distance;
		++hitCount;
	}

	// Handle exit hit
	{
		float32 distance = tExit;
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hits[hitCount].m_normal = Vec2::k_zero;
		out_hits[hitCount].m_normal.m_values[exitAxis] = Sign(in_ray.m_direction.m_values[exitAxis]);
		out_hits[hitCount].m_distance = distance;
		++hitCount;
	}

	return hitCount;
}
template int32 RaycastAll<RaycastType2D::e_startsInside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);
template int32 RaycastAll<RaycastType2D::e_startsOutside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);

DEEP_NAMESPACE_END
