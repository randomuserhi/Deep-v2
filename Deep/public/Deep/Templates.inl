#pragma once

#include "Deep.h"
#include "Deep/Templates.h"

DEEP_NAMESPACE_BEGIN

template<typename T, typename... Args>
[[nodiscard]] constexpr T
ConstructorArgs<T, Args...>::Construct() && noexcept(std::is_nothrow_constructible_v<T, Args&&...>) {
	return std::move(*this).ConstructImpl(std::index_sequence_for<Args...>{});
}

template<typename T, typename... Args>
template<std::size_t... Is>
constexpr T ConstructorArgs<T, Args...>::ConstructImpl(std::index_sequence<Is...>) && noexcept(
	std::is_nothrow_constructible_v<T, Args&&...>) {
	return T(std::move(m_args).template Get<Is>()...);
}

template<typename T, typename... Args>
void ConstructorArgs<T, Args...>::Construct(T* in_destination) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>) {
	std::move(*this).ConstructImpl(in_destination, std::index_sequence_for<Args...>{});
}

template<typename T, typename... Args>
template<std::size_t... Is>
void ConstructorArgs<T, Args...>::ConstructImpl(T* in_destination, std::index_sequence<Is...>) && noexcept(
	std::is_nothrow_constructible_v<T, Args&&...>) {
	::new (in_destination) T(std::move(m_args).template Get<Is>()...);
}

template<typename T, typename... Args>
[[nodiscard]] constexpr inline ConstructorArgs<T, Detail::ResolveConstructorArgType<Args>...> ConstructWith(Args&&... args) {
	return ConstructorArgs<T, Detail::ResolveConstructorArgType<Args>...>{ std::forward<Args>(args)... };
}

DEEP_NAMESPACE_END
