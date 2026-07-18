#pragma once

#include "Deep.h"
#include "Deep/Bit.h"
#include "Deep/Concepts.h"
#include "Deep/Templates.h"
#include <iterator>
#include <type_traits>

DEEP_NAMESPACE_BEGIN

namespace SetBit {

template<_Integer in_IterMask, typename... Components>
class ArchetypeIterator {
public:
	using IterMask = in_IterMask;

	using reference = Tuple<Components&...>;
	using iterator_category = std::forward_iterator_tag;

public:
	inline ArchetypeIterator(IterMask in_mask, Components*... in_components);

	inline reference operator*() const;
	inline ArchetypeIterator& operator++();

	inline bool operator==(Sentinel) const;
	inline bool operator!=(Sentinel) const;

private:
	DEEP_PRIVATE_TESTABLE

	// Helper that returns tuple of items
	template<std::size_t... Is>
	reference Deref(std::index_sequence<Is...>) const;

	//

	IterMask m_remaining;
	size_t m_index;
	Tuple<Components*...> m_components;
};

template<_Integer in_IterMask, typename... Components>
class ArchetypeView {
public:
	using IterMask = in_IterMask;

public:
	inline ArchetypeView(IterMask in_mask, Components*... in_components);

	inline ArchetypeIterator<IterMask, Components...> begin() const;
	inline Sentinel end() const;

private:
	DEEP_PRIVATE_TESTABLE

	// Helper to unpack component buffers into an iterator
	template<std::size_t... Is>
	inline ArchetypeIterator<IterMask, Components...> CreateIterator(std::index_sequence<Is...>) const;

	//

	IterMask m_mask;
	Tuple<Components*...> m_components;
};

// Helper class to produce inheritance chain in archetype template:
template<typename T>
struct ComponentStorage {
	T* m_data = nullptr;

	inline const T* Get() const;
	inline T* Get();
};

template<_Integer in_IterMask, typename... Components>
class FixedSizeArchetype : private ComponentStorage<Components>... {
	static_assert(((std::is_object_v<Components> && !std::is_volatile_v<Components> && !std::is_const_v<Components>) && ...),
	              "Components must be non-volatile and non-const value types.");

public:
	using IterMask = in_IterMask;

	// NOTE(randomuserhi): Since bit masks are stuck to integer types, on traditional systems, the max id is 64
	//                     `uint8` can store up to 256 indices which is more than enough.
	using IndexType = uint8;
	static_assert(std::is_integral_v<IndexType> && std::is_unsigned_v<IndexType>,
	              "IndexType must be an unsigned integer type.");

public:
	inline FixedSizeArchetype(const FixedSizeArchetype&);
	inline FixedSizeArchetype(FixedSizeArchetype&&);
	inline FixedSizeArchetype& operator=(const FixedSizeArchetype&);
	inline FixedSizeArchetype& operator=(FixedSizeArchetype&&);

	explicit inline FixedSizeArchetype(size_t in_capacity);

	//

	inline ~FixedSizeArchetype();

	//

	inline bool IsActive(IndexType in_index) const;
	inline size_t Size() const;

	// Get component of an entity
	template<typename T>
	inline const T& GetComponent(IndexType in_index) const;
	template<typename T>
	inline T& GetComponent(IndexType in_index);

	// Get components of an entity
	template<typename... Ts>
	inline Tuple<const Ts&...> GetComponents(IndexType in_index) const;
	template<typename... Ts>
	inline Tuple<Ts&...> GetComponents(IndexType in_index);

	// Construct an entity at the given index position.
	template<_ConstructorArgs... TaggedArgs>
	inline void ConstructEntity(IndexType in_index, TaggedArgs&&... in_componentArgs);

	// Destruct an entity at the given index position.
	inline void DestructEntity(IndexType in_index);

	// Iterate entity components.
	//
	// A mask can be provided to skip certain entities. Note that only active (constructed) entities are iterated regardless
	// of the mask value.
	template<typename... Ts>
	inline ArchetypeView<IterMask, Ts...> View(IterMask in_mask = ~0);
	template<typename... Ts>
	inline ArchetypeView<IterMask, const Ts...> View(IterMask in_mask = ~0) const;

	// Range iterator begin/end implementation
	inline ArchetypeIterator<IterMask, Components...> begin();
	inline ArchetypeIterator<IterMask, const Components...> begin() const;
	inline Sentinel end() const;

	//

	template<typename T>
	constexpr static bool s_validComponent = (std::is_same_v<T, Components> || ...);

	//

	// SetBit uses a bit mask to handle iteration of entities.
	// This means the maximum capacity of an archetype is limited to the number of bits the mask has.
	constexpr static size_t k_maxCapacity = NumBits<IterMask>();

private:
	DEEP_PRIVATE_TESTABLE

	// Get raw pointer to component array
	template<typename T>
	inline const T* GetComponentPtr() const;
	template<typename T>
	inline T* GetComponentPtr();

	// Utility that allocates memory for a given component, `T`, buffer.
	template<typename T>
	inline void AllocateComponent(size_t in_size);

	// Utility that deallocates memory for a given component, `T`, buffer.
	template<typename T>
	inline void DeallocateComponent();

	// Utility that constructs component `T` from `ConstructorArg`.
	//
	// If the given `ConstructorArg` is not for type `T`, reduces to a noop.
	// Does not default construct `T`.
	//
	// Used by ConstructComponent to handle unpacking of `TaggedArgs` list.
	template<typename T, _ConstructorArgs TaggedArg>
	constexpr inline void ConstructComponentImpl(TaggedArg&& in_componentArg, T* in_ptr);

	// Utility that constructs component `T` given a set of `ConstructorArg`s.
	//
	// If a `ConstructorArg` exists for `T`, `T` is constructed using the given `ConstructorArg`,
	// otherwise `T` is default constructed.
	//
	// Used by `CreateEntity` to construct components from given `TaggedArgs` list.
	template<typename T, _ConstructorArgs... TaggedArgs>
	constexpr inline void ConstructComponent(IndexType in_index, TaggedArgs&&... in_componentArgs);

	// Utility that destructs the component `T`.
	//
	// Used by `DestructEntity` to destruct components.
	template<typename T>
	constexpr inline void DestructComponent(IndexType in_index);

	//

	// Bitmask for which entities have been constructed via `ConstructEntity`.
	// Its undefined behaviour access entities that have not been constructed.
	IterMask m_activeMask;

	size_t m_capacity;
};

} // namespace SetBit

DEEP_NAMESPACE_END

#include "Deep/Containers/SetBit.inl" // IWYU pragma: export
