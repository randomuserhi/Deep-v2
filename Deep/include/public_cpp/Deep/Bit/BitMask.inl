#pragma once

#include "./BitMask.h"

#include "Deep/Bit.h"
#include "Deep/Memory.h"

DEEP_NAMESPACE_BEGIN

namespace detail::_BitMask {

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

} // namespace detail::_BitMask

#define BITMASK_TEMPLATE template<size_t in_N, StoragePolicy in_Policy>
#define BITMASK BitMask<in_N, in_Policy>
#define ARG_BITMASK Arg_BitMask<in_N, in_Policy>

BITMASK_TEMPLATE
constexpr BITMASK::BitMask(std::nullptr_t) :
	BitMask{} {}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator=(std::nullptr_t) {
	Clear();
	return *this;
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
	for (size_t i = 0; i < k_numChunks; ++i) {
		if (m_chunks[i] != 0) return true;
	}
	return false;
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::NumSetBits() const {
	size_t count = 0;
	for (size_t i = 0; i < k_numChunks; ++i) {
		count += Deep::CountSetBits(m_chunks[i]);
	}
	return count;
}

BITMASK_TEMPLATE
constexpr void BITMASK::Set(size_t in_index, bool in_value) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	const size_t chunkIndex = in_index >> k_pageShift;
	const size_t bitIndex = in_index & k_itemMask;
	ChunkType& chunk = m_chunks[chunkIndex];

	const ChunkType clear_mask = ~static_cast<ChunkType>(ChunkType{ 1 } << bitIndex);
	const ChunkType new_bit = static_cast<ChunkType>(ChunkType{ in_value } << bitIndex);
	chunk = (chunk & clear_mask) | new_bit;
}

BITMASK_TEMPLATE
template<bool in_value>
constexpr void BITMASK::Set(size_t in_index) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	const size_t chunkIndex = in_index >> k_pageShift;
	const size_t bitIndex = in_index & k_itemMask;
	ChunkType& chunk = m_chunks[chunkIndex];

	const ChunkType mask = static_cast<ChunkType>(ChunkType{ 1 } << bitIndex);
	if constexpr (in_value) {
		chunk |= mask;
	} else {
		chunk &= ~mask;
	}
}

BITMASK_TEMPLATE
constexpr void BITMASK::Inverse() {
	for (size_t i = 0; i < k_numChunks; ++i) {
		m_chunks[i] = static_cast<ChunkType>(~m_chunks[i]);
	}
	constexpr size_t usedBitsInLastChunk = in_N & k_itemMask;
	if constexpr (usedBitsInLastChunk != 0) {
		constexpr ChunkType mask = static_cast<ChunkType>((ChunkType{ 1 } << usedBitsInLastChunk) - 1);
		m_chunks[k_numChunks - 1] &= mask;
	}
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::IndexOfLowestSetBit() const {
	Deep_Assert(Any(), "No bits are set, cannot get lowest set bit.");
	for (size_t i = 0; i < k_numChunks; ++i) {
		if (m_chunks[i] != 0) {
			return (i << k_pageShift) + Deep::CountTrailingZeros(m_chunks[i]);
		}
	}
	return in_N;
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
	for (size_t i = 0; i < k_numChunks; ++i) {
		if (m_chunks[i] != 0) {
			m_chunks[i] &= m_chunks[i] - 1;
			return;
		}
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
	for (size_t i = 0; i < k_numChunks; ++i) {
		m_chunks[i] &= in_other.m_chunks[i];
	}
	return *this;
}
BITMASK_TEMPLATE
constexpr BITMASK operator&(BITMASK in_a, ARG_BITMASK in_b) {
	return in_a &= in_b;
}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator^=(ARG_BITMASK in_other) {
	for (size_t i = 0; i < k_numChunks; ++i) {
		m_chunks[i] ^= in_other.m_chunks[i];
	}
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
	constexpr auto k_numChunks = BITMASK::k_numChunks;
	for (size_t i = 0; i < k_numChunks; ++i) {
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
