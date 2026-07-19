#pragma once

#include "Deep.h"

#include <type_traits>

DEEP_NAMESPACE_BEGIN

// Utility to ensure that all types in a type list `Ts` are unique
template<typename... Ts>
struct AreUnique;

template<>
struct AreUnique<> : std::true_type {};

template<typename T, typename... Rest>
struct AreUnique<T, Rest...> : std::bool_constant<(!std::is_same_v<T, Rest> && ...) && AreUnique<Rest...>::value> {};

// Utility to get the index of a type in a type list `Ts`
template<typename T, typename... Ts>
struct TypeIndex;

template<typename T, typename... Rest>
struct TypeIndex<T, T, Rest...> : std::integral_constant<std::size_t, 0> {};

template<typename T, typename U, typename... Rest>
struct TypeIndex<T, U, Rest...> : std::integral_constant<std::size_t, 1 + TypeIndex<T, Rest...>::value> {};

template<typename T>
struct TypeIndex<T> {
	static_assert(!std::is_same_v<T, T>, "Type is not present in the type list");
};

namespace impl_TypeAt {

template<size_t I, typename... Ts>
struct TypeAt;

// First element
template<typename T, typename... Rest>
struct TypeAt<0, T, Rest...> {
	using Value = T;
};

// Recursively traverse type list
template<size_t I, typename T, typename... Rest>
struct TypeAt<I, T, Rest...> : TypeAt<I - 1, Rest...> {};

// Index is out of bounds error
template<size_t I>
struct TypeAt<I> {
	static_assert(I != I, "Index is out of bounds for the type list");
};

} // namespace impl_TypeAt

// Utility to get the type at index I in a type list `Ts`
template<size_t I, typename... Ts>
using TypeAt = typename impl_TypeAt::TypeAt<I, Ts...>::Value;

// Utility to find the maximum alignment from a type list `Ts`
template<typename... Ts>
struct MaxAlignment;

template<>
struct MaxAlignment<> {
	static constexpr std::size_t value = 1;
};

template<typename T, typename... Rest>
struct MaxAlignment<T, Rest...> {
	static constexpr std::size_t value = [] {
		constexpr auto rest = MaxAlignment<Rest...>::value;
		return alignof(T) > rest ? alignof(T) : rest;
	}();
};

DEEP_NAMESPACE_END
