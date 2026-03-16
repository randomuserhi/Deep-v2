#pragma once

#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/ContactInfo.h"
#include "Deep/Physics3D/Ray.h"

DEEP_NAMESPACE_BEGIN

struct DEEP_EXPORT Aabb3D {
	Vec3 m_center;
	Vec3 m_extents;
};

using Arg_Aabb3D = const Aabb3D&;

DEEP_EXPORT bool IsOverlapping(Vec3 in_point, Arg_Aabb3D in_a);

DEEP_EXPORT bool IsOverlapping(Arg_Aabb3D in_a, Arg_Aabb3D in_b);
DEEP_EXPORT int IsOverlapping(Arg_Aabb3D in_a, Arg_Aabb3D in_b, ContactInfo* out_contactInfo);

template<RaycastType in_queryType = RaycastType::e_default>
bool Raycast(Ray3D in_ray, Arg_Aabb3D in_box);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsInside>(Ray3D, Arg_Aabb3D);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsOutside>(Ray3D, Arg_Aabb3D);

template<RaycastType in_queryType = RaycastType::e_default>
bool Raycast(Ray3D in_ray, Arg_Aabb3D in_box, RayHit3D* out_hit);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsInside>(Ray3D, Arg_Aabb3D, RayHit3D*);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsOutside>(Ray3D, Arg_Aabb3D, RayHit3D*);

template<RaycastType in_queryType = RaycastType::e_default>
int RaycastAll(Ray3D in_ray, Arg_Aabb3D in_box, RayHit3D* out_hits);
extern template DEEP_EXPORT int RaycastAll<RaycastType::e_startsInside>(Ray3D, Arg_Aabb3D, RayHit3D*);
extern template DEEP_EXPORT int RaycastAll<RaycastType::e_startsOutside>(Ray3D, Arg_Aabb3D, RayHit3D*);

DEEP_NAMESPACE_END
