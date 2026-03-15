#pragma once

#include "Deep/Tuple.h"

DEEP_NAMESPACE_BEGIN

namespace Detail {

template<typename T>
constexpr auto& TupleStorage<T>::get() {
	return data;
}

template<typename T>
constexpr const auto& TupleStorage<T>::get() const {
	return data;
}

template<std::size_t... Is, typename... Ts>
template<typename... Args>
constexpr TupleImpl<std::index_sequence<Is...>, Ts...>::TupleImpl(Args&&... args) :
	TupleStorage<TupleIndexTag<Is, Ts>>{ std::forward<Args>(args) }... {}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::get() {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	return static_cast<TupleStorage<TupleIndexTag<I, T>>&>(*this).get();
}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::get() const {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	return static_cast<const TupleStorage<TupleIndexTag<I, T>>&>(*this).get();
}

} // namespace Detail

template<typename... Ts>
constexpr auto MakeTuple(Ts&&... in_args) {
	return Tuple<std::decay_t<Ts>...>(std::forward<Ts>(in_args)...);
}

template<typename... Ts>
constexpr auto Tie(Ts&... in_args) {
	return Tuple<Ts&...>(in_args...);
}

DEEP_NAMESPACE_END

namespace std {

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template get<I>();
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(const Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template get<I>();
}

} // namespace std
