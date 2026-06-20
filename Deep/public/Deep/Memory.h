#pragma once

#include "Deep.h"

DEEP_NAMESPACE_BEGIN

Deep_ForceInline void* Malloc(size_t in_size);
Deep_ForceInline void* Realloc(void* in_old, size_t in_size);
Deep_ForceInline void Free(void* in_ptr);

Deep_ForceInline void* Memcpy(void* Deep_Restrict in_dest, const void* Deep_Restrict in_src, size_t in_size);
Deep_ForceInline void* Memset(void* in_dest, int32 in_value, size_t in_size);

template<typename T>
Deep_ForceInline T* TMalloc(size_t in_size);
template<typename T>
Deep_ForceInline T* TRealloc(T* in_old, size_t in_size);
template<typename T>
Deep_ForceInline void TFree(T* in_ptr);

template<typename T>
Deep_ForceInline T* TMemcpy(T* Deep_Restrict in_dest, const T* Deep_Restrict in_src, size_t in_size);
template<typename T>
Deep_ForceInline T* TMemset(T* in_dest, int32 in_value, size_t in_size);

// Determines whether two buffers overlap
template<typename A, typename B = A>
constexpr inline bool DoBuffersOverlap(A* in_a, size_t in_sizeA, B* in_b, size_t in_sizeB);

Deep_ForceInline void* AlignedMalloc(size_t in_size, size_t in_alignment);
Deep_ForceInline void AlignedFree(void* in_ptr);

DEEP_NAMESPACE_END

#include "Memory.inl" // IWYU pragma: export
