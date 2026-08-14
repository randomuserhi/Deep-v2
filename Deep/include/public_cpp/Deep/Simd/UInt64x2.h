#pragma once

#include "Deep.h"
#include "Deep/Asm/Intrinsics.h"
#include "Deep/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE2)
	#define DEEP_VEC_ALIGNMENT alignof(__m128i)
#elif defined(DEEP_USE_NEON)
	#define DEEP_VEC_ALIGNMENT alignof(uint64x2_t)
#elif defined(DEEP_USE_WASM_SIMD128)
	#define DEEP_VEC_ALIGNMENT alignof(v128_t)
#else
	#define DEEP_VEC_ALIGNMENT alignof(int64)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for __m128i SIMD type representing 2 independent values

struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) UInt64x2 {
#if defined(DEEP_USE_SSE2)
	using Type = __m128i;
#elif defined(DEEP_USE_NEON)
	using Type = uint64x2_t;
#elif defined(DEEP_USE_WASM_SIMD128)
	using Type = v128_t;
#else
	using Type = struct {
		uint64 m_values[2];
	};
#endif

	//

	constexpr UInt64x2() = default;
	constexpr UInt64x2(const UInt64x2&) = default;
	constexpr UInt64x2& operator=(const UInt64x2&) = default;
	inline UInt64x2(uint64 in_x, uint64 in_y);
	inline UInt64x2(Type in_internal);
	explicit inline UInt64x2(Int64x2);

	constexpr static inline UInt64x2 Constexpr(uint64 in_x, uint64 in_y);

	//

	// Implicit conversion
	inline operator Type() const;

	//

	// Replicate the given value across all components
	static inline UInt64x2 s_Replicate(uint64 in_value);

	// Treats the 2 components as booleans where true is when the most significant bit is set
	// and returns it as a bit mask.
	//
	// Bit 0 is set if X is true, Bit 1 is set if Y is true.
	inline uint32 ToBooleanBitMask() const;

	// Returns a 2 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int64x2 s_Equals(Arg_UInt64x2 in_a, Arg_UInt64x2 in_b);

	template<uint32 in_x, uint32 in_y>
	static inline UInt64x2 s_Shuffle(Arg_UInt64x2 in_value);

	//

	Deep_ForceInline constexpr uint64& operator[](size_t);
	Deep_ForceInline constexpr const uint64& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_UInt64x2 in_a, Arg_UInt64x2 in_b);
	friend inline bool operator==(Arg_UInt64x2 in_a, Arg_UInt64x2 in_b);

	// Component wise logical shift right
	inline UInt64x2& operator>>=(int32 in_count);
	friend inline UInt64x2 operator>>(UInt64x2 in_a, int32 in_count);

	// Component wise logical shift left
	inline UInt64x2& operator<<=(int32 in_count);
	friend inline UInt64x2 operator<<(UInt64x2 in_a, int32 in_count);

	// Component wise logical OR
	inline UInt64x2& operator|=(Arg_UInt64x2 in_other);
	friend inline UInt64x2 operator|(Arg_UInt64x2 in_a, Arg_UInt64x2 in_b);

	// Component wise logical AND
	inline UInt64x2& operator&=(Arg_UInt64x2 in_other);
	friend inline UInt64x2 operator&(Arg_UInt64x2 in_a, Arg_UInt64x2 in_b);

	// Component wise logical XOR
	inline UInt64x2& operator^=(Arg_UInt64x2 in_other);
	friend inline UInt64x2 operator^(Arg_UInt64x2 in_a, Arg_UInt64x2 in_b);

	// Component wise logical NOT
	friend inline UInt64x2 operator~(UInt64x2 in_value);

	//

	union {
		Type m_internal;
		uint64 m_values[2];
		struct {
			uint64 x;
			uint64 y;
		};
	};
};

static_assert(std::is_trivial_v<UInt64x2>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<UInt64x2>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "./UInt64x2.inl" // IWYU pragma: export
