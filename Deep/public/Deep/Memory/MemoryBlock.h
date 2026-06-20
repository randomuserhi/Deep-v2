#pragma once

#include "Deep.h"
#include "Deep/Memory.h"

#include <concepts>

DEEP_NAMESPACE_BEGIN

template<typename T, Ownership in_ownership = Ownership::Owned, typename in_allocator = RawAllocator<T>>
	requires std::default_initializable<T> && std::copy_constructible<T> && _RawAllocator<in_allocator, T>
class MemoryBlock;

template<typename T, Ownership in_ownership = Ownership::Owned, typename in_allocator = RawAllocator<T>>
	requires std::default_initializable<T> && std::copy_constructible<T> && _RawAllocator<in_allocator, T>
using Arg_MemoryBlock = const MemoryBlock<T, in_ownership, in_allocator>;

// NOTE(randomuserhi): Is not compatible with memory allocated by `new` operator
template<typename T, Ownership in_ownership, typename in_allocator>
	requires std::default_initializable<T> && std::copy_constructible<T> && _RawAllocator<in_allocator, T>
class MemoryBlock {
public:
	inline MemoryBlock(const MemoryBlock&);
	inline MemoryBlock(MemoryBlock&&);
	inline MemoryBlock& operator=(const MemoryBlock&);
	inline MemoryBlock& operator=(MemoryBlock&&);
	inline MemoryBlock();
	explicit inline MemoryBlock(size_t in_size);
	explicit inline MemoryBlock(T* in_ptr, size_t in_size);

	//

	inline ~MemoryBlock();

	//

	inline operator T*() const;

	//

	Deep_ForceInline T& operator[](size_t);
	Deep_ForceInline const T& operator[](size_t) const;

	friend Deep_ForceInline bool operator==(Arg_MemoryBlock<T, in_ownership, in_allocator>, void*);
	friend Deep_ForceInline bool operator!=(Arg_MemoryBlock<T, in_ownership, in_allocator>, void*);
	friend Deep_ForceInline bool operator==(void*, Arg_MemoryBlock<T, in_ownership, in_allocator>);
	friend Deep_ForceInline bool operator!=(void*, Arg_MemoryBlock<T, in_ownership, in_allocator>);

	friend Deep_ForceInline T* operator+(Arg_MemoryBlock<T, in_ownership, in_allocator>, size_t);
	friend Deep_ForceInline T* operator+(size_t, Arg_MemoryBlock<T, in_ownership, in_allocator>);
	friend Deep_ForceInline T* operator-(Arg_MemoryBlock<T, in_ownership, in_allocator>, size_t);
	friend Deep_ForceInline T* operator-(size_t, Arg_MemoryBlock<T, in_ownership, in_allocator>);

	//

	Deep_ForceInline size_t size() const;
	Deep_ForceInline T* ptr() const;

private:
	DEEP_PRIVATE_TESTABLE

	T* m_ptr;
	size_t m_size;
};

DEEP_NAMESPACE_END

#include "MemoryBlock.inl" // IWYU pragma: export
