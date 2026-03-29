#pragma once

#include <Deep.h>

DEEP_NAMESPACE_BEGIN

Deep_ForceInline void* Malloc(size_t in_size);
Deep_ForceInline void* Realloc(void* in_old, size_t in_size);
Deep_ForceInline void Free(void* in_ptr);

template<typename T>
Deep_ForceInline T* Malloc(size_t in_size);
template<typename T>
Deep_ForceInline T* Realloc(T* in_old, size_t in_size);

Deep_ForceInline void* AlignedMalloc(size_t in_size, size_t in_alignment);
Deep_ForceInline void AlignedFree(void* in_ptr);

Deep_ForceInline void* Memcpy(void* Deep_Restrict in_dest, const void* Deep_Restrict in_src, size_t in_size);

Deep_ForceInline void* Memset(void* in_dest, int32 in_value, size_t in_size);

DEEP_NAMESPACE_END

#include "Memory.inl" // IWYU pragma: export
