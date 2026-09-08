#include "Deep.h"
#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Colliders/Sphere.h"

DEEP_NAMESPACE_BEGIN

int IsOverlapping(Arg_Sphere in_sphereA, Arg_Sphere in_sphereB, ContactInfo3D* out_contactInfo) {
	Deep_Assert(in_sphereA.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_sphereB.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(out_contactInfo != nullptr, "Out param must not be nullptr.");

	Vec3 delta = in_sphereB.m_Center4() - in_sphereA.m_Center4();
	float32 distanceSqrd = delta.m_SqrdMagnitude();

	float32 radiusSum = in_sphereA.m_radius + in_sphereB.m_radius;
	float32 radiusSumSqrd = radiusSum * radiusSum;

	if (distanceSqrd >= radiusSumSqrd) {
		return 0;
	}

	ContactInfo3D contactInfo;

	float32 distance = Sqrt(distanceSqrd);

	if (distance > k_absTolerance) {
		contactInfo.m_Normal4() = delta / distance;
	} else {
		// Arbitrary normal if centers coincide
		contactInfo.m_Normal4() = Vec3::k_up;
	}

	contactInfo.m_penetrationDistance = radiusSum - distance;

	*out_contactInfo = contactInfo;
	return 1;
}

template<RaycastType3D in_queryType>
bool Raycast(Arg_Ray3D in_ray, Arg_Sphere in_sphere) {
	Deep_Assert(in_sphere.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	Vec3 delta = in_ray.m_origin - in_sphere.m_Center4();
	float32 dot = Vec3::s_Dot(delta, in_ray.m_direction);
	float32 distance = delta.m_SqrdMagnitude() - in_sphere.m_radius * in_sphere.m_radius;

	if constexpr (in_queryType == RaycastType3D::e_startsOutside) {
		if (distance < 0.0f) return false;
	}

	float32 discriminant = dot * dot - distance;
	if (discriminant < 0.0f) return false;

	discriminant = Sqrt(discriminant);
	float32 tExit = -dot + discriminant;

	return tExit > 0.0f;
}
template bool Raycast<RaycastType3D::e_startsInside>(Arg_Ray3D, Arg_Sphere);
template bool Raycast<RaycastType3D::e_startsOutside>(Arg_Ray3D, Arg_Sphere);

template<RaycastType3D in_queryType>
bool Raycast(Arg_Ray3D in_ray, Arg_Sphere in_sphere, RayHit3D* out_hit) {
	Deep_Assert(in_sphere.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hit != nullptr, "Out param must not be nullptr.");

	Vec3 delta = in_ray.m_origin - in_sphere.m_Center4();
	float32 dot = Vec3::s_Dot(delta, in_ray.m_direction);
	float32 distance = delta.m_SqrdMagnitude() - in_sphere.m_radius * in_sphere.m_radius;

	if constexpr (in_queryType == RaycastType3D::e_startsOutside) {
		if (distance < 0.0f) return false;
	}

	float32 discriminant = dot * dot - distance;
	if (discriminant < 0.0f) return false;

	discriminant = Sqrt(discriminant);
	float32 tEnter = -dot - discriminant;
	float32 tExit = -dot + discriminant;

	if (tExit <= 0.0f) return false;

	float32 invRadius = 1.0f / in_sphere.m_radius;
	float32 hitDistance = tEnter >= 0.0f ? tEnter : tExit;
	out_hit->m_point = in_ray.m_origin + in_ray.m_direction * hitDistance;
	out_hit->m_Normal4() = (out_hit->m_point - in_sphere.m_Center4()) * invRadius;
	out_hit->m_distance = hitDistance;

	return true;
}
template bool Raycast<RaycastType3D::e_startsInside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);
template bool Raycast<RaycastType3D::e_startsOutside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);

template<RaycastType3D in_queryType>
int32 RaycastAll(Arg_Ray3D in_ray, Arg_Sphere in_sphere, RayHit3D* out_hits) {
	Deep_Assert(in_sphere.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");
	Deep_Assert(out_hits != nullptr, "Out param must not be nullptr.");

	Vec3 delta = in_ray.m_origin - in_sphere.m_Center4();
	float32 dot = Vec3::s_Dot(delta, in_ray.m_direction);
	float32 distance = delta.m_SqrdMagnitude() - in_sphere.m_radius * in_sphere.m_radius;

	if constexpr (in_queryType == RaycastType3D::e_startsOutside) {
		if (distance < 0.0f) return 0;
	}

	float32 discriminant = dot * dot - distance;
	if (discriminant < 0.0f) return 0;

	discriminant = Sqrt(discriminant);
	float32 tEnter = -dot - discriminant;
	float32 tExit = -dot + discriminant;

	if (tExit <= 0.0f) return 0;

	float32 invRadius = 1.0f / in_sphere.m_radius;
	int32 hitCount = 0;

	// Handle entry hit
	if (tEnter >= 0.0f) {
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * tEnter;
		out_hits[hitCount].m_Normal4() = (out_hits[hitCount].m_point - in_sphere.m_Center4()) * invRadius;
		out_hits[hitCount].m_distance = tEnter;
		++hitCount;
	}

	// Handle exit hit
	{
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * tExit;
		out_hits[hitCount].m_Normal4() = (out_hits[hitCount].m_point - in_sphere.m_Center4()) * invRadius;
		out_hits[hitCount].m_distance = tExit;
		++hitCount;
	}

	return hitCount;
}
template int32 RaycastAll<RaycastType3D::e_startsInside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);
template int32 RaycastAll<RaycastType3D::e_startsOutside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);

DEEP_NAMESPACE_END
