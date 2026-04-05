#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct DEEP_EXPORT [[nodiscard]] Float3 {
	//

	Float3() = default;
	Float3(const Float3&) = default;
	Float3& operator=(const Float3&) = default;
	inline Float3(float32 in_x, float32 in_y, float32 in_z);
	explicit inline Float3(Arg_Vec3 in_vec);

	//

	explicit inline operator Vec3() const;

	//

	union {
		float32 m_values[3];
		struct {
			float32 x;
			float32 y;
			float32 z;
		};
	};
};

static_assert(std::is_trivial<Float3>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Float3>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Float3.inl" // IWYU pragma: export
