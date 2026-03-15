#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Aabb.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Vec3 in_point, Arg_Aabb3D in_a) {
	Deep_Assert(in_a.m_extents.x > 0 && in_a.m_extents.y > 0 && in_a.m_extents.z > 0, "Extents of box must be > 0.");
	Vec3 minA = in_a.m_center - in_a.m_extents;
	Vec3 maxA = in_a.m_center + in_a.m_extents;
	return in_point.x > minA.x && in_point.x < maxA.x && //
	       in_point.y > minA.y && in_point.y < maxA.y && //
	       in_point.z > minA.z && in_point.z < maxA.z;
}

bool IsOverlapping(Arg_Aabb3D in_a, Arg_Aabb3D in_b) {
	Deep_Assert(in_a.m_extents.x > 0 && in_a.m_extents.y > 0 && in_a.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_b.m_extents.x > 0 && in_b.m_extents.y > 0 && in_b.m_extents.z > 0, "Extents of box must be > 0.");

	Vec3 minA = in_a.m_center - in_a.m_extents;
	Vec3 maxA = in_a.m_center + in_a.m_extents;
	Vec3 minB = in_b.m_center - in_b.m_extents;
	Vec3 maxB = in_b.m_center + in_b.m_extents;

	return minA.x < maxB.x && maxA.x > minB.x && //
	       minA.y < maxB.y && maxA.y > minB.y && //
	       minA.z < maxB.z && maxA.z > minB.z;
}

int IsOverlapping(Arg_Aabb3D in_a, Arg_Aabb3D in_b, ContactInfo* out_contactInfo) {
	Deep_Assert(in_a.m_extents.x > 0 && in_a.m_extents.y > 0 && in_a.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_b.m_extents.x > 0 && in_b.m_extents.y > 0 && in_b.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(out_contactInfo != nullptr, "Out param must not be nullptr.");

	ContactInfo contactInfo;

	Vec3 minA = in_a.m_center - in_a.m_extents;
	Vec3 maxA = in_a.m_center + in_a.m_extents;
	Vec3 minB = in_b.m_center - in_b.m_extents;
	Vec3 maxB = in_b.m_center + in_b.m_extents;

	float32 dx1 = maxB.x - minA.x;
	float32 dx2 = maxA.x - minB.x;
	contactInfo.m_penetrationDistance = Deep::Min(dx1, dx2);
	contactInfo.m_normal = { dx1 < dx2 ? -1.0f : 1.0f, 0.0f, 0.0f };

	if (contactInfo.m_penetrationDistance <= 0) return 0;

	float32 dy1 = maxB.y - minA.y;
	float32 dy2 = maxA.y - minB.y;
	float32 py = Deep::Min(dy1, dy2);
	if (py <= 0) return 0;
	if (py < contactInfo.m_penetrationDistance) {
		contactInfo.m_penetrationDistance = py;
		contactInfo.m_normal = { dy1 < dy2 ? -1.0f : 1.0f, 0.0f, 0.0f };
	}

	float32 dz1 = maxB.z - minA.z;
	float32 dz2 = maxA.z - minB.z;
	float32 pz = Deep::Min(dz1, dz2);
	if (pz <= 0) return 0;
	if (pz < contactInfo.m_penetrationDistance) {
		contactInfo.m_penetrationDistance = pz;
		contactInfo.m_normal = { dz1 < dz2 ? -1.0f : 1.0f, 0.0f, 0.0f };
	}

	*out_contactInfo = contactInfo;
	return 1;
}

bool Raycast(Ray3D in_ray, Arg_Aabb3D in_box) {
	// TODO(randomuserhi): API for if ray should hit starting collider (if ray originates from inside of it)
	//                     Currently hits colliders its starts inside of

	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	Vec3 min = in_box.m_center - in_box.m_extents;
	Vec3 max = in_box.m_center + in_box.m_extents;

	Vec3 invDir = 1.0f / in_ray.m_direction;

	Vec3 t1 = (min - in_ray.m_origin) * invDir;
	Vec3 t2 = (max - in_ray.m_origin) * invDir;

	Xmm tmin = Xmm::Min(t1.xmm, t2.xmm);
	Xmm tmax = Xmm::Max(t1.xmm, t2.xmm);

	float32 tEnter = Deep::Max(Deep::Max(tmin.x, tmin.y), tmin.z);
	float32 tExit = Deep::Min(Deep::Min(tmax.x, tmax.y), tmax.z);

	return tEnter <= tExit && tExit > 0.0f;
}

bool Raycast(Ray3D in_ray, Arg_Aabb3D in_box, RayHit3D* out_hit) {
	// TODO(randomuserhi): API for if ray should hit starting collider (if ray originates from inside of it)
	//                     Currently hits colliders its starts inside of

	Deep_Assert(in_box.m_extents.x > 0 && in_box.m_extents.y > 0 && in_box.m_extents.z > 0, "Extents of box must be > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hit != nullptr, "Out param must not be nullptr.");

	Vec3 min = in_box.m_center - in_box.m_extents;
	Vec3 max = in_box.m_center + in_box.m_extents;

	Vec3 invDir = 1.0f / in_ray.m_direction;

	Vec3 t1 = (min - in_ray.m_origin) * invDir;
	Vec3 t2 = (max - in_ray.m_origin) * invDir;

	Xmm tmin = Xmm::Min(t1.xmm, t2.xmm);
	Xmm tmax = Xmm::Max(t1.xmm, t2.xmm);

	int32 axis = (tmin.y > tmin.x) ? 1 : 0;
	float32 tEnter = (tmin.y > tmin.x) ? tmin.y : tmin.x;
	axis = (tmin.z > tEnter) ? 2 : axis;
	tEnter = (tmin.z > tEnter) ? tmin.z : tEnter;

	float32 tExit = Deep::Min(Deep::Min(tmax.x, tmax.y), tmax.z);

	if (tEnter > tExit || tExit < 0.0f) return false;

	out_hit->m_distance = tEnter;
	out_hit->m_point = in_ray.m_origin + in_ray.m_direction * out_hit->m_distance;

	out_hit->m_normal = Vec3::k_zero;
	out_hit->m_normal.m_values[axis] = Deep::Sign(in_ray.m_direction.m_values[axis]) * -1.0f;

	return true;
}

int RaycastAll(Ray3D in_ray, Arg_Aabb3D in_box, RayHit3D* out_hits) {
	// TODO(randomuserhi): Implementation returning all hits (entry + exit)
	//                     API for if ray should hit starting collider (if ray originates from inside of it)
	//                     - In that case, should just return exit hit
	return 0;
}

DEEP_NAMESPACE_END