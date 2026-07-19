#pragma once

#include "./SoALayout.h"

DEEP_NAMESPACE_BEGIN

template<typename... Ts>
constexpr SoALayout<Ts...>::SoALayout(size_t in_itemSize) :
	m_itemCount(in_itemSize) {
	size_t offset = 0;

	for (size_t i = 0; i < k_numArrays; ++i) {
		offset = detail::_SoALayout::AlignUp(offset, k_alignments[i]);

		m_offsets[i] = offset;

		offset += k_elementSizes[i] * m_itemCount;
	}

	m_size = offset;
}

template<typename... Ts>
constexpr const size_t& SoALayout<Ts...>::ItemCount() const {
	return m_itemCount;
}

template<typename... Ts>
constexpr const size_t& SoALayout<Ts...>::Size() const {
	return m_size;
}

template<typename... Ts>
constexpr const size_t& SoALayout<Ts...>::Alignment() const {
	return k_alignment;
}

template<typename... Ts>
template<typename T>
constexpr size_t SoALayout<Ts...>::OffsetOf() const {
	constexpr size_t index = TypeIndex<T, Ts...>::value;
	return m_offsets[index];
}

template<typename... Ts>
template<typename T>
constexpr size_t SoALayout<Ts...>::SizeOf() const {
	return sizeof(T) * m_itemCount;
}

DEEP_NAMESPACE_END
