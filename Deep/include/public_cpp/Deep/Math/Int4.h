#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct [[nodiscard]] Int4 {
	//

	Int4() = default;
	Int4(const Int4&) = default;
	Int4& operator=(const Int4&) = default;
	inline Int4(int32 in_x, int32 in_y, int32 in_z, int32 in_w);
	inline Int4(Arg_Vec4i in_vec);

	//

	inline operator Vec4i() const;

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

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

static_assert(std::is_trivial_v<Int4>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<Int4>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "./Int4.inl" // IWYU pragma: export
