#pragma once

#include "Deep.h"

#include <concepts>

DEEP_NAMESPACE_BEGIN

template<typename T>
concept _Integer = std::integral<T> && !std::same_as<T, bool>;

DEEP_NAMESPACE_END
