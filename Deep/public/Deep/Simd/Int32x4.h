#pragma once

#include "Deep.h"
#include "Deep/Asm/Intrinsics.h"
#include "Deep/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE)
	#define DEEP_VEC_ALIGNMENT alignof(__m128i)
#elif defined(DEEP_USE_NEON)
	#define DEEP_VEC_ALIGNMENT alignof(int32x4_t)
#elif defined(DEEP_USE_WASM_SIMD128)
	#define DEEP_VEC_ALIGNMENT alignof(v128_t)
#else
	#define DEEP_VEC_ALIGNMENT alignof(int32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for __m128i SIMD type representing 4 independent values
//

struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Int32x4 {
#if defined(DEEP_USE_SSE)
	using Type = __m128i;
#elif defined(DEEP_USE_NEON)
	using Type = int32x4_t;
#elif defined(DEEP_USE_WASM_SIMD128)
	using Type = v128_t;
#else
	using Type = struct {
		int32 m_values[4];
	};
#endif

	//

	constexpr Int32x4() = default;
	constexpr Int32x4(const Int32x4&) = default;
	constexpr Int32x4& operator=(const Int32x4&) = default;
	inline Int32x4(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
	inline Int32x4(Type in_internal);
	inline Int32x4(Int32x2 in_low);
	inline Int32x4(Int32x2 in_low, Int32x2 in_high);

	constexpr static inline Int32x4 Constexpr(int32 in_x, int32 in_y, int32 in_z, int32 in_w);

	//

	// Implicit conversion
	inline operator Type() const;

	//

	// Convert into __m128, converting each component to a float
	inline Float32x4 ToFloat() const;
	constexpr inline Float32x4 Constexpr_ToFloat() const;

	// Reinterpret bytes as a __m128
	inline Float32x4 ReinterpretAsFloat() const;
	constexpr inline Float32x4 Constexpr_ReinterpretAsFloat() const;

	// Treats the 4 components as booleans where true is when the most significant bit is set
	// and returns it as a bit mask.
	//
	// Bit 0 is set if X is true, Bit 1 is set if Y is true, Bit 2 is set if Z is true and Bit 3 is set if W is true.
	inline int32 ToBooleanBitMask() const;

	// Replicate the given value across all components
	static inline Int32x4 s_Replicate(int32 in_value);

	// Component wise min/max
	static inline Int32x4 s_Min(Arg_Int32x4 in_a, Arg_Int32x4 in_b);
	static inline Int32x4 s_Max(Arg_Int32x4 in_a, Arg_Int32x4 in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int32x4 s_Equals(Arg_Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline Int32x4 s_Select(Arg_Int32x4 in_a, Arg_Int32x4 in_b, Arg_Int32x4 in_control);

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Int32x4 in_a, Arg_Int32x4 in_b);
	friend inline bool operator==(Arg_Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise logical shift right
	inline Int32x4& operator>>=(int32 in_count);
	friend inline Int32x4 operator>>(Int32x4 in_a, int32 in_count);

	// Component wise logical shift left
	inline Int32x4& operator<<=(int32 in_count);
	friend inline Int32x4 operator<<(Int32x4 in_a, int32 in_count);

	// Component wise logical OR
	inline Int32x4& operator|=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator|(Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise logical AND
	inline Int32x4& operator&=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator&(Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise logical XOR
	inline Int32x4& operator^=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator^(Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise Add
	inline Int32x4& operator+=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator+(Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise Sub
	inline Int32x4& operator-=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator-(Int32x4 in_a, Arg_Int32x4 in_b);
	friend inline Int32x4 operator-(Arg_Int32x4 in_a);

	// Component wise Mul
	inline Int32x4& operator*=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator*(Int32x4 in_a, Arg_Int32x4 in_b);

	// Component wise Div
	inline Int32x4& operator/=(Arg_Int32x4 in_other);
	friend inline Int32x4 operator/(Int32x4 in_a, Arg_Int32x4 in_b);

	// Mul components with float
	inline Int32x4& operator*=(int32 in_other);
	friend inline Int32x4 operator*(Int32x4 in_vec, int32 in_val);
	friend inline Int32x4 operator*(int32 in_val, Arg_Int32x4 in_vec);

	// Div components with float
	inline Int32x4& operator/=(int32 in_other);
	friend inline Int32x4 operator/(Int32x4 in_vec, int32 in_val);
	friend inline Int32x4 operator/(int32 in_val, Arg_Int32x4 in_vec);

	//

	union {
		Type m_internal;
		int32 m_values[4];
		struct {
			int32 x;
			int32 y;
			int32 z;
			int32 w;
		};
	};
};

static_assert(std::is_trivial_v<Int32x4>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<Int32x4>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "./Int32x4.inl" // IWYU pragma: export
