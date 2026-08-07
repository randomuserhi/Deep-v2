#include "Tests.h"

#include "Deep.h"
#include "Deep/Types.h"
#include "Deep/Simd/Float32x4.h"

TEST(Float32x4, Select) {
	Deep::Float32x4 a{ 1, 2, 3, 4 };
	Deep::Float32x4 b{ 5, 6, 7, 8 };

	Deep::Float32x4 result =
		Deep::Float32x4::s_Select(a, b, Deep::Int32x4{ 0x00000000, 0x00000001, int32(0x80000000), int32(0xffffffff) });

	EXPECT_EQ(result.x, 1);
	EXPECT_EQ(result.y, 2); // nonzero, but sign bit isn't set
	EXPECT_EQ(result.z, 7);
	EXPECT_EQ(result.w, 8);
}
