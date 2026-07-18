#pragma once

#include "Deep.h"
#include "Deep/Bit.h"
#include "Deep/Concepts.h"

#include <concepts>

DEEP_NAMESPACE_BEGIN

// Describes the shape required for a bitmask type.
// Allows for both fixed length and dynamic length bit masks.
template<typename T>
concept c_BitMask = std::copyable<T> && requires(T a, const T ca, std::size_t i, bool v) {
	{ T::k_maxNumBits } -> std::convertible_to<size_t>;

	{ ca == ca } -> std::same_as<bool>;

	// Returns true if any bits in the bitmask are set, otherwise false.
	{ ca.Any() } -> std::same_as<bool>;

	// Return the number of set bits
	{ ca.NumSetBits() } -> std::same_as<size_t>;

	// Return true if the bit at position i is set, otherwise false.
	{ ca[i] } -> std::same_as<bool>;

	// Set/Unset a bit
	{ a.Set(i, v) } -> std::same_as<void>;

	// Gets the index of the lowest set bit.
	//
	// Undefined behaviour when no bits are set.
	{ ca.IndexOfLowestSetBit() } -> std::same_as<size_t>;

	// Pops the lowest set bit of the mask and returns its index.
	// E.g 0b1010 will become 0b1000 and return 1.
	//
	// Undefined behaviour when no bits are set.
	{ a.PopLowestSetBit() } -> std::same_as<size_t>;

	// Union & Intersect operators
	{ a |= ca } -> std::same_as<T&>;
	{ ca | ca } -> std::same_as<T>;
	{ a &= ca } -> std::same_as<T&>;
	{ ca & ca } -> std::same_as<T>;
};

template<c_UnsignedInteger T>
class IntegerBitMask;

template<c_UnsignedInteger T>
using Arg_IntegerBitMask = const IntegerBitMask<T>;

template<c_UnsignedInteger T>
constexpr Deep_ForceInline IntegerBitMask<T> operator|(IntegerBitMask<T>, Arg_IntegerBitMask<T>);

template<c_UnsignedInteger T>
constexpr Deep_ForceInline IntegerBitMask<T> operator&(IntegerBitMask<T>, Arg_IntegerBitMask<T>);

template<c_UnsignedInteger T>
constexpr Deep_ForceInline bool operator==(Arg_IntegerBitMask<T>, Arg_IntegerBitMask<T>);

// Bitmask implementation for common integer types that allows them to be used with
// `_BitMask` concept.
template<c_UnsignedInteger T>
class IntegerBitMask {
public:
	IntegerBitMask() = default;
	IntegerBitMask(T in_value);

	constexpr Deep_ForceInline bool operator[](size_t in_index) const;

	constexpr Deep_ForceInline bool Any() const;

	constexpr Deep_ForceInline size_t NumSetBits() const;

	constexpr Deep_ForceInline void Set(size_t in_index, bool in_value);

	constexpr Deep_ForceInline size_t IndexOfLowestSetBit() const;

	constexpr Deep_ForceInline size_t PopLowestSetBit();

	friend constexpr Deep_ForceInline bool operator== <>(Arg_IntegerBitMask<T>, Arg_IntegerBitMask<T>);

	constexpr Deep_ForceInline IntegerBitMask& operator|=(Arg_IntegerBitMask<T> in_other);
	friend constexpr Deep_ForceInline IntegerBitMask operator| <>(IntegerBitMask, Arg_IntegerBitMask<T>);

	constexpr Deep_ForceInline IntegerBitMask& operator&=(Arg_IntegerBitMask<T> in_other);
	friend constexpr Deep_ForceInline IntegerBitMask operator& <>(IntegerBitMask, Arg_IntegerBitMask<T>);

	//

	constexpr static size_t k_maxNumBits = Deep::NumBits<T>();

private:
	T m_bits;
};

// Explicit instantiation for common integer types
extern template class IntegerBitMask<uint8>;
extern template class IntegerBitMask<uint16>;
extern template class IntegerBitMask<uint32>;
extern template class IntegerBitMask<uint64>;

DEEP_NAMESPACE_END

#include "Deep/Bit/IntegerBitMask.inl" // IWYU pragma: export
