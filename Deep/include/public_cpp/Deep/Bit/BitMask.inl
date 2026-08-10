#pragma once

#include "./BitMask.h"

DEEP_NAMESPACE_BEGIN

#define BITMASK_TEMPLATE template<size_t in_N>
#define BITMASK BitMask<in_N>
#define ARG_BITMASK Arg_BitMask<in_N>

BITMASK_TEMPLATE
BITMASK::BitMask(unsigned long long in_value) :
	m_bits{ in_value } {}

BITMASK_TEMPLATE
constexpr bool BITMASK::Test(size_t in_index) const {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");
	return m_bits.test(in_index);
}

BITMASK_TEMPLATE
constexpr bool BITMASK::Any() const {
	return m_bits.any();
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::NumSetBits() const {
	return m_bits.count();
}

BITMASK_TEMPLATE
constexpr void BITMASK::Set(size_t in_index, bool in_value) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");
	m_bits[in_index] = in_value;
}

BITMASK_TEMPLATE
template<bool in_value>
constexpr void BITMASK::Set(size_t in_index) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	if constexpr (in_value) {
		m_bits.set(in_index);
	} else {
		m_bits.reset(in_index);
	}
}

BITMASK_TEMPLATE
constexpr void BITMASK::Inverse() {
	m_bits = ~m_bits;
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::IndexOfLowestSetBit() const {
	Deep_Assert(Any(), "No bits are set, cannot get lowest set bit.");
	// TODO(randomuserhi): Migrate to use our own implementation of std::bitset
	//                     so we can optimize this operation to work in chunks.
	for (std::size_t i = 0; i < in_N; ++i) {
		if (m_bits[i]) return i;
	}
	return in_N;
}

BITMASK_TEMPLATE
constexpr size_t BITMASK::PopLowestSetBit() {
	const size_t index = IndexOfLowestSetBit();
	ClearLowestSetBit();
	return index;
}

BITMASK_TEMPLATE
constexpr void BITMASK::ClearLowestSetBit() {
	Deep_Assert(Any(), "No bits are set, cannot clear lowest set bit.");
	m_bits &= m_bits - 1;
}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator|=(ARG_BITMASK in_other) {
	m_bits |= in_other.m_bits;
	return *this;
}
BITMASK_TEMPLATE
constexpr Deep_ForceInline BITMASK operator|(BITMASK in_a, ARG_BITMASK in_b) {
	return in_a |= in_b;
}

BITMASK_TEMPLATE
constexpr BITMASK& BITMASK::operator&=(ARG_BITMASK in_other) {
	m_bits &= in_other.m_bits;
	return *this;
}
BITMASK_TEMPLATE
constexpr Deep_ForceInline BITMASK operator&(BITMASK in_a, ARG_BITMASK in_b) {
	return in_a &= in_b;
}

BITMASK_TEMPLATE
constexpr Deep_ForceInline BITMASK operator~(BITMASK in_a) {
	in_a.Inverse();
	return in_a;
}

BITMASK_TEMPLATE
constexpr Deep_ForceInline bool operator==(ARG_BITMASK in_a, ARG_BITMASK in_b) {
	return in_a.m_bits == in_b.m_bits;
}

#undef BITMASK_TEMPLATE
#undef BITMASK

DEEP_NAMESPACE_END
