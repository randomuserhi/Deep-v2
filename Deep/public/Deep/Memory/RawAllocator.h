#pragma once

#include "Deep.h"
#include "Deep/Memory.h"

#include <type_traits> // IWYU pragma: keep
#include <concepts>

DEEP_NAMESPACE_BEGIN

template<typename T, typename Type>
concept c_RawAllocator = requires {
	{ T::s_Malloc } -> std::convertible_to<Type * (*)(size_t)>;
	{ T::s_Free } -> std::convertible_to<void (*)(Type*)>;
};

template<typename T>
struct RawAllocator {
	constexpr static T* (*s_Malloc)(size_t) = Deep::TMalloc<T>;
	constexpr static void (*s_Free)(T*) = Deep::TFree<T>;
};

DEEP_NAMESPACE_END
