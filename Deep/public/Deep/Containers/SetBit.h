#pragma once

#include "Deep.h"
#include "Deep/Bit/IntegerBitMask.h"
#include "Deep/Templates/ConstructWith.h"
#include "Deep/Templates/SoALayout.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Iterator for set-bit archetypes.
// Uses a bitmask to skip over entities via set-bit iteration, allowing for fast sparse iteration over small blocks.
//
// NOTE(randomuserhi): `operator*()` always calls `BitMask::IndexOfLowestSetBit` to get the iterator item. As a result,
//                     repeatedly dereferencing the iterator for the same item can result in extra work, always cache the
//                     dereferenced item and reuse it.
template<c_BitMask in_BitMask, typename... Components>
class SetBitArchetypeIterator {
public:
	using BitMask = in_BitMask;

	using reference = Tuple<Components&...>;

public:
	inline SetBitArchetypeIterator(BitMask in_mask, Components*... in_components);

	inline reference operator*() const;
	inline SetBitArchetypeIterator& operator++();

	inline bool operator==(Sentinel) const;
	inline bool operator!=(Sentinel) const;

private:
	DEEP_PRIVATE_TESTABLE

	// Helper that returns tuple of items
	template<std::size_t... Is>
	reference Deref(std::index_sequence<Is...>) const;

	//

	BitMask m_remaining;
	Tuple<Components*...> m_components;
};

// View for set-bit archetypes.
// Represents a view of a subset of components of an archetype.
//
// Can also be constructed to treat a set of buffers as an archetype view.
//
// The mask represents the currently active members (or a subset of them).
template<c_BitMask in_BitMask, typename... Components>
class SetBitArchetypeView {
public:
	using BitMask = in_BitMask;

public:
	inline SetBitArchetypeView(BitMask in_activeMask, Components*... in_components);

	inline SetBitArchetypeIterator<BitMask, Components...> begin() const;
	inline Sentinel end() const;

private:
	DEEP_PRIVATE_TESTABLE

	// Helper to unpack component buffers into an iterator
	template<std::size_t... Is>
	inline SetBitArchetypeIterator<BitMask, Components...> CreateIterator(std::index_sequence<Is...>) const;

	//

	BitMask m_activeMask;
	Tuple<Components*...> m_components;
};

namespace detail::_SetBitArchetype {

// Utility object to generate inheritance chain for archetype storage
template<typename T>
struct ComponentStorage {
	T* m_data = nullptr;

	inline const T* Get() const;
	inline T* Get();
};

} // namespace detail::_SetBitArchetype

// Fixed size, set-bit variant of the Archetype container.
//
// Stores components as a Structure-Of-Arrays (SoA) where access is performed via set-bit iteration.
//
// Set-bit iteration is where items are iterated via a mask such as 0b1011. This iterates the first 2 elements, skips 1 and
// then the last element. As it uses bit masks, these containers are inherently designed for small blocks. For varying
// block sizes, you can specify a smaller mask type such as `uint16` for 16 items, `uint32` for 32 items or `uint64` for 64
// items.
//
// The container maintains a mask for active (constructed) entities which is considered by default during iteration, make
// sure to construct all entities via `ConstructEntity` before using them.
//
// The use of masks is helpful as it allows easy combining to iterate mixtures of entities:
//
// ```cpp
// BitMask stunned = 0b1001;
// BitMask airborne = 0b0010;
// BitMask canMove = ~(stunned | airborne);
//
// for (auto [transform] : archetype.View<Transform>(canMove)) {
//   // Iterate the transforms of all entities that can move
// }
// ```
//
// NOTE(randomuserhi): Currently does not support components with constructors or copy that can throw
// TODO(randomuserhi): Support components that throw on copy or construction
template<c_BitMask in_BitMask, typename... Components>
class FixedSizeSetBitArchetype : private detail::_SetBitArchetype::ComponentStorage<Components>... {
	static_assert(sizeof...(Components) > 0, "Must have atleast one component");

	static_assert(((std::is_object_v<Components> && !std::is_volatile_v<Components> && !std::is_const_v<Components>) && ...),
	              "Components must be non-volatile and non-const value types.");

	using Layout = SoALayout<Components...>;

public:
	using BitMask = in_BitMask;

public:
	inline FixedSizeSetBitArchetype(const FixedSizeSetBitArchetype&);
	inline FixedSizeSetBitArchetype(FixedSizeSetBitArchetype&&) noexcept;
	inline FixedSizeSetBitArchetype& operator=(const FixedSizeSetBitArchetype&);
	inline FixedSizeSetBitArchetype& operator=(FixedSizeSetBitArchetype&&) noexcept;

	explicit inline FixedSizeSetBitArchetype(size_t in_capacity);

	//

	inline ~FixedSizeSetBitArchetype();

	//

	// Checks if the given item at `in_index` is active (has been constructed)
	inline bool IsActive(size_t in_index) const;

	// Get the number of active (constructed) items
	inline size_t Size() const;

	// Get the max number of entities
	Deep_ForceInline const size_t& m_Capacity() const;

	// Get component of an entity
	template<typename T>
	inline const T& GetComponent(size_t in_index) const;
	template<typename T>
	inline T& GetComponent(size_t in_index);

	// Get components of an entity
	template<typename... Ts>
	inline Tuple<const Ts&...> GetComponents(size_t in_index) const;
	template<typename... Ts>
	inline Tuple<Ts&...> GetComponents(size_t in_index);

	// Construct an entity at the given index position
	//
	// For components without default constructors, they can be constructed via `Deep::ConstructWith<T>`:
	// ```cpp
	// // Where `A` is the component to forward params to
	// arch.ConstructEntity(0, Deep::ConstructWith<A>(param0, param1));
	// ```
	template<c_ConstructorArgs... TaggedArgs>
	inline void ConstructEntity(size_t in_index, TaggedArgs&&... in_componentArgs);

	// Destruct an entity at the given index position
	inline void DestructEntity(size_t in_index);

	// Iterate entity components.
	//
	// A mask can be provided to skip certain entities. Note that only active (constructed) entities are iterated regardless
	// of the mask value.
	template<typename... Ts>
	inline SetBitArchetypeView<BitMask, Ts...> View(BitMask in_mask);
	template<typename... Ts>
	inline SetBitArchetypeView<BitMask, const Ts...> View(BitMask in_mask) const;
	template<typename... Ts>
	inline SetBitArchetypeView<BitMask, Ts...> View();
	template<typename... Ts>
	inline SetBitArchetypeView<BitMask, const Ts...> View() const;

	// Range iterator begin/end implementation
	inline SetBitArchetypeIterator<BitMask, Components...> begin();
	inline SetBitArchetypeIterator<BitMask, const Components...> begin() const;
	inline Sentinel end() const;

	//

	// SetBit uses a bit mask to handle iteration of entities.
	// This means the maximum capacity of an archetype is limited to the number of bits the mask has.
	constexpr static size_t k_maxCapacity = BitMask::k_maxNumBits;

private:
	DEEP_PRIVATE_TESTABLE

	// Get raw pointer to component array
	template<typename T>
	inline const T* GetComponentPtr() const;
	template<typename T>
	inline T* GetComponentPtr();

	// Utility that assigns a component ptr.
	template<typename T>
	inline void SetComponentPtr(void* in_ptr);

	// Utility that moves the component buffer `T` from one archetype to another.
	// The move sets the given component buffer `T` in `in_other` to nullptr.
	//
	// Used by move assignment/constructor.
	template<typename T>
	inline void MoveComponent(FixedSizeSetBitArchetype&& in_other);

	// Utility that constructs component `T` from `ConstructorArg`.
	//
	// If the given `ConstructorArg` is not for type `T`, reduces to a noop.
	// Does not default construct `T`.
	//
	// Used by ConstructComponent to handle unpacking of `TaggedArgs` list.
	template<typename T, c_ConstructorArgs TaggedArg>
	constexpr inline void ConstructComponentImpl(TaggedArg&& in_componentArg, T* in_ptr);

	// Utility that constructs component `T` given a set of `ConstructorArg`s.
	//
	// If a `ConstructorArg` exists for `T`, `T` is constructed using the given `ConstructorArg`,
	// otherwise `T` is default constructed.
	//
	// Used by `CreateEntity` to construct components from given `TaggedArgs` list.
	template<typename T, c_ConstructorArgs... TaggedArgs>
	constexpr inline void ConstructComponent(size_t in_index, TaggedArgs&&... in_componentArgs);

	// Utility that destructs the component `T`.
	//
	// Used by `DestructEntity` to destruct components.
	template<typename T>
	inline void DestructComponent(size_t in_index);

	// Allocates component storage for the current `m_capacity`
	inline void AllocateStorage();

	// Deallocates component storage
	inline void DeallocateStorage();

	//

	template<typename T>
	constexpr static bool s_validComponent = (std::is_same_v<typename std::remove_const_t<T>, Components> || ...);

	//

	// Bitmask for which entities have been constructed via `ConstructEntity`.
	// Its undefined behaviour access entities that have not been constructed.
	BitMask m_activeMask;

	// The maximum number of entities the archetype can store.
	size_t m_capacity;
};

DEEP_NAMESPACE_END

#include "./SetBit.inl" // IWYU pragma: export
