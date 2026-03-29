#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/ContactInfo.h"
#include "Deep/Physics3D/Ray.h"

DEEP_NAMESPACE_BEGIN

struct DEEP_EXPORT Sphere {
	Vec3 m_center;
	float32 m_radius;
};

using Arg_Sphere = const Sphere&;

inline bool IsOverlapping(Arg_Vec3 in_point, Arg_Sphere in_a);
inline bool IsOverlapping(Arg_Sphere in_a, Arg_Sphere in_b);
DEEP_EXPORT int IsOverlapping(Arg_Sphere in_a, Arg_Sphere in_b, ContactInfo* out_contactInfo);

template<RaycastType in_queryType = RaycastType::e_default>
bool Raycast(Arg_Ray3D in_ray, Arg_Sphere in_sphere);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere);

// TODO(randomuserhi)
template<RaycastType in_queryType = RaycastType::e_default>
bool Raycast(Arg_Ray3D in_ray, Arg_Sphere in_sphere, RayHit3D* out_hit);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);

// TODO(randomuserhi)
template<RaycastType in_queryType = RaycastType::e_default>
int32 RaycastAll(Arg_Ray3D in_ray, Arg_Sphere in_sphere, RayHit3D* out_hits);
extern template DEEP_EXPORT int32 RaycastAll<RaycastType::e_startsInside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);
extern template DEEP_EXPORT int32 RaycastAll<RaycastType::e_startsOutside>(Arg_Ray3D, Arg_Sphere, RayHit3D*);

DEEP_NAMESPACE_END

#include "Deep/Physics3D/Sphere.inl" // IWYU pragma: export
