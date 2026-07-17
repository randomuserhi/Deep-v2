#pragma once

#include "Deep.h"

#include <type_traits>
#include <utility>

DEEP_NAMESPACE_BEGIN

namespace Detail {

template<typename... Ts>
struct Tuple;

}

template<typename T, typename... Args>
class ConstructorArgs;

namespace Detail {

template<typename T>
struct IsConstructorArgs : std::false_type {};

template<typename T, typename... Args>
struct IsConstructorArgs<ConstructorArgs<T, Args...>> : std::true_type {};

} // namespace Detail

template<typename T>
concept _ConstructorArgs = Detail::IsConstructorArgs<std::remove_cvref_t<T>>::value;

// A non-owning single use container for constructor arguments.
//
// Useful for providing tagged constructor arguments:
// ```cpp
// template<typename... Specs>
// [[nodiscard]] constexpr auto ConstructAll(Specs&&... specs) {
// 	return Deep::ConstructTuple(std::forward<Specs>(specs)...);
// }
//
// struct Widget {
// 	Widget(int32 in_id, const char* in_name) :
// 		m_id{ in_id }, m_name{ in_name } {}
//
// 	int32 m_id;
// 	const char* m_name;
// };
//
// struct Connection {
// 	Connection(const char* in_address, int32 in_port) :
// 		m_port{ in_port }, m_address{ in_address } {}
//
// 	int32 m_port;
// 	const char* m_address;
// };
//
// auto [widget, connection] = ConstructAll(
//     Deep::ConstructWith<Widget>(42, "Main"),
//     Deep::ConstructWith<Connection>("localhost", 8080u)
// );
// ```
//
// As it is non-owning, its undefined behaviour to store the descriptor as it may result in dangling references.
// Always consume it immediately:
// ```cpp
// auto ctorArgs = ConstructorArgs<A>{...}; // Invalid
// ```
template<typename T, typename... Args>
class ConstructorArgs final {
public:
	using Type = T;
	using ArgumentsTuple = Detail::Tuple<Args&&...>;

	constexpr explicit ConstructorArgs(Args&&... in_args) noexcept(
		std::is_nothrow_constructible_v<ArgumentsTuple, Args&&...>) :
		m_args(std::forward<Args>(in_args)...) {}

	ConstructorArgs(const ConstructorArgs&) = delete;
	ConstructorArgs& operator=(const ConstructorArgs&) = delete;

	ConstructorArgs(ConstructorArgs&&) = default;
	ConstructorArgs& operator=(ConstructorArgs&&) = delete;

	// Constructs the object that this constructor template describes.
	//
	// Once called, this template is no longer valid (it has been consumed).
	// Calling it again is undefined behaviour.
	[[nodiscard]] constexpr T Construct() && noexcept(std::is_nothrow_constructible_v<T, Args&&...>);

	// Constructs the object at a given pointer location.
	//
	// Once called, this template is no longer valid (it has been consumed).
	// Calling it again is undefined behaviour.
	void Construct(T* in_destination) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>);

	T Construct() & = delete;
	T Construct() const& = delete;
	T Construct() const&& = delete;

private:
	template<std::size_t... Is>
	constexpr T ConstructImpl(std::index_sequence<Is...>) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>);

	template<std::size_t... Is>
	void ConstructImpl(T*, std::index_sequence<Is...>) && noexcept(std::is_nothrow_constructible_v<T, Args&&...>);

	ArgumentsTuple m_args;
};

template<typename T, typename... Args>
[[nodiscard]] constexpr inline auto ConstructWith(Args&&... args);

DEEP_NAMESPACE_END

#include "Deep/Containers/Tuple.h" // IWYU pragma: export
#include "Deep/Templates.inl"      // IWYU pragma: export
