#include "Tests.h"

#define DEEP_USE_TYPE_ALIASES
#include "Deep.h"
#include "Deep/Memory.h"
#include "Deep/ConstructWith.h"
#include "Deep/Containers/Tuple.h"

template<typename... Specs>
[[nodiscard]] constexpr auto ConstructAll(Specs&&... specs) {
	return Deep::ConstructTuple(std::forward<Specs>(specs)...);
}

struct Widget {
	Widget(int32 in_id, const char* in_name) :
		m_id{ in_id }, m_name{ in_name } {}

	int32 m_id;
	const char* m_name;
};

struct Connection {
	Connection(const char* in_address, int32 in_port) :
		m_port{ in_port }, m_address{ in_address } {}

	int32 m_port;
	const char* m_address;
};

TEST(Templates, ConstructAll) {
	const char* str0 = "Main";
	const char* str1 = "localhost";
	auto [widget, connection] =
		ConstructAll(Deep::ConstructWith<Widget>(42, str0), Deep::ConstructWith<Connection>(str1, 8080));

	EXPECT_EQ(widget.m_id, 42);
	EXPECT_EQ(widget.m_name, str0);

	EXPECT_EQ(connection.m_port, 8080);
	EXPECT_EQ(connection.m_address, str1);
}

TEST(Templates, Construct) {
	TEST_CASE(Default) {
		struct A {
			int32 m_value;
		};

		A a = Deep::ConstructWith<A>(42).Construct();

		EXPECT_EQ(a.m_value, 42);
	}

	TEST_CASE(Explicit) {
		const char* str = "Main";
		auto widget = Deep::ConstructWith<Widget>(42, str).Construct();

		EXPECT_EQ(widget.m_id, 42);
		EXPECT_EQ(widget.m_name, str);
	}

	TEST_CASE(PlacementNew) {
		const char* str = "Main";
		Widget* widget = Deep::TMalloc<Widget>(1);
		Deep::ConstructWith<Widget>(42, str).Construct(widget);

		EXPECT_EQ(widget->m_id, 42);
		EXPECT_EQ(widget->m_name, str);

		Deep::TFree(widget);
	}

	TEST_CASE(Reference) {
		struct A {
			A(int32& in_value) :
				m_value{ in_value } {}

			int32& m_value;
		};

		int32 value = 42;
		auto a = Deep::ConstructWith<A>(value).Construct();

		EXPECT_EQ(a.m_value, 42);
		value = 12;
		EXPECT_EQ(a.m_value, 12);
	}
}
