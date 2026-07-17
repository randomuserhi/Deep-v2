#pragma once

#include "Deep.h"

#include <functional>
#include <type_traits>
#include <utility>

DEEP_NAMESPACE_BEGIN

namespace Detail {

template<typename T>
struct TupleStorage {
public:
	using Type = typename T::Type;

public:
	constexpr TupleStorage() = default;

	template<typename U>
	constexpr TupleStorage(U&& in_val);

	//

	constexpr inline decltype(auto) Get() &;
	constexpr inline decltype(auto) Get() const&;
	constexpr inline decltype(auto) Get() &&;
	constexpr inline decltype(auto) Get() const&&;

private:
	DEEP_PRIVATE_TESTABLE

	Type m_data;
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
		requires(sizeof...(Args) == sizeof...(Ts) && (std::is_constructible_v<Ts, Args &&> && ...))
	constexpr inline TupleImpl(Args&&... args);

	template<std::size_t I>
	constexpr inline decltype(auto) Get() &;

	template<std::size_t I>
	constexpr inline decltype(auto) Get() const&;

	template<std::size_t I>
	constexpr inline decltype(auto) Get() &&;

	template<std::size_t I>
	constexpr inline decltype(auto) Get() const&&;
};

template<typename F, typename TupleType, std::size_t... Is>
constexpr inline decltype(auto) ApplyImpl(F&& in_function, TupleType&& in_tuple, std::index_sequence<Is...>) noexcept(
	noexcept(std::invoke(std::forward<F>(in_function), std::forward<TupleType>(in_tuple).template Get<Is>()...)));

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

template<typename... Ts>
struct Tuple : Detail::TupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...> {
	using BaseType = typename Detail::TupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
	using BaseType::BaseType;
};

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) get(Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) get(const Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) get(Tuple<Ts...>&& in_tuple);

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) get(const Tuple<Ts...>&& in_tuple);

} // namespace Detail

// Custom tuple class that is actually zero cost :
// https://stackoverflow.com/questions/63719249/why-does-stdtuple-break-small-size-struct-calling-convention-optimization-in-c
template<typename... Ts>
using Tuple = Detail::Tuple<Ts...>;

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) Get(Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) Get(const Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) Get(Tuple<Ts...>&& in_tuple);

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
constexpr inline decltype(auto) Get(const Tuple<Ts...>&& in_tuple);

template<typename... Ts>
constexpr inline auto MakeTuple(Ts&&... in_args);

template<typename... Ts>
constexpr inline auto Tie(Ts&... in_args);

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, Tuple<Ts...>& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{})));

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, const Tuple<Ts...>& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{})));

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, Tuple<Ts...>&& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{})));

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, const Tuple<Ts...>&& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{})));

DEEP_NAMESPACE_END

namespace std {

template<typename... Ts>
struct tuple_size<::Deep::Tuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template<std::size_t I, typename... Ts>
struct tuple_element<I, ::Deep::Tuple<Ts...>> {
	using type = typename ::Deep::Detail::TupleTypeAtIndex<I, Ts...>::type;
};

} // namespace std

#include "./Tuple.inl" // IWYU pragma: export
