#pragma once

#include "Deep.h"
#include "Deep/Physics3D/Ray.h"

DEEP_NAMESPACE_BEGIN

Vec3& RayHit3D::m_Normal4() {
	return *reinterpret_cast<Vec3*>(&m_normal);
}

const Vec3& RayHit3D::m_Normal4() const {
	return *reinterpret_cast<const Vec3*>(&m_normal);
}

Vec3 RayHit3D::m_Normal() const {
	return Vec3::s_FixW(m_Normal4());
}

DEEP_NAMESPACE_END
