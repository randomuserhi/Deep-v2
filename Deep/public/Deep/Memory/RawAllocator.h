#pragma once

#include "Deep.h"
#include "Deep/Memory.h"

#include <type_traits> // IWYU pragma: keep

DEEP_NAMESPACE_BEGIN

template<typename T, typename Type>
concept _RawAllocator = requires {
	{ T::Malloc } -> std::convertible_to<Type * (*)(size_t)>;
	{ T::Free } -> std::convertible_to<void (*)(Type*)>;
};

template<typename T>
struct RawAllocator {
	constexpr static T* (*Malloc)(size_t) = Deep::TMalloc<T>;
	constexpr static void (*Free)(T*) = Deep::TFree<T>;
};

DEEP_NAMESPACE_END
