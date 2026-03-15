#include "Tests.h"

#include "Deep.h"
#include "Deep/Tuple.h"

TEST(Tuple, Primitives) {
	Deep::Tuple<int32, int32> tuple{ 5, 10 };

	EXPECT_EQ(std::get<0>(tuple), 5);
	EXPECT_EQ(std::get<1>(tuple), 10);

	int a = 5;
	int b = 10;

	auto tie = Deep::Tie(a, b);

	EXPECT_EQ(std::get<0>(tie), 5);
	EXPECT_EQ(std::get<1>(tie), 10);
}

TEST(Tuple, TrivialStructs) {
	struct A {
		int32 a;
		int32 b;
	};

	Deep::Tuple<A, A> tuple{ A{ 5, 10 }, A{ 2, 3 } };

	EXPECT_EQ(std::get<0>(tuple).a, 5);
	EXPECT_EQ(std::get<0>(tuple).b, 10);

	EXPECT_EQ(std::get<1>(tuple).a, 2);
	EXPECT_EQ(std::get<1>(tuple).b, 3);
}
