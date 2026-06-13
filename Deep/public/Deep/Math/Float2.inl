#pragma once

#include "Deep/Math/Float2.h"
#include "Deep/Math/Vec2.h"

DEEP_NAMESPACE_BEGIN

Float2::Float2(float32 in_x, float32 in_y) :
	x{ in_x }, y{ in_y } {}

Float2::Float2(Arg_Vec2 in_vec) :
	x{ in_vec.x }, y{ in_vec.y } {}

Float2::operator Vec2() const {
	return Vec2{ x, y };
}

constexpr float32& Float2::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Float2::operator[](size_t in_index) const {
	return m_values[in_index];
}

DEEP_NAMESPACE_END
