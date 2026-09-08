#include "Tests.h"

#include "Deep/Math/Ops.h"
#include <limits>

TEST(Ops, ClampAndSign) {
	static_assert(Deep::Clamp(7, 2, 5) == 5);
	static_assert(Deep::Clamp01(-1.0f) == 0.0f);
	for (int value = -8; value <= 8; ++value) {
		int expected = value < -3 ? -3 : value > 4 ? 4 : value;
		EXPECT_EQ(Deep::Clamp(value, -3, 4), expected);
		EXPECT_EQ(Deep::Sign(value), value < 0 ? -1 : 1);
	}
	EXPECT_EQ(Deep::Clamp01(0.25f), 0.25f);
	EXPECT_EQ(Deep::Clamp01(2.0f), 1.0f);
	EXPECT_EQ(Deep::Sign(-0.0f), 1.0f);
}

TEST(Ops, MinMaxSecondOperand) {
	const float values[] = { -0.0f,
		                     0.0f,
		                     -2.0f,
		                     3.0f,
		                     std::numeric_limits<float>::infinity(),
		                     -std::numeric_limits<float>::infinity(),
		                     Deep::AsFloat(0x7fc12345),
		                     Deep::AsFloat(0xffc54321) };
	for (const float& a : values) {
		for (const float& b : values) {
			EXPECT_EQ(&Deep::Min(a, b), a < b ? &a : &b);
			EXPECT_EQ(&Deep::Max(a, b), a > b ? &a : &b);
		}
	}
}

TEST(Ops, IsCloseTolerances) {
	EXPECT_TRUE(Deep::IsClose(1.0f, 1.0f, 0.0f, 0.0f));
	EXPECT_TRUE(Deep::IsClose(0.125f, 0.0f, 0.0f, 0.125f));
	EXPECT_FALSE(Deep::IsClose(0.25f, 0.0f, 0.0f, 0.125f));
	EXPECT_TRUE(Deep::IsClose(112.0f, 100.0f, 0.125f, 0.0f));
	EXPECT_FALSE(Deep::IsClose(113.0f, 100.0f, 0.125f, 0.0f));
	EXPECT_FALSE(Deep::IsClose(Deep::AsFloat(0x7fc12345), 1.0f));
}

TEST(Ops, ElementaryFunctions) {
	EXPECT_EQ(Deep::Sqrt(0.0f), 0.0f);
	EXPECT_EQ(Deep::Sqrt(81.0f), 9.0f);
	EXPECT_EQ(Deep::Exp(0.0f), 1.0f);
	EXPECT_EQ(Deep::Exp2(-3.0f), 0.125f);
	EXPECT_EQ(Deep::Abs(-12), 12);
	EXPECT_EQ(Deep::Sin(0.0f), 0.0f);
	EXPECT_EQ(Deep::Cos(0.0f), 1.0f);
	EXPECT_TRUE(Deep::IsClose(Deep::ATan2(1.0f, 0.0f), Deep::k_pi * 0.5f));
	EXPECT_TRUE(Deep::IsClose(Deep::ATan2(-1.0f, 0.0f), -Deep::k_pi * 0.5f));
}
