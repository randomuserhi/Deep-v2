#include "Tests.h"

#include "Deep/Simd/Float32x2.h"

TEST(Float32x2, Equality) {
	Deep::Float32x2 a{ 1, 4 };
	Deep::Float32x2 b{ 1, 4 };
	EXPECT_EQ(a, b);
}
