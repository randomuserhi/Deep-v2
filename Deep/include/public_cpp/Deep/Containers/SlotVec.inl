#pragma once

#include "./SlotVec.h"

#include <cstdlib>
#include <utility>

DEEP_NAMESPACE_BEGIN

template<typename T>
SlotVec<T>::SlotVec(size_t in_capacity) {
	Reserve(in_capacity);
}

template<typename T>
SlotVec<T>::SlotVec(SlotVec&& in_other) noexcept {
	Swap(in_other);
}

template<typename T>
SlotVec<T>& SlotVec<T>::operator=(const SlotVec& in_other)
	requires std::is_copy_constructible_v<T>
{
	if (this == &in_other) return *this;
	SlotVec temp{ in_other };
	Swap(temp);
	return *this;
}

template<typename T>
SlotVec<T>& SlotVec<T>::operator=(SlotVec&& in_other) noexcept {
	if (this == &in_other) return *this;
	SlotVec temp{ std::move(in_other) };
	Swap(temp);
	return *this;
}

template<typename T>
size_t SlotVec<T>::Size() const {
	return m_slots.size();
}

template<typename T>
bool SlotVec<T>::IsActive(Handle in_handle) const {
	const Handle index = in_handle >> k_versionBits;
	if (index >= m_handleToSlot.size()) return false;
	const size_t slot = m_handleToSlot[index];
	return slot < Size() && m_slotToHandle[slot] == in_handle;
}

template<typename T>
T& SlotVec<T>::operator[](Handle in_handle) {
	Deep_Assert(IsActive(in_handle), "Inactive SlotVec handle.");
	return m_slots[m_handleToSlot[in_handle >> k_versionBits]];
}

template<typename T>
const T& SlotVec<T>::operator[](Handle in_handle) const {
	Deep_Assert(IsActive(in_handle), "Inactive SlotVec handle.");
	return m_slots[m_handleToSlot[in_handle >> k_versionBits]];
}

template<typename T>
template<typename... Args>
typename SlotVec<T>::Handle SlotVec<T>::Emplace(Args&&... in_args) {
	const size_t slot = Size();
	if (slot >= k_maxCapacity) {
		Deep_Assert(false, "SlotVec capacity exceeds the 48-bit index limit.");
		std::abort();
	}
	if (slot == m_slotToHandle.size()) {
		// Grow the maps first so arguments referring to existing items remain valid.
#if DEEP_CPP_EXCEPTIONS_ENABLED
		try {
#endif
			m_handleToSlot.push_back(slot);
			m_slotToHandle.push_back(static_cast<Handle>(slot) << k_versionBits);
			m_slots.emplace_back(std::forward<Args>(in_args)...);
#if DEEP_CPP_EXCEPTIONS_ENABLED
		} catch (...) {
			// Restore both maps if allocation or item construction fails.
			m_handleToSlot.resize(slot);
			m_slotToHandle.resize(slot);
			throw;
		}
#endif
	} else {
		m_slots.emplace_back(std::forward<Args>(in_args)...);
	}
	return m_slotToHandle[slot];
}

template<typename T>
typename SlotVec<T>::Handle SlotVec<T>::PushBack(const T& in_item) {
	return Emplace(in_item);
}

template<typename T>
typename SlotVec<T>::Handle SlotVec<T>::PushBack(T&& in_item) {
	return Emplace(std::move(in_item));
}

template<typename T>
void SlotVec<T>::Remove(Handle in_handle) {
	Deep_Assert(IsActive(in_handle), "Inactive SlotVec handle.");
	const size_t slot = m_handleToSlot[in_handle >> k_versionBits];
	const size_t last = Size() - 1;
	if (slot != last) {
		std::swap(m_slots[slot], m_slots[last]);
		std::swap(m_slotToHandle[slot], m_slotToHandle[last]);
		m_handleToSlot[m_slotToHandle[slot] >> k_versionBits] = slot;
		m_handleToSlot[in_handle >> k_versionBits] = last;
	}
	// Increment only the version, without carrying into the index on wraparound.
	m_slotToHandle[last] = (in_handle & ~k_versionMask) | ((in_handle + 1) & k_versionMask);
	m_slots.pop_back();
}

template<typename T>
void SlotVec<T>::Clear() {
	for (size_t slot = 0; slot < Size(); ++slot) {
		const Handle handle = m_slotToHandle[slot];
		m_slotToHandle[slot] = (handle & ~k_versionMask) | ((handle + 1) & k_versionMask);
	}
	m_slots.clear();
}

template<typename T>
void SlotVec<T>::Reserve(size_t in_capacity) {
	if (in_capacity > k_maxCapacity) {
		Deep_Assert(false, "SlotVec capacity exceeds the 48-bit index limit.");
		std::abort();
	}
	m_handleToSlot.reserve(in_capacity);
	m_slotToHandle.reserve(in_capacity);
	m_slots.reserve(in_capacity);
}

template<typename T>
void SlotVec<T>::Swap(SlotVec& in_other) noexcept {
	m_slots.swap(in_other.m_slots);
	m_handleToSlot.swap(in_other.m_handleToSlot);
	m_slotToHandle.swap(in_other.m_slotToHandle);
}

template<typename T>
typename SlotVec<T>::Handle SlotVec<T>::GetId(size_t in_slot) const {
	Deep_Assert(in_slot < Size(), "Out of range.");
	return m_slotToHandle[in_slot];
}

template<typename T>
size_t SlotVec<T>::GetSlot(Handle in_handle) const {
	Deep_Assert(IsActive(in_handle), "Inactive SlotVec handle.");
	return m_handleToSlot[in_handle >> k_versionBits];
}

template<typename T>
typename std::vector<T>::iterator SlotVec<T>::begin() {
	return m_slots.begin();
}

template<typename T>
typename std::vector<T>::const_iterator SlotVec<T>::begin() const {
	return m_slots.begin();
}

template<typename T>
typename std::vector<T>::iterator SlotVec<T>::end() {
	return m_slots.end();
}

template<typename T>
typename std::vector<T>::const_iterator SlotVec<T>::end() const {
	return m_slots.end();
}

DEEP_NAMESPACE_END
