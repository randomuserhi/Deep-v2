#pragma once

#include "Deep.h"
#include "Deep/Templates.h"

DEEP_NAMESPACE_BEGIN

#define CONSTRUCTOR_ARGS_TEMPLATE template<typename T, typename... Args>
#define CONSTRUCTOR_ARGS ConstructorArgs<T, Args...>

CONSTRUCTOR_ARGS_TEMPLATE
[[nodiscard]] constexpr T CONSTRUCTOR_ARGS::Construct() && noexcept(std::is_nothrow_constructible_v<T, Args&&...>) {
	return std::move(*this).ConstructImpl(std::index_sequence_for<Args...>{});
}

CONSTRUCTOR_ARGS_TEMPLATE
template<std::size_t... Is>
constexpr T
CONSTRUCTOR_ARGS::ConstructImpl(std::index_sequence<Is...>) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>) {
	return T(std::move(m_args).template Get<Is>()...);
}

CONSTRUCTOR_ARGS_TEMPLATE
void CONSTRUCTOR_ARGS::Construct(T* in_destination) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>) {
	std::move(*this).ConstructImpl(in_destination, std::index_sequence_for<Args...>{});
}

CONSTRUCTOR_ARGS_TEMPLATE
template<std::size_t... Is>
void CONSTRUCTOR_ARGS::ConstructImpl(T* in_destination,
                                     std::index_sequence<Is...>) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>) {
	::new (in_destination) T(std::move(m_args).template Get<Is>()...);
}

CONSTRUCTOR_ARGS_TEMPLATE
[[nodiscard]] constexpr inline auto ConstructWith(Args&&... args) {
	return ConstructorArgs<T, std::unwrap_reference_t<Args>...>{ std::forward<Args>(args)... };
}

template<typename T, _ConstructorArgs... in_Args>
[[nodiscard]] constexpr static inline size_t CountConstructorArgs() {
	return (0 + ... + std::is_same_v<T, typename std::decay_t<in_Args>::Type>);
}

#undef CONSTRUCTOR_ARGS_TEMPLATE
#undef CONSTRUCTOR_ARGS

DEEP_NAMESPACE_END
