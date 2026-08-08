#pragma once

#include "./Float4.h"

#include "Deep/Math/Vec4.h"

DEEP_NAMESPACE_BEGIN

Float4::Float4(float32 in_x, float32 in_y, float32 in_z, float32 in_w) :
	x{ in_x }, y{ in_y }, z{ in_z }, w{ in_w } {}

Float4::Float4(Arg_Vec4 in_vec) :
	x{ in_vec.x }, y{ in_vec.y }, z{ in_vec.z }, w{ in_vec.w } {}

Float4::operator Vec4() const {
	return Vec4{ x, y, z, w };
}

constexpr float32& Float4::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Float4::operator[](size_t in_index) const {
	return m_values[in_index];
}

DEEP_NAMESPACE_END
