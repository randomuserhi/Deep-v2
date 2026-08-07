#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

struct ContactInfo3D {
	// For better packing, a Float3 (12 bytes) is used for the normal rather than a Vec3 (16 bytes).
	// As a result, `m_normal` and `m_penetrationDistance` form a full Vec4 - and can be loaded as a Vec3.
	alignas(alignof(Vec3)) Float3 m_normal;
	float32 m_penetrationDistance;

	// Returns `m_normal` and `m_penetrationDistance` as a packed Vec3.
	// Writing to this may effect `m_penetrationDistance` which is stored in the `._w` component.
	//
	// Vec3 is implemented with the invariant that `z == _w` so use this only if you know what you are doing!
	//
	// Prefer `m_Normal()` and `m_Normal(Arg_Vec3)` functions which return a copy of the Vec3 and handle storing
	// just XYZ for you.
	inline Vec3& m_Normal4();
	inline const Vec3& m_Normal4() const;

	inline Vec3 m_Normal() const;
	inline void m_Normal(Arg_Vec3 in_normal);
};
DEEP_ASSERT_PACKED_VEC3(ContactInfo3D, m_normal, m_penetrationDistance);

using Arg_ContactInfo3D = const ContactInfo3D&;

DEEP_NAMESPACE_END

#include "./ContactInfo.inl" // IWYU pragma: export
