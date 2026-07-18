#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

struct ContactInfo {
	Vec2 m_normal;
	float32 m_penetrationDistance;
};

using Arg_ContactInfo = const ContactInfo&;

DEEP_NAMESPACE_END

#include "Deep/Physics2D/ContactInfo.inl" // IWYU pragma: export
