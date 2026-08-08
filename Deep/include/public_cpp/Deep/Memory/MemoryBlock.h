#pragma once

#include "Deep.h"
#include "Deep/Memory/RawAllocator.h"

#include <concepts>
#include <type_traits>

DEEP_NAMESPACE_BEGIN

template<typename T, typename in_allocator = RawAllocator<T>>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
class MemoryBlock;

template<typename T, typename in_allocator = RawAllocator<T>>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
using Arg_MemoryBlock = const MemoryBlock<T, in_allocator>&;

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator==(Arg_MemoryBlock<T, in_allocator>, void*);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator!=(Arg_MemoryBlock<T, in_allocator>, void*);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator==(void*, Arg_MemoryBlock<T, in_allocator>);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline bool operator!=(void*, Arg_MemoryBlock<T, in_allocator>);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline const T* operator+(Arg_MemoryBlock<T, in_allocator>, size_t);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline const T* operator+(size_t, Arg_MemoryBlock<T, in_allocator>);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline const T* operator-(Arg_MemoryBlock<T, in_allocator>, size_t);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline T* operator+(MemoryBlock<T, in_allocator>&, size_t);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline T* operator+(size_t, MemoryBlock<T, in_allocator>&);

template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
Deep_ForceInline T* operator-(MemoryBlock<T, in_allocator>&, size_t);

// Light weight wrapper around a memory block (ptr + size)
//
// TODO(randomuserhi): Provide an implicit conversion into Deep::Span or some other non-owning memory view
//                     datastructure. This allows you to pass the memory block using short-struct optimization
//                     through registers rather than via `const MemorBlock<T>&` which isn't guaranteed to optimize.
//
template<typename T, typename in_allocator>
	requires std::copy_constructible<T> && c_RawAllocator<in_allocator, T>
class MemoryBlock {
	static_assert(std::is_nothrow_destructible_v<T>, "MemoryBlock<T> requires T to have a non-throwing destructor.");

public:
	inline MemoryBlock(const MemoryBlock&) noexcept(noexcept(in_allocator::s_Malloc(std::declval<size_t>()))
	                                                && std::is_nothrow_copy_constructible_v<T>);
	inline MemoryBlock(MemoryBlock&&) noexcept;
	inline MemoryBlock& operator=(const MemoryBlock&) noexcept(noexcept(in_allocator::s_Malloc(std::declval<size_t>()))
	                                                           && std::is_nothrow_copy_constructible_v<T>);
	inline MemoryBlock& operator=(MemoryBlock&&) noexcept;
	inline MemoryBlock() noexcept;

	// Allocates a block of size `in_size`.
	explicit inline MemoryBlock(size_t in_size) noexcept(noexcept(in_allocator::s_Malloc(std::declval<size_t>()))
	                                                     && std::is_nothrow_default_constructible_v<T>)
		requires std::default_initializable<T>;

	// Take ownership of an existing block.
	//
	// The block must be assigned the appropriate allocator to properly destruct the buffer.
	// By default, uses Deep::TFree<T> to free the given pointer, and thus expects the pointer to be allocated
	// with Deep::TMalloc<T>.
	explicit inline MemoryBlock(T* in_ptr, size_t in_size) noexcept;

	//

	inline ~MemoryBlock() noexcept;

	//

	inline operator T*();
	inline operator const T*() const;

	//

	Deep_ForceInline T& operator[](size_t);
	Deep_ForceInline const T& operator[](size_t) const;

	friend bool operator== <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, void*);
	friend bool operator!= <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, void*);
	friend bool operator== <T, in_allocator>(void*, Arg_MemoryBlock<T, in_allocator>);
	friend bool operator!= <T, in_allocator>(void*, Arg_MemoryBlock<T, in_allocator>);

	friend const T* operator+ <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, size_t);
	friend const T* operator+ <T, in_allocator>(size_t, Arg_MemoryBlock<T, in_allocator>);
	friend const T* operator- <T, in_allocator>(Arg_MemoryBlock<T, in_allocator>, size_t);

	friend T* operator+ <T, in_allocator>(MemoryBlock<T, in_allocator>&, size_t);
	friend T* operator+ <T, in_allocator>(size_t, MemoryBlock<T, in_allocator>&);
	friend T* operator- <T, in_allocator>(MemoryBlock<T, in_allocator>&, size_t);

	//

	Deep_ForceInline size_t m_Size() const;

	Deep_ForceInline T* m_Ptr();
	Deep_ForceInline const T* m_Ptr() const;

private:
	DEEP_PRIVATE_TESTABLE

	// Destructs all items in the block and deallocates the memory ptr
	inline void Deallocate() noexcept;

	T* m_ptr;
	size_t m_size;
};

DEEP_NAMESPACE_END

#include "./MemoryBlock.inl" // IWYU pragma: export
