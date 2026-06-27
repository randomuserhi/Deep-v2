#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export
#include "Deep/Simd.h"

#include <type_traits>
#include <ostream>

DEEP_NAMESPACE_BEGIN

struct [[nodiscard]] alignas(Float32x2) Vec2 {
	//

	Vec2() = default;
	Vec2(const Vec2&) = default;
	Vec2& operator=(const Vec2&) = default;
	inline Vec2(float32 in_x, float32 in_y);
	explicit inline Vec2(Float32x2 in_xmmi);
	explicit inline Vec2(Arg_Vec3 in_vec);
	explicit inline Vec2(Arg_Vec4 in_vec);

	//

	inline operator Float32x2() const;
	explicit inline operator Vec2i() const;

	//

	inline Vec2& Normalize();
	[[nodiscard]] inline Vec2 m_Normalized() const;
	[[nodiscard]] inline bool IsNormalized(float in_tolerance = 1.0e-6f) const;

	[[nodiscard]] inline float32 m_SqrdMagnitude() const;
	[[nodiscard]] inline float32 m_Magnitude() const;

	[[nodiscard]] static inline float32 s_Dot(Arg_Vec2 in_a, Arg_Vec2 in_b);
	[[nodiscard]] static inline float32 s_Cross(Arg_Vec2 in_a, Arg_Vec2 in_b);

	[[nodiscard]] static inline Vec2 s_Rotate(Arg_Vec2 in_a, float in_radians);
	[[nodiscard]] static inline float32 s_Angle(Arg_Vec2 in_a, Arg_Vec2 in_b);

	[[nodiscard]] static inline Vec2 s_Lerp(Arg_Vec2 in_a, Arg_Vec2 in_b, float32 in_t);

	//

	Deep_ForceInline constexpr float32& operator[](size_t);
	Deep_ForceInline constexpr const float32& operator[](size_t) const;

	// Equality
	friend inline bool operator!=(Arg_Vec2 in_a, Arg_Vec2 in_b);
	friend inline bool operator==(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Add vectors
	inline Vec2& operator+=(Arg_Vec2 in_other);
	friend inline Vec2 operator+(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Sub vectors
	inline Vec2& operator-=(Arg_Vec2 in_other);
	friend inline Vec2 operator-(Arg_Vec2 in_a, Arg_Vec2 in_b);
	friend inline Vec2 operator-(Arg_Vec2 in_a);

	// Mul vectors
	inline Vec2& operator*=(Arg_Vec2 in_other);
	friend inline Vec2 operator*(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Div vectors
	inline Vec2& operator/=(Arg_Vec2 in_other);
	friend inline Vec2 operator/(Arg_Vec2 in_a, Arg_Vec2 in_b);

	// Mul vector and float
	inline Vec2& operator*=(float32 in_other);
	friend inline Vec2 operator*(Arg_Vec2 in_vec, float32 in_val);
	friend inline Vec2 operator*(float32 in_val, Arg_Vec2 in_vec);

	// Div vector and float
	inline Vec2& operator/=(float32 other);
	friend inline Vec2 operator/(Arg_Vec2 in_vec, float32 in_val);
	friend inline Vec2 operator/(float32 in_val, Arg_Vec2 in_vec);

	// Apply a quaternion to a vector
	friend inline Vec2 operator*(Arg_Quat in_quat, Arg_Vec2 in_vec);

	//

	// NOTE(randomuserhi): The underlying type is a Vec4 for vectorisation
	union {
		Float32x2 m_float32x2;
		float32 m_values[2];
		struct {
			float32 x;
			float32 y;
		};
	};

	DEEP_EXPORT static const Vec2 k_zero;
	DEEP_EXPORT static const Vec2 k_one;

	DEEP_EXPORT static const Vec2 k_up;
	DEEP_EXPORT static const Vec2 k_down;
	DEEP_EXPORT static const Vec2 k_left;
	DEEP_EXPORT static const Vec2 k_right;
};

static_assert(std::is_trivial<Vec2>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Vec2>(), "Is supposed to be standard layout!");

inline std::ostream& operator<<(std::ostream& in_os, const Vec2& in_vec);

DEEP_NAMESPACE_END

#include "Deep/Math/Vec2.inl" // IWYU pragma: export
