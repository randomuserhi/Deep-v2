#pragma once

#include "Constants.h"
#include <Deep.h>
#include <Deep/Bit.h>

#include <cmath>

DEEP_NAMESPACE_BEGIN

// NOTE(randomuserhi):
// https://stackoverflow.com/questions/13721839/if-stdmax-returns-by-reference-as-it-must-might-that-lead-to-a-dangling-r

template<typename T>
inline constexpr const T& Min(const T& in_a, const T& in_b) {
	// NOTE(randomuserhi): Matches the behaviour of MIN(SRC1, SRC2) from Intel's Instruction Set Reference, A-Z
	//                     https://cdrdv2-public.intel.com/782156/325383-sdm-vol-2abcd.pdf
	//                     This is important in regards to keeping SSE and Non-SSE operations consistent.
	return (in_a < in_b) ? in_a : in_b;
}

template<typename T>
inline constexpr const T& Max(const T& in_a, const T& in_b) {
	// NOTE(randomuserhi): Matches the behaviour of MAX(SRC1, SRC2) from Intel's Instruction Set Reference, A-Z
	//                     https://cdrdv2-public.intel.com/782156/325383-sdm-vol-2abcd.pdf
	//                     This is important in regards to keeping SSE and Non-SSE operations consistent.
	return (in_a > in_b) ? in_a : in_b;
}

template<typename T>
inline constexpr const T& Clamp(const T& in_val, const T& in_lower, const T& in_upper) {
	return Min(in_upper, Max(in_val, in_lower));
}

template<typename T>
inline constexpr T Clamp01(const T& in_val) {
	return Clamp(in_val, T{ 0 }, T{ 1 });
}

template<typename T>
inline constexpr const T Sqrt(const T& in_val) {
	return std::sqrt(in_val);
}

template<typename T>
inline constexpr const T Exp(const T& in_val) {
	return std::exp(in_val);
}

template<typename T>
inline constexpr const T Exp2(const T& in_val) {
	return std::exp2(in_val);
}

template<typename T>
inline constexpr const T Abs(const T& in_val) {
	return std::abs(in_val);
}

template<typename T>
inline constexpr T Sign(const T& in_val) {
	return in_val < 0 ? static_cast<T>(-1) : static_cast<T>(1);
}

template<typename T>
inline constexpr T Sin(const T& in_val) {
	return std::sin(in_val);
}

template<typename T>
inline constexpr T Cos(const T& in_val) {
	return std::cos(in_val);
}

template<typename T>
inline constexpr T ATan2(const T& in_y, const T& in_x) {
	return std::atan2(in_y, in_x);
}

// Based on https://numpy.org/devdocs/reference/generated/numpy.isclose.html
// NOTE(randomuserhi): Does not account for NaN
template<typename T>
inline constexpr bool IsClose(const T& in_a, const T& in_b, const T& in_relTolerance = static_cast<T>(k_relTolerance),
                              const T& in_absTolerance = static_cast<T>(k_absTolerance)) {
	return Abs(in_a - in_b) <= (in_absTolerance + in_relTolerance * Abs(in_b));
}

DEEP_NAMESPACE_END
