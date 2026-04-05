#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] Float2 {
	//

	Float2() = default;
	Float2(const Float2&) = default;
	Float2& operator=(const Float2&) = default;
	inline Float2(float32 in_x, float32 in_y);
	explicit inline Float2(Arg_Vec2 in_vec);

	//

	explicit inline operator Vec2() const;

	//

	union {
		float32 m_values[2];
		struct {
			float32 x;
			float32 y;
		};
	};
};

static_assert(std::is_trivial<Float2>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Float2>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Float2.inl" // IWYU pragma: export
