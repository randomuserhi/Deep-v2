#include "Deep.h"
#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/Ray.h"
#include "Deep/Physics3D/Sphere.h"

DEEP_NAMESPACE_BEGIN

int IsOverlapping(Arg_Sphere in_a, Arg_Sphere in_b, ContactInfo* out_contactInfo) {
	Deep_Assert(in_a.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(in_b.m_radius > 0, "Radius of sphere > 0.");
	Deep_Assert(out_contactInfo != nullptr, "Out param must not be nullptr.");

	Vec3 delta = in_b.m_center - in_a.m_center;
	float32 distanceSqrd = delta.sqrdMagnitude();

	float32 radiusSum = in_a.m_radius + in_b.m_radius;
	float32 radiusSumSqrd = radiusSum * radiusSum;

	if (distanceSqrd >= radiusSumSqrd) {
		return 0;
	}

	ContactInfo contactInfo;

	float32 distance = Sqrt(distanceSqrd);

	if (distance > k_absTolerance) {
		contactInfo.m_normal = delta / distance;
	} else {
		// Arbitrary normal if centers coincide
		contactInfo.m_normal = Vec3::k_up;
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

	float32 b = Vec3::Dot(delta, in_ray.m_direction);
	float32 c = Vec3::Dot(delta, delta) - in_sphere.m_radius * in_sphere.m_radius;

	if constexpr (in_queryType == RaycastType::e_startsOutside) {
		if (c < 0.0f) return false;
	}

	if (c > 0.0f && b > 0.0f) {
		return false;
	}

	float32 discriminant = b * b - c;

	return discriminant >= 0.0f;
}
template bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere);
template bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere);

DEEP_NAMESPACE_END
