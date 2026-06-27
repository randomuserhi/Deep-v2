#pragma once

#include "Deep.h"
#include "Deep/Simd/SimdArgs.h"

#if defined(DEEP_USE_SSE)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#include <immintrin.h>
DEEP_SUPPRESS_WARNINGS_STD_END
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int64)
#elif defined(DEEP_USE_NEON)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
	#ifdef DEEP_COMPILER_MSVC
		#include <intrin.h>
		#include <arm64_neon.h>
	#else
		#include <arm_neon.h>
	#endif
DEEP_SUPPRESS_WARNINGS_STD_END
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int32x2_t)
#else
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for int32x2_t SIMD type representing 2 independent values
// We do not use __m64 for x86_64 systems as its an outdated mmx technology, on x86_64 this degrades to scalar operations
//

struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Int32x2 {
#if defined(DEEP_USE_NEON)
	using Type = int32x2_t;
#else
	using Type = struct {
		int32 m_values[2];
	};
#endif

	//

	constexpr Int32x2() = default;
	constexpr Int32x2(const Int32x2&) = default;
	constexpr Int32x2& operator=(const Int32x2&) = default;
	inline Int32x2(int32 in_x, int32 in_y);
	inline Int32x2(Type in_internal);
	inline Int32x2(Int32x4);

	constexpr static inline Int32x2 Constexpr(int32 in_x, int32 in_y);

	//

	// Implicit conversion
	inline operator Type() const;

	//

	// Convert into float32x2_t, converting each component to a float
	inline Float32x2 ToFloat() const;
	constexpr inline Float32x2 Constexpr_ToFloat() const;

	// Reinterpret bytes as a float32x2_t
	inline Float32x2 ReinterpretAsFloat() const;
	constexpr inline Float32x2 Constexpr_ReinterpretAsFloat() const;

	// Treats the 4 components as booleans where true is when the most significant bit is set
	// and returns it as a bit mask.
	//
	// Bit 0 is set if X is true, Bit 1 is set if Y is true, Bit 2 is set if Z is true and Bit 3 is set if W is true.
	inline int32 ToBooleanBitMask() const;

	// Replicate the given value across all components
	static inline Int32x2 s_Replicate(int32 in_value);

	// Component wise min/max
	static inline Int32x2 s_Min(Arg_Int32x2 in_a, Arg_Int32x2 in_b);
	static inline Int32x2 s_Max(Arg_Int32x2 in_a, Arg_Int32x2 in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Int32x2 s_Equals(Arg_Int32x2 in_a, Arg_Int32x2 in_b);
	constexpr static inline Int32x2 Constexpr_s_Equals(Arg_Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline Int32x2 s_Select(Arg_Int32x2 in_a, Arg_Int32x2 in_b, Arg_Int32x2 in_control);

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Int32x2 in_a, Arg_Int32x2 in_b);
	friend inline bool operator==(Arg_Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise logical shift right
	inline Int32x2& operator>>=(int32 in_count);
	friend inline Int32x2 operator>>(Int32x2 in_a, int32 in_count);

	// Component wise logical shift left
	inline Int32x2& operator<<=(int32 in_count);
	friend inline Int32x2 operator<<(Int32x2 in_a, int32 in_count);

	// Component wise logical OR
	inline Int32x2& operator|=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator|(Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise logical AND
	inline Int32x2& operator&=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator&(Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise logical XOR
	inline Int32x2& operator^=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator^(Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise Add
	inline Int32x2& operator+=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator+(Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise Sub
	inline Int32x2& operator-=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator-(Int32x2 in_a, Arg_Int32x2 in_b);
	friend inline Int32x2 operator-(Arg_Int32x2 in_a);

	// Component wise Mul
	inline Int32x2& operator*=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator*(Int32x2 in_a, Arg_Int32x2 in_b);

	// Component wise Div
	inline Int32x2& operator/=(Arg_Int32x2 in_other);
	friend inline Int32x2 operator/(Int32x2 in_a, Arg_Int32x2 in_b);

	// Mul components with float
	inline Int32x2& operator*=(int32 in_other);
	friend inline Int32x2 operator*(Int32x2 in_vec, int32 in_val);
	friend inline Int32x2 operator*(int32 in_val, Arg_Int32x2 in_vec);

	// Div components with float
	inline Int32x2& operator/=(int32 in_other);
	friend inline Int32x2 operator/(Int32x2 in_vec, int32 in_val);
	friend inline Int32x2 operator/(int32 in_val, Arg_Int32x2 in_vec);

	//

	union {
		Type m_internal;
		int64 m_swar64; // 64 bit register representation for SWAR (simd with a register)
		int32 m_values[2];
		struct {
			int32 x;
			int32 y;
		};
	};
};

static_assert(std::is_trivial<Int32x2>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Int32x2>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "Deep/Simd/Int32x2.inl" // IWYU pragma: export
