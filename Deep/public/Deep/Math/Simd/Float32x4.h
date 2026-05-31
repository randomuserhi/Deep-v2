#pragma once

#include "Deep.h"
#include "Deep/Math/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#include <immintrin.h>
DEEP_SUPPRESS_WARNINGS_STD_END
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(__m128)
#elif defined(DEEP_USE_NEON)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#ifdef DEEP_COMPILER_MSVC
		#include <intrin.h>
		#include <arm64_neon.h>
	#else
		#include <arm_neon.h>
	#endif
DEEP_SUPPRESS_WARNINGS_STD_END
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(float32x4_t)
#else
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(float32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for __m128 SIMD type representing 4 independent values
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Float32x4 {
#if defined(DEEP_USE_SSE)
	using Type = __m128;
#elif defined(DEEP_USE_NEON)
	using Type = float32x4_t;
#else
	using Type = struct {
		float32 m_values[4];
	};
#endif

	//

	constexpr Float32x4() = default;
	constexpr Float32x4(const Float32x4&) = default;
	constexpr Float32x4& operator=(const Float32x4&) = default;
	inline Float32x4(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
	constexpr inline Float32x4(Type in_internal);

	constexpr static inline Float32x4 Constexpr(float32 in_x, float32 in_y, float32 in_z, float32 in_w);

	//

	// Implicit conversion
	constexpr inline operator Type() const;

	//

	// Convert into __m128i, converting each component to an integer
	inline Int32x4 ToInt() const;
	constexpr inline Int32x4 Constexpr_ToInt() const;

	// Reinterpret bytes as a __m128i
	inline Int32x4 ReinterpretAsInt() const;
	constexpr inline Int32x4 Constexpr_ReinterpretAsInt() const;

	// Replicate the given value across all components
	static inline Float32x4 Replicate(float32 in_value);

	// Component wise min/max
	static inline Float32x4 Min(Arg_Float32x4 in_a, Arg_Float32x4 in_b);
	static inline Float32x4 Max(Arg_Float32x4 in_a, Arg_Float32x4 in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int32x4 Equals(Arg_Float32x4 in_a, Arg_Float32x4 in_b);
	constexpr static inline Int32x4 Constexpr_Equals(Arg_Float32x4 in_a, Arg_Float32x4 in_b);

	// Returns 0xFFFFFFFF for each lane where val < 0.0f
	static inline Int32x4 IsNegative(Arg_Float32x4 in_value);

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline Float32x4 Select(Arg_Float32x4 in_a, Arg_Float32x4 in_b, const Arg_Int32x4 in_control);

	//

	// Equality
	friend inline bool operator!=(Arg_Float32x4 in_a, Arg_Float32x4 in_b);
	friend inline bool operator==(Arg_Float32x4 in_a, Arg_Float32x4 in_b);

	// Component wise logical OR
	inline Float32x4& operator|=(Arg_Float32x4 in_other);
	friend inline Float32x4 operator|(Float32x4 in_a, Arg_Float32x4 in_b);

	// Component wise logical AND
	inline Float32x4& operator&=(Arg_Float32x4 in_other);
	friend inline Float32x4 operator&(Float32x4 in_a, Arg_Float32x4 in_b);

	// Component wise logical XOR
	inline Float32x4& operator^=(Arg_Float32x4 in_other);
	friend inline Float32x4 operator^(Float32x4 in_a, Arg_Float32x4 in_b);

	// Component wise Add
	inline Float32x4& operator+=(Arg_Float32x4 roOther);
	friend inline Float32x4 operator+(Float32x4 in_a, Arg_Float32x4 in_b);

	// Component wise Sub
	inline Float32x4& operator-=(Arg_Float32x4 in_other);
	friend inline Float32x4 operator-(Float32x4 in_a, Arg_Float32x4 in_b);
	friend inline Float32x4 operator-(Arg_Float32x4 in_other);

	// Component wise Mul
	inline Float32x4& operator*=(Arg_Float32x4 in_other);
	friend inline Float32x4 operator*(Float32x4 in_a, Arg_Float32x4 in_b);

	// Component wise Div
	inline Float32x4& operator/=(Arg_Float32x4 in_other);
	friend inline Float32x4 operator/(Float32x4 in_a, Arg_Float32x4 in_b);

	// Mul components with float
	inline Float32x4& operator*=(float32 in_other);
	friend inline Float32x4 operator*(Float32x4 in_vec, float32 in_val);
	friend inline Float32x4 operator*(float32 in_val, Arg_Float32x4 in_vec);

	// Div components with float
	inline Float32x4& operator/=(float32 in_other);
	friend inline Float32x4 operator/(Float32x4 in_vec, float32 in_val);
	friend inline Float32x4 operator/(float32 in_val, Arg_Float32x4 in_vec);

	// Calculate the sin and cosin for each component and store the result
	// in `sin` and `cos` respectively
	inline void SinCos(Float32x4& out_sin, Float32x4& out_cos);

	//

	union {
		Type _internal;
		float32 m_values[4];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
	};
};

static_assert(std::is_trivial<Float32x4>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Float32x4>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "Deep/Math/Simd/Float32x4.inl" // IWYU pragma: export
