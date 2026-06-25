#include "Tests.h"

#include "Deep/Simd/Int32x2.h"

TEST(Int32x2, Equality) {
	Deep::Int32x2 a{ 1, 4 };
	Deep::Int32x2 b{ 1, 4 };
	EXPECT_EQ(a, b);
}

TEST(Int32x2, Add) {
	Deep::Int32x2 a{ 2, 4 };
	Deep::Int32x2 b{ 1, 5 };
	Deep::Int32x2 c = a + b;
	EXPECT_EQ(c.x, 3);
	EXPECT_EQ(c.y, 9);
}
