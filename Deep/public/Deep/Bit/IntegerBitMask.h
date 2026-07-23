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

	// Set/Unset a bit
	{ a.Set(i, v) } -> std::same_as<void>;
	{ a.template Set<true>(i) } -> std::same_as<void>;
	{ a.template Set<false>(i) } -> std::same_as<void>;

	// Return true if the bit at position i is set, otherwise false.
	{ a.Test(i) } -> std::same_as<bool>;

	// Gets the index of the lowest set bit.
	//
	// Undefined behaviour when no bits are set.
	{ ca.IndexOfLowestSetBit() } -> std::same_as<size_t>;

	// Clears the lowest set bit of the mask.
	// E.g 0b1010 will become 0b1000 and return 1.
	//
	// Undefined behaviour when no bits are set.
	{ a.ClearLowestSetBit() } -> std::same_as<void>;

	// Clears the lowest set bit of the mask and returns its index.
	// E.g 0b1010 will become 0b1000 and return 1.
	//
	// Undefined behaviour when no bits are set.
	{ a.PopLowestSetBit() } -> std::same_as<size_t>;

	// Union & Intersect operators
	{ a |= ca } -> std::same_as<T&>;
	{ ca | ca } -> std::same_as<T>;
	{ a &= ca } -> std::same_as<T&>;
	{ ca & ca } -> std::same_as<T>;

	// Not operator
	{ a.Inverse() } -> std::same_as<void>;
	{ ~ca } -> std::same_as<T>;
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

// TODO(randomuserhi): Implement a `template<size_t N> class BitMask` that wraps `template<size_t N> std::bitset`

DEEP_NAMESPACE_END

#include "./IntegerBitMask.inl" // IWYU pragma: export
