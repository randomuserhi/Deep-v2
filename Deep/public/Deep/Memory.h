#pragma once

#include "Deep.h"

#include <type_traits> // IWYU pragma: keep

DEEP_NAMESPACE_BEGIN

enum class Ownership {
	Borrowed,
	Owned,
};

Deep_ForceInline void* Malloc(size_t in_size);
Deep_ForceInline void* Realloc(void* in_old, size_t in_size);
Deep_ForceInline void Free(void* in_ptr);

template<typename T>
Deep_ForceInline T* Malloc(size_t in_size);
template<typename T>
Deep_ForceInline T* Realloc(T* in_old, size_t in_size);
template<typename T>
Deep_ForceInline void Free(T* in_ptr);

Deep_ForceInline void* AlignedMalloc(size_t in_size, size_t in_alignment);
Deep_ForceInline void AlignedFree(void* in_ptr);

Deep_ForceInline void* Memcpy(void* Deep_Restrict in_dest, const void* Deep_Restrict in_src, size_t in_size);
Deep_ForceInline void* Memset(void* in_dest, int32 in_value, size_t in_size);

template<typename T>
Deep_ForceInline T* Memcpy(T* Deep_Restrict in_dest, const T* Deep_Restrict in_src, size_t in_size);
template<typename T>
Deep_ForceInline T* Memset(T* in_dest, int32 in_value, size_t in_size);

template<typename A, typename B = A>
constexpr inline bool DoBuffersOverlap(A* in_a, size_t in_sizeA, B* in_b, size_t in_sizeB);

template<typename T, typename Type>
concept _RawAllocator = requires {
	{ T::Malloc } -> std::convertible_to<Type * (*)(size_t)>;
	{ T::Free } -> std::convertible_to<void (*)(Type*)>;
};

template<typename T>
struct RawAllocator {
	constexpr static T* (*Malloc)(size_t) = Deep::Malloc<T>;
	constexpr static void (*Free)(T*) = Deep::Free<T>;
};

DEEP_NAMESPACE_END

#include "Memory.inl" // IWYU pragma: export
