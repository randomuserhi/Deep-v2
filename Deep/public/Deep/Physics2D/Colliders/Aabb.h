#pragma once

#include "Deep.h"
#include "Deep/Math/Vec2.h"
#include "Deep/Physics2D/ContactInfo.h"
#include "Deep/Physics2D/Ray.h"

DEEP_NAMESPACE_BEGIN

struct DEEP_EXPORT Aabb2D {
	Vec2 m_center;
	Vec2 m_extents;
};

using Arg_Aabb2D = const Aabb2D&;

inline bool IsOverlapping(Arg_Vec3 in_point, Arg_Aabb2D in_box);
inline bool IsOverlapping(Arg_Aabb2D in_boxA, Arg_Aabb2D in_boxB);
DEEP_EXPORT int IsOverlapping(Arg_Aabb2D in_boxA, Arg_Aabb2D in_boxB, ContactInfo* out_contactInfo);

template<RaycastType in_queryType = RaycastType::e_default>
bool Raycast(Arg_Ray2D in_ray, Arg_Aabb2D in_box);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsInside>(Arg_Ray2D, Arg_Aabb2D);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsOutside>(Arg_Ray2D, Arg_Aabb2D);

template<RaycastType in_queryType = RaycastType::e_default>
bool Raycast(Arg_Ray2D in_ray, Arg_Aabb2D in_box, RayHit2D* out_hit);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsInside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);
extern template DEEP_EXPORT bool Raycast<RaycastType::e_startsOutside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);

template<RaycastType in_queryType = RaycastType::e_default>
int32 RaycastAll(Arg_Ray2D in_ray, Arg_Aabb2D in_box, RayHit2D* out_hits);
extern template DEEP_EXPORT int32 RaycastAll<RaycastType::e_startsInside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);
extern template DEEP_EXPORT int32 RaycastAll<RaycastType::e_startsOutside>(Arg_Ray2D, Arg_Aabb2D, RayHit2D*);

DEEP_NAMESPACE_END

#include "./Aabb.inl" // IWYU pragma: export
