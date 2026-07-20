#pragma once

#include "./SetBit.h"

#include "Deep.h"
#include "Deep/Memory.h"
#include "Deep/Templates/ConstructWith.h"
#include "Deep/Templates/TypeLists.h"

#include <malloc.h>
#include <type_traits>
#include <utility>

DEEP_NAMESPACE_BEGIN

#define ARCHETYPE_ITERATOR_TEMPLATE template<c_BitMask in_BitMask, typename... Components>
#define ARCHETYPE_ITERATOR SetBitArchetypeIterator<in_BitMask, Components...>

ARCHETYPE_ITERATOR_TEMPLATE
ARCHETYPE_ITERATOR::SetBitArchetypeIterator(BitMask in_mask, Components*... in_components) :
	m_remaining{ in_mask }, m_components{ in_components... } {}

ARCHETYPE_ITERATOR_TEMPLATE
template<std::size_t... Is>
ARCHETYPE_ITERATOR::reference ARCHETYPE_ITERATOR::Deref(std::index_sequence<Is...>) const {
	Deep_Assert(*this != Sentinel{}, "End of iterator.");
	const size_t index = m_remaining.IndexOfLowestSetBit();
	return reference{ Get<Is>(m_components)[index]... };
}

ARCHETYPE_ITERATOR_TEMPLATE
ARCHETYPE_ITERATOR::reference ARCHETYPE_ITERATOR::operator*() const {
	return Deref(std::index_sequence_for<Components...>{});
}

ARCHETYPE_ITERATOR_TEMPLATE
bool ARCHETYPE_ITERATOR::operator==(Sentinel) const {
	return !m_remaining.Any();
}

ARCHETYPE_ITERATOR_TEMPLATE
bool ARCHETYPE_ITERATOR::operator!=(Sentinel) const {
	return m_remaining.Any();
}

ARCHETYPE_ITERATOR_TEMPLATE
ARCHETYPE_ITERATOR& ARCHETYPE_ITERATOR::operator++() {
	Deep_Assert(*this != Sentinel{}, "End of iterator.");
	m_remaining.PopLowestSetBit();
	return *this;
}

#undef ARCHETYPE_ITERATOR_TEMPLATE
#undef ARCHETYPE_ITERATOR

#define ARCHETYPE_VIEW_TEMPLATE template<c_BitMask in_BitMask, typename... Components>
#define ARCHETYPE_VIEW SetBitArchetypeView<in_BitMask, Components...>

ARCHETYPE_VIEW_TEMPLATE
ARCHETYPE_VIEW::SetBitArchetypeView(BitMask in_activeMask, Components*... in_components) :
	m_activeMask{ in_activeMask }, m_components{ in_components... } {}

ARCHETYPE_VIEW_TEMPLATE
template<std::size_t... Is>
SetBitArchetypeIterator<typename ARCHETYPE_VIEW::BitMask, Components...>
ARCHETYPE_VIEW::CreateIterator(std::index_sequence<Is...>) const {
	return SetBitArchetypeIterator<BitMask, Components...>{ m_activeMask, Get<Is>(m_components)... };
}

ARCHETYPE_VIEW_TEMPLATE
SetBitArchetypeIterator<typename ARCHETYPE_VIEW::BitMask, Components...> ARCHETYPE_VIEW::begin() const {
	return CreateIterator(std::index_sequence_for<Components...>{});
}

ARCHETYPE_VIEW_TEMPLATE
Sentinel ARCHETYPE_VIEW::end() const {
	return {};
}

#undef ARCHETYPE_VIEW_TEMPLATE
#undef ARCHETYPE_VIEW

template<typename T>
const T* detail::_SetBitArchetype::ComponentStorage<T>::Get() const {
	return m_data;
}

template<typename T>
T* detail::_SetBitArchetype::ComponentStorage<T>::Get() {
	return m_data;
}

#define FIXED_SIZE_ARCHETYPE_TEMPLATE template<c_BitMask in_BitMask, typename... Components>
#define FIXED_SIZE_ARCHETYPE FixedSizeSetBitArchetype<in_BitMask, Components...>

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
inline void FIXED_SIZE_ARCHETYPE::SetComponentPtr(void* in_ptr) {
	static_cast<detail::_SetBitArchetype::ComponentStorage<T>&>(*this).m_data = static_cast<T*>(in_ptr);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
inline void FIXED_SIZE_ARCHETYPE::AllocateStorage() {
	Layout layout{ m_capacity };
	byte* ptr = static_cast<byte*>(AlignedMalloc(layout.m_Size(), layout.m_Alignment()));
	(SetComponentPtr<Components>(ptr + layout.template OffsetOf<Components>()), ...);
	Deep_Assert((static_cast<const void*>(GetComponentPtr<TypeAt<0, Components...>>()) == static_cast<const void*>(ptr)),
	            "First component ptr must be the same as the main storage pointer.");
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
inline void FIXED_SIZE_ARCHETYPE::DeallocateStorage() {
	// The first component in the type list should be the main storage pointer
	detail::_SetBitArchetype::ComponentStorage<TypeAt<0, Components...>>& storage =
		static_cast<detail::_SetBitArchetype::ComponentStorage<TypeAt<0, Components...>>&>(*this);
	AlignedFree(storage.m_data);

	// Clear component pointers
	(SetComponentPtr<Components>(nullptr), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
inline void FIXED_SIZE_ARCHETYPE::MoveComponent(FixedSizeSetBitArchetype&& in_other) {
	detail::_SetBitArchetype::ComponentStorage<T>& storage =
		static_cast<detail::_SetBitArchetype::ComponentStorage<T>&>(*this);
	detail::_SetBitArchetype::ComponentStorage<T>& other =
		static_cast<detail::_SetBitArchetype::ComponentStorage<T>&>(in_other);

	storage.m_data = other.m_data;
	other.m_data = nullptr;
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
FIXED_SIZE_ARCHETYPE::FixedSizeSetBitArchetype(size_t in_capacity) :
	m_activeMask{ 0 }, m_capacity{ in_capacity } {
	Deep_Assert(m_capacity > 0, "Archetype must have a non-zero size.");
	Deep_Assert(m_capacity <= k_maxCapacity, "Size must be smaller than k_maxCapacity.");
	AllocateStorage();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
FIXED_SIZE_ARCHETYPE::FixedSizeSetBitArchetype(const FixedSizeSetBitArchetype& in_other) :
	m_activeMask{ 0 }, m_capacity{ in_other.m_capacity } {
	Deep_Assert(m_capacity > 0, "Archetype must have a non-zero size.");
	Deep_Assert(m_capacity <= k_maxCapacity, "Size must be smaller than k_maxCapacity.");
	AllocateStorage();

	// Copy active entities
	BitMask remaining = in_other.m_activeMask;
	while (remaining.Any()) {
		const int32 index = remaining.PopLowestSetBit();

		// Construct components using copy constructor
		ConstructEntity(index, ConstructWith<Components>(in_other.GetComponent<Components>(index))...);
	}
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
FIXED_SIZE_ARCHETYPE::FixedSizeSetBitArchetype(FixedSizeSetBitArchetype&& in_other) noexcept :
	m_activeMask{ in_other.m_activeMask }, m_capacity{ in_other.m_capacity } {
	Deep_Assert(m_capacity > 0, "Archetype must have a non-zero size.");
	Deep_Assert(m_capacity <= k_maxCapacity, "Size must be smaller than k_maxCapacity.");

	// Invalidate and move buffers from the other archetype
	(MoveComponent<Components>(std::move(in_other)), ...);
	in_other.m_activeMask = 0;
	in_other.m_capacity = 0;
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
FIXED_SIZE_ARCHETYPE& FIXED_SIZE_ARCHETYPE::operator=(const FixedSizeSetBitArchetype& in_other) {
	if (this == &in_other) return *this;

	// Destruct current entities
	{
		BitMask remaining = m_activeMask;
		while (remaining.Any()) {
			const int32 index = remaining.PopLowestSetBit();

			DestructEntity(index);
		}
	}

	// Free current buffer
	DeallocateStorage();

	m_capacity = in_other.m_capacity;
	Deep_Assert(m_capacity > 0, "Archetype must have a non-zero size.");
	Deep_Assert(m_capacity <= k_maxCapacity, "Size must be smaller than k_maxCapacity.");

	// Allocate new buffer
	AllocateStorage();

	// Copy active entities
	{
		BitMask remaining = in_other.m_activeMask;
		while (remaining.Any()) {
			const int32 index = remaining.PopLowestSetBit();

			// Construct components using copy constructor
			ConstructEntity(index, ConstructWith<Components>(in_other.GetComponent<Components>(index))...);
		}
	}

	return *this;
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
FIXED_SIZE_ARCHETYPE& FIXED_SIZE_ARCHETYPE::operator=(FixedSizeSetBitArchetype&& in_other) noexcept {
	if (this == &in_other) return *this;

	// Destruct current entities
	{
		BitMask remaining = m_activeMask;
		while (remaining.Any()) {
			const int32 index = remaining.PopLowestSetBit();

			DestructEntity(index);
		}
	}

	// Deallocate existing buffers
	DeallocateStorage();

	m_activeMask = in_other.m_activeMask;
	m_capacity = in_other.m_capacity;
	Deep_Assert(m_capacity > 0, "Archetype must have a non-zero size.");
	Deep_Assert(m_capacity <= k_maxCapacity, "Size must be smaller than k_maxCapacity.");

	// Invalidate and move buffers from the other archetype
	(MoveComponent<Components>(std::move(in_other)), ...);
	in_other.m_activeMask = 0;
	in_other.m_capacity = 0;

	return *this;
}

FIXED_SIZE_ARCHETYPE_TEMPLATE FIXED_SIZE_ARCHETYPE::~FixedSizeSetBitArchetype() {
	BitMask remaining = m_activeMask;
	while (remaining.Any()) {
		const int32 index = remaining.PopLowestSetBit();

		DestructEntity(index);
	}

	m_activeMask = 0;
	m_capacity = 0;
	DeallocateStorage();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
const T* FIXED_SIZE_ARCHETYPE::GetComponentPtr() const {
	static_assert(s_validComponent<T>, "Component type not in archetype");
	return static_cast<const detail::_SetBitArchetype::ComponentStorage<T>&>(*this).Get();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
T* FIXED_SIZE_ARCHETYPE::GetComponentPtr() {
	static_assert(s_validComponent<T>, "Component type not in archetype");
	return static_cast<detail::_SetBitArchetype::ComponentStorage<T>&>(*this).Get();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T, c_ConstructorArgs TaggedArg>
constexpr inline void FIXED_SIZE_ARCHETYPE::ConstructComponentImpl(TaggedArg&& in_componentArg, T* in_ptr) {
	if constexpr (std::is_same_v<T, typename std::decay_t<TaggedArg>::Type>) {
		std::move(in_componentArg).Construct(in_ptr);
	}
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T, c_ConstructorArgs... TaggedArgs>
constexpr inline void FIXED_SIZE_ARCHETYPE::ConstructComponent(size_t in_index, TaggedArgs&&... in_componentArgs) {
	T* ptr = GetComponentPtr<T>() + in_index;

	constexpr size_t occurences = CountConstructorArgs<T, TaggedArgs...>();
	if constexpr (occurences == 0) {
		::new (ptr) T{};
	} else if constexpr (occurences == 1) {
		(ConstructComponentImpl<T, TaggedArgs>(std::forward<TaggedArgs>(in_componentArgs), ptr), ...);
	} else {
		static_assert(false, "Multiple ComponentArgs were provided for the same component type. "
		                     "Ambigous as to which to use to construct the component.");
	}
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
inline void FIXED_SIZE_ARCHETYPE::DestructComponent(size_t in_index) {
	if constexpr (!std::is_trivially_destructible_v<T>) {
		GetComponentPtr<T>()[in_index].~T();
	}
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<c_ConstructorArgs... TaggedArgs>
void FIXED_SIZE_ARCHETYPE::ConstructEntity(size_t in_index, TaggedArgs&&... in_componentArgs) {
	static_assert((s_validComponent<typename std::decay_t<TaggedArgs>::Type> && ...),
	              "Archetype must contain all components that the given constructor args specify.");

	Deep_Assert(in_index < m_capacity, "Invalid index.");
	Deep_Assert(!IsActive(in_index), "An entity is already constructed here.");

	// Mark entity as active
	m_activeMask.Set(in_index, true);

	// Construct components
	(ConstructComponent<Components, TaggedArgs...>(in_index, std::forward<TaggedArgs>(in_componentArgs)...), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
void FIXED_SIZE_ARCHETYPE::DestructEntity(size_t in_index) {
	Deep_Assert(in_index < m_capacity, "Invalid index.");
	Deep_Assert(IsActive(in_index), "Cannot destruct an entity that has not been constructed yet.");

	// Mark entity as inactive
	m_activeMask.Set(in_index, false);

	// Destruct components
	(DestructComponent<Components>(in_index), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
bool FIXED_SIZE_ARCHETYPE::IsActive(size_t in_index) const {
	return m_activeMask[in_index];
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
size_t FIXED_SIZE_ARCHETYPE::Size() const {
	return m_activeMask.NumSetBits();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
const size_t& FIXED_SIZE_ARCHETYPE::m_Capacity() const {
	return m_capacity;
}

FIXED_SIZE_ARCHETYPE_TEMPLATE template<typename T>
const T& FIXED_SIZE_ARCHETYPE::GetComponent(size_t in_index) const {
	static_assert(s_validComponent<T>, "Archetype must contain component 'T'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return GetComponentPtr<T>()[in_index];
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
T& FIXED_SIZE_ARCHETYPE::GetComponent(size_t in_index) {
	static_assert(s_validComponent<T>, "Archetype must contain component 'T'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return GetComponentPtr<T>()[in_index];
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
Tuple<const Ts&...> FIXED_SIZE_ARCHETYPE::GetComponents(size_t in_index) const {
	static_assert((s_validComponent<Ts> && ...), "Archetype must contain all component 'Ts'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return Tuple<const Ts&...>(GetComponentPtr<Ts>()[in_index]...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
Tuple<Ts&...> FIXED_SIZE_ARCHETYPE::GetComponents(size_t in_index) {
	static_assert((s_validComponent<Ts> && ...), "Archetype must contain all component 'Ts'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return Tuple<Ts&...>(GetComponentPtr<Ts>()[in_index]...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
SetBitArchetypeView<typename FIXED_SIZE_ARCHETYPE::BitMask, Ts...> FIXED_SIZE_ARCHETYPE::View(BitMask in_mask) {
	// NOTE(randomuserhi): We `std::decay_t` the component type in views as we allow promotion of `Ts` to `const Ts`
	//                     to generate const views of components from non-const archetypes.
	static_assert((s_validComponent<typename std::decay_t<Ts>> && ...), "Archetype must contain all component 'Ts'.");
	in_mask &= m_activeMask;
	return SetBitArchetypeView<BitMask, Ts...>{ in_mask, GetComponentPtr<typename std::decay_t<Ts>>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
SetBitArchetypeView<typename FIXED_SIZE_ARCHETYPE::BitMask, const Ts...> FIXED_SIZE_ARCHETYPE::View(BitMask in_mask) const {
	// NOTE(randomuserhi): We `std::decay_t` the component type in views as we allow promotion of `Ts` to `const Ts`
	//                     to generate const views of components from non-const archetypes.
	//                     Although this is the const version, we still allow it to keep syntax consistent with the non-const
	//                     version.
	static_assert((s_validComponent<typename std::decay_t<Ts>> && ...), "Archetype must contain all component 'Ts'.");
	in_mask &= m_activeMask;
	return SetBitArchetypeView<BitMask, const Ts...>{ in_mask, GetComponentPtr<typename std::decay_t<Ts>>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
SetBitArchetypeIterator<typename FIXED_SIZE_ARCHETYPE::BitMask, Components...> FIXED_SIZE_ARCHETYPE::begin() {
	return SetBitArchetypeIterator<BitMask, Components...>{ m_activeMask, GetComponentPtr<Components>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
SetBitArchetypeIterator<typename FIXED_SIZE_ARCHETYPE::BitMask, const Components...> FIXED_SIZE_ARCHETYPE::begin() const {
	return SetBitArchetypeIterator<BitMask, const Components...>{ m_activeMask, GetComponentPtr<Components>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
Sentinel FIXED_SIZE_ARCHETYPE::end() const {
	return {};
}

#undef FIXED_SIZE_ARCHETYPE_TEMPLATE
#undef FIXED_SIZE_ARCHETYPE

DEEP_NAMESPACE_END
