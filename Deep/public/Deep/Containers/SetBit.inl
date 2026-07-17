#pragma once

#include "Deep.h"
#include "Deep/Containers/SetBit.h"

DEEP_NAMESPACE_BEGIN

namespace SetBit {

template<_Integer in_maskType, typename... Components>
template<typename T>
T* Archetype<in_maskType, Components...>::GetComponentPtr() const {
	static_assert(s_validComponent<T>, "Component type not in archetype");
	return static_cast<const ComponentStorage<T>&>(*this).Get();
}

template<_Integer in_maskType, typename... Components>
template<typename T, typename... TaggedArgs>
constexpr inline size_t Archetype<in_maskType, Components...>::s_CountArgTags() {
	return (0 + ... + std::is_same_v<T, typename std::decay_t<TaggedArgs>::Type>);
}

template<_Integer in_maskType, typename... Components>
template<typename T, typename TaggedArg>
constexpr void Archetype<in_maskType, Components...>::s_ConstructComponentFromArgs(T* in_ptr, TaggedArg&& in_componentArgs) {
	if constexpr (std::is_same_v<T, typename std::decay_t<TaggedArg>::Type>) {
		in_componentArgs.Construct(in_ptr);
	}
}

template<_Integer in_maskType, typename... Components>
template<_ConstructorArgs... TaggedArgs>
Archetype<in_maskType, Components...>::Handle
Archetype<in_maskType, Components...>::CreateEntity(TaggedArgs&&... in_componentArgs) {
	// Allocate static handle
	size_t index = m_size;

	// TODO(randomuserhi): Reallocate based on m_capacity

	// Construct components
	(ConstructComponent<Components, TaggedArgs...>(index, std::forward<TaggedArgs>(in_componentArgs)...), ...);

	// Increment indices
	++m_size;

	// TODO(randomuserhi): Ensure that index is < k_maxSize;
	return static_cast<Handle>(index);
}

template<_Integer in_maskType, typename... Components>
template<typename T, typename... TaggedArgs>
constexpr void Archetype<in_maskType, Components...>::ConstructComponent(uint32 in_index, TaggedArgs&&... in_componentArgs) {
	constexpr size_t occurences = s_CountArgTags<T, TaggedArgs...>();
	static_assert(occurences <= 1, "Duplicate ComponentArgs were provided for the same component type.");

	T* ptr = GetComponentPtr<T>() + in_index;

	if constexpr ((std::is_same_v<T, typename std::decay_t<TaggedArgs>::Type> || ...)) {
		(ConstructComponentFromArgs<T>(ptr, std::forward<TaggedArgs>(in_componentArgs)), ...);
	} else {
		::new (ptr) T();
	}
}

} // namespace SetBit

DEEP_NAMESPACE_END
