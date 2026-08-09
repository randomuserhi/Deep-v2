#pragma once

#include "Deep.h"
#include "Deep/Asm/Intrinsics.h"
#include "Deep/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE2)
	#define DEEP_VEC_ALIGNMENT alignof(int64)
#elif defined(DEEP_USE_NEON)
	#define DEEP_VEC_ALIGNMENT alignof(uint32x2_t)
#elif defined(DEEP_USE_WASM_SIMD128)
	#define DEEP_VEC_ALIGNMENT alignof(int64)
#else
	#define DEEP_VEC_ALIGNMENT alignof(int64)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for UInt32x2_t SIMD type representing 2 independent values
// We do not use __m64 for x86_64 systems as its an outdated mmx technology, on x86_64 this degrades to scalar operations
//

struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) UInt32x2 {
#if defined(DEEP_USE_NEON)
	using Type = uint32x2_t;
#else
	using Type = struct {
		union {
			uint64 m_swar64;
			uint32 m_values[2];
		};
	};
#endif

	//

	constexpr UInt32x2() = default;
	constexpr UInt32x2(const UInt32x2&) = default;
	constexpr UInt32x2& operator=(const UInt32x2&) = default;
	inline UInt32x2(uint32 in_x, uint32 in_y);
	inline UInt32x2(Type in_internal);
	explicit inline UInt32x2(Int32x2);
	inline UInt32x2(UInt32x4);

	constexpr static inline UInt32x2 Constexpr(uint32 in_x, uint32 in_y);

	//

	// Implicit conversion
	inline operator Type() const;

	//

	// Convert into float32x2_t, converting each component to a float
	inline Float32x2 ToFloat() const;

	// Reinterpret bytes as a float32x2_t
	inline Float32x2 ReinterpretAsFloat() const;

	// Treats the 4 components as booleans where true is when the most significant bit is set
	// and returns it as a bit mask.
	//
	// Bit 0 is set if X is true, Bit 1 is set if Y is true, Bit 2 is set if Z is true and Bit 3 is set if W is true.
	inline uint32 ToBooleanBitMask() const;

	// Replicate the given value across all components
	static inline UInt32x2 s_Replicate(uint32 in_value);

	// Component wise min/max
	static inline UInt32x2 s_Min(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b);
	static inline UInt32x2 s_Max(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int32x2 s_Equals(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline UInt32x2 s_Select(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b, Arg_Int32x2 in_control);

	//

	Deep_ForceInline constexpr uint32& operator[](size_t);
	Deep_ForceInline constexpr const uint32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b);
	friend inline bool operator==(Arg_UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise logical shift right
	inline UInt32x2& operator>>=(int32 in_count);
	friend inline UInt32x2 operator>>(UInt32x2 in_a, int32 in_count);

	// Component wise logical shift left
	inline UInt32x2& operator<<=(int32 in_count);
	friend inline UInt32x2 operator<<(UInt32x2 in_a, int32 in_count);

	// Component wise logical OR
	inline UInt32x2& operator|=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator|(UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise logical AND
	inline UInt32x2& operator&=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator&(UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise logical XOR
	inline UInt32x2& operator^=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator^(UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise logical NOT
	friend inline UInt32x2 operator~(UInt32x2 in_value);

	// Component wise Add
	inline UInt32x2& operator+=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator+(UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise Sub
	inline UInt32x2& operator-=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator-(UInt32x2 in_a, Arg_UInt32x2 in_b);
	friend inline UInt32x2 operator-(Arg_UInt32x2 in_a);

	// Component wise Mul
	inline UInt32x2& operator*=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator*(UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Component wise Div
	inline UInt32x2& operator/=(Arg_UInt32x2 in_other);
	friend inline UInt32x2 operator/(UInt32x2 in_a, Arg_UInt32x2 in_b);

	// Mul components with float
	inline UInt32x2& operator*=(uint32 in_other);
	friend inline UInt32x2 operator*(UInt32x2 in_vec, uint32 in_val);
	friend inline UInt32x2 operator*(uint32 in_val, Arg_UInt32x2 in_vec);

	// Div components with float
	inline UInt32x2& operator/=(uint32 in_other);
	friend inline UInt32x2 operator/(UInt32x2 in_vec, uint32 in_val);
	friend inline UInt32x2 operator/(uint32 in_val, Arg_UInt32x2 in_vec);

	//

	union {
		Type m_internal;
		uint64 m_swar64; // 64 bit register representation for SWAR (simd with a register)
		uint32 m_values[2];
		struct {
			uint32 x;
			uint32 y;
		};
	};
};

static_assert(std::is_trivial_v<UInt32x2>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<UInt32x2>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "./UInt32x2.inl" // IWYU pragma: export
