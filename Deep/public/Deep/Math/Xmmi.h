#pragma once

#include "Deep.h"

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
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int32x4_t)
#else
	#define DEEP_VEC_ALIGNMENT Deep_AlignOf(int32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct Xmm;
using XmmArg = Xmm;

struct Xmmi;
using XmmiArg = Xmmi;

// Abstraction layer for __m128i SIMD type representing 4 independent values
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Xmmi {
#if defined(DEEP_USE_SSE)
	using Type = __m128i;
#else
	using Type = struct {
		uint32 values[4];
	};
#endif

	//

	constexpr Xmmi() = default;
	constexpr Xmmi(const Xmmi&) = default;
	constexpr Xmmi& operator=(const Xmmi&) = default;
	inline Xmmi(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
	constexpr inline Xmmi(Type in_internal);

	constexpr static inline Xmmi Constexpr(int32 in_x, int32 in_y, int32 in_z, int32 in_w);

	//

	// Implicit conversion
	inline operator Type() const;

	//

	// Convert into __m128, converting each component to a float
	inline Xmm ToFloat() const;
	constexpr inline Xmm Constexpr_ToFloat() const;

	// Reinterpret bytes as a __m128
	inline Xmm ReinterpretAsFloat() const;
	constexpr inline Xmm Constexpr_ReinterpretAsFloat() const;

	// Treats the 4 components as booleans where true is when the most significant bit is set
	// and returns it as a bit mask.
	//
	// Bit 0 is set if X is true, Bit 1 is set if Y is true, Bit 2 is set if Z is true and Bit 3 is set if W is true.
	inline int32 ToBooleanBitMask() const;

	// Replicate the given value across all components
	static inline Xmmi Replicate(int in_value);

	// Component wise logical AND
	static inline Xmmi And(const XmmiArg in_a, const XmmiArg in_b);
	constexpr static inline Xmmi Constexpr_And(const XmmiArg in_a, const XmmiArg in_b);

	// Component wise logical XOR
	static inline Xmmi Xor(const XmmiArg in_a, const XmmiArg in_b);
	constexpr static inline Xmmi Constexpr_Xor(const XmmiArg in_a, const XmmiArg in_b);

	// Component wise min/max
	static inline Xmmi Min(const XmmiArg in_a, const XmmiArg in_b);
	static inline Xmmi Max(const XmmiArg in_a, const XmmiArg in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Xmmi Equals(const XmmiArg in_a, const XmmiArg in_b);
	constexpr static inline Xmmi Constexpr_Equals(const XmmiArg in_a, const XmmiArg in_b);

	// Shift all components by `Count` bits to the left
	template<const uint32 Count>
	inline Xmmi LogicalShiftLeft() const;

	// Shift all components by `Count` bits to the right
	template<const uint32 Count>
	inline Xmmi LogicalShiftRight() const;

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline Xmmi Select(const XmmiArg in_a, const XmmiArg in_b, const XmmiArg in_control);

	//

	// Equality
	friend inline bool operator!=(const XmmiArg in_a, const XmmiArg in_b);
	friend inline bool operator==(const XmmiArg in_a, const XmmiArg in_b);

	// Component wise Add
	inline Xmmi& operator+=(const XmmiArg in_other);
	friend inline Xmmi operator+(Xmmi in_a, const XmmiArg in_b);

	// Component wise Sub
	inline Xmmi& operator-=(const XmmiArg in_other);
	friend inline Xmmi operator-(Xmmi in_a, const XmmiArg in_b);
	friend inline Xmmi operator-(const XmmiArg in_a);

	// Component wise Mul
	inline Xmmi& operator*=(const XmmiArg in_other);
	friend inline Xmmi operator*(Xmmi in_a, const XmmiArg in_b);

	// Component wise Div
	inline Xmmi& operator/=(const XmmiArg in_other);
	friend inline Xmmi operator/(Xmmi in_a, const XmmiArg in_b);

	// Mul components with float
	inline Xmmi& operator*=(int32 in_other);
	friend inline Xmmi operator*(Xmmi in_vec, int32 in_val);
	friend inline Xmmi operator*(int32 in_val, const XmmiArg in_vec);

	// Div components with float
	inline Xmmi& operator/=(int32 in_other);
	friend inline Xmmi operator/(Xmmi in_vec, int32 in_val);
	friend inline Xmmi operator/(int32 in_val, const XmmiArg in_vec);

	//

	union {
		Type _internal;
		int32 values[4];
		struct {
			int32 x;
			int32 y;
			int32 z;
			int32 w;
		};
	};
};

static_assert(std::is_trivial<Xmmi>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Xmmi>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "Deep/Math/Xmmi.inl" // IWYU pragma: export
