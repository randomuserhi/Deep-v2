#include "Tests.h"

#include "Deep.h"
#include "Deep/Types.h"
#include "Deep/Simd/Float32x4.h"
#include "Deep/Bit.h"
#include <cmath>
#include <limits>

TEST(Float32x4, Select) {
	Deep::Float32x4 a{ 1, 2, 3, 4 };
	Deep::Float32x4 b{ 5, 6, 7, 8 };

	Deep::Float32x4 result = Deep::Float32x4::s_Select(
		a, b, Deep::Int32x4{ 0x00000000, 0x00000001, Deep::int32(0x80000000), Deep::int32(0xffffffff) });

	EXPECT_EQ(result.x, 1);
	EXPECT_EQ(result.y, 2); // nonzero, but sign bit isn't set
	EXPECT_EQ(result.z, 7);
	EXPECT_EQ(result.w, 8);
}

TEST(Float32x4, ArithmeticAndConversions) {
	Deep::Float32x4 a{ -3.75f, 2.5f, -1.25f, 4.75f };
	Deep::Float32x4 b{ 2.0f, -4.0f, 5.0f, 8.0f };
	auto sum = a + b;
	auto difference = a - b;
	auto product = a * b;
	auto quotient = a / b;
	auto integers = a.ToInt();
	auto floats = integers.ToFloat();
	for (size_t i = 0; i < 4; ++i) {
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

TEST(Float32x4, MinMaxSecondOperand) {
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
			auto min = Deep::Float32x4::s_Min(Deep::Float32x4::s_Replicate(a), Deep::Float32x4::s_Replicate(b));
			auto max = Deep::Float32x4::s_Max(Deep::Float32x4::s_Replicate(a), Deep::Float32x4::s_Replicate(b));
			for (size_t i = 0; i < 4; ++i) {
				EXPECT_EQ(Deep::BitCast<Deep::uint32>(min[i]), Deep::BitCast<Deep::uint32>(a < b ? a : b));
				EXPECT_EQ(Deep::BitCast<Deep::uint32>(max[i]), Deep::BitCast<Deep::uint32>(a > b ? a : b));
			}
		}
	}
}

TEST(Float32x4, MasksAndSinCos) {
	for (Deep::uint32 mask = 0; mask < 16; ++mask) {
		Deep::Int32x4 control;
		for (size_t i = 0; i < 4; ++i)
			control[i] = (mask & (1u << i)) ? Deep::int32(0x80000001) : 1;
		EXPECT_EQ(control.ToBooleanBitMask(), mask);
		auto selected =
			Deep::Float32x4::s_Select(Deep::Float32x4::s_Replicate(3), Deep::Float32x4::s_Replicate(-4), control);
		EXPECT_EQ(Deep::Float32x4::s_IsNegative(selected).ToBooleanBitMask(), mask);
		EXPECT_EQ(Deep::Float32x4::s_Equals(selected, Deep::Float32x4::s_Replicate(-4)).ToBooleanBitMask(), mask);
	}
	for (Deep::int32 step = -32; step <= 32; ++step) {
		Deep::float32 angle = static_cast<Deep::float32>(step) * 0.25f;
		auto input = Deep::Float32x4::s_Replicate(angle);
		Deep::Float32x4 sine, cosine;
		input.SinCos(sine, cosine);
		for (size_t i = 0; i < 4; ++i) {
			EXPECT_TRUE(std::abs(sine[i] - std::sin(angle)) < 0.000001f);
			EXPECT_TRUE(std::abs(cosine[i] - std::cos(angle)) < 0.000001f);
		}
	}
}

TEST(Float32x4, ConstructorsAndConstants) {
	constexpr auto constant = Deep::Float32x4::Constexpr(1, 2, 3, 4);
	static_assert(constant.m_values[0] == 1 && constant.m_values[3] == 4);
	Deep::Float32x2 low{ 1, 2 };
	Deep::Float32x2 high{ 3, 4 };
	Deep::Float32x4 combined{ low, high };
	Deep::Float32x4 extended{ low };
	EXPECT_EQ(combined, (Deep::Float32x4{ 1, 2, 3, 4 }));
	EXPECT_EQ(extended, (Deep::Float32x4{ 1, 2, 0, 0 }));
	EXPECT_EQ(Deep::Float32x2{ combined }, low);
	Deep::Float32x4 native{ combined.m_internal };
	EXPECT_EQ(native, combined);
	combined[3] = Deep::float32{ 9 };
	EXPECT_EQ(combined.w, 9);
}
