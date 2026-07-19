#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct [[nodiscard]] Int3 {
	//

	Int3() = default;
	Int3(const Int3&) = default;
	Int3& operator=(const Int3&) = default;
	inline Int3(int32 in_x, int32 in_y, int32 in_z);
	explicit inline Int3(Arg_Vec3i in_vec);

	//

	explicit inline operator Vec3i() const;

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

	//

	union {
		int32 m_values[3];
		struct {
			int32 x;
			int32 y;
			int32 z;
		};
	};
};

static_assert(std::is_trivial<Int3>(), "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout<Int3>(), "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "./Int3.inl" // IWYU pragma: export
