#include "Tests.h"

#include "Deep/Simd/Int64x2.h"

TEST(Int64x2, Equality) {
	Deep::Int64x2 a{ 0, 0 };
	Deep::Int64x2 b{ 0, 0 };
	EXPECT_EQ(a, b);
}
