#pragma once

#include "Deep.h"
#include "Deep/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#include <immintrin.h>
DEEP_SUPPRESS_WARNINGS_STD_END
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(__m128i)
#elif defined(DEEP_USE_NEON)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#ifdef DEEP_COMPILER_MSVC
		#include <intrin.h>
		#include <arm64_neon.h>
	#else
		#include <arm_neon.h>
	#endif
DEEP_SUPPRESS_WARNINGS_STD_END
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int64x2_t)
#else
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int64)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for __m128i SIMD type representing 2 independent values
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Int64x2 {
#if defined(DEEP_USE_SSE)
	using Type = __m128i;
#else
	using Type = struct {
		int64 m_values[2];
	};
#endif

	//

	constexpr Int64x2() = default;
	constexpr Int64x2(const Int64x2&) = default;
	constexpr Int64x2& operator=(const Int64x2&) = default;
	inline Int64x2(int64 in_x, int64 in_y);
	constexpr inline Int64x2(Type in_internal);

	constexpr static inline Int64x2 Constexpr(int64 in_x, int64 in_y);

	//

	// Implicit conversion
	inline operator Type() const;

	//

	// Replicate the given value across all components
	static inline Int64x2 Replicate(int64 in_value);

	// Treats the 2 components as booleans where true is when the most significant bit is set
	// and returns it as a bit mask.
	//
	// Bit 0 is set if X is true, Bit 1 is set if Y is true.
	inline int64 ToBooleanBitMask() const;

	// Returns a 2 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int64x2 Equals(Arg_Int64x2 in_a, Arg_Int64x2 in_b);

	//

	Deep_ForceInline constexpr int64& operator[](size_t);
	Deep_ForceInline constexpr const int64& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Int64x2 in_a, Arg_Int64x2 in_b);
	friend inline bool operator==(Arg_Int64x2 in_a, Arg_Int64x2 in_b);

	// Component wise logical shift right
	inline Int64x2& operator>>=(int32 in_count);
	friend inline Int64x2 operator>>(Int64x2 in_a, int32 in_count);

	// Component wise logical shift left
	inline Int64x2& operator<<=(int32 in_count);
	friend inline Int64x2 operator<<(Int64x2 in_a, int32 in_count);

	// Component wise logical OR
	inline Int64x2& operator|=(Arg_Int64x2 in_other);
	friend inline Int64x2 operator|(Arg_Int64x2 in_a, Arg_Int64x2 in_b);

	// Component wise logical AND
	inline Int64x2& operator&=(Arg_Int64x2 in_other);
	friend inline Int64x2 operator&(Arg_Int64x2 in_a, Arg_Int64x2 in_b);

	// Component wise logical XOR
	inline Int64x2& operator^=(Arg_Int64x2 in_other);
	friend inline Int64x2 operator^(Arg_Int64x2 in_a, Arg_Int64x2 in_b);

	//

	union {
		Type _internal;
		int64 m_values[2];
		struct {
			int64 x;
			int64 y;
		};
	};
};

static_assert(std::is_trivial<Int64x2>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Int64x2>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "Deep/Simd/Int64x2.inl" // IWYU pragma: export
