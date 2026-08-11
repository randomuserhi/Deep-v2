#pragma once

#include "Deep.h"
#include "Deep/Bit.h"
#include "Deep/Memory/StoragePolicy.h"
#include "Deep/Bit/c_BitMask.h"

#include <array>
#include <limits>

DEEP_NAMESPACE_BEGIN

// TODO(randomuserhi): Move away from std::bitset for performance of IndexOfLowestSetBit.
//                     Also allows implementation of a storage policy if bitset is too large for the stack.
//                     (Uses a std::array<Chunk, in_N> instead of a Chunk*):
//                     template<size_t in_N, StoragePolicy in_StoragePolicy>
//                     - StoragePolicy::e_stack, StoragePolicy::e_heap

template<size_t in_N, StoragePolicy in_Policy = StoragePolicy::e_stack>
class BitMask;

template<size_t in_N, StoragePolicy in_Policy>
using Arg_BitMask = const BitMask<in_N, in_Policy>&;

template<size_t in_N, StoragePolicy in_Policy>
constexpr Deep_ForceInline BitMask<in_N, in_Policy> operator|(BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);

template<size_t in_N, StoragePolicy in_Policy>
constexpr Deep_ForceInline BitMask<in_N, in_Policy> operator&(BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);

template<size_t in_N, StoragePolicy in_Policy>
constexpr Deep_ForceInline BitMask<in_N, in_Policy> operator^(BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);

template<size_t in_N, StoragePolicy in_Policy>
constexpr Deep_ForceInline BitMask<in_N, in_Policy> operator~(BitMask<in_N, in_Policy>);

template<size_t in_N, StoragePolicy in_Policy>
constexpr inline bool operator==(Arg_BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);
template<size_t in_N, StoragePolicy in_Policy>
constexpr Deep_ForceInline bool operator!=(Arg_BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);

namespace detail::_Bitmask {

template<size_t in_N>
class StorageInfo {
public:
	using ChunkType = uint64;
	static_assert(std::is_unsigned_v<ChunkType>, "Must be unsigned.");

	constexpr static size_t k_bitsPerChunk = std::numeric_limits<ChunkType>().digits;
	static_assert(Deep::IsPowerOf2(k_bitsPerChunk), "Bits per chunk must be a power of 2.");

	constexpr static size_t k_pageShift = Deep::CountTrailingZeros(k_bitsPerChunk);
	constexpr static size_t k_itemMask = k_bitsPerChunk - 1;

	constexpr static size_t k_numLeafChunks = ((in_N - 1) >> k_pageShift) + 1;
	constexpr static size_t k_numLevels = []() -> size_t {
		if (in_N == 0) return 0;

		size_t numChunks = k_numLeafChunks;
		size_t numLevels = 1;
		while (numChunks > 1) {
			numChunks = ((numChunks - 1) >> k_pageShift) + 1;
			++numLevels;
		}
		return numLevels;
	}();
	constexpr static size_t k_numChunks = []() -> size_t {
		if (in_N == 0) return 0;

		size_t numChunks = k_numLeafChunks;
		size_t total = 0;
		while (true) {
			total += numChunks;
			if (numChunks == 1) return total;
			numChunks = ((numChunks - 1) >> k_pageShift) + 1;
		}
	}();

	constexpr static auto k_levelOffsets = []() {
		std::array<size_t, k_numLevels> offsets{};
		size_t offset = 0;
		size_t numChunks = k_numLeafChunks;

		for (size_t level = 0; level < k_numLevels; ++level) {
			offsets[level] = offset;

			offset += numChunks;
			numChunks = ((numChunks - 1) >> k_pageShift) + 1;
		}
		return offsets;
	}();
	constexpr static auto k_levelSizes = []() {
		std::array<size_t, k_numLevels> sizes{};
		size_t numChunks = k_numLeafChunks;

		for (size_t level = 0; level < k_numLevels; ++level) {
			sizes[level] = numChunks;
			numChunks = ((numChunks - 1) >> k_pageShift) + 1;
		}
		return sizes;
	}();
};

template<size_t in_N, StoragePolicy in_policy>
class Storage;

template<size_t in_N>
class Storage<in_N, StoragePolicy::e_stack> : public StorageInfo<in_N> {
public:
	using typename StorageInfo<in_N>::ChunkType;

	using StorageInfo<in_N>::k_bitsPerChunk;

	using StorageInfo<in_N>::k_pageShift;
	using StorageInfo<in_N>::k_itemMask;

	using StorageInfo<in_N>::k_numLeafChunks;
	using StorageInfo<in_N>::k_numLevels;
	using StorageInfo<in_N>::k_numChunks;

	using StorageInfo<in_N>::k_levelOffsets;
	using StorageInfo<in_N>::k_levelSizes;

	constexpr Deep_ForceInline ChunkType& operator[](size_t in_index);
	constexpr Deep_ForceInline const ChunkType& operator[](size_t in_index) const;

	//

	constexpr Deep_ForceInline ChunkType* m_Ptr();
	constexpr Deep_ForceInline const ChunkType* m_Ptr() const;

private:
	std::array<ChunkType, k_numChunks> m_chunks{};
};

template<size_t in_N>
class Storage<in_N, StoragePolicy::e_heap> : public StorageInfo<in_N> {
public:
	using typename StorageInfo<in_N>::ChunkType;

	using StorageInfo<in_N>::k_bitsPerChunk;

	using StorageInfo<in_N>::k_pageShift;
	using StorageInfo<in_N>::k_itemMask;

	using StorageInfo<in_N>::k_numLeafChunks;
	using StorageInfo<in_N>::k_numLevels;
	using StorageInfo<in_N>::k_numChunks;

	using StorageInfo<in_N>::k_levelOffsets;
	using StorageInfo<in_N>::k_levelSizes;

	inline Storage(const Storage& in_other) noexcept;
	inline Storage(Storage&& in_other) noexcept;
	inline Storage& operator=(const Storage& in_other) noexcept;
	inline Storage& operator=(Storage&& in_other) noexcept;
	inline Storage() noexcept;

	//

	inline ~Storage() noexcept;

	//

	Deep_ForceInline ChunkType& operator[](size_t in_index);
	Deep_ForceInline const ChunkType& operator[](size_t in_index) const;

	//

	Deep_ForceInline ChunkType* m_Ptr();
	Deep_ForceInline const ChunkType* m_Ptr() const;

private:
	ChunkType* m_chunks;
};

} // namespace detail::_Bitmask

// Bitmask wrapper for std::bitset
template<size_t in_N, StoragePolicy in_Policy>
class BitMask {
	static_assert(in_N > 0, "Bit mask must contain more than 1 bit.");
	using Storage = detail::_Bitmask::Storage<in_N, in_Policy>;

public:
	using ChunkType = typename Storage::ChunkType;

public:
	BitMask() = default;

	[[nodiscard]] constexpr Deep_ForceInline bool Any() const;

	[[nodiscard]] constexpr Deep_ForceInline bool Test(size_t in_index) const;

	[[nodiscard]] constexpr inline size_t NumSetBits() const;

	template<bool in_value>
	constexpr inline void Set(size_t in_index);
	constexpr inline void Set(size_t in_index, bool in_value);

	constexpr inline void Inverse();

	[[nodiscard]] constexpr inline size_t IndexOfLowestSetBit() const;

	constexpr inline void ClearLowestSetBit();

	constexpr inline void Clear();

	[[nodiscard]] constexpr inline size_t PopLowestSetBit();

	friend constexpr bool operator== <in_N, in_Policy>(Arg_BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);
	friend constexpr bool operator!= <in_N, in_Policy>(Arg_BitMask<in_N, in_Policy>, Arg_BitMask<in_N, in_Policy>);

	constexpr inline BitMask& operator|=(Arg_BitMask<in_N, in_Policy> in_other);
	friend constexpr BitMask operator| <in_N, in_Policy>(BitMask, Arg_BitMask<in_N, in_Policy>);

	constexpr inline BitMask& operator&=(Arg_BitMask<in_N, in_Policy> in_other);
	friend constexpr BitMask operator& <in_N, in_Policy>(BitMask, Arg_BitMask<in_N, in_Policy>);

	constexpr inline BitMask& operator^=(Arg_BitMask<in_N, in_Policy> in_other);
	friend constexpr BitMask operator^ <in_N, in_Policy>(BitMask, Arg_BitMask<in_N, in_Policy>);

	friend constexpr BitMask operator~ <in_N, in_Policy>(BitMask);

	//

	constexpr static size_t k_maxNumBits = in_N;

	constexpr static size_t k_bitsPerChunk = Storage::k_bitsPerChunk;
	constexpr static size_t k_numLeafChunks = Storage::k_numLeafChunks;

private:
	DEEP_PRIVATE_TESTABLE

	constexpr void RebuildHierarchy();
	constexpr void MaskUnusedBits();
	constexpr void PropagateOccupancy(size_t in_childLevel, size_t in_childIndex, bool in_occupied);

	//

	constexpr static size_t k_numChunks = Storage::k_numChunks;
	constexpr static size_t k_numLevels = Storage::k_numLevels;

	constexpr static auto k_levelOffsets = Storage::k_levelOffsets;
	constexpr static auto k_levelSizes = Storage::k_levelSizes;

	constexpr static size_t k_pageShift = Storage::k_pageShift;
	constexpr static size_t k_itemMask = Storage::k_itemMask;

	Storage m_chunks;
};

static_assert(c_BitMask<BitMask<32, StoragePolicy::e_stack>>);
static_assert(c_BitMask<BitMask<32, StoragePolicy::e_heap>>);

DEEP_NAMESPACE_END

#include "./BitMask.inl" // IWYU pragma: export
