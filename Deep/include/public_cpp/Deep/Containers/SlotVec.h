#pragma once

#include "Deep.h"
#include "Deep/Math/Ops.h"

#include <limits>
#include <type_traits>
#include <vector>

DEEP_NAMESPACE_BEGIN

// Dense storage with stable IDs. Access and removal are O(1); append is amortized O(1).
// IDs contain a 48-bit index and a 16-bit version, limiting capacity to 2^48 items.
// Removing an item or clearing the container increments its version before reuse. Versions wrap after 65,536
// removals of the same index, at which point an old ID can match again. IDs belong to a container's current contents;
// copying preserves IDs, while assignment and swapping replace the contents and their IDs.
// References and iterators are not stable across growth or removal. Iteration visits live items in slot order,
// which can change on removal. Allocation limits may be lower than k_maxCapacity on a given platform.
template<typename T>
class SlotVec {
	static_assert(std::is_object_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T> && !std::is_same_v<T, bool>,
	              "T must be a non-const, non-volatile value type other than bool.");

public:
	using Handle = uint64;

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

	inline T& operator[](Handle in_handle);
	inline const T& operator[](Handle in_handle) const;

	//

	inline Handle GetId(size_t in_slot) const;
	inline size_t GetSlot(Handle in_handle) const;
	inline bool IsActive(Handle in_handle) const;

	inline size_t Size() const;
	inline void Reserve(size_t in_capacity);

	inline void Clear();

	// Construct an item and return its stable ID. Arguments may refer to existing items.
	template<typename... Args>
	inline Handle Emplace(Args&&... in_args);
	inline Handle PushBack(const T& in_item);
	inline Handle PushBack(T&& in_item);

	// Swap with the last slot and destroy the removed item. T must support swapping.
	inline void Remove(Handle in_handle);

	inline void Swap(SlotVec& in_other) noexcept;

	inline typename std::vector<T>::iterator begin();
	inline typename std::vector<T>::const_iterator begin() const;
	inline typename std::vector<T>::iterator end();
	inline typename std::vector<T>::const_iterator end() const;

	//

	constexpr static uint64 k_maxCapacity = uint64{ 1 } << 48;

private:
	DEEP_PRIVATE_TESTABLE

	constexpr static size_t k_versionBits = 16;
	constexpr static Handle k_versionMask = (Handle{ 1 } << k_versionBits) - 1;

	std::vector<T> m_slots;
	std::vector<size_t> m_handleToSlot;
	// Live IDs followed by reusable IDs. Retained across Clear() to preserve versions.
	std::vector<Handle> m_slotToHandle;
};

DEEP_NAMESPACE_END

#include "./SlotVec.inl" // IWYU pragma: export
