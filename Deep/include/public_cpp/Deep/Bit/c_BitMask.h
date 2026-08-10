#pragma once

#include "Deep.h"

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

DEEP_NAMESPACE_END
