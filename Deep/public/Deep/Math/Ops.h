#pragma once

#include <Deep.h>
#include <Deep/Bit.h>

#include <cmath>

DEEP_NAMESPACE_BEGIN

// NOTE(randomuserhi):
// https://stackoverflow.com/questions/13721839/if-stdmax-returns-by-reference-as-it-must-might-that-lead-to-a-dangling-r

template<typename T>
Deep_Inline constexpr const T& Min(const T& in_a, const T& in_b) {
	return (in_b < in_a) ? in_b : in_a;
}

template<typename T>
Deep_Inline constexpr const T& Max(const T& in_a, const T& in_b) {
	return (in_b > in_a) ? in_b : in_a;
}

template<typename T>
Deep_Inline constexpr const T& Clamp(const T& in_val, const T& in_lower, const T& in_upper) {
	return Min(in_upper, Max(in_val, in_lower));
}

template<typename T>
Deep_Inline constexpr const T& Clamp01(const T& in_val) {
	return Clamp(in_val, 0.0f, 1.0f);
}

template<typename T>
Deep_Inline constexpr const T Sqrt(const T& in_val) {
	return std::sqrt(in_val);
}

template<typename T>
Deep_Inline constexpr const T Exp(const T& in_val) {
	return std::exp(in_val);
}

template<typename T>
Deep_Inline constexpr const T Exp2(const T& in_val) {
	return std::exp2(in_val);
}

template<typename T>
Deep_Inline constexpr const T Abs(const T& in_val) {
	return std::abs(in_val);
}

template<typename T>
Deep_Inline constexpr T Sign(const T& in_val) {
	return in_val < 0 ? static_cast<T>(-1) : static_cast<T>(1);
}

template<typename T>
Deep_Inline constexpr T Sin(const T& in_val) {
	return std::sin(in_val);
}

template<typename T>
Deep_Inline constexpr T Cos(const T& in_val) {
	return std::cos(in_val);
}

template<typename T>
Deep_Inline constexpr T ATan2(const T& in_y, const T& in_x) {
	return std::atan2(in_y, in_x);
}

template<typename T>
Deep_Inline constexpr bool isClose(const T& in_a, const T& in_b, const T& in_relTolerance = 1e-05f,
                                   const T& in_absoluteTolerance = 1e-06f) {
	// NOTE(randomuserhi): https://numpy.org/devdocs/reference/generated/numpy.isclose.html, uses `a` instead `b` as
	// relative point NOTE(randomuserhi): Does not account for NaN
	return Abs(in_a - in_b) <= (in_absoluteTolerance + in_relTolerance * Abs(in_a));
}

DEEP_NAMESPACE_END