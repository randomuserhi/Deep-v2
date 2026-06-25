#pragma once

#include "Deep/Math/Ops.h"
#include "Deep/Math/Vec2.h"
#include "Deep/Math/Vec2i.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Math/Vec4.h"
#include "Deep/Math/VecArgs.h"

DEEP_NAMESPACE_BEGIN

Vec2::Vec2(float32 in_x, float32 in_y) :
	m_float32x2{ in_x, in_y } {}

Vec2::Vec2(Float32x2 in_float32x2) :
	m_float32x2{ in_float32x2 } {}

Vec2::Vec2(Arg_Vec3 in_vec) :
	m_float32x2{ in_vec.m_float32x4 } {}

Vec2::Vec2(Arg_Vec4 in_vec) :
	m_float32x2{ in_vec.m_float32x4 } {}

Vec2& Vec2::Normalize() {
	float32 magnitudeSqrd = sqrdMagnitude();
	if (magnitudeSqrd != 0.0f) *this /= Sqrt(magnitudeSqrd);
	return *this;
}
Vec2 Vec2::normalized() const {
	Vec2 v = *this;
	return v.Normalize();
}
bool Vec2::IsNormalized(float tolerance) const {
	return Abs(sqrdMagnitude() - 1.0f) <= tolerance;
}

float32 Vec2::sqrdMagnitude() const {
	return Dot(*this, *this);
}
float32 Vec2::magnitude() const {
	return Sqrt(sqrdMagnitude());
}

float32 Vec2::Dot(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return in_a.x * in_b.x + in_a.y * in_b.y;
}

Vec2::operator Float32x2() const {
	return m_float32x2;
}

float32 Vec2::Cross(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return in_a.m_values[0] * in_b.m_values[1] - in_a.m_values[1] * in_b.m_values[0];
}

Vec2 Vec2::Rotate(Arg_Vec2 in_a, float in_radians) {
	float32 cos = Cos(in_radians);
	float32 sin = Sin(in_radians);
	return Vec2{ in_a.x * cos - in_a.y * sin, in_a.x * sin + in_a.y * cos };
}

float32 Vec2::Angle(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	float32 dot = Vec2::Dot(in_a, in_b);
	float32 cross = Vec2::Cross(in_a, in_b);
	return ATan2(cross, dot);
}

Vec2 Vec2::Lerp(Arg_Vec2 in_a, Arg_Vec2 in_b, float32 in_t) {
	return (in_b - in_a) * in_t + in_a;
}

Vec2::operator Vec2i() const {
	return Vec2i{ static_cast<int>(x), static_cast<int>(y) };
}

constexpr float32& Vec2::operator[](size_t in_index) {
	return m_values[in_index];
}
constexpr const float32& Vec2::operator[](size_t in_index) const {
	return m_values[in_index];
}

bool operator!=(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return in_a.m_float32x2 != in_b.m_float32x2;
}

bool operator==(Arg_Vec2 a, Arg_Vec2 b) {
	return !(a != b);
}

Vec2& Vec2::operator+=(Arg_Vec2 in_other) {
	m_float32x2 += in_other.m_float32x2;
	return *this;
}

Vec2 operator+(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return Vec2{ in_a.m_float32x2 + in_b.m_float32x2 };
}

Vec2& Vec2::operator-=(Arg_Vec2 in_other) {
	m_float32x2 -= in_other.m_float32x2;
	return *this;
}

Vec2 operator-(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return Vec2{ in_a.m_float32x2 - in_b.m_float32x2 };
}

Vec2 operator-(Arg_Vec2 in_a) {
	return Vec2{ -in_a.m_float32x2 };
}

Vec2& Vec2::operator*=(Arg_Vec2 in_other) {
	m_float32x2 *= in_other.m_float32x2;
	return *this;
}
Vec2 operator*(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return Vec2{ in_a.m_float32x2 * in_b.m_float32x2 };
}

Vec2& Vec2::operator*=(float32 in_other) {
	m_float32x2 *= in_other;
	return *this;
}

Vec2 operator*(Arg_Vec2 in_vec, float32 in_val) {
	return Vec2{ in_vec.m_float32x2 * in_val };
}

Vec2 operator*(float32 in_val, Arg_Vec2 in_vec) {
	return Vec2{ in_val * in_vec.m_float32x2 };
}

Vec2& Vec2::operator/=(Arg_Vec2 in_other) {
	m_float32x2 /= in_other.m_float32x2;
	return *this;
}
Vec2 operator/(Arg_Vec2 in_a, Arg_Vec2 in_b) {
	return Vec2{ in_a.m_float32x2 / in_b.m_float32x2 };
}

Vec2& Vec2::operator/=(float32 in_other) {
	m_float32x2 /= in_other;
	return *this;
}

Vec2 operator/(Arg_Vec2 in_vec, float32 in_val) {
	return Vec2{ in_vec.m_float32x2 / in_val };
}

Vec2 operator/(float32 in_val, Arg_Vec2 in_vec) {
	return Vec2{ in_val / in_vec.m_float32x2 };
}

std::ostream& operator<<(std::ostream& in_os, const Vec2& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << " }";
}

DEEP_NAMESPACE_END
