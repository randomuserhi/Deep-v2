#pragma once

#include "Deep.h"

DEEP_NAMESPACE_BEGIN

inline int64 Epoch();
Deep_ForceInline uint64 ReadTsc(uint32* out_aux);

DEEP_NAMESPACE_END

#include "./Time.inl" // IWYU pragma: export
