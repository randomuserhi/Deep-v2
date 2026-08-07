#pragma once

#include "Deep.h"
#include "Deep/Asm/Intrinsics.h"
#include "Deep/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE)
	#define DEEP_VEC_ALIGNMENT alignof(float32)
#elif defined(DEEP_USE_NEON)
	#define DEEP_VEC_ALIGNMENT alignof(float32x2_t)
#elif defined(DEEP_USE_WASM_SIMD128)
	#define DEEP_VEC_ALIGNMENT alignof(float32)
#else
	#define DEEP_VEC_ALIGNMENT alignof(float32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for float32x2_t SIMD type representing 4 independent values
// We do not use __m64 for x86_64 systems as its an outdated mmx technology, rather the lower 64 bits of __m128 are used
//

struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Float32x2 {
#if defined(DEEP_USE_NEON)
	using Type = float32x2_t;
#else
	using Type = struct {
		float32 m_values[2];
	};
#endif

	//

	constexpr Float32x2() = default;
	constexpr Float32x2(const Float32x2&) = default;
	constexpr Float32x2& operator=(const Float32x2&) = default;
	inline Float32x2(float32 in_x, float32 in_y);
	inline Float32x2(Type in_internal);
	inline Float32x2(Float32x4);

	constexpr static inline Float32x2 Constexpr(float32 in_x, float32 in_y);

	//

	// Implicit conversion
	constexpr inline operator Type() const;

	//

	// Convert into int32x2_t, converting each component to an integer
	inline Int32x2 ToInt() const;
	constexpr inline Int32x2 Constexpr_ToInt() const;

	// Reinterpret bytes as a int32x2_t
	inline Int32x2 ReinterpretAsInt() const;
	constexpr inline Int32x2 Constexpr_ReinterpretAsInt() const;

	// Replicate the given value across all components
	static inline Float32x2 s_Replicate(float32 in_value);

	// Component wise min/max
	static inline Float32x2 s_Min(Arg_Float32x2 in_a, Arg_Float32x2 in_b);
	static inline Float32x2 s_Max(Arg_Float32x2 in_a, Arg_Float32x2 in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int32x2 s_Equals(Arg_Float32x2 in_a, Arg_Float32x2 in_b);

	// Returns 0xFFFFFFFF for each lane where val < 0.0f
	static inline Int32x2 s_IsNegative(Arg_Float32x2 in_value);

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline Float32x2 s_Select(Arg_Float32x2 in_a, Arg_Float32x2 in_b, const Arg_Int32x2 in_control);

	//

	Deep_ForceInline constexpr float32& operator[](size_t);
	Deep_ForceInline constexpr const float32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Float32x2 in_a, Arg_Float32x2 in_b);
	friend inline bool operator==(Arg_Float32x2 in_a, Arg_Float32x2 in_b);

	// Component wise logical OR
	inline Float32x2& operator|=(Arg_Float32x2 in_other);
	friend inline Float32x2 operator|(Float32x2 in_a, Arg_Float32x2 in_b);

	// Component wise logical AND
	inline Float32x2& operator&=(Arg_Float32x2 in_other);
	friend inline Float32x2 operator&(Float32x2 in_a, Arg_Float32x2 in_b);

	// Component wise logical XOR
	inline Float32x2& operator^=(Arg_Float32x2 in_other);
	friend inline Float32x2 operator^(Float32x2 in_a, Arg_Float32x2 in_b);

	// Component wise Add
	inline Float32x2& operator+=(Arg_Float32x2 roOther);
	friend inline Float32x2 operator+(Float32x2 in_a, Arg_Float32x2 in_b);

	// Component wise Sub
	inline Float32x2& operator-=(Arg_Float32x2 in_other);
	friend inline Float32x2 operator-(Float32x2 in_a, Arg_Float32x2 in_b);
	friend inline Float32x2 operator-(Arg_Float32x2 in_other);

	// Component wise Mul
	inline Float32x2& operator*=(Arg_Float32x2 in_other);
	friend inline Float32x2 operator*(Float32x2 in_a, Arg_Float32x2 in_b);

	// Component wise Div
	inline Float32x2& operator/=(Arg_Float32x2 in_other);
	friend inline Float32x2 operator/(Float32x2 in_a, Arg_Float32x2 in_b);

	// Mul components with float
	inline Float32x2& operator*=(float32 in_other);
	friend inline Float32x2 operator*(Float32x2 in_vec, float32 in_val);
	friend inline Float32x2 operator*(float32 in_val, Arg_Float32x2 in_vec);

	// Div components with float
	inline Float32x2& operator/=(float32 in_other);
	friend inline Float32x2 operator/(Float32x2 in_vec, float32 in_val);
	friend inline Float32x2 operator/(float32 in_val, Arg_Float32x2 in_vec);

	// Calculate the sin and cosin for each component and store the result
	// in `sin` and `cos` respectively
	inline void SinCos(Float32x2& out_sin, Float32x2& out_cos);

	//

	union {
		Type m_internal;
		float32 m_values[2];
		struct {
			float32 x;
			float32 y;
		};
	};
};

static_assert(std::is_trivial_v<Float32x2>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<Float32x2>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "./Float32x2.inl" // IWYU pragma: export
