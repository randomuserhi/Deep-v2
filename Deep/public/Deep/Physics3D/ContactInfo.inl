#pragma once

#include "./ContactInfo.h"

DEEP_NAMESPACE_BEGIN

Vec3& ContactInfo::m_Normal4() {
	return *reinterpret_cast<Vec3*>(&m_normal);
}

const Vec3& ContactInfo::m_Normal4() const {
	return *reinterpret_cast<const Vec3*>(&m_normal);
}

Vec3 ContactInfo::m_Normal() const {
	return Vec3::s_FixW(m_Normal4());
}

void ContactInfo::m_Normal(Arg_Vec3 in_normal) {
	m_Normal4().XYZ(in_normal);
}

DEEP_NAMESPACE_END
