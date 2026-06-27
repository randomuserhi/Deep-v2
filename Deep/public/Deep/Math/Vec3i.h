#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Simd.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Actually represents storage of 4 values (x, y, z, _w)
// On initialization, `_w = z`
// On division, `_w` must match `z` to prevent divide by zero errors. The method `s_FixW` can be used to enforce this.
// Apart from division or initialization, the state of `_w` is undefined.
struct [[nodiscard]] alignas(Int32x4) Vec3i {
	//

	Vec3i() = default;
	Vec3i(const Vec3i&) = default;
	Vec3i& operator=(const Vec3i&) = default;
	inline Vec3i(int32 in_x, int32 in_y, int32 in_z);
	explicit inline Vec3i(Int32x4 in_xmmi);
	explicit inline Vec3i(Arg_Vec4i in_vec);

	//

	inline operator Int32x4() const;
	explicit inline operator Vec3() const;

	//

	[[nodiscard]] inline int32 m_ManhattanDistance() const;

	[[nodiscard]] inline float32 m_SqrdMagnitude() const;
	[[nodiscard]] inline float32 m_Magnitude() const;

	[[nodiscard]] static inline int32 s_Dot(Arg_Vec3i in_a, Arg_Vec3i in_b);

	static inline Vec3i s_FixW(Arg_Vec3i in_vec);
	static Deep_ForceInline void s_CheckW(Arg_Vec3i in_vec);

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

	// Copy only the xyz components, retaining the current _w value
	inline void XYZ(Arg_Vec3i in_vec);

	// Equality
	friend inline bool operator!=(Arg_Vec3i in_a, Arg_Vec3i in_b);
	friend inline bool operator==(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Add vectors
	inline Vec3i& operator+=(Arg_Vec3i in_other);
	friend inline Vec3i operator+(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Sub vectors
	inline Vec3i& operator-=(Arg_Vec3i in_other);
	friend inline Vec3i operator-(Arg_Vec3i in_a, Arg_Vec3i in_b);
	friend inline Vec3i operator-(Arg_Vec3i in_a);

	// Mul vectors
	inline Vec3i& operator*=(Arg_Vec3i in_other);
	friend inline Vec3i operator*(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Div vectors
	inline Vec3i& operator/=(Arg_Vec3i in_other);
	friend inline Vec3i operator/(Arg_Vec3i in_a, Arg_Vec3i in_b);

	// Mul vector and float
	inline Vec3i& operator*=(int32 in_other);
	friend inline Vec3i operator*(Arg_Vec3i in_vec, int32 in_val);
	friend inline Vec3i operator*(int32 in_val, Arg_Vec3i in_vec);

	// Div vector and float
	inline Vec3i& operator/=(int32 other);
	friend inline Vec3i operator/(Arg_Vec3i in_vec, int32 in_val);
	friend inline Vec3i operator/(int32 in_val, Arg_Vec3i in_vec);

	//

	// NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
	union {
		Float32x4 m_float32x4;
		Int32x4 m_int32x4;
		int32 m_values[4];
		struct {
			int32 x;
			int32 y;
			int32 z;
			int32 _w;
		};
	};

	DEEP_EXPORT static const Vec3i k_zero;

	DEEP_EXPORT static const Vec3i k_up;
	DEEP_EXPORT static const Vec3i k_down;
	DEEP_EXPORT static const Vec3i k_left;
	DEEP_EXPORT static const Vec3i k_right;
	DEEP_EXPORT static const Vec3i k_forwards;
	DEEP_EXPORT static const Vec3i k_backwards;
};

static_assert(std::is_trivial<Vec3i>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec3i>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Vec3i.inl" // IWYU pragma: export
