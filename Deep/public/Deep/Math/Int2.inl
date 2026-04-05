#pragma once

#include "Deep/Math/Int2.h"
#include "Deep/Math/Vec2i.h"

DEEP_NAMESPACE_BEGIN

Int2::Int2(int32 in_x, int32 in_y) :
	x{ in_x }, y{ in_y } {}

Int2::Int2(Arg_Vec2i in_vec) :
	x{ in_vec.x }, y{ in_vec.y } {}

Int2::operator Vec2i() const {
	return Vec2i{ x, y };
}

DEEP_NAMESPACE_END
