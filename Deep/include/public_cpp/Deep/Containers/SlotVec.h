#pragma once

#include "Deep.h"
#include <type_traits>
#include <vector>

DEEP_NAMESPACE_BEGIN

// Dense storage with stable IDs. Access and removal are O(1); append is amortized O(1).
// Removing an item invalidates its ID, which may be reused by the next append. References and iterators are not stable
// across growth or removal. Iteration visits live items in slot order, which can change on removal.
template<typename T>
class SlotVec {
	static_assert(std::is_object_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T> && !std::is_same_v<T, bool>,
	              "T must be a non-const, non-volatile value type other than bool.");

public:
	inline SlotVec(const SlotVec&)
		requires std::is_copy_constructible_v<T>
	= default;
	inline SlotVec(SlotVec&& in_other) noexcept;
	inline SlotVec& operator=(const SlotVec& in_other)
		requires std::is_copy_constructible_v<T>;
	inline SlotVec& operator=(SlotVec&& in_other) noexcept;
	inline SlotVec() = default;
	explicit inline SlotVec(size_t in_capacity);

	//

	inline T& operator[](size_t in_id);
	inline const T& operator[](size_t in_id) const;

	//

	inline size_t GetId(size_t in_slot) const;
	inline size_t GetSlot(size_t in_id) const;
	inline bool IsActive(size_t in_id) const;

	inline size_t Size() const;
	inline void Reserve(size_t in_capacity);

	inline void Clear();

	// Construct an item and return its stable ID. Arguments may refer to existing items.
	template<typename... Args>
	inline size_t Emplace(Args&&... in_args);
	inline size_t PushBack(const T& in_item);
	inline size_t PushBack(T&& in_item);

	// Swap with the last slot and destroy the removed item. T must support swapping.
	inline void Remove(size_t in_id);

	inline void Swap(SlotVec& in_other) noexcept;

	inline typename std::vector<T>::iterator begin();
	inline typename std::vector<T>::const_iterator begin() const;
	inline typename std::vector<T>::iterator end();
	inline typename std::vector<T>::const_iterator end() const;

private:
	DEEP_PRIVATE_TESTABLE

	std::vector<T> m_slots;
	std::vector<size_t> m_idToSlot;
	std::vector<size_t> m_slotToId;
};

DEEP_NAMESPACE_END

#include "./SlotVec.inl" // IWYU pragma: export
