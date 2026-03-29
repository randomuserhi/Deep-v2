#pragma once

#include "Deep/Math/Vec2i.h"
#include "Deep/Math/Vec2.h"
#include "Deep/Math/Vec4i.h"

DEEP_NAMESPACE_BEGIN

Vec2i::Vec2i(int32 in_x, int32 in_y) :
	x(in_x), y(in_y) {}

Vec2i::Vec2i(Arg_Vec3i in_vec) :
	x(in_vec.x), y(in_vec.y) {}

Vec2i::Vec2i(Arg_Vec4i in_vec) :
	x(in_vec.x), y(in_vec.y) {}

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

Vec2i::operator Vec2() const {
	return Vec2{ static_cast<float>(x), static_cast<float>(y) };
}

inline bool operator!=(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return in_a.x != in_b.x || in_a.y != in_b.y;
}
inline bool operator==(Arg_Vec2i a, Arg_Vec2i b) {
	return !(a != b);
}

Vec2i& Vec2i::operator+=(Arg_Vec2i in_other) {
	x += in_other.x;
	y += in_other.y;
	return *this;
}

Vec2i operator+(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.x + in_b.x, in_a.y + in_b.y };
}

Vec2i& Vec2i::operator-=(Arg_Vec2i in_other) {
	x -= in_other.x;
	y -= in_other.y;
	return *this;
}

Vec2i operator-(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.x - in_b.x, in_a.y - in_b.y };
}

Vec2i operator-(Arg_Vec2i in_a) {
	// NOTE(randomuserhi): 0 - x to stay consistent with vectorised version
	return Vec2i{ 0 - in_a.x, 0 - in_a.y };
}

Vec2i& Vec2i::operator*=(Arg_Vec2i in_other) {
	x *= in_other.x;
	y *= in_other.y;
	return *this;
}
Vec2i operator*(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.x * in_b.x, in_a.y * in_b.y };
}

Vec2i& Vec2i::operator*=(int32 in_other) {
	x *= in_other;
	y *= in_other;
	return *this;
}

Vec2i operator*(Arg_Vec2i in_vec, int32 in_val) {
	return Vec2i{ in_vec.x * in_val, in_vec.y * in_val };
}

Vec2i operator*(int32 in_val, Arg_Vec2i in_vec) {
	return Vec2i{ in_val * in_vec.x, in_val * in_vec.y };
}

Vec2i& Vec2i::operator/=(Arg_Vec2i in_other) {
	x /= in_other.x;
	y /= in_other.y;
	return *this;
}
Vec2i operator/(Arg_Vec2i in_a, Arg_Vec2i in_b) {
	return Vec2i{ in_a.x / in_b.x, in_a.y / in_b.y };
}

Vec2i& Vec2i::operator/=(int32 in_other) {
	x /= in_other;
	y /= in_other;
	return *this;
}

Vec2i operator/(Arg_Vec2i in_vec, int32 in_val) {
	return Vec2i{ in_vec.x / in_val, in_vec.y / in_val };
}

Vec2i operator/(int32 in_val, Arg_Vec2i in_vec) {
	return Vec2i{ in_val / in_vec.x, in_val / in_vec.y };
}

DEEP_NAMESPACE_END