#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

struct ContactInfo {
	// For better packing, a Float3 (12 bytes) is used for the normal rather than a Vec3 (16 bytes).
	// As a result, `m_normal` and `m_penetrationDistance` form a full Vec4 - and can be loaded as a Vec3.
	alignas(Deep_AlignOf(Vec3)) Float3 m_normal;
	float32 m_penetrationDistance;

	// Returns `m_normal` and `m_penetrationDistance` as a packed Vec3.
	// NOTE(randomuserhi): Writing to this may effect `m_penetrationDistance` which is stored in the `._w` component, use
	//                     with caution.
	inline Vec3& m_Normal4();
	inline const Vec3& m_Normal4() const;

	inline Vec3 m_Normal() const;
};

using Arg_ContactInfo = const ContactInfo&;

DEEP_NAMESPACE_END

#include "Deep/Physics3D/ContactInfo.inl" // IWYU pragma: export
