#pragma once

#include "Deep/Math/Vec2i.h"
#include "Deep/Math/Vec2.h"
#include "Deep/Math/Vec4i.h"

DEEP_NAMESPACE_BEGIN

Vec2i::Vec2i(int32 in_x, int32 in_y) :
	m_int32x2{ in_x, in_y } {}

Vec2i::Vec2i(Int32x2 in_int32x2) :
	m_int32x2{ in_int32x2 } {}

Vec2i::Vec2i(Arg_Vec3i in_vec) :
	m_int32x2{ in_vec.m_int32x4 } {}

Vec2i::Vec2i(Arg_Vec4i in_vec) :
	m_int32x2{ in_vec.m_int32x4 } {}

float32 Vec2i::sqrdMagnitude() const {
	return static_cast<Vec2>(*this).sqrdMagnitude();
}

float32 Vec2i::magnitude() const {
	return static_cast<Vec2>(*this).magnitude();
}

int32 Vec2i::manhattanDistance() const {
	return x + y;
}

int32 Vec2i::Dot(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return in_a.x * in_b.x + in_a.y * in_b.y;
}

Vec2i::operator Int32x2() const {
	return m_int32x2;
}

Vec2i::operator Vec2() const {
	return Vec2{ static_cast<float>(x), static_cast<float>(y) };
}

constexpr int32& Vec2i::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const int32& Vec2i::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return in_a.m_int32x2 != in_b.m_int32x2;
}

bool operator==(Arg_Vec2i a, Arg_Vec2i b) {
	return !(a != b);
}

Vec2i& Vec2i::operator+=(Arg_Vec2i in_other) {
	m_int32x2 += in_other.m_int32x2;
	return *this;
}

Vec2i operator+(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.m_int32x2 + in_b.m_int32x2 };
}

Vec2i& Vec2i::operator-=(Arg_Vec2i in_other) {
	m_int32x2 -= in_other.m_int32x2;
	return *this;
}

Vec2i operator-(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.m_int32x2 - in_b.m_int32x2 };
}

Vec2i operator-(Arg_Vec2i in_a) {
	return Vec2i{ -in_a.m_int32x2 };
}

Vec2i& Vec2i::operator*=(Arg_Vec2i in_other) {
	m_int32x2 *= in_other.m_int32x2;
	return *this;
}
Vec2i operator*(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.m_int32x2 * in_b.m_int32x2 };
}

Vec2i& Vec2i::operator*=(int32 in_other) {
	m_int32x2 *= in_other;
	return *this;
}

Vec2i operator*(Arg_Vec2i in_vec, int32 in_val) {
	return Vec2i{ in_vec.m_int32x2 * in_val };
}

Vec2i operator*(int32 in_val, Arg_Vec2i in_vec) {
	return Vec2i{ in_val * in_vec.m_int32x2 };
}

Vec2i& Vec2i::operator/=(Arg_Vec2i in_other) {
	m_int32x2 /= in_other.m_int32x2;
	return *this;
}
Vec2i operator/(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.m_int32x2 / in_b.m_int32x2 };
}

Vec2i& Vec2i::operator/=(int32 in_other) {
	m_int32x2 /= in_other;
	return *this;
}

Vec2i operator/(Arg_Vec2i in_vec, int32 in_val) {
	return Vec2i{ in_vec.m_int32x2 / in_val };
}

Vec2i operator/(int32 in_val, Arg_Vec2i in_vec) {
	return Vec2i{ in_val / in_vec.m_int32x2 };
}

DEEP_NAMESPACE_END
