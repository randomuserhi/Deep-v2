#pragma once

#include "./MemoryBlock.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::MemoryBlock() noexcept :
	m_ptr{ nullptr }, m_size{ 0 } {}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::MemoryBlock(size_t in_size) noexcept(noexcept(in_allocator::s_Malloc(std::declval<size_t>()))
                                                                   && std::is_nothrow_default_constructible_v<T>) :
	m_size{ in_size } {
	if (m_size == 0) {
		m_ptr = nullptr;
		return;
	}

	m_ptr = in_allocator::s_Malloc(m_size);
#if DEEP_CPP_EXCEPTIONS_ENABLED
	try {
		new (m_ptr) T[m_size];
	} catch (...) {
		in_allocator::s_Free(m_ptr);
		m_ptr = nullptr;
		m_size = 0;
		throw;
	}
#else
	new (m_ptr) T[m_size];
#endif
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::MemoryBlock(T* in_ptr, size_t in_size) noexcept :
	m_ptr{ in_ptr }, m_size{ in_size } {}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::MemoryBlock(const MemoryBlock& in_other) noexcept(
	noexcept(in_allocator::s_Malloc(std::declval<size_t>())) && std::is_nothrow_copy_constructible_v<T>) :
	m_size{ in_other.m_size } {
	if (m_size == 0) {
		m_ptr = nullptr;
		return;
	}

	m_ptr = in_allocator::s_Malloc(m_size);
	if constexpr (std::is_trivially_copyable_v<T>) {
		TMemcpy<T>(m_ptr, in_other.m_ptr, m_size);
	} else {
#if DEEP_CPP_EXCEPTIONS_ENABLED
		if constexpr (std::is_nothrow_copy_constructible_v<T>) {
			for (size_t i = 0; i < m_size; ++i) {
				new (m_ptr + i) T{ in_other[i] };
			}
		} else {
			size_t constructed = 0;
			try {
				for (; constructed < m_size; ++constructed) {
					new (m_ptr + constructed) T{ in_other[constructed] };
				}
			} catch (...) {
				while (constructed > 0) {
					m_ptr[--constructed].~T();
				}
				in_allocator::s_Free(m_ptr);
				m_ptr = nullptr;
				m_size = 0;
				throw;
			}
		}
#else
		for (size_t i = 0; i < m_size; ++i) {
			new (m_ptr + i) T{ in_other[i] };
		}
#endif
	}
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::MemoryBlock(MemoryBlock&& in_other) noexcept :
	m_ptr{ in_other.m_ptr }, m_size{ in_other.m_size } {
	in_other.m_ptr = nullptr;
	in_other.m_size = 0;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::operator T*() {
	return m_ptr;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::operator const T*() const {
	return m_ptr;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>& MemoryBlock<T, in_allocator>::operator=(MemoryBlock&& in_other) noexcept {
	if (this == &in_other) {
		return *this;
	}

	Deallocate();

	m_ptr = in_other.m_ptr;
	m_size = in_other.m_size;

	in_other.m_ptr = nullptr;
	in_other.m_size = 0;

	return *this;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>& MemoryBlock<T, in_allocator>::operator=(const MemoryBlock& in_other) noexcept(
	noexcept(in_allocator::s_Malloc(std::declval<size_t>())) && std::is_nothrow_copy_constructible_v<T>) {
	if (this == &in_other) {
		return *this;
	}

#if DEEP_CPP_EXCEPTIONS_ENABLED
	MemoryBlock temp{ in_other };
	std::swap(m_ptr, temp.m_ptr);
	std::swap(m_size, temp.m_size);
#else
	Deallocate();

	m_size = in_other.m_size;
	if (m_size == 0) {
		m_ptr = nullptr;
		return *this;
	}

	m_ptr = in_allocator::s_Malloc(m_size);
	if constexpr (std::is_trivially_copyable_v<T>) {
		TMemcpy<T>(m_ptr, in_other.m_ptr, m_size);
	} else {
		for (size_t i = 0; i < m_size; ++i) {
			new (m_ptr + i) T{ in_other[i] };
		}
	}
#endif

	return *this;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
MemoryBlock<T, in_allocator>::~MemoryBlock() {
	Deallocate();
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
void MemoryBlock<T, in_allocator>::Deallocate() {
	if constexpr (!std::is_trivially_destructible_v<T>) {
		Deep_Assert(m_ptr != nullptr || m_size == 0, "Invalid memory block state.");
		for (size_t i = 0; i < m_size; ++i) {
			m_ptr[i].~T();
		}
	}
	in_allocator::s_Free(m_ptr);
	m_ptr = nullptr;
	m_size = 0;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
T& MemoryBlock<T, in_allocator>::operator[](size_t in_index) {
	Deep_Assert(m_ptr != nullptr && in_index < m_size, "Out of range.");
	return m_ptr[in_index];
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
const T& MemoryBlock<T, in_allocator>::operator[](size_t in_index) const {
	Deep_Assert(m_ptr != nullptr && in_index < m_size, "Out of range.");
	return m_ptr[in_index];
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
size_t MemoryBlock<T, in_allocator>::m_Size() const {
	return m_size;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
T* MemoryBlock<T, in_allocator>::m_Ptr() {
	return m_ptr;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
const T* MemoryBlock<T, in_allocator>::m_Ptr() const {
	return m_ptr;
}

//

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
bool operator!=(Arg_MemoryBlock<T, in_allocator> in_a, void* in_b) {
	return in_a.m_ptr != in_b;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
bool operator==(Arg_MemoryBlock<T, in_allocator> in_a, void* in_b) {
	return in_a.m_ptr == in_b;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
bool operator!=(void* in_a, Arg_MemoryBlock<T, in_allocator> in_b) {
	return in_a != in_b.m_ptr;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
bool operator==(void* in_a, Arg_MemoryBlock<T, in_allocator> in_b) {
	return in_a == in_b.m_ptr;
}

//

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
const T* operator+(Arg_MemoryBlock<T, in_allocator> in_block, size_t in_value) {
	return in_block.m_ptr + in_value;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
const T* operator+(size_t in_value, Arg_MemoryBlock<T, in_allocator> in_block) {
	return in_value + in_block.m_ptr;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
const T* operator-(Arg_MemoryBlock<T, in_allocator> in_block, size_t in_value) {
	return in_block.m_ptr - in_value;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
T* operator+(MemoryBlock<T, in_allocator>& in_block, size_t in_value) {
	return in_block.m_ptr + in_value;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
T* operator+(size_t in_value, MemoryBlock<T, in_allocator>& in_block) {
	return in_value + in_block.m_ptr;
}

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
T* operator-(MemoryBlock<T, in_allocator>& in_block, size_t in_value) {
	return in_block.m_ptr - in_value;
}

DEEP_NAMESPACE_END
