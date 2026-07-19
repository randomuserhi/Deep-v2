#pragma once

#include "Deep.h"
#include "Deep/Physics3D/Colliders/Aabb.h"   // IWYU pragma: export
#include "Deep/Physics3D/Colliders/Sphere.h" // IWYU pragma: export

DEEP_NAMESPACE_BEGIN

DEEP_EXPORT bool IsOverlapping(Arg_Aabb3D in_box, Arg_Sphere in_sphere);
Deep_ForceInline bool IsOverlapping(Arg_Sphere in_sphere, Arg_Aabb3D in_box);

DEEP_EXPORT int IsOverlapping(Arg_Aabb3D in_box, Arg_Sphere in_sphere, ContactInfo* out_contactInfo);
Deep_ForceInline int IsOverlapping(Arg_Sphere in_sphere, Arg_Aabb3D in_box, ContactInfo* out_contactInfo);

DEEP_NAMESPACE_END

#include "./Colliders.inl" // IWYU pragma: export
