/// ostream helpers for various types in Deep
/// TODO(randomuserhi): Migrate to Deep variants when non-STL implementations are made

#pragma once

#include <ostream>

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

inline std::ostream& operator<<(std::ostream& in_os, const Vec2& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << " }";
}

inline std::ostream& operator<<(std::ostream& in_os, const Vec3& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << ", " << in_vec.z << " }";
}

inline std::ostream& operator<<(std::ostream& in_os, const Vec4& in_vec) {
	return in_os << "{ " << in_vec.x << ", " << in_vec.y << ", " << in_vec.z << ", " << in_vec.w << " }";
}

DEEP_NAMESPACE_END
