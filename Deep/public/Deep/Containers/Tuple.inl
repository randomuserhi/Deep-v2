#pragma once

#include "Deep/Containers/Tuple.h"

DEEP_NAMESPACE_BEGIN

namespace Detail {

template<typename T>
constexpr auto& TupleStorage<T>::Get() {
	return data;
}

template<typename T>
constexpr const auto& TupleStorage<T>::Get() const {
	return data;
}

template<std::size_t... Is, typename... Ts>
template<typename... Args>
constexpr TupleImpl<std::index_sequence<Is...>, Ts...>::TupleImpl(Args&&... args) :
	TupleStorage<TupleIndexTag<Is, Ts>>{ std::forward<Args>(args) }... {}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::Get() {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	return static_cast<TupleStorage<TupleIndexTag<I, T>>&>(*this).Get();
}

template<std::size_t... Is, typename... Ts>
template<std::size_t I>
constexpr decltype(auto) TupleImpl<std::index_sequence<Is...>, Ts...>::Get() const {
	using T = typename TupleTypeAtIndex<I, Ts...>::type;
	return static_cast<const TupleStorage<TupleIndexTag<I, T>>&>(*this).Get();
}

} // namespace Detail

template<typename... Ts>
constexpr auto MakeTuple(Ts&&... in_args) {
	return Tuple<Deep::Detail::Unwrap<Ts>...>(std::forward<Ts>(in_args)...);
}

template<typename... Ts>
constexpr auto Tie(Ts&... in_args) {
	return Tuple<Ts&...>(in_args...);
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) Get(::Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template Get<I>();
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) Get(const ::Deep::Tuple<Ts...>& in_tuple) {
	return in_tuple.template Get<I>();
}

DEEP_NAMESPACE_END

namespace std {

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(::Deep::Tuple<Ts...>& in_tuple) {
	return Deep::Get<I, Ts...>(in_tuple);
}

template<std::size_t I, typename... Ts>
constexpr decltype(auto) get(const ::Deep::Tuple<Ts...>& in_tuple) {
	return Deep::Get<I, Ts...>(in_tuple);
}

} // namespace std
