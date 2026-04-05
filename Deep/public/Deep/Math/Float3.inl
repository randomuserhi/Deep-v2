#pragma once

#include "Deep/Math/Float3.h"
#include "Deep/Math/Vec3.h"

DEEP_NAMESPACE_BEGIN

Float3::Float3(float32 in_x, float32 in_y, float32 in_z) :
	x{ in_x }, y{ in_y }, z{ in_z } {}

Float3::Float3(Arg_Vec3 in_vec) :
	x{ in_vec.x }, y{ in_vec.y }, z{ in_vec.z } {}

Float3::operator Vec3() const {
	return Vec3{ x, y, z };
}

DEEP_NAMESPACE_END
