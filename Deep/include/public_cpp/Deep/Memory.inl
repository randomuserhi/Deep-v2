#pragma once

#include "./Memory.h"

#include "Deep.h"
#include "Deep/Bit.h"

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <cstddef>
#include <cstdlib>
#include <cstring>

#if DEEP_CPP_EXCEPTIONS_ENABLED
	#include <new>
#endif
DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

[[noreturn]] Deep_ForceInline void OnAllocationFail() DEEP_ALLOC_NOEXCEPT {
#if DEEP_CPP_EXCEPTIONS_ENABLED
	throw std::bad_alloc{};
#else
	std::abort();
#endif
}

void* Malloc(size_t in_size) DEEP_ALLOC_NOEXCEPT {
	Deep_Assert(in_size > 0, "Allocation size must be greater than zero.");

	void* ptr = std::malloc(in_size);

	if (ptr == nullptr) {
		OnAllocationFail();
	}

	return ptr;
}

void* Realloc(void* in_old, size_t in_size) DEEP_ALLOC_NOEXCEPT {
	Deep_Assert(in_size > 0, "Allocation size must be greater than zero.");

	void* ptr = std::realloc(in_old, in_size);

	if (ptr == nullptr) {
		OnAllocationFail();
	}

	return ptr;
}

void Free(void* in_ptr) noexcept {
	std::free(in_ptr);
}

template<typename T>
T* TMalloc(size_t in_size) DEEP_ALLOC_NOEXCEPT {
	constexpr size_t alignment = alignof(T);
	constexpr size_t defaultAlignment = alignof(std::max_align_t);

	// Prevent integer overflow of `in_size * sizeof(T)`
	if (in_size > std::numeric_limits<size_t>::max() / sizeof(T)) {
		OnAllocationFail();
	}

	if constexpr (alignment > defaultAlignment) {
		return static_cast<T*>(AlignedMalloc(in_size * sizeof(T), alignment));
	} else {
		return static_cast<T*>(Malloc(in_size * sizeof(T)));
	}
}

template<typename T>
T* TRealloc(T* in_old, size_t in_size) DEEP_ALLOC_NOEXCEPT {
	static_assert(alignof(T) <= alignof(std::max_align_t), "Realloc is not supported for the given type 'T'.");

	// Prevent integer overflow of `in_size * sizeof(T)`
	if (in_size > std::numeric_limits<size_t>::max() / sizeof(T)) {
		OnAllocationFail();
	}

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

void* AlignedMalloc(size_t in_size, size_t in_alignment) DEEP_ALLOC_NOEXCEPT {
	Deep_Assert(in_size > 0, "Allocation size must be greater than zero.");
	Deep_Assert(IsPowerOf2(in_alignment), "Alignment must be a power of 2.");

	in_alignment = in_alignment < alignof(void*) ? alignof(void*) : in_alignment;
	void* ptr = nullptr;

#if defined(DEEP_PLATFORM_WINDOWS)
	ptr = _aligned_malloc(in_size, in_alignment);
#elif defined(DEEP_PLATFORM_UNIX) || defined(DEEP_PLATFORM_MAC) || defined(DEEP_PLATFORM_WASM)
	if (posix_memalign(&ptr, in_alignment, in_size) != 0) {
		ptr = nullptr;
	}
#else
	#error Unsupported platform
#endif

	if (ptr == nullptr) {
		OnAllocationFail();
	}

	return ptr;
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
bool DoBuffersOverlap(const A* in_a, size_t in_sizeA, const B* in_b, size_t in_sizeB) noexcept {
	if (in_sizeA == 0 || in_sizeB == 0) return false;

	if (in_a == nullptr || in_b == nullptr) {
		Deep_Assert(false, "Non-empty buffers cannot have null pointers.");
		return true;
	}

	constexpr size_t maxSize = std::numeric_limits<size_t>::max();
	if (in_sizeA > maxSize / sizeof(A) || in_sizeB > maxSize / sizeof(B)) {
		Deep_Assert(false, "Buffer size overflow.");
		return true;
	}

	const size_t sizeA = in_sizeA * sizeof(A);
	const size_t sizeB = in_sizeB * sizeof(B);

	const uintptr_t addressA = reinterpret_cast<uintptr_t>(in_a);
	const uintptr_t addressB = reinterpret_cast<uintptr_t>(in_b);

	if (addressA <= addressB) {
		return addressB - addressA < sizeA;
	}

	return addressA - addressB < sizeB;
}

DEEP_NAMESPACE_END
