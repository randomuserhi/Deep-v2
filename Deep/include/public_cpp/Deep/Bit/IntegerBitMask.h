#pragma once

#include "Deep.h"
#include "Deep/Bit.h"
#include "Deep/Bit/c_BitMask.h"
#include "Deep/Concepts.h"

DEEP_NAMESPACE_BEGIN

template<c_UnsignedInteger T>
class IntegerBitMask;

template<c_UnsignedInteger T>
using Arg_IntegerBitMask = const IntegerBitMask<T>;

template<c_UnsignedInteger T>
constexpr Deep_ForceInline IntegerBitMask<T> operator|(IntegerBitMask<T>, Arg_IntegerBitMask<T>);

template<c_UnsignedInteger T>
constexpr Deep_ForceInline IntegerBitMask<T> operator&(IntegerBitMask<T>, Arg_IntegerBitMask<T>);

template<c_UnsignedInteger T>
constexpr Deep_ForceInline IntegerBitMask<T> operator~(IntegerBitMask<T>);

template<c_UnsignedInteger T>
constexpr Deep_ForceInline bool operator==(Arg_IntegerBitMask<T>, Arg_IntegerBitMask<T>);

// Bitmask implementation for common integer types that allows them to be used with
// `_BitMask` concept.
template<c_UnsignedInteger T>
class IntegerBitMask {
public:
	IntegerBitMask() = default;
	IntegerBitMask(T in_value);

	// TODO(randomuserhi): Implement a bit accessor reference to support references, refer to std::vector<bool>
	//                     specialization
	// [[nodiscard]] constexpr Deep_ForceInline bool operator[](size_t in_index) const;

	[[nodiscard]] constexpr Deep_ForceInline bool Any() const;

	[[nodiscard]] constexpr Deep_ForceInline bool Test(size_t in_index) const;

	[[nodiscard]] constexpr Deep_ForceInline size_t NumSetBits() const;

	template<bool in_value>
	constexpr Deep_ForceInline void Set(size_t in_index);
	constexpr Deep_ForceInline void Set(size_t in_index, bool in_value);

	constexpr Deep_ForceInline void Inverse();

	[[nodiscard]] constexpr Deep_ForceInline size_t IndexOfLowestSetBit() const;

	constexpr Deep_ForceInline void ClearLowestSetBit();

	[[nodiscard]] constexpr Deep_ForceInline size_t PopLowestSetBit();

	friend constexpr bool operator== <T>(Arg_IntegerBitMask<T>, Arg_IntegerBitMask<T>);

	constexpr Deep_ForceInline IntegerBitMask& operator|=(Arg_IntegerBitMask<T> in_other);
	friend constexpr IntegerBitMask operator| <T>(IntegerBitMask, Arg_IntegerBitMask<T>);

	constexpr Deep_ForceInline IntegerBitMask& operator&=(Arg_IntegerBitMask<T> in_other);
	friend constexpr IntegerBitMask operator& <T>(IntegerBitMask, Arg_IntegerBitMask<T>);

	friend constexpr IntegerBitMask operator~ <T>(IntegerBitMask);

	//

	constexpr static size_t k_maxNumBits = Deep::NumBits<T>();

private:
	DEEP_PRIVATE_TESTABLE

	T m_bits;
};

// Explicit instantiation for common integer types
extern template class IntegerBitMask<uint8>;
extern template class IntegerBitMask<uint16>;
extern template class IntegerBitMask<uint32>;
extern template class IntegerBitMask<uint64>;

static_assert(c_BitMask<IntegerBitMask<uint8>>);
static_assert(c_BitMask<IntegerBitMask<uint16>>);
static_assert(c_BitMask<IntegerBitMask<uint32>>);
static_assert(c_BitMask<IntegerBitMask<uint64>>);

DEEP_NAMESPACE_END

#include "./IntegerBitMask.inl" // IWYU pragma: export
