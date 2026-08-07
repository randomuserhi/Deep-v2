#pragma once

#include "Deep.h"
#include "Deep/Math/VecArgs.h" // IWYU pragma: export

#include <type_traits>

DEEP_NAMESPACE_BEGIN

struct [[nodiscard]] Int2 {
	//

	Int2() = default;
	Int2(const Int2&) = default;
	Int2& operator=(const Int2&) = default;
	inline Int2(int32 in_x, int32 in_y);
	explicit inline Int2(Arg_Vec2i in_vec);

	//

	explicit inline operator Vec2i() const;

	//

	Deep_ForceInline constexpr int32& operator[](size_t);
	Deep_ForceInline constexpr const int32& operator[](size_t) const;

	//

	union {
		int32 m_values[2];
		struct {
			int32 x;
			int32 y;
		};
	};
};

static_assert(std::is_trivial_v<Int2>, "Is supposed to be a trivial type!");
static_assert(std::is_standard_layout_v<Int2>, "Is supposed to be standard layout!");

DEEP_NAMESPACE_END

#include "./Int2.inl" // IWYU pragma: export
