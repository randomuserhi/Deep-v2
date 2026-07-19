#pragma once

#include "Deep.h"

#include <type_traits> // IWYU pragma: keep
#include <concepts>

DEEP_NAMESPACE_BEGIN

template<typename T, typename Type>
concept c_RawAllocator = requires(Type* in_ptr, size_t in_size) {
	{ T::s_Malloc(in_size) } -> std::same_as<Type*>;
	{ T::s_Free(in_ptr) } noexcept;
};

template<typename T>
struct RawAllocator {
	static Deep_ForceInline T* s_Malloc(size_t in_size);
	static Deep_ForceInline void s_Free(T* in_ptr) noexcept;
};

DEEP_NAMESPACE_END

#include "./RawAllocator.inl" // IWYU pragma: export
