#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Implementation based on Jolt: https://github.com/jrouwe/JoltPhysics/tree/master/Jolt/Math
struct [[nodiscard]] Int4 {
	//

	Int4() = default;
	Int4(const Int4&) = default;
	Int4& operator=(const Int4&) = default;
	inline Int4(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
	explicit inline Int4(Arg_Vec4i in_vec);

	//

	explicit inline operator Vec4i() const;

	//

	union {
		int32 m_values[4];
		struct {
			int32 x;
			int32 y;
			int32 z;
			int32 w;
		};
	};
};

static_assert(std::is_trivial<Int4>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Int4>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "Deep/Math/Int4.inl" // IWYU pragma: export
