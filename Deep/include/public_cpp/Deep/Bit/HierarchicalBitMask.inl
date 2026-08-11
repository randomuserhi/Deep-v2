#pragma once

#include "./HierarchicalBitMask.h"

#include "Deep/Bit.h"
#include "Deep/Memory.h"

DEEP_NAMESPACE_BEGIN

namespace detail::_HierarchicalBitMask {

#define STORAGE_TEMPLATE template<size_t in_N>
#define STORAGE Storage<in_N, StoragePolicy::e_stack>

STORAGE_TEMPLATE
constexpr STORAGE::ChunkType& STORAGE::operator[](size_t in_index) {
	Deep_Assert(in_index < k_numChunks, "Index out of range.");
	return m_chunks[in_index];
}

STORAGE_TEMPLATE
constexpr const STORAGE::ChunkType& STORAGE::operator[](size_t in_index) const {
	Deep_Assert(in_index < k_numChunks, "Index out of range.");
	return m_chunks[in_index];
}

STORAGE_TEMPLATE
constexpr Deep_ForceInline STORAGE::ChunkType* STORAGE::m_Ptr() {
	return m_chunks.data();
}

STORAGE_TEMPLATE
constexpr Deep_ForceInline const STORAGE::ChunkType* STORAGE::m_Ptr() const {
	return m_chunks.data();
}

#undef STORAGE_TEMPLATE
#undef STORAGE

#define STORAGE_TEMPLATE template<size_t in_N>
#define STORAGE Storage<in_N, StoragePolicy::e_heap>

STORAGE_TEMPLATE
STORAGE::Storage(const Storage& in_other) noexcept {
	Deep_Assert(in_other.m_chunks != nullptr, "Cannot copy from a nullptr storage.");
	m_chunks = new ChunkType[k_numChunks];
	Deep::TMemcpy<ChunkType>(m_chunks, in_other.m_chunks, k_numChunks);
}

STORAGE_TEMPLATE
STORAGE::Storage(Storage&& in_other) noexcept :
	m_chunks{ in_other.m_chunks } {
	Deep_Assert(in_other.m_chunks != nullptr, "Cannot move from a nullptr storage.");
	in_other.m_chunks = nullptr;
}

STORAGE_TEMPLATE
STORAGE& STORAGE::operator=(const Storage& in_other) noexcept {
	if (this == &in_other) return *this;
	Deep_Assert(in_other.m_chunks != nullptr, "Cannot copy from a nullptr storage.");

	if (m_chunks == nullptr) {
		m_chunks = new ChunkType[k_numChunks];
	}
	Deep::TMemcpy<ChunkType>(m_chunks, in_other.m_chunks, k_numChunks);

	return *this;
}

STORAGE_TEMPLATE
STORAGE& STORAGE::operator=(Storage&& in_other) noexcept {
	if (this == &in_other) return *this;
	Deep_Assert(in_other.m_chunks != nullptr, "Cannot move from a nullptr storage.");

	delete[] m_chunks;
	m_chunks = in_other.m_chunks;
	in_other.m_chunks = nullptr;

	return *this;
}

STORAGE_TEMPLATE
STORAGE::Storage() noexcept {
	m_chunks = new ChunkType[k_numChunks]{};
}

STORAGE_TEMPLATE
STORAGE::~Storage() noexcept {
	delete m_chunks;
}

STORAGE_TEMPLATE
STORAGE::ChunkType& STORAGE::operator[](size_t in_index) {
	Deep_Assert(m_chunks != nullptr, "Storage is nullptr.");
	Deep_Assert(in_index < k_numChunks, "Index out of range.");
	return m_chunks[in_index];
}
STORAGE_TEMPLATE
const STORAGE::ChunkType& STORAGE::operator[](size_t in_index) const {
	Deep_Assert(m_chunks != nullptr, "Storage is nullptr.");
	Deep_Assert(in_index < k_numChunks, "Index out of range.");
	return m_chunks[in_index];
}

STORAGE_TEMPLATE
STORAGE::ChunkType* STORAGE::m_Ptr() {
	return m_chunks;
}

STORAGE_TEMPLATE
const STORAGE::ChunkType* STORAGE::m_Ptr() const {
	return m_chunks;
}

#undef STORAGE_TEMPLATE
#undef STORAGE

} // namespace detail::_HierarchicalBitMask

#define BITMASK_TEMPLATE template<size_t in_N, StoragePolicy in_Policy>
#define BITMASK HierarchicalBitMask<in_N, in_Policy>
#define ARG_BITMASK Arg_HierarchicalBitMask<in_N, in_Policy>

BITMASK_TEMPLATE
constexpr void BITMASK::RebuildHierarchy() {
	for (size_t level = 1; level < k_numLevels; ++level) {
		const size_t childOffset = k_levelOffsets[level - 1];
		const size_t childCount = k_levelSizes[level - 1];

		const size_t parentOffset = k_levelOffsets[level];
		const size_t parentCount = k_levelSizes[level];

		// Clear this summary level first.
		for (size_t i = 0; i < parentCount; ++i) {
			m_chunks[parentOffset + i] = 0;
		}

		for (size_t childIndex = 0; childIndex < childCount; ++childIndex) {
			if (m_chunks[childOffset + childIndex] == 0) continue;

			const size_t parentChunk = childIndex >> k_pageShift;
			const size_t parentBit = childIndex & k_itemMask;
			m_chunks[parentOffset + parentChunk] |= static_cast<ChunkType>(ChunkType{ 1 } << parentBit);
		}
	}
}

BITMASK_TEMPLATE
constexpr void BITMASK::PropagateOccupancy(size_t in_childLevel, size_t in_childIndex, bool in_occupied) {
	size_t childIndex = in_childIndex;
	bool occupied = in_occupied;

	for (size_t level = in_childLevel; level + 1 < k_numLevels; ++level) {
		const size_t parentChunkIndex = childIndex >> k_pageShift;
		const size_t parentBitIndex = childIndex & k_itemMask;
		ChunkType& parent = m_chunks[k_levelOffsets[level + 1] + parentChunkIndex];

		const bool parentWasOccupied = parent != 0;
		const ChunkType clear_mask = ~static_cast<ChunkType>(ChunkType{ 1 } << parentBitIndex);
		const ChunkType new_bit = static_cast<ChunkType>(ChunkType{ occupied } << parentBitIndex);
		parent = (parent & clear_mask) | new_bit;
		const bool parentIsOccupied = parent != 0;

		if (parentWasOccupied == parentIsOccupied) break;

		childIndex = parentChunkIndex;
		occupied = parentIsOccupied;
	}
}

BITMASK_TEMPLATE
constexpr bool BITMASK::Test(size_t in_index) const {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	size_t page = in_index >> Storage::k_pageShift;
	size_t item = in_index & Storage::k_itemMask;
	return (m_chunks[page] & (typename Storage::ChunkType{ 1 } << item)) != 0;
}

BITMASK_TEMPLATE
constexpr bool BITMASK::Any() const {
	return m_chunks[Storage::k_levelOffsets[Storage::k_numLevels - 1]] != 0;
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::NumSetBits() const {
	if constexpr (k_numLevels == 1) {
		return Deep::CountSetBits(m_chunks[0]);
	} else {
		constexpr size_t k_occupancyLevel = 1;
		constexpr size_t k_occupancyOffset = k_levelOffsets[k_occupancyLevel];
		constexpr size_t k_numOccupancyChunks = k_levelSizes[k_occupancyLevel];

		size_t count = 0;
		for (size_t i = 0; i < k_numOccupancyChunks; ++i) {
			ChunkType occupancy = m_chunks[k_occupancyOffset + i];
			while (occupancy != 0) {
				const size_t bit = Deep::CountTrailingZeros(occupancy);
				const size_t leafIndex = (i << k_pageShift) + bit;
				if (leafIndex < k_numLeafChunks) {
					count += Deep::CountSetBits(m_chunks[leafIndex]);
				}
				occupancy = occupancy & (occupancy - ChunkType{ 1 });
			}
		}
		return count;
	}
}

BITMASK_TEMPLATE
constexpr void BITMASK::Set(size_t in_index, bool in_value) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	const size_t chunkIndex = in_index >> k_pageShift;
	const size_t bitIndex = in_index & k_itemMask;
	ChunkType& chunk = m_chunks[chunkIndex];

	const bool wasOccupied = chunk != 0;
	const ChunkType clear_mask = ~static_cast<ChunkType>(ChunkType{ 1 } << bitIndex);
	const ChunkType new_bit = static_cast<ChunkType>(ChunkType{ in_value } << bitIndex);
	chunk = (chunk & clear_mask) | new_bit;
	const bool isOccupied = chunk != 0;

	if (wasOccupied != isOccupied) {
		PropagateOccupancy(0, chunkIndex, isOccupied);
	}
}

BITMASK_TEMPLATE
template<bool in_value>
constexpr void BITMASK::Set(size_t in_index) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	const size_t chunkIndex = in_index >> k_pageShift;
	const size_t bitIndex = in_index & k_itemMask;
	ChunkType& chunk = m_chunks[chunkIndex];

	const ChunkType mask = static_cast<ChunkType>(ChunkType{ 1 } << bitIndex);

	const bool wasOccupied = chunk != 0;
	if constexpr (in_value) {
		chunk |= mask;
	} else {
		chunk &= static_cast<ChunkType>(~mask);
	}
	const bool isOccupied = chunk != 0;

	if (wasOccupied != isOccupied) {
		PropagateOccupancy(0, chunkIndex, isOccupied);
	}
}

BITMASK_TEMPLATE
constexpr void BITMASK::Inverse() {
	for (size_t i = 0; i < k_numLeafChunks; ++i) {
		m_chunks[i] = static_cast<ChunkType>(~m_chunks[i]);
	}
	constexpr size_t usedBitsInLastChunk = in_N & k_itemMask;
	if constexpr (usedBitsInLastChunk != 0) {
		constexpr ChunkType mask = static_cast<ChunkType>((ChunkType{ 1 } << usedBitsInLastChunk) - 1);
		m_chunks[k_numLeafChunks - 1] &= mask;
	}

	RebuildHierarchy();
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::IndexOfLowestSetBit() const {
	Deep_Assert(Any(), "No bits are set, cannot get lowest set bit.");

	constexpr size_t rootLevel = k_numLevels - 1;
	const ChunkType root = m_chunks[k_levelOffsets[rootLevel]];

	if (root == 0) return in_N;

	size_t chunkIndex = 0;
	for (size_t level = rootLevel; level > 0; --level) {
		const ChunkType chunk = m_chunks[k_levelOffsets[level] + chunkIndex];
		const size_t child = Deep::CountTrailingZeros(chunk);
		chunkIndex = (chunkIndex << k_pageShift) + child;
	}

	const ChunkType leaf = m_chunks[chunkIndex];
	const size_t bitIndex = static_cast<size_t>(std::countr_zero(leaf));

	return (chunkIndex << k_pageShift) + bitIndex;
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::PopLowestSetBit() {
	const size_t index = IndexOfLowestSetBit();
	Set<false>(index);
	return index;
}

BITMASK_TEMPLATE
constexpr void BITMASK::ClearLowestSetBit() {
	Deep_Assert(Any(), "No bits are set, cannot clear lowest set bit.");

	size_t chunkIndex = 0;
	constexpr size_t rootLevel = k_numLevels - 1;
	for (size_t level = rootLevel; level > 0; --level) {
		const ChunkType chunk = m_chunks[k_levelOffsets[level] + chunkIndex];
		const size_t childIndex = Deep::CountTrailingZeros(chunk);
		chunkIndex = (chunkIndex << k_pageShift) + childIndex;
	}

	ChunkType& leaf = m_chunks[chunkIndex];
	leaf = static_cast<ChunkType>(leaf & (leaf - ChunkType{ 1 }));
	if (leaf == 0) {
		PropagateOccupancy(0, chunkIndex, false);
	}
}

BITMASK_TEMPLATE
constexpr void BITMASK::Clear() {
	Deep::TMemset<ChunkType>(m_chunks.m_Ptr(), 0, k_numChunks);
}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator|=(ARG_BITMASK in_other) {
	for (size_t i = 0; i < k_numChunks; ++i) {
		m_chunks[i] |= in_other.m_chunks[i];
	}
	return *this;
}
BITMASK_TEMPLATE
constexpr BITMASK operator|(BITMASK in_a, ARG_BITMASK in_b) {
	return in_a |= in_b;
}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator&=(ARG_BITMASK in_other) {
	for (size_t i = 0; i < k_numLeafChunks; ++i) {
		m_chunks[i] &= in_other.m_chunks[i];
	}

	RebuildHierarchy();
	return *this;
}
BITMASK_TEMPLATE
constexpr BITMASK operator&(BITMASK in_a, ARG_BITMASK in_b) {
	return in_a &= in_b;
}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator^=(ARG_BITMASK in_other) {
	for (size_t i = 0; i < k_numLeafChunks; ++i) {
		m_chunks[i] ^= in_other.m_chunks[i];
	}

	RebuildHierarchy();
	return *this;
}
BITMASK_TEMPLATE
constexpr BITMASK operator^(BITMASK in_a, ARG_BITMASK in_b) {
	return in_a ^= in_b;
}

BITMASK_TEMPLATE
constexpr BITMASK operator~(BITMASK in_a) {
	in_a.Inverse();
	return in_a;
}

BITMASK_TEMPLATE
constexpr bool operator==(ARG_BITMASK in_a, ARG_BITMASK in_b) {
	constexpr auto k_numLeafChunks = BITMASK::k_numLeafChunks;
	for (size_t i = 0; i < k_numLeafChunks; ++i) {
		if (in_a.m_chunks[i] != in_b.m_chunks[i]) return false;
	}
	return true;
}
BITMASK_TEMPLATE
constexpr bool operator!=(ARG_BITMASK in_a, ARG_BITMASK in_b) {
	return !(in_a == in_b);
}

#undef BITMASK_TEMPLATE
#undef BITMASK

DEEP_NAMESPACE_END
