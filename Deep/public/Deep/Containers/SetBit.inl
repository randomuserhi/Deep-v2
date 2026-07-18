#pragma once

#include "Deep.h"
#include "Deep/Bit.h"
#include "Deep/Memory.h"
#include "Deep/Templates.h"
#include "Deep/Containers/SetBit.h"

#include <type_traits>
#include <utility>

DEEP_NAMESPACE_BEGIN

namespace SetBit {

#define ARCHETYPE_ITERATOR_TEMPLATE template<_Integer in_IterMask, typename... Components>
#define ARCHETYPE_ITERATOR ArchetypeIterator<in_IterMask, Components...>

ARCHETYPE_ITERATOR_TEMPLATE
ARCHETYPE_ITERATOR::ArchetypeIterator(IterMask in_mask, Components*... in_components) :
	m_remaining{ in_mask },
	m_index{ in_mask != 0 ? static_cast<size_t>(CountTrailingZeros(m_remaining)) : 0 },
	m_components{ in_components... } {}

ARCHETYPE_ITERATOR_TEMPLATE
template<std::size_t... Is>
ARCHETYPE_ITERATOR::reference ARCHETYPE_ITERATOR::Deref(std::index_sequence<Is...>) const {
	return reference{ Get<Is>(m_components)[m_index]... };
}

ARCHETYPE_ITERATOR_TEMPLATE
ARCHETYPE_ITERATOR::reference ARCHETYPE_ITERATOR::operator*() const {
	return Deref(std::index_sequence_for<Components...>{});
}

ARCHETYPE_ITERATOR_TEMPLATE
bool ARCHETYPE_ITERATOR::operator==(Sentinel) const {
	return m_remaining == 0;
}

ARCHETYPE_ITERATOR_TEMPLATE
bool ARCHETYPE_ITERATOR::operator!=(Sentinel) const {
	return m_remaining != 0;
}

ARCHETYPE_ITERATOR_TEMPLATE
ARCHETYPE_ITERATOR& ARCHETYPE_ITERATOR::operator++() {
	Deep_Assert(m_remaining != 0, "End of iterator.");
	m_remaining &= m_remaining - 1;
	if (m_remaining != 0) {
		m_index = static_cast<size_t>(CountTrailingZeros(m_remaining));
	}
	return *this;
}

#undef ARCHETYPE_ITERATOR_TEMPLATE
#undef ARCHETYPE_ITERATOR

#define ARCHETYPE_VIEW_TEMPLATE template<_Integer in_IterMask, typename... Components>
#define ARCHETYPE_VIEW ArchetypeView<in_IterMask, Components...>

ARCHETYPE_VIEW_TEMPLATE
ARCHETYPE_VIEW::ArchetypeView(IterMask in_activeMask, Components*... in_components) :
	m_activeMask{ in_activeMask }, m_components{ in_components... } {}

ARCHETYPE_VIEW_TEMPLATE
template<std::size_t... Is>
ArchetypeIterator<typename ARCHETYPE_VIEW::IterMask, Components...>
ARCHETYPE_VIEW::CreateIterator(std::index_sequence<Is...>) const {
	return ArchetypeIterator<IterMask, Components...>{ m_activeMask, Get<Is>(m_components)... };
}

ARCHETYPE_VIEW_TEMPLATE
ArchetypeIterator<typename ARCHETYPE_VIEW::IterMask, Components...> ARCHETYPE_VIEW::begin() const {
	return CreateIterator(std::index_sequence_for<Components...>{});
}

ARCHETYPE_VIEW_TEMPLATE
Sentinel ARCHETYPE_VIEW::end() const {
	return {};
}

#undef ARCHETYPE_VIEW_TEMPLATE
#undef ARCHETYPE_VIEW

template<typename T>
const T* impl_Archetype::ComponentStorage<T>::Get() const {
	return m_data;
}

template<typename T>
T* impl_Archetype::ComponentStorage<T>::Get() {
	return m_data;
}

#define FIXED_SIZE_ARCHETYPE_TEMPLATE template<_Integer in_IterMask, typename... Components>
#define FIXED_SIZE_ARCHETYPE FixedSizeArchetype<in_IterMask, Components...>

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
inline void FIXED_SIZE_ARCHETYPE::AllocateComponent(size_t in_size) {
	static_cast<impl_Archetype::ComponentStorage<T>&>(*this).m_data = TMalloc<T>(in_size);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
inline void FIXED_SIZE_ARCHETYPE::DeallocateComponent() {
	TFree<T>(static_cast<impl_Archetype::ComponentStorage<T>&>(*this).m_data);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
FIXED_SIZE_ARCHETYPE::FixedSizeArchetype(size_t in_capacity) :
	m_activeMask{ 0 }, m_capacity{ in_capacity } {
	Deep_Assert(m_capacity <= k_maxCapacity, "Size must be smaller than k_maxCapacity.");
	(AllocateComponent<Components>(m_capacity), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE FIXED_SIZE_ARCHETYPE::~FixedSizeArchetype() {
	IterMask remaining = m_activeMask;
	while (remaining != 0) {
		const int32 index = CountTrailingZeros(remaining);
		remaining &= remaining - 1;

		DestructEntity(index);
	}

	m_activeMask = 0;
	m_capacity = 0;

	(DeallocateComponent<Components>(), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
const T* FIXED_SIZE_ARCHETYPE::GetComponentPtr() const {
	static_assert(s_validComponent<T>, "Component type not in archetype");
	return static_cast<const impl_Archetype::ComponentStorage<T>&>(*this).Get();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
T* FIXED_SIZE_ARCHETYPE::GetComponentPtr() {
	static_assert(s_validComponent<T>, "Component type not in archetype");
	return static_cast<impl_Archetype::ComponentStorage<T>&>(*this).Get();
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T, _ConstructorArgs TaggedArg>
constexpr inline void FIXED_SIZE_ARCHETYPE::ConstructComponentImpl(TaggedArg&& in_componentArg, T* in_ptr) {
	if constexpr (std::is_same_v<T, typename std::decay_t<TaggedArg>::Type>) {
		std::move(in_componentArg).Construct(in_ptr);
	}
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T, _ConstructorArgs... TaggedArgs>
constexpr inline void FIXED_SIZE_ARCHETYPE::ConstructComponent(IndexType in_index, TaggedArgs&&... in_componentArgs) {
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
constexpr inline void FIXED_SIZE_ARCHETYPE::DestructComponent(IndexType in_index) {
	if constexpr (!std::is_trivially_destructible_v<T>) {
		GetComponentPtr<T>()[in_index].~T();
	}
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<_ConstructorArgs... TaggedArgs>
void FIXED_SIZE_ARCHETYPE::ConstructEntity(IndexType in_index, TaggedArgs&&... in_componentArgs) {
	static_assert((s_validComponent<typename std::decay_t<TaggedArgs>::Type> && ...),
	              "Archetype must contain all components that the given constructor args specify.");

	Deep_Assert(in_index < m_capacity, "Invalid index.");
	Deep_Assert(!IsActive(in_index), "An entity is already constructed here.");

	// Mark entity as active
	m_activeMask |= IterMask{ 1 } << in_index;

	// Construct components
	(ConstructComponent<Components, TaggedArgs...>(in_index, std::forward<TaggedArgs>(in_componentArgs)...), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
void FIXED_SIZE_ARCHETYPE::DestructEntity(IndexType in_index) {
	Deep_Assert(in_index < m_capacity, "Invalid index.");
	Deep_Assert(IsActive(in_index), "Cannot destruct an entity that has not been constructed yet.");

	// Mark entity as inactive
	m_activeMask &= ~(IterMask{ 1 } << in_index);

	// Destruct components
	(DestructComponent<Components>(in_index), ...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
bool FIXED_SIZE_ARCHETYPE::IsActive(IndexType in_index) const {
	Deep_Assert(in_index < m_capacity, "Invalid index.");
	IterMask flag = IterMask{ 1 } << in_index;
	return (m_activeMask & flag) != 0;
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
size_t FIXED_SIZE_ARCHETYPE::Size() const {
	return CountSetBits(m_activeMask);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE template<typename T>
const T& FIXED_SIZE_ARCHETYPE::GetComponent(IndexType in_index) const {
	static_assert(s_validComponent<T>, "Archetype must contain component 'T'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return GetComponentPtr<T>()[in_index];
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename T>
T& FIXED_SIZE_ARCHETYPE::GetComponent(IndexType in_index) {
	static_assert(s_validComponent<T>, "Archetype must contain component 'T'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return GetComponentPtr<T>()[in_index];
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
Tuple<const Ts&...> FIXED_SIZE_ARCHETYPE::GetComponents(IndexType in_index) const {
	static_assert((s_validComponent<Ts> && ...), "Archetype must contain all component 'Ts'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return Tuple<const Ts&...>(GetComponentPtr<Ts>()[in_index]...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
Tuple<Ts&...> FIXED_SIZE_ARCHETYPE::GetComponents(IndexType in_index) {
	static_assert((s_validComponent<Ts> && ...), "Archetype must contain all component 'Ts'.");
	Deep_Assert(IsActive(in_index), "Cannot get a component that is not active. Construct it first with 'ConstructEntity'.");

	return Tuple<Ts&...>(GetComponentPtr<Ts>()[in_index]...);
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
ArchetypeView<typename FIXED_SIZE_ARCHETYPE::IterMask, Ts...> FIXED_SIZE_ARCHETYPE::View(IterMask in_mask) {
	// NOTE(randomuserhi): We `std::decay_t` the component type in views as we allow promotion of `Ts` to `const Ts`
	//                     to generate const views of components from non-const archetypes.
	static_assert((s_validComponent<typename std::decay_t<Ts>> && ...), "Archetype must contain all component 'Ts'.");
	in_mask &= m_activeMask;
	return ArchetypeView<IterMask, Ts...>{ in_mask, GetComponentPtr<typename std::decay_t<Ts>>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
template<typename... Ts>
ArchetypeView<typename FIXED_SIZE_ARCHETYPE::IterMask, const Ts...> FIXED_SIZE_ARCHETYPE::View(IterMask in_mask) const {
	// NOTE(randomuserhi): We `std::decay_t` the component type in views as we allow promotion of `Ts` to `const Ts`
	//                     to generate const views of components from non-const archetypes.
	//                     Although this is the const version, we still allow it to keep syntax consistent.
	static_assert((s_validComponent<typename std::decay_t<Ts>> && ...), "Archetype must contain all component 'Ts'.");
	in_mask &= m_activeMask;
	return ArchetypeView<IterMask, const Ts...>{ in_mask, GetComponentPtr<typename std::decay_t<Ts>>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
ArchetypeIterator<typename FIXED_SIZE_ARCHETYPE::IterMask, Components...> FIXED_SIZE_ARCHETYPE::begin() {
	return ArchetypeIterator<IterMask, Components...>{ m_activeMask, GetComponentPtr<Components>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
ArchetypeIterator<typename FIXED_SIZE_ARCHETYPE::IterMask, const Components...> FIXED_SIZE_ARCHETYPE::begin() const {
	return ArchetypeIterator<IterMask, const Components...>{ m_activeMask, GetComponentPtr<Components>()... };
}

FIXED_SIZE_ARCHETYPE_TEMPLATE
Sentinel FIXED_SIZE_ARCHETYPE::end() const {
	return {};
}

#undef FIXED_SIZE_ARCHETYPE_TEMPLATE
#undef FIXED_SIZE_ARCHETYPE

} // namespace SetBit

DEEP_NAMESPACE_END
