#pragma once

#include "Deep/Containers/Tuple.h"
#include "Deep/Templates.h"

DEEP_NAMESPACE_BEGIN

namespace Detail {

template<typename T>
template<typename U>
	requires std::is_constructible_v<typename TupleStorage<T>::Type, U&&>
constexpr TupleStorage<T>::TupleStorage(U&& in_val) noexcept(std::is_nothrow_constructible_v<Type, U&&>) :
	m_data(std::forward<U>(in_val)) {}

template<typename T>
template<typename... Args>
constexpr TupleStorage<T>::TupleStorage(ConstructorArgs<Type, Args...>&& in_constructor) noexcept(
	noexcept(std::move(in_constructor).Construct())) :
	m_data(std::move(in_constructor).Construct()) {}

template<typename T>
[[nodiscard]] constexpr decltype(auto) TupleStorage<T>::Get() & {
	return (m_data);
}

template<typename T>
[[nodiscard]] constexpr decltype(auto) TupleStorage<T>::Get() const& {
	return (m_data);
}

template<typename T>
[[nodiscard]] constexpr decltype(auto) TupleStorage<T>::Get() && {
	return std::forward<Type>(m_data);
}

template<typename T>
[[nodiscard]] constexpr decltype(auto) TupleStorage<T>::Get() const&& {
	using ReturnType = std::conditional_t<std::is_reference_v<Type>, Type, const Type>;
	return std::forward<ReturnType>(m_data);
}

template<std::size_t... Is, typename... Ts>
template<typename... Args>
	requires(sizeof...(Args) == sizeof...(Ts)
             && (std::is_constructible_v<TupleStorage<TupleIndexTag<Is, Ts>>, Args &&> && ...))
constexpr TupleImpl<std::index_sequence<Is...>, Ts...>::TupleImpl(Args&&... in_args) noexcept(
	(std::is_nothrow_constructible_v<TupleStorage<TupleIndexTag<Is, Ts>>, Args&&> && ...)) :
	TupleStorage<TupleIndexTag<Is, Ts>>{ std::forward<Args>(in_args) }... {}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::Get() & {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	using Storage = TupleStorage<TupleIndexTag<I, T>>;

	return static_cast<Storage&>(*this).Get();
}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::Get() const& {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	using Storage = TupleStorage<TupleIndexTag<I, T>>;

	return static_cast<const Storage&>(*this).Get();
}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::Get() && {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	using Storage = TupleStorage<TupleIndexTag<I, T>>;

	return static_cast<Storage&&>(*this).Get();
}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::Get() const&& {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	using Storage = TupleStorage<TupleIndexTag<I, T>>;

	return static_cast<const Storage&&>(*this).Get();
}

template<typename F, typename TupleType, std::size_t... Is>
constexpr decltype(auto) ApplyImpl(F&& in_function, TupleType&& in_tuple, std::index_sequence<Is...>) noexcept(
	noexcept(std::invoke(std::forward<F>(in_function), std::forward<TupleType>(in_tuple).template Get<Is>()...))) {
	return std::invoke(std::forward<F>(in_function), std::forward<TupleType>(in_tuple).template Get<Is>()...);
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) get(Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template Get<I>();
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) get(const Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template Get<I>();
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) get(Deep::Tuple<Ts...>&& in_tuple) {
	return std::move(in_tuple).template Get<I>();
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) get(const Deep::Tuple<Ts...>&& in_tuple) {
	return std::move(in_tuple).template Get<I>();
}

} // namespace Detail

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) Get(Deep::Tuple<Ts...>& in_tuple) {
	return Detail::get<I, Ts...>(in_tuple);
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) Get(const Deep::Tuple<Ts...>& in_tuple) {
	return Detail::get<I, Ts...>(in_tuple);
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) Get(Deep::Tuple<Ts...>&& in_tuple) {
	return Detail::get<I, Ts...>(std::move(in_tuple));
}

template<std::size_t I, typename... Ts>
[[nodiscard]] constexpr decltype(auto) Get(const Deep::Tuple<Ts...>&& in_tuple) {
	return Detail::get<I, Ts...>(std::move(in_tuple));
}

template<typename... Ts>
[[nodiscard]] constexpr auto MakeTuple(Ts&&... in_args) {
	return Tuple<std::unwrap_ref_decay_t<Ts>...>(std::forward<Ts>(in_args)...);
}

template<_ConstructorArgs... Args>
[[nodiscard]] constexpr auto ConstructTuple(Args&&... in_constructorArgs) noexcept(
	noexcept(Tuple<typename std::remove_cvref_t<Args>::Type...>{ std::forward<Args>(in_constructorArgs)... })) {
	static_assert((!std::is_lvalue_reference_v<Args> && ...), "Constructor specifications must be consumed immediately");

	return Tuple<typename std::remove_cvref_t<Args>::Type...>{ std::forward<Args>(in_constructorArgs)... };
}

template<typename... Ts>
[[nodiscard]] constexpr auto Tie(Ts&... in_args) {
	return Tuple<Ts&...>(in_args...);
}

template<typename F, typename... Ts>
constexpr decltype(auto) Apply(F&& in_function, Tuple<Ts...>& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{}))) {
	return Detail::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{});
}

template<typename F, typename... Ts>
constexpr decltype(auto) Apply(F&& in_function, const Tuple<Ts...>& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{}))) {
	return Detail::ApplyImpl(std::forward<F>(in_function), in_tuple, std::index_sequence_for<Ts...>{});
}

template<typename F, typename... Ts>
constexpr decltype(auto) Apply(F&& in_function, Tuple<Ts...>&& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{}))) {
	return Detail::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{});
}

template<typename F, typename... Ts>
constexpr decltype(auto) Apply(F&& in_function, const Tuple<Ts...>&& in_tuple) noexcept(
	noexcept(Detail::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{}))) {
	return Detail::ApplyImpl(std::forward<F>(in_function), std::move(in_tuple), std::index_sequence_for<Ts...>{});
}

DEEP_NAMESPACE_END
