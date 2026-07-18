#pragma once

#include "Deep.h"
#include "Deep/Templates.h"

#include <functional>
#include <type_traits>
#include <utility>

DEEP_NAMESPACE_BEGIN

namespace impl_Tuple {

template<typename T>
struct TupleStorage {
public:
	using Type = typename T::Type;

public:
	constexpr TupleStorage() = default;

	template<typename U>
		requires std::is_constructible_v<Type, U&&>
	constexpr inline explicit TupleStorage(U&& in_val) noexcept(std::is_nothrow_constructible_v<Type, U&&>);

	template<typename... Args>
	constexpr inline explicit TupleStorage(ConstructorArgs<Type, Args...>&& in_constructor) noexcept(
		noexcept(std::move(in_constructor).Construct()));

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
		requires(sizeof...(Args) == sizeof...(Ts)
	             && (std::is_constructible_v<TupleStorage<TupleIndexTag<Is, Ts>>, Args &&> && ...))
	constexpr inline explicit TupleImpl(Args&&... in_args) noexcept(
		(std::is_nothrow_constructible_v<TupleStorage<TupleIndexTag<Is, Ts>>, Args&&> && ...));

	template<std::size_t I>
	[[nodiscard]] constexpr inline decltype(auto) Get() &;

	template<std::size_t I>
	[[nodiscard]] constexpr inline decltype(auto) Get() const&;

	template<std::size_t I>
	[[nodiscard]] constexpr inline decltype(auto) Get() &&;

	template<std::size_t I>
	[[nodiscard]] constexpr inline decltype(auto) Get() const&&;
};

template<typename F, typename TupleType, std::size_t... Is>
constexpr inline decltype(auto) ApplyImpl(F&& in_function, TupleType&& in_tuple, std::index_sequence<Is...>) noexcept(
	noexcept(std::invoke(std::forward<F>(in_function), std::forward<TupleType>(in_tuple).template Get<Is>()...)));

template<typename... Ts>
struct Tuple : impl_Tuple::TupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...> {
	using BaseType = typename impl_Tuple::TupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
	using BaseType::BaseType;
};

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) get(Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) get(const Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) get(Tuple<Ts...>&& in_tuple);

// Get an item by index from a tuple
//
// Lower-case as its needed for ADL of:
// ```cpp
// auto [x, y] = Deep::Tuple<int32, int32>(1, 1);
// ```
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) get(const Tuple<Ts...>&& in_tuple);

template<typename LeftTuple, typename RightTuple, typename IndexSequence>
struct TupleEqualityComparable;

template<typename LeftTuple, typename RightTuple, std::size_t... Is>
using TupleEqualityComparableBoolConstant = std::bool_constant<(requires(const LeftTuple& lhs, const RightTuple& rhs) {
	static_cast<bool>(lhs.template Get<Is>() == rhs.template Get<Is>());
} && ...)>;

template<typename LeftTuple, typename RightTuple, std::size_t... Is>
struct TupleEqualityComparable<LeftTuple, RightTuple, std::index_sequence<Is...>>
	: TupleEqualityComparableBoolConstant<LeftTuple, RightTuple, Is...> {};

template<typename LeftTuple, typename RightTuple, std::size_t... Is>
[[nodiscard]] constexpr inline bool
TupleEqualImpl(const LeftTuple& in_left, const RightTuple& in_right, std::index_sequence<Is...>) noexcept(
	(noexcept(static_cast<bool>(in_left.template Get<Is>() == in_right.template Get<Is>())) && ...));

template<typename... LeftTs, typename... RightTs>
	requires(
		sizeof...(LeftTs) == sizeof...(RightTs)
		&& TupleEqualityComparable<Tuple<LeftTs...>, Tuple<RightTs...>, std::make_index_sequence<sizeof...(LeftTs)>>::value)
[[nodiscard]] constexpr inline bool operator==(const Tuple<LeftTs...>& in_left, const Tuple<RightTs...>& in_right) noexcept(
	noexcept(TupleEqualImpl(in_left, in_right, std::make_index_sequence<sizeof...(LeftTs)>{})));

} // namespace impl_Tuple

// Custom tuple class that is actually zero cost :
// https://stackoverflow.com/questions/63719249/why-does-stdtuple-break-small-size-struct-calling-convention-optimization-in-c
template<typename... Ts>
using Tuple = impl_Tuple::Tuple<Ts...>;

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) Get(Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) Get(const Tuple<Ts...>& in_tuple);

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) Get(Tuple<Ts...>&& in_tuple);

// Get an item by index from a tuple
template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr inline decltype(auto) Get(const Tuple<Ts...>&& in_tuple);

template<typename... Ts>
[[nodiscard]] constexpr inline auto MakeTuple(Ts&&... in_args);

template<_ConstructorArgs... Args>
[[nodiscard]] constexpr inline auto ConstructTuple(Args&&... in_constructorArgs) noexcept(
	noexcept(Tuple<typename std::remove_cvref_t<Args>::Type...>{ std::forward<Args>(in_constructorArgs)... }));

template<typename... Ts>
[[nodiscard]] constexpr inline auto Tie(Ts&... in_args);

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, Tuple<Ts...>& in_tuple) noexcept(
	noexcept(impl_Tuple::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{})));

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, const Tuple<Ts...>& in_tuple) noexcept(
	noexcept(impl_Tuple::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{})));

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, Tuple<Ts...>&& in_tuple) noexcept(
	noexcept(impl_Tuple::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{})));

template<typename F, typename... Ts>
constexpr inline decltype(auto) Apply(F&& in_function, const Tuple<Ts...>&& in_tuple) noexcept(
	noexcept(impl_Tuple::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{})));

DEEP_NAMESPACE_END

namespace std {

template<typename... Ts>
struct tuple_size<::Deep::Tuple<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template<std::size_t I, typename... Ts>
struct tuple_element<I, ::Deep::Tuple<Ts...>> {
	using type = typename ::Deep::impl_Tuple::TupleTypeAtIndex<I, Ts...>::type;
};

} // namespace std

#include "./Tuple.inl" // IWYU pragma: export
