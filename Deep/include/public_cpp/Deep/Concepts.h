#pragma once

#include "Deep.h"

#include <concepts>

DEEP_NAMESPACE_BEGIN

template<typename T>
concept c_Integer = std::integral<T> && !std::same_as<T, bool>;

template<typename T>
concept c_UnsignedInteger = std::unsigned_integral<T>;

DEEP_NAMESPACE_END
