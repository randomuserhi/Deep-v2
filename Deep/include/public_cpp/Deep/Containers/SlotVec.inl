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
bool SlotVec<T>::IsActive(size_t in_id) const {
	return in_id < m_idToSlot.size() && m_idToSlot[in_id] < Size();
}

template<typename T>
T& SlotVec<T>::operator[](size_t in_id) {
	Deep_Assert(IsActive(in_id), "Inactive SlotVec ID.");
	return m_slots[m_idToSlot[in_id]];
}

template<typename T>
const T& SlotVec<T>::operator[](size_t in_id) const {
	Deep_Assert(IsActive(in_id), "Inactive SlotVec ID.");
	return m_slots[m_idToSlot[in_id]];
}

template<typename T>
template<typename... Args>
size_t SlotVec<T>::Emplace(Args&&... in_args) {
	const size_t slot = Size();
	if (slot == m_slotToId.size()) {
		// Grow the maps first so arguments referring to existing items remain valid.
#if DEEP_CPP_EXCEPTIONS_ENABLED
		try {
#endif
			m_idToSlot.push_back(slot);
			m_slotToId.push_back(slot);
			m_slots.emplace_back(std::forward<Args>(in_args)...);
#if DEEP_CPP_EXCEPTIONS_ENABLED
		} catch (...) {
			// Restore both maps if allocation or item construction fails.
			m_idToSlot.resize(slot);
			m_slotToId.resize(slot);
			throw;
		}
#endif
	} else {
		m_slots.emplace_back(std::forward<Args>(in_args)...);
	}
	return m_slotToId[slot];
}

template<typename T>
size_t SlotVec<T>::PushBack(const T& in_item) {
	return Emplace(in_item);
}

template<typename T>
size_t SlotVec<T>::PushBack(T&& in_item) {
	return Emplace(std::move(in_item));
}

template<typename T>
void SlotVec<T>::Remove(size_t in_id) {
	Deep_Assert(IsActive(in_id), "Inactive SlotVec ID.");
	const size_t slot = m_idToSlot[in_id];
	const size_t last = Size() - 1;
	if (slot != last) {
		std::swap(m_slots[slot], m_slots[last]);
		std::swap(m_slotToId[slot], m_slotToId[last]);
		m_idToSlot[m_slotToId[slot]] = slot;
		m_idToSlot[in_id] = last;
	}
	m_slots.pop_back();
}

template<typename T>
void SlotVec<T>::Clear() {
	m_slots.clear();
	m_idToSlot.clear();
	m_slotToId.clear();
}

template<typename T>
void SlotVec<T>::Reserve(size_t in_capacity) {
	m_idToSlot.reserve(in_capacity);
	m_slotToId.reserve(in_capacity);
	m_slots.reserve(in_capacity);
}

template<typename T>
void SlotVec<T>::Swap(SlotVec& in_other) noexcept {
	m_slots.swap(in_other.m_slots);
	m_idToSlot.swap(in_other.m_idToSlot);
	m_slotToId.swap(in_other.m_slotToId);
}

template<typename T>
size_t SlotVec<T>::GetId(size_t in_slot) const {
	Deep_Assert(in_slot < Size(), "Out of range.");
	return m_slotToId[in_slot];
}

template<typename T>
size_t SlotVec<T>::GetSlot(size_t in_id) const {
	Deep_Assert(IsActive(in_id), "Inactive SlotVec ID.");
	return m_idToSlot[in_id];
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
