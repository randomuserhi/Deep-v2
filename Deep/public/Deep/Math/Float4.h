#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct [[nodiscard]] Float4 {
	//

	Float4() = default;
	Float4(const Float4&) = default;
	Float4& operator=(const Float4&) = default;
	inline Float4(float32 in_x, float32 in_y, float32 in_z, float32 in_w);
	explicit inline Float4(Arg_Vec4 in_vec);

	//

	explicit inline operator Vec4() const;

	//

	union {
		float32 m_values[4];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
	};
};

static_assert(std::is_trivial<Float4>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Float4>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Float4.inl" // IWYU pragma: export
