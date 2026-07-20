#pragma once

#include "Deep.h"
#include "Deep/Math.h"
#include "Deep/Math/VecArgs.h"

#if !defined(DEEP_RAYCAST_DEFAULT_TYPE)
	#define DEEP_RAYCAST_DEFAULT_TYPE e_startsOutside
#endif

DEEP_NAMESPACE_BEGIN

struct Ray3D {
	Vec3 m_origin;
	Vec3 m_direction;
};

using Arg_Ray3D = const Ray3D&;

struct RayHit3D {
	Vec3 m_point;

	// For better packing, a Float3 (12 bytes) is used for the normal rather than a Vec3 (16 bytes).
	// As a result, `m_normal` and `m_distance` form a full Vec4 - and can be loaded as a Vec3.
	alignas(alignof(Vec3)) Float3 m_normal;
	float32 m_distance;

	// Returns `m_normal` and `m_distance` as a packed Vec3.
	// Writing to this may effect `m_distance` which is stored in the `._w` component.
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
DEEP_ASSERT_PACKED_VEC3(RayHit3D, m_normal, m_distance);

using Arg_RayHit3D = const RayHit3D&;

enum class RaycastType {
	e_startsInside,  // Rays will collide with colliders they start inside of
	e_startsOutside, // Rays will not collide with colliders they start inside of
	e_default = DEEP_RAYCAST_DEFAULT_TYPE
};

DEEP_NAMESPACE_END

#include "./Ray.inl" // IWYU pragma: export
