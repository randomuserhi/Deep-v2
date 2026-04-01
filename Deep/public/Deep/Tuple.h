#pragma once

#include "Deep.h"

#include <type_traits>
#include <utility>

DEEP_NAMESPACE_BEGIN

// Custom tuple class that is actually zero cost :
// https://stackoverflow.com/questions/63719249/why-does-stdtuple-break-small-size-struct-calling-convention-optimization-in-c
namespace Detail {

template<typename T>
struct TupleStorage {
	using Type = typename T::Type;
	Type data;

	constexpr TupleStorage() = default;

	template<typename U>
	constexpr TupleStorage(U&& in_val) :
		data(std::forward<U>(in_val)) {}

	constexpr inline auto& get();

	constexpr inline const auto& get() const;
};

template<std::size_t I, typename T>
struct TupleIndexTag {
	using Type = T;
};

template<std::size_t I, typename T, typename... Rest>
struct TupleTypeAtIndex : TupleTypeAtIndex<I - 1, Rest...> {};

template<typename T, typename... Rest>
struct TupleTypeAtIndex<0, T, Rest...> {
	using type = T;
};

template<typename IndexSequence, typename... Ts>
struct TupleImpl;

template<std::size_t... Is, typename... Ts>
struct TupleImpl<std::index_sequence<Is...>, Ts...> : TupleStorage<TupleIndexTag<Is, Ts>>... {
	constexpr TupleImpl() = default;
	constexpr TupleImpl(const TupleImpl&) = default;
	constexpr TupleImpl(TupleImpl&&) = default;
	constexpr TupleImpl& operator=(const TupleImpl&) = default;
	constexpr TupleImpl& operator=(TupleImpl&&) = default;

	template<typename... Args>
	constexpr inline TupleImpl(Args&&... args);

	template<std::size_t I>
	constexpr inline decltype(auto) get();

	template<std::size_t I>
	constexpr inline decltype(auto) get() const;
};

#if __cplusplus >= 202002L
template<typename T>
using Unwrap = std::unwrap_ref_decay_t<T>;
#else
// Manual implementation of std::unwrap_ref_decay_t<T> for C++11/14/17
template<typename T>
struct UnwrapType {
	using type = std::decay_t<T>;
};

template<typename T>
struct UnwrapType<std::reference_wrapper<T>> {
	using type = T&;
};

template<typename T>
using Unwrap = typename UnwrapType<std::decay_t<T>>::type;
#endif

} // namespace Detail

template<typename... Ts>
struct Tuple : Detail::TupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...> {
	using BaseType = typename Detail::TupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
	using BaseType::BaseType;
};

template<typename... Ts>
constexpr inline auto MakeTuple(Ts&&... in_args);

template<typename... Ts>
constexpr inline auto Tie(Ts&... in_args);

DEEP_NAMESPACE_END

namespace std {

template<typename... Ts>
struct tuple_size<::Deep::Tuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template<std::size_t I, typename... Ts>
struct tuple_element<I, ::Deep::Tuple<Ts...>> {
	using type = typename ::Deep::Detail::TupleTypeAtIndex<I, Ts...>::type;
};

template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) get(::Deep::Tuple<Ts...>& in_tuple);

template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) get(const ::Deep::Tuple<Ts...>& in_tuple);

} // namespace std

#include "./Tuple.inl" // IWYU pragma: export
