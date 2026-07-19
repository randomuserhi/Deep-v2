#pragma once

#include "Deep.h"

// Every allocation request with a valid, non-zero size and valid alignment, returns a non-null suitably aligned pointer,
// throw std::bad_alloc when exceptions are enabled, or abort when exceptions are disabled.
//
// Zero size allocations are asserted against and are treated as implementation defined behaviour. Avoid relying on this
// behaviour.

DEEP_NAMESPACE_BEGIN

Deep_ForceInline void* Malloc(size_t in_size) DEEP_ALLOC_NOEXCEPT;
Deep_ForceInline void* Realloc(void* in_old, size_t in_size) DEEP_ALLOC_NOEXCEPT;
Deep_ForceInline void Free(void* in_ptr) noexcept;

Deep_ForceInline void* Memcpy(void* Deep_Restrict in_dest, const void* Deep_Restrict in_src, size_t in_size) noexcept;
Deep_ForceInline void* Memset(void* in_dest, int32 in_value, size_t in_size) noexcept;

Deep_ForceInline void* AlignedMalloc(size_t in_size, size_t in_alignment) DEEP_ALLOC_NOEXCEPT;
Deep_ForceInline void AlignedFree(void* in_ptr) noexcept;

// Typed version of Malloc, size refers to the number of `T` elements rather than bytes.
// Automatically switches to AlignedMalloc if alignment cannot be managed by Malloc.
template<typename T>
Deep_ForceInline T* TMalloc(size_t in_size) DEEP_ALLOC_NOEXCEPT;

// Typed version of Realloc, size refers to the number of `T` elements rather than bytes.
// Does not support types that require alignment outside of Realloc's range.
template<typename T>
Deep_ForceInline T* TRealloc(T* in_old, size_t in_size) DEEP_ALLOC_NOEXCEPT;

// Typed version of Free, size refers to the number of `T` elements rather than bytes.
// Automatically switches to AlignedFree if alignment cannot be managed by Free.
template<typename T>
Deep_ForceInline void TFree(T* in_ptr) noexcept;

// Typed version of Memcpy, size refers to the number of `T` elements rather than bytes.
template<typename T>
Deep_ForceInline T* TMemcpy(T* Deep_Restrict in_dest, const T* Deep_Restrict in_src, size_t in_size) noexcept;

// Typed version of Memset, size refers to the number of `T` elements rather than bytes.
template<typename T>
Deep_ForceInline T* TMemset(T* in_dest, int32 in_value, size_t in_size) noexcept;

// Determines whether two buffers overlap
template<typename A, typename B = A>
constexpr inline bool DoBuffersOverlap(A* in_a, size_t in_sizeA, B* in_b, size_t in_sizeB) noexcept;

DEEP_NAMESPACE_END

#include "./Memory.inl" // IWYU pragma: export
