#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

struct ContactInfo2D {
	Vec2 m_normal;
	float32 m_penetrationDistance;
};

using Arg_ContactInfo2D = const ContactInfo2D&;

DEEP_NAMESPACE_END

#include "./ContactInfo.inl" // IWYU pragma: export
