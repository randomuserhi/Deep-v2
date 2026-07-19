#pragma once

#include "Deep.h"
#include "Deep/Memory/RawAllocator.h"

#include <concepts>

DEEP_NAMESPACE_BEGIN

template<typename T, typename in_allocator = RawAllocator<T>>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
class MemoryBlock;

template<typename T, typename in_allocator = RawAllocator<T>>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
using Arg_MemoryBlock = const MemoryBlock<T, in_allocator>&;

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator==(Arg_MemoryBlock<T, in_allocator>, void*);

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator!=(Arg_MemoryBlock<T, in_allocator>, void*);

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator==(void*, Arg_MemoryBlock<T, in_allocator>);

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator!=(void*, Arg_MemoryBlock<T, in_allocator>);

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline T* operator+(Arg_MemoryBlock<T, in_allocator>, size_t);

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline T* operator+(size_t, Arg_MemoryBlock<T, in_allocator>);

template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline T* operator-(Arg_MemoryBlock<T, in_allocator>, size_t);

// Light weight wrapper around a memory block (ptr + size)
//
// NOTE(randomuserhi): Currently does not support T with constructors or copy that can throw
// TODO(randomuserhi): Support T that throw on copy or construct
template<typename T, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
class MemoryBlock {
public:
	inline MemoryBlock(const MemoryBlock&);
	inline MemoryBlock(MemoryBlock&&) noexcept;
	inline MemoryBlock& operator=(const MemoryBlock&);
	inline MemoryBlock& operator=(MemoryBlock&&) noexcept;
	inline MemoryBlock();

	// Allocates a block of size `in_size`.
	explicit inline MemoryBlock(size_t in_size);

	// Take ownership of an existing block.
	//
	// The block must be assigned the appropriate allocator to properly destruct the buffer.
	// By default, uses Deep::TFree<T> to free the given pointer, and thus expects the pointer to be allocated
	// with Deep::TMalloc<T>.
	explicit inline MemoryBlock(T* in_ptr, size_t in_size);

	//

	inline ~MemoryBlock();

	//

	inline operator T*() const;

	//

	Deep_ForceInline T& operator[](size_t);
	Deep_ForceInline const T& operator[](size_t) const;

	friend bool operator== <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, void*);
	friend bool operator!= <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, void*);
	friend bool operator== <T, in_allocator>(void*, Arg_MemoryBlock<T, in_allocator>);
	friend bool operator!= <T, in_allocator>(void*, Arg_MemoryBlock<T, in_allocator>);

	friend T* operator+ <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, size_t);
	friend T* operator+ <T, in_allocator>(size_t, Arg_MemoryBlock<T, in_allocator>);
	friend T* operator- <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, size_t);

	//

	Deep_ForceInline size_t m_Size() const;

	Deep_ForceInline T* m_Ptr();
	Deep_ForceInline const T* m_Ptr() const;

private:
	DEEP_PRIVATE_TESTABLE

	// Destructs all items in the block and deallocates the memory ptr
	inline void Deallocate();

	T* m_ptr;
	size_t m_size;
};

DEEP_NAMESPACE_END

#include "MemoryBlock.inl" // IWYU pragma: export
