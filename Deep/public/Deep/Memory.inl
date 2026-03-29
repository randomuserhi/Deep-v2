#pragma once

#include "Deep.h"

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <cstdlib>
#include <cstring>
DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

void* Malloc(size_t in_size) {
	Deep_Assert(in_size > 0, "Size must be greater than 0.");
	return std::malloc(in_size);
}

void* Realloc(void* in_old, size_t in_size) {
	Deep_Assert(in_size > 0, "Size must be greater than 0.");
	return std::realloc(in_old, in_size);
}

template<typename T>
T* Malloc(size_t in_size) {
	return reinterpret_cast<T*>(Malloc(in_size * sizeof(T)));
}

template<typename T>
T* Realloc(T* in_old, size_t in_size) {
	return reinterpret_cast<T*>(Realloc(in_old, in_size * sizeof(T)));
}

void Free(void* in_ptr) {
	std::free(in_ptr);
}

void* AlignedMalloc(size_t in_size, size_t in_alignment) {
	Deep_Assert(in_size > 0 && in_alignment > 0, "Size and alignment must be greater than 0.");

#ifdef DEEP_PLATFORM_WINDOWS
	return _aligned_malloc(in_size, in_alignment);
#else
	void* block = nullptr;

	DEEP_SUPPRESS_WARNING_PUSH
	DEEP_CLANG_SUPPRESS_WARNING("-Wunused-result")
	posix_memalign(&block, in_alignment, in_size);
	DEEP_SUPPRESS_WARNING_POP

	return block;
#endif
}

void AlignedFree(void* in_ptr) {
#ifdef DEEP_PLATFORM_WINDOWS
	_aligned_free(in_ptr);
#else
	free(in_ptr);
#endif
}

void* Memcpy(void* Deep_Restrict in_dest, const void* Deep_Restrict in_src, size_t in_size) {
	return std::memcpy(in_dest, in_src, in_size);
}

void* Memset(void* in_dest, int32 in_value, size_t in_size) {
	return std::memset(in_dest, in_value, in_size);
}

DEEP_NAMESPACE_END
