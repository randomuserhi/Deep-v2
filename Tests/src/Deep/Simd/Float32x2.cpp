#include "Tests.h"

#include "Deep/Simd/Float32x2.h"
#include "Deep/Bit.h"
#include <cmath>
#include <limits>

TEST(Float32x2, Equality) {
	Deep::Float32x2 a{ 1, 4 };
	Deep::Float32x2 b{ 1, 4 };
	EXPECT_EQ(a, b);
}

TEST(Float32x2, ArithmeticAndConversions) {
	Deep::Float32x2 a{ -3.75f, 2.5f };
	Deep::Float32x2 b{ 2.0f, -4.0f };
	auto sum = a + b;
	auto difference = a - b;
	auto product = a * b;
	auto quotient = a / b;
	auto integers = a.ToInt();
	auto floats = integers.ToFloat();
	for (size_t i = 0; i < 2; ++i) {
		EXPECT_EQ(sum[i], a[i] + b[i]);
		EXPECT_EQ(difference[i], a[i] - b[i]);
		EXPECT_EQ(product[i], a[i] * b[i]);
		EXPECT_EQ(quotient[i], a[i] / b[i]);
		EXPECT_EQ(integers[i], static_cast<Deep::int32>(a[i]));
		EXPECT_EQ(floats[i], static_cast<Deep::float32>(integers[i]));
		EXPECT_EQ(static_cast<Deep::float32>((-a)[i]), 0.0f - a[i]);
		EXPECT_EQ(static_cast<Deep::float32>((a * 2.0f)[i]), a[i] * 2.0f);
		EXPECT_EQ(static_cast<Deep::float32>((2.0f * a)[i]), 2.0f * a[i]);
		EXPECT_EQ(static_cast<Deep::float32>((a / 2.0f)[i]), a[i] / 2.0f);
		EXPECT_EQ(static_cast<Deep::float32>((2.0f / a)[i]), 2.0f / a[i]);
	}
	EXPECT_EQ(a.ReinterpretAsInt().ReinterpretAsFloat(), a);
	EXPECT_EQ((a | b).ReinterpretAsInt(), (a.ReinterpretAsInt() | b.ReinterpretAsInt()));
	EXPECT_EQ((a & b).ReinterpretAsInt(), (a.ReinterpretAsInt() & b.ReinterpretAsInt()));
	EXPECT_EQ((a ^ b).ReinterpretAsInt(), (a.ReinterpretAsInt() ^ b.ReinterpretAsInt()));
}

TEST(Float32x2, MinMaxSecondOperand) {
	const Deep::float32 nan = Deep::BitCast<Deep::float32>(Deep::uint32{ 0x7fc12345 });
	const Deep::float32 values[] = { nan,
		                             Deep::BitCast<Deep::float32>(Deep::uint32{ 0xffc54321 }),
		                             -std::numeric_limits<Deep::float32>::infinity(),
		                             -2.0f,
		                             -0.0f,
		                             0.0f,
		                             2.0f,
		                             std::numeric_limits<Deep::float32>::infinity() };
	for (Deep::float32 a : values) {
		for (Deep::float32 b : values) {
			auto min = Deep::Float32x2::s_Min(Deep::Float32x2::s_Replicate(a), Deep::Float32x2::s_Replicate(b));
			auto max = Deep::Float32x2::s_Max(Deep::Float32x2::s_Replicate(a), Deep::Float32x2::s_Replicate(b));
			for (size_t i = 0; i < 2; ++i) {
				EXPECT_EQ(Deep::BitCast<Deep::uint32>(min[i]), Deep::BitCast<Deep::uint32>(a < b ? a : b));
				EXPECT_EQ(Deep::BitCast<Deep::uint32>(max[i]), Deep::BitCast<Deep::uint32>(a > b ? a : b));
			}
		}
	}
}

TEST(Float32x2, MasksAndSinCos) {
	for (Deep::uint32 mask = 0; mask < 4; ++mask) {
		Deep::Int32x2 control;
		for (size_t i = 0; i < 2; ++i)
			control[i] = (mask & (1u << i)) ? Deep::int32(0x80000001) : 1;
		EXPECT_EQ(control.ToBooleanBitMask(), mask);
		auto selected =
			Deep::Float32x2::s_Select(Deep::Float32x2::s_Replicate(3), Deep::Float32x2::s_Replicate(-4), control);
		EXPECT_EQ(Deep::Float32x2::s_IsNegative(selected).ToBooleanBitMask(), mask);
		EXPECT_EQ(Deep::Float32x2::s_Equals(selected, Deep::Float32x2::s_Replicate(-4)).ToBooleanBitMask(), mask);
	}
	for (Deep::int32 step = -32; step <= 32; ++step) {
		Deep::float32 angle = static_cast<Deep::float32>(step) * 0.25f;
		auto input = Deep::Float32x2::s_Replicate(angle);
		Deep::Float32x2 sine, cosine;
		input.SinCos(sine, cosine);
		for (size_t i = 0; i < 2; ++i) {
			EXPECT_TRUE(std::abs(sine[i] - std::sin(angle)) < 0.000001f);
			EXPECT_TRUE(std::abs(cosine[i] - std::cos(angle)) < 0.000001f);
		}
	}
}
