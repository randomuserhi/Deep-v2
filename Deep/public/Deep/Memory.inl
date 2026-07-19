#pragma once

#include "Deep.h"
#include "Deep/Memory.h"

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <cstddef>
#include <cstdlib>
#include <cstring>
DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

void* Malloc(size_t in_size) noexcept {
	Deep_Assert(in_size > 0, "Size must be greater than 0.");
	return std::malloc(in_size);
}

void* Realloc(void* in_old, size_t in_size) noexcept {
	Deep_Assert(in_size > 0, "Size must be greater than 0.");
	return std::realloc(in_old, in_size);
}

void Free(void* in_ptr) noexcept {
	std::free(in_ptr);
}

template<typename T>
T* TMalloc(size_t in_size) noexcept {
	constexpr size_t alignment = alignof(T);
	constexpr size_t defaultAlignment = alignof(std::max_align_t);

	void* ptr;
	if constexpr (alignment > defaultAlignment) {
		ptr = AlignedMalloc(in_size * sizeof(T), alignment);
	} else {
		ptr = Malloc(in_size * sizeof(T));
	}

	return static_cast<T*>(ptr);
}

template<typename T>
T* TRealloc(T* in_old, size_t in_size) noexcept {
	static_assert(alignof(T) <= alignof(std::max_align_t), "Realloc is not supported for the given type 'T'.");
	return static_cast<T*>(Realloc(in_old, in_size * sizeof(T)));
}

template<typename T>
void TFree(T* in_ptr) noexcept {
	constexpr size_t alignment = alignof(T);
	constexpr size_t defaultAlignment = alignof(std::max_align_t);

	if constexpr (alignment > defaultAlignment) {
		AlignedFree(in_ptr);
	} else {
		Free(static_cast<void*>(in_ptr));
	}
}

void* AlignedMalloc(size_t in_size, size_t in_alignment) noexcept {
	Deep_Assert(in_size > 0 && in_alignment > 0, "Size and alignment must be greater than 0.");

#ifdef DEEP_PLATFORM_WINDOWS
	return _aligned_malloc(in_size, in_alignment);
#else
	void* block = nullptr;
	const int32 result = posix_memalign(&block, in_alignment, in_size);
	return result == 0 ? block : nullptr;
#endif
}

void AlignedFree(void* in_ptr) noexcept {
#ifdef DEEP_PLATFORM_WINDOWS
	_aligned_free(in_ptr);
#else
	free(in_ptr);
#endif
}

void* Memcpy(void* Deep_Restrict in_dest, const void* Deep_Restrict in_src, size_t in_size) noexcept {
	return std::memcpy(in_dest, in_src, in_size);
}

void* Memset(void* in_dest, int32 in_value, size_t in_size) noexcept {
	return std::memset(in_dest, in_value, in_size);
}

template<typename T>
T* TMemcpy(T* Deep_Restrict in_dest, const T* Deep_Restrict in_src, size_t in_size) noexcept {
	return static_cast<T*>(std::memcpy(in_dest, in_src, in_size * sizeof(T)));
}

template<typename T>
T* TMemset(T* in_dest, int32 in_value, size_t in_size) noexcept {
	return static_cast<T*>(std::memset(in_dest, in_value, in_size * sizeof(T)));
}

template<typename A, typename B>
constexpr bool DoBuffersOverlap(A* in_a, size_t in_sizeA, B* in_b, size_t in_sizeB) noexcept {
	if (in_a == nullptr || in_b == nullptr) return false;
	if (in_sizeA == 0 || in_sizeB == 0) return false;

	byte* a_begin = reinterpret_cast<byte*>(in_a);
	byte* b_begin = reinterpret_cast<byte*>(in_b);

	size_t a_bytes = in_sizeA * sizeof(A);
	size_t b_bytes = in_sizeB * sizeof(B);

	byte* a_end = a_begin + a_bytes;
	byte* b_end = b_begin + b_bytes;

	return (a_begin < b_end) && (b_begin < a_end);
}

DEEP_NAMESPACE_END
