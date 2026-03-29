#pragma once

#include "Deep.h"
#include "Deep/Math/XmmArgs.h"

#if defined(DEEP_USE_SSE)
DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <immintrin.h>
DEEP_SUPPRESS_WARNINGS_STD_END
#define DEEP_VEC_ALIGNMENT Deep_AlignOf(__m128)
#else
#define DEEP_VEC_ALIGNMENT Deep_AlignOf(float32)
#endif

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Abstraction layer for __m128 SIMD type representing 4 independent values
//
// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] alignas(DEEP_VEC_ALIGNMENT) Xmm {
#if defined(DEEP_USE_SSE)
	using Type = __m128;
#else
	using Type = struct {
		float32 values[4];
	};
#endif

	//

	constexpr Xmm() = default;
	constexpr Xmm(const Xmm&) = default;
	constexpr Xmm& operator=(const Xmm&) = default;
	inline Xmm(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
	constexpr inline Xmm(Type in_internal);

	constexpr static inline Xmm Constexpr(float32 in_x, float32 in_y, float32 in_z, float32 in_w);

	//

	// Implicit conversion
	constexpr inline operator Type() const;

	//

	// Convert into __m128i, converting each component to an integer
	inline Xmmi ToInt() const;
	constexpr inline Xmmi Constexpr_ToInt() const;

	// Reinterpret bytes as a __m128i
	inline Xmmi ReinterpretAsInt() const;
	constexpr inline Xmmi Constexpr_ReinterpretAsInt() const;

	// Replicate the given value across all components
	static inline Xmm Replicate(float32 in_value);

	// Component wise logical AND
	static inline Xmm And(const XmmArg in_a, const XmmArg in_b);
	constexpr static inline Xmm Constexpr_And(const XmmArg in_a, const XmmArg in_b);

	// Component wise logical XOR
	static inline Xmm Xor(const XmmArg in_a, const XmmArg in_b);
	constexpr static inline Xmm Constexpr_Xor(const XmmArg in_a, const XmmArg in_b);

	// Component wise min/max
	static inline Xmm Min(const XmmArg in_a, const XmmArg in_b);
	static inline Xmm Max(const XmmArg in_a, const XmmArg in_b);

	// Returns a 4 component integer value representing the equality of each component
	// in `a` or `b`.
	//
	// True is represented by the most significant bit being set.
	static inline Xmmi Equals(const XmmArg in_a, const XmmArg in_b);
	constexpr static inline Xmmi Constexpr_Equals(const XmmArg in_a, const XmmArg in_b);

	// Component wise select, returns `a` when highest bit of `control` = 0 and `b` when highest bit of `control` = 1
	static inline Xmm Select(const XmmArg in_a, const XmmArg in_b, const XmmiArg in_control);

	//

	// Equality
	friend bool operator!=(const XmmArg in_a, const XmmArg in_b);
	friend bool operator==(const XmmArg in_a, const XmmArg in_b);

	// Component wise Add
	inline Xmm& operator+=(const XmmArg roOther);
	friend inline Xmm operator+(Xmm in_a, const XmmArg in_b);

	// Component wise Sub
	inline Xmm& operator-=(const XmmArg in_other);
	friend inline Xmm operator-(Xmm in_a, const XmmArg in_b);
	friend inline Xmm operator-(const XmmArg in_other);

	// Component wise Mul
	inline Xmm& operator*=(const XmmArg in_other);
	friend inline Xmm operator*(Xmm in_a, const XmmArg in_b);

	// Component wise Div
	inline Xmm& operator/=(const XmmArg in_other);
	friend inline Xmm operator/(Xmm in_a, const XmmArg in_b);

	// Mul components with float
	inline Xmm& operator*=(float32 in_other);
	friend inline Xmm operator*(Xmm in_vec, float32 in_val);
	friend inline Xmm operator*(float32 in_val, const XmmArg in_vec);

	// Div components with float
	inline Xmm& operator/=(float32 in_other);
	friend inline Xmm operator/(Xmm in_vec, float32 in_val);
	friend inline Xmm operator/(float32 in_val, const XmmArg in_vec);

	// Calculate the sin and cosin for each component and store the result
	// in `sin` and `cos` respectively
	inline void SinCos(Xmm& out_sin, Xmm& out_cos);

	//

	union {
		Type _internal;
		float32 values[4];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
	};
};

static_assert(std::is_trivial<Xmm>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Xmm>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#undef DEEP_VEC_ALIGNMENT

#include "Deep/Math/Xmm.inl" // IWYU pragma: export
