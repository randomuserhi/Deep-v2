#pragma once

#include "Deep.h"
#include "Deep/Bit/IntegerBitMask.h"

DEEP_NAMESPACE_BEGIN

#define INTEGER_BITMASK_TEMPLATE template<c_UnsignedInteger T>
#define INTEGER_BITMASK IntegerBitMask<T>

INTEGER_BITMASK_TEMPLATE
INTEGER_BITMASK::IntegerBitMask(T in_value) :
	m_bits{ in_value } {};

INTEGER_BITMASK_TEMPLATE
constexpr bool INTEGER_BITMASK::operator[](size_t in_index) const {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	T flag = static_cast<T>(1 << in_index);
	return (m_bits & flag) != 0;
}

INTEGER_BITMASK_TEMPLATE
constexpr bool INTEGER_BITMASK::Any() const {
	return m_bits != 0;
}

INTEGER_BITMASK_TEMPLATE
constexpr size_t INTEGER_BITMASK::NumSetBits() const {
	return CountSetBits(m_bits);
}

INTEGER_BITMASK_TEMPLATE
constexpr void INTEGER_BITMASK::Set(size_t in_index, bool in_value) {
	Deep_Assert(in_index < k_maxNumBits, "Index out of range.");

	const T clear_mask = static_cast<T>(~(1 << in_index));
	const T new_bit = static_cast<T>(in_value) << in_index;
	m_bits = (m_bits & clear_mask) | new_bit;
}

INTEGER_BITMASK_TEMPLATE
constexpr void INTEGER_BITMASK::Inverse() {
	m_bits = ~m_bits;
}

INTEGER_BITMASK_TEMPLATE
constexpr size_t INTEGER_BITMASK::IndexOfLowestSetBit() const {
	Deep_Assert(Any(), "No bits are set, cannot get lowest set bit.");

	return CountTrailingZeros(m_bits);
}

INTEGER_BITMASK_TEMPLATE
constexpr size_t INTEGER_BITMASK::PopLowestSetBit() {
	const size_t index = IndexOfLowestSetBit();
	m_bits &= m_bits - 1;
	return index;
}

INTEGER_BITMASK_TEMPLATE
constexpr INTEGER_BITMASK& INTEGER_BITMASK::operator|=(Arg_IntegerBitMask<T> in_other) {
	m_bits |= in_other.m_bits;
	return *this;
}
INTEGER_BITMASK_TEMPLATE
constexpr Deep_ForceInline INTEGER_BITMASK operator|(INTEGER_BITMASK in_a, Arg_IntegerBitMask<T> in_b) {
	return in_a |= in_b;
}

INTEGER_BITMASK_TEMPLATE
constexpr INTEGER_BITMASK& INTEGER_BITMASK::operator&=(Arg_IntegerBitMask<T> in_other) {
	m_bits &= in_other.m_bits;
	return *this;
}
INTEGER_BITMASK_TEMPLATE
constexpr Deep_ForceInline INTEGER_BITMASK operator&(INTEGER_BITMASK in_a, Arg_IntegerBitMask<T> in_b) {
	return in_a &= in_b;
}

INTEGER_BITMASK_TEMPLATE
constexpr Deep_ForceInline INTEGER_BITMASK operator~(INTEGER_BITMASK in_a) {
	in_a.Inverse();
	return in_a;
}

INTEGER_BITMASK_TEMPLATE
constexpr Deep_ForceInline bool operator==(Arg_IntegerBitMask<T> in_a, Arg_IntegerBitMask<T> in_b) {
	return in_a.m_bits == in_b.m_bits;
}

#undef INTEGER_BITMASK_TEMPLATE
#undef INTEGER_BITMASK

DEEP_NAMESPACE_END
