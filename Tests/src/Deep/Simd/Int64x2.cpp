#include "Tests.h"

#include "Deep/Types.h"
#include "Deep/Simd/Int64x2.h"

TEST(Int64x2, Equality) {
	Deep::Int64x2 a{ 0, 0 };
	Deep::Int64x2 b{ 0, 0 };
	EXPECT_EQ(a, b);
}

TEST(Int64x2, Shift) {
	TEST_CASE(ShiftLeft) {
		Deep::Int64x2 a{ int64{ 3 }, int64{ 6 } };
		a <<= 10;
		EXPECT_EQ(a.x, int64{ 3 } << 10);
		EXPECT_EQ(a.y, int64{ 6 } << 10);
	}

	TEST_CASE(ShiftRight) {
		Deep::Int64x2 a{ int64{ 0xacacbacacaca }, int64{ 0xfeffefefefefef } };
		a >>= 10;
		EXPECT_EQ(a.x, int64{ 0xacacbacacaca } >> 10);
		EXPECT_EQ(a.y, int64{ 0xfeffefefefefef } >> 10);
	}
}
