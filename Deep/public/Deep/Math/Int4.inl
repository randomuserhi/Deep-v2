#pragma once

#include "Deep/Math/Int4.h"
#include "Deep/Math/Vec4i.h"

DEEP_NAMESPACE_BEGIN

Int4::Int4(int32 in_x, int32 in_y, int32 in_z, int32 in_w) :
	x{ in_x }, y{ in_y }, z{ in_z }, w{ in_w } {}

Int4::Int4(Arg_Vec4i in_vec) :
	x{ in_vec.x }, y{ in_vec.y }, z{ in_vec.z }, w{ in_vec.w } {}

Int4::operator Vec4i() const {
	return Vec4i{ x, y, z, w };
}

constexpr int32& Int4::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int32& Int4::operator[](size_t in_index) const {
	return m_values[in_index];
}

DEEP_NAMESPACE_END
