#pragma once

#include "Deep.h"
#include "Deep/Bit/c_BitMask.h"

#include <bitset>

DEEP_NAMESPACE_BEGIN

// TODO(randomuserhi): Move away from std::bitset for performance of IndexOfLowestSetBit.
//                     Also allows implementation of a storage policy if bitset is too large for the stack.
//                     (Uses a std::array<Chunk, in_N> instead of a Chunk*):
//                     template<size_t in_N, StoragePolicy in_StoragePolicy>
//                     - StoragePolicy::e_stack, StoragePolicy::e_heap

template<size_t in_N>
class BitMask;

template<size_t in_N>
using Arg_BitMask = const BitMask<in_N>;

template<size_t in_N>
constexpr Deep_ForceInline BitMask<in_N> operator|(BitMask<in_N>, Arg_BitMask<in_N>);

template<size_t in_N>
constexpr Deep_ForceInline BitMask<in_N> operator&(BitMask<in_N>, Arg_BitMask<in_N>);

template<size_t in_N>
constexpr Deep_ForceInline BitMask<in_N> operator~(BitMask<in_N>);

template<size_t in_N>
constexpr Deep_ForceInline bool operator==(Arg_BitMask<in_N>, Arg_BitMask<in_N>);

// Bitmask wrapper for std::bitset
template<size_t in_N>
class BitMask {
public:
	BitMask() = default;
	BitMask(unsigned long long in_value);

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

	friend constexpr bool operator== <in_N>(Arg_BitMask<in_N>, Arg_BitMask<in_N>);

	constexpr Deep_ForceInline BitMask& operator|=(Arg_BitMask<in_N> in_other);
	friend constexpr BitMask operator| <in_N>(BitMask, Arg_BitMask<in_N>);

	constexpr Deep_ForceInline BitMask& operator&=(Arg_BitMask<in_N> in_other);
	friend constexpr BitMask operator& <in_N>(BitMask, Arg_BitMask<in_N>);

	friend constexpr BitMask operator~ <in_N>(BitMask);

	//

	constexpr static size_t k_maxNumBits = in_N;

private:
	DEEP_PRIVATE_TESTABLE

	std::bitset<in_N> m_bits;
};

static_assert(c_BitMask<BitMask<32>>);

DEEP_NAMESPACE_END

#include "./BitMask.inl" // IWYU pragma: export
