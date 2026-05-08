#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

struct ContactInfo {
	// For better packing, a Float3 (12 bytes) is used for the normal rather than a Vec3 (16 bytes)
	// As a result, `m_normal` and `m_penetrationDistance` form a full Vec4 - but can be loaded as a Vec3.
	alignas(Deep_AlignOf(Vec3)) Float3 m_normal;
	float32 m_penetrationDistance;

	// Returns `m_normal` and `m_penetrationDistance` as a packed Vec3
	inline Vec3& normal4();
	inline const Vec3& normal4() const;
};

using Arg_ContactInfo = const ContactInfo&;

DEEP_NAMESPACE_END

#include "Deep/Physics3D/ContactInfo.inl" // IWYU pragma: export
