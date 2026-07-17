#pragma once

#include "Deep.h"
#include "Deep/Bit.h"
#include "Deep/Concepts.h"
#include "Deep/Templates.h"

DEEP_NAMESPACE_BEGIN

namespace SetBit {

// Iterator
template<_Integer in_maskType, typename T>
class Iterator {
public:
	using IterMask = in_maskType;

public:
private:
	DEEP_PRIVATE_TESTABLE
};

// Helper class to produce inheritance chain in archetype template:
template<typename T>
struct ComponentStorage {
	T* data = nullptr;

	inline T* Get() const;
};

template<_Integer in_maskType, typename... Components>
class Archetype final : private ComponentStorage<Components>... {
	static_assert(((std::is_copy_assignable_v<Components> && std::is_move_assignable_v<Components>) && ...),
	              "Components must be copy assignable and move assignable.");

public:
	using IterMask = in_maskType;

	// NOTE(randomuserhi): Since bit masks are stuck to integer types, on traditional systems, the max id is 64
	//                     `uint8` can store up to 256 indices which is more than enough.
	using Handle = uint8;

public:
	inline Archetype(const Archetype&) = default;
	inline Archetype(Archetype&&) = default;
	inline Archetype& operator=(const Archetype&) = default;
	inline Archetype& operator=(Archetype&&) = default;
	inline Archetype() = default;

	//

	inline ~Archetype();

	//

	// Get raw pointer to component array
	template<typename T>
	inline T* GetComponentPtr() const;

	template<_ConstructorArgs... TaggedArgs>
	inline Handle CreateEntity(TaggedArgs&&... in_componentArgs);

	//

	template<typename T>
	constexpr static bool s_validComponent = std::is_same_v<T, Handle> || (std::is_same_v<T, Components> || ...);

	//

	constexpr static size_t k_maxSize = NumBits<IterMask>();

private:
	DEEP_PRIVATE_TESTABLE

	template<typename T, typename... TaggedArgs>
	constexpr inline void ConstructComponent(uint32 in_index, TaggedArgs&&... in_componentArgs);

	//

	template<typename T, typename... TaggedArgs>
	constexpr static inline size_t s_CountArgTags();

	template<typename T, typename TaggedArg>
	constexpr static inline void s_ConstructComponentFromArgs(T* in_ptr, TaggedArg&& in_componentArgs);

	//

	size_t m_capacity;
	size_t m_size;
};

} // namespace SetBit

DEEP_NAMESPACE_END

#include "Deep/Containers/SetBit.inl" // IWYU pragma: export
