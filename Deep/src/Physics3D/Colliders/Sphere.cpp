#include "Deep.h"
#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Colliders/Sphere.h"

DEEP_NAMESPACE_BEGIN

int IsOverlapping(Arg_Sphere in_sphereA, Arg_Sphere in_sphereB, ContactInfo* out_contactInfo) {
	Deep_Assert(in_sphereA.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_sphereB.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(out_contactInfo != nullptr, "Out param must not be nullptr.");

	Vec3 delta = in_sphereB.m_center - in_sphereA.m_center;
	float32 distanceSqrd = delta.sqrdMagnitude();

	float32 radiusSum = in_sphereA.m_radius + in_sphereB.m_radius;
	float32 radiusSumSqrd = radiusSum * radiusSum;

	if (distanceSqrd >= radiusSumSqrd) {
		return 0;
	}

	ContactInfo contactInfo;

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

template<RaycastType in_queryType>
bool Raycast(Arg_Ray3D in_ray, Arg_Sphere in_sphere) {
	Deep_Assert(in_sphere.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	Vec3 delta = in_ray.m_origin - in_sphere.m_center;

	float32 dot = Vec3::Dot(delta, in_ray.m_direction);
	float32 distance = delta.sqrdMagnitude() - in_sphere.m_radius * in_sphere.m_radius;

	if constexpr (in_queryType == RaycastType::e_startsOutside) {
		if (distance < 0.0f) return false;
	}

	if (dot > 0.0f) {
		return false;
	}

	float32 discriminant = dot * dot - distance;

	return discriminant >= 0.0f;
}
template bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere);
template bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere);

template<RaycastType in_queryType>
bool Raycast(Arg_Ray3D in_ray, Arg_Sphere in_sphere, RayHit3D* out_hits) {
	Deep_Assert(in_sphere.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	// TODO(randomuserhi)
	(void)in_ray;
	(void)in_sphere;
	(void)out_hits;

	return false;
}
template bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);
template bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);

template<RaycastType in_queryType>
int32 RaycastAll(Arg_Ray3D in_ray, Arg_Sphere in_sphere, RayHit3D* out_hits) {
	Deep_Assert(in_sphere.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_ray.m_direction.IsNormalized(), "Direction should be normalized.");

	// TODO(randomuserhi): Fairly sure logic here is wrong, needs testing

	Vec3 delta = in_ray.m_origin - in_sphere.m_center;

	float32 dot = Vec3::Dot(delta, in_ray.m_direction);
	float32 distance = delta.sqrdMagnitude() - in_sphere.m_radius * in_sphere.m_radius;

	float32 discriminant = dot * dot - distance;

	// No hits
	if (discriminant < 0) {
		return 0;
	}

	int32 hitCount = 0;
	float32 invRadius;

	if constexpr (in_queryType == RaycastType::e_startsOutside) {
		if (distance < 0.0f) return 0;
		discriminant = Sqrt(discriminant);
		invRadius = 1.0f / in_sphere.m_radius;
	} else {
		discriminant = Sqrt(discriminant);
		invRadius = 1.0f / in_sphere.m_radius;

		// Handle entry hit
		if (distance > 0.0f) {
			float32 t = -dot - discriminant;
			out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * t;
			out_hits[hitCount].m_Normal4() =
				(out_hits[hitCount].m_point - in_sphere.m_center) * invRadius; // normalize via division
			++hitCount;
		}
	}

	if (dot > 0.0f) {
		return hitCount;
	}

	// Exit hit
	{
		float32 t = -dot + discriminant;
		out_hits[hitCount].m_point = in_ray.m_origin + in_ray.m_direction * t;
		out_hits[hitCount].m_Normal4() =
			(out_hits[hitCount].m_point - in_sphere.m_center) * invRadius; // normalize via division
		++hitCount;
	}

	return hitCount;
}
template int32 RaycastAll<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);
template int32 RaycastAll<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);

DEEP_NAMESPACE_END
