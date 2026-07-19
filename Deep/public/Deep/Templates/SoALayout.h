#pragma once

#include "Deep.h"
#include "Deep/Templates/TypeLists.h"

#include <array>

DEEP_NAMESPACE_BEGIN

namespace impl_SoALayout {

// Increase an offset the nearest alignment boundary
constexpr size_t AlignUp(size_t in_offset, size_t in_alignment) {
	return ((in_offset + in_alignment - 1) / in_alignment) * in_alignment;
}

} // namespace impl_SoALayout

// Utility for managing a single allocation for multiple arrays, where each array stores `in_size` elements.
//
// The layout keeps track of each array's offset and size within a raw allocation such that they follow the memory layout:
// [T0 array][padding][T1 array][padding][T2 array]
//
// Offsets and sizes are in bytes to allow indexing of a raw byte buffer.
template<typename... Ts>
class SoALayout {
public:
	explicit constexpr SoALayout(size_t in_itemSize);

	// Number of items each array contains
	[[nodiscard]] constexpr Deep_ForceInline const size_t& ItemCount() const;

	// Size in bytes required to store a SoA of this layout
	[[nodiscard]] constexpr Deep_ForceInline const size_t& Size() const;

	// Alignment memory required to store a SoA of this layout
	[[nodiscard]] constexpr Deep_ForceInline const size_t& Alignment() const;

	template<typename T>
	[[nodiscard]] constexpr size_t OffsetOf() const;

	template<typename T>
	[[nodiscard]] constexpr size_t SizeOf() const;

private:
	// Number of items each array contains
	size_t m_itemCount;

	// Total number of bytes the entire layout requires
	size_t m_size;

	// Offset information of array
	static constexpr size_t k_numArrays = sizeof...(Ts);
	std::array<size_t, k_numArrays> m_offsets;

	//

	static constexpr size_t k_alignment = MaxAlignment<Ts...>::value;

	constexpr static std::array<size_t, k_numArrays> k_elementSizes = { sizeof(Ts)... };
	constexpr static std::array<size_t, k_numArrays> k_alignments = { alignof(Ts)... };
};

DEEP_NAMESPACE_END

#include "Deep/Templates/SoALayout.inl" // IWYU pragma: export
