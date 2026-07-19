#pragma once

#include "./RawAllocator.h"

#include "Deep/Memory.h"

DEEP_NAMESPACE_BEGIN

template<typename T>
T* RawAllocator<T>::s_Malloc(size_t in_size) {
	return Deep::TMalloc<T>(in_size);
}

template<typename T>
void RawAllocator<T>::s_Free(T* in_ptr) noexcept {
	return Deep::TFree<T>(in_ptr);
}

DEEP_NAMESPACE_END
