#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Colliders/Aabb.h"

DEEP_NAMESPACE_BEGIN

int IsOverlapping(Arg_Aabb3D in_boxA, Arg_Aabb3D in_boxB, ContactInfo* out_contactInfo) {
	Deep_Assert(in_boxA.m_extents.x > 0 && in_boxA.m_extents.y > 0 && in_boxA.m_extents.z > 0,
	            "Extents of box must be > 0.");
	Deep_Assert(in_boxB.m_extents.x > 0 && in_boxB.m_extents.y > 0 && in_boxB.m_extents.z > 0,
	            "Extents of box must be > 0.");
	Deep_Assert(out_contactInfo != nullptr, "Out param must not be nullptr.");

	ContactInfo contactInfo;

	Vec3 minA = in_boxA.m_center - in_boxA.m_extents;
	Vec3 maxA = in_boxA.m_center + in_boxA.m_extents;
	Vec3 minB = in_boxB.m_center - in_boxB.m_extents;
	Vec3 maxB = in_boxB.m_center + in_boxB.m_extents;

	float32 dx1 = maxB.x - minA.x;
	float32 dx2 = maxA.x - minB.x;
	contactInfo.m_penetrationDistance = Min(dx1, dx2);
	contactInfo.m_Normal4() = { dx1 < dx2 ? -1.0f : 1.0f, 0.0f, 0.0f };

	if (contactInfo.m_penetrationDistance <= 0) return 0;

	float32 dy1 = maxB.y - minA.y;
	float32 dy2 = maxA.y - minB.y;
	float32 py = Min(dy1, dy2);
	if (py <= 0) return 0;
	if (py < contactInfo.m_penetrationDistance) {
		contactInfo.m_Normal4() = { dy1 < dy2 ? -1.0f : 1.0f, 0.0f, 0.0f };
		contactInfo.m_penetrationDistance = py;
	}

	float32 dz1 = maxB.z - minA.z;
	float32 dz2 = maxA.z - minB.z;
	float32 pz = Min(dz1, dz2);
	if (pz <= 0) return 0;
	if (pz < contactInfo.m_penetrationDistance) {
		contactInfo.m_Normal4() = { dz1 < dz2 ? -1.0f : 1.0f, 0.0f, 0.0f };
		contactInfo.m_penetrationDistance = pz;
	}

	*out_contactInfo = contactInfo;
	return 1;
}

template<RaycastType in_queryType>
bool Raycast(Arg_Ray3D in_ray, Arg_Aabb3D in_box) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	Vec3 min = in_box.m_center - in_box.m_extents;
	Vec3 max = in_box.m_center + in_box.m_extents;

	Vec3 invDir = 1.0f / in_ray.m_direction;

	Vec3 t1 = (min - in_ray.m_origin) * invDir;
	Vec3 t2 = (max - in_ray.m_origin) * invDir;

	Float32x4 tmin = Float32x4::s_Min(t1, t2);
	Float32x4 tmax = Float32x4::s_Max(t1, t2);

	// NOTE(randomuserhi): Order of max/min comparisons matches that of Raycast(Arg_Ray3D, Arg_Aabb3D, RayHit3D*)
	//                     This is necessary to keep behaviour of nan and inf values consistent.
	//
	//                     Refer to Deep::Max and Deep::Min, which follow Intel Instruction convention where if either
	//                     values are nan/inf, the second provided value is defaulted to as the result.
	//
	// TODO(randomuserhi): Modify the algorithm to be robust across architectures - currently relies on Intel Instruction
	//                     convention which is not reliable for ARM Neon

	float32 tEnter = Max(tmin.z, Max(tmin.y, tmin.x));
	float32 tExit = Min(tmax.z, Min(tmax.y, tmax.x));

	if constexpr (in_queryType == RaycastType::e_startsOutside) {
		if (tEnter < 0.0f) return false;
	}
	return tEnter <= tExit && tExit > 0.0f;
}
template bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Aabb3D);
template bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Aabb3D);

template<RaycastType in_queryType>
bool Raycast(Arg_Ray3D in_ray, Arg_Aabb3D in_box, RayHit3D* out_hit) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hit != nullptr, "Out param must not be nullptr.");

	Vec3 min = in_box.m_center - in_box.m_extents;
	Vec3 max = in_box.m_center + in_box.m_extents;

	Vec3 invDir = 1.0f / in_ray.m_direction;

	Vec3 t1 = (min - in_ray.m_origin) * invDir;
	Vec3 t2 = (max - in_ray.m_origin) * invDir;

	Float32x4 tmin = Float32x4::s_Min(t1, t2);
	Float32x4 tmax = Float32x4::s_Max(t1, t2);

	// NOTE(randomuserhi): Order of max/min comparisons matches that of Raycast(Arg_Ray3D, Arg_Aabb3D)
	//                     This is necessary to keep behaviour of nan and inf values consistent.
	//
	//                     Refer to Deep::Max and Deep::Min, which follow Intel Instruction convention where if either
	//                     values are nan/inf, the second provided value is defaulted to as the result.
	//
	// TODO(randomuserhi): Modify the algorithm to be robust across architectures - currently relies on Intel Instruction
	//                     convention which is not reliable for ARM Neon

	int32 enterAxis = (tmin.y > tmin.x) ? 1 : 0;
	float32 tEnter = (tmin.y > tmin.x) ? tmin.y : tmin.x;
	enterAxis = (tmin.z > tEnter) ? 2 : enterAxis;
	tEnter = (tmin.z > tEnter) ? tmin.z : tEnter;

	int32 exitAxis = (tmax.y < tmax.x) ? 1 : 0;
	float32 tExit = (tmax.y < tmax.x) ? tmax.y : tmax.x;
	exitAxis = (tmax.z < tExit) ? 2 : exitAxis;
	tExit = (tmax.z < tExit) ? tmax.z : tExit;

	bool outside = tEnter >= 0;
	if constexpr (in_queryType == RaycastType::e_startsOutside) {
		if (!outside || tEnter > tExit || tExit <= 0.0f) return false;

		const int32 axis = enterAxis;
		const float32 distance = tEnter;
		const float32 multiplier = -1.0f;
		out_hit->m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hit->m_Normal4() = Vec3::k_zero;
		out_hit->m_normal.m_values[axis] = Sign(in_ray.m_direction.m_values[axis]) * multiplier;
		out_hit->m_distance = distance;
	} else {
		if (tEnter > tExit || tExit <= 0.0f) return false;

		const int32 axis = outside ? enterAxis : exitAxis;
		const float32 distance = outside ? tEnter : tExit;
		const float32 multiplier = outside ? -1.0f : 1.0f;
		out_hit->m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hit->m_Normal4() = Vec3::k_zero;
		out_hit->m_normal.m_values[axis] = Sign(in_ray.m_direction.m_values[axis]) * multiplier;
		out_hit->m_distance = distance;
	}

	return true;
}
template bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Aabb3D, RayHit3D*);
template bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Aabb3D, RayHit3D*);

template<RaycastType in_queryType>
int32 RaycastAll(Arg_Ray3D in_ray, Arg_Aabb3D in_box, RayHit3D* out_hits) {
	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hits != nullptr, "Out param must not be nullptr.");

	Vec3 min = in_box.m_center - in_box.m_extents;
	Vec3 max = in_box.m_center + in_box.m_extents;

	Vec3 invDir = 1.0f / in_ray.m_direction;

	Vec3 t1 = (min - in_ray.m_origin) * invDir;
	Vec3 t2 = (max - in_ray.m_origin) * invDir;

	Float32x4 tmin = Float32x4::s_Min(t1, t2);
	Float32x4 tmax = Float32x4::s_Max(t1, t2);

	// NOTE(randomuserhi): Order of max/min comparisons matches that of Raycast(Arg_Ray3D, Arg_Aabb3D)
	//                     This is necessary to keep behaviour of nan and inf values consistent.
	//
	//                     Refer to Deep::Max and Deep::Min, which follow Intel Instruction convention where if either
	//                     values are nan/inf, the second provided value is defaulted to as the result.
	//
	// TODO(randomuserhi): Modify the algorithm to be robust across architectures - currently relies on Intel Instruction
	//                     convention which is not reliable for ARM Neon

	int32 enterAxis = (tmin.y > tmin.x) ? 1 : 0;
	float32 tEnter = (tmin.y > tmin.x) ? tmin.y : tmin.x;
	enterAxis = (tmin.z > tEnter) ? 2 : enterAxis;
	tEnter = (tmin.z > tEnter) ? tmin.z : tEnter;

	int32 exitAxis = (tmax.y < tmax.x) ? 1 : 0;
	float32 tExit = (tmax.y < tmax.x) ? tmax.y : tmax.x;
	exitAxis = (tmax.z < tExit) ? 2 : exitAxis;
	tExit = (tmax.z < tExit) ? tmax.z : tExit;

	if (tEnter > tExit || tExit <= 0.0f) return 0;

	if constexpr (in_queryType == RaycastType::e_startsOutside) {
		if (tEnter < 0.0f) return 0;
	}

	int32 hitCount = 0;

	// Handle entry hit
	if (tEnter >= 0.0f) {
		float32 distance = tEnter;
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hits[hitCount].m_Normal4() = Vec3::k_zero;
		out_hits[hitCount].m_normal.m_values[enterAxis] = Sign(in_ray.m_direction.m_values[enterAxis]) * -1.0f;
		out_hits[hitCount].m_distance = distance;
		++hitCount;
	}

	// Handle exit hit
	{
		float32 distance = tExit;
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * distance;
		out_hits[hitCount].m_Normal4() = Vec3::k_zero;
		out_hits[hitCount].m_normal.m_values[exitAxis] = Sign(in_ray.m_direction.m_values[exitAxis]);
		out_hits[hitCount].m_distance = distance;
		++hitCount;
	}

	return hitCount;
}
template int32 RaycastAll<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Aabb3D, RayHit3D*);
template int32 RaycastAll<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Aabb3D, RayHit3D*);

DEEP_NAMESPACE_END
