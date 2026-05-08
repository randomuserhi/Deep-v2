#pragma once

#include "Deep.h"
#include "Deep/Physics3D/Ray.h"

DEEP_NAMESPACE_BEGIN

Vec3& RayHit3D::normal4() {
	return *reinterpret_cast<Vec3*>(&m_normal);
}

const Vec3& RayHit3D::normal4() const {
	return *reinterpret_cast<const Vec3*>(&m_normal);
}

DEEP_NAMESPACE_END
