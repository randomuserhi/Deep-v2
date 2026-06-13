#pragma once

#include "Deep/Math/Int3.h"
#include "Deep/Math/Vec3i.h"

DEEP_NAMESPACE_BEGIN

Int3::Int3(int32 in_x, int32 in_y, int32 in_z) :
	x{ in_x }, y{ in_y }, z{ in_z } {}

Int3::Int3(Arg_Vec3i in_vec) :
	x{ in_vec.x }, y{ in_vec.y }, z{ in_vec.z } {}

Int3::operator Vec3i() const {
	return Vec3i{ x, y, z };
}

constexpr int32& Int3::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int32& Int3::operator[](size_t in_index) const {
	return m_values[in_index];
}

DEEP_NAMESPACE_END
