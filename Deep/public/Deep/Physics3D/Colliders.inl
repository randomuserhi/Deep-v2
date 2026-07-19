#pragma once

#include "./Colliders.h"

DEEP_NAMESPACE_BEGIN

bool IsOverlapping(Arg_Sphere in_sphere, Arg_Aabb3D in_box) {
	return IsOverlapping(in_box, in_sphere);
}

int IsOverlapping(Arg_Sphere in_sphere, Arg_Aabb3D in_box, ContactInfo* out_contactInfo) {
	return IsOverlapping(in_box, in_sphere, out_contactInfo);
}

DEEP_NAMESPACE_END
