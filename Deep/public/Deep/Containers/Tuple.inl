#pragma once

#include "Deep/Containers/Tuple.h"

DEEP_NAMESPACE_BEGIN

namespace Detail {

template<typename T>
template<typename U>
constexpr TupleStorage<T>::TupleStorage(U&& in_val) :
	m_data{ std::forward<U>(in_val) } {}

template<typename T>
constexpr decltype(auto) TupleStorage<T>::Get() & {
	return (m_data);
}

template<typename T>
constexpr decltype(auto) TupleStorage<T>::Get() const& {
	return (m_data);
}

template<typename T>
constexpr decltype(auto) TupleStorage<T>::Get() && {
	return std::forward<Type>(m_data);
}

template<typename T>
constexpr decltype(auto) TupleStorage<T>::Get() const&& {
	using ReturnType = std::conditional_t<std::is_reference_v<Type>, Type, const Type>;
	return std::forward<ReturnType>(m_data);
}

template<std::size_t... Is, typename... Ts>
template<typename... Args>
	requires(sizeof...(Args) == sizeof...(Ts) && (std::is_constructible_v<Ts, Args &&> && ...))
constexpr TupleImpl<std::index_sequence<Is...>, Ts...>::TupleImpl(Args&&... args) :
	TupleStorage<TupleIndexTag<Is, Ts>>{ std::forward<Args>(args) }... {}

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
constexpr decltype(auto) get(Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template Get<I>();
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(const Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template Get<I>();
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(Deep::Tuple<Ts...>&& in_tuple) {
	return std::move(in_tuple).template Get<I>();
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(const Deep::Tuple<Ts...>&& in_tuple) {
	return std::move(in_tuple).template Get<I>();
}

} // namespace Detail

template<std::size_t I, typename... Ts>
constexpr decltype(auto) Get(Deep::Tuple<Ts...>& in_tuple) {
	return Detail::get<I, Ts...>(in_tuple);
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) Get(const Deep::Tuple<Ts...>& in_tuple) {
	return Detail::get<I, Ts...>(in_tuple);
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) Get(Deep::Tuple<Ts...>&& in_tuple) {
	return Detail::get<I, Ts...>(std::move(in_tuple));
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) Get(const Deep::Tuple<Ts...>&& in_tuple) {
	return Detail::get<I, Ts...>(std::move(in_tuple));
}

template<typename... Ts>
constexpr auto MakeTuple(Ts&&... in_args) {
	return Tuple<std::unwrap_ref_decay_t<Ts>...>(std::forward<Ts>(in_args)...);
}

template<typename... Ts>
constexpr auto Tie(Ts&... in_args) {
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
