#include "Tests.h"

#include "Deep/Simd/Int32x4.h"
#include <limits>

TEST(Int32x4, LanesMasksAndShifts) {
	using Scalar = Deep::int32;
	using Unsigned = Deep::uint32;
	const Scalar values[] = { 0, 1, std::numeric_limits<Scalar>::max(), std::numeric_limits<Scalar>::min() };
	for (Scalar value : values) {
		auto a = Deep::Int32x4::s_Replicate(value);
		EXPECT_EQ(Deep::Int32x4::s_Equals(a, a).ToBooleanBitMask(), 15u);
		for (Deep::int32 shift = 0; shift < 32; ++shift) {
			auto left = a << shift;
			auto right = a >> shift;
			for (size_t i = 0; i < 4; ++i) {
				EXPECT_EQ(left[i], static_cast<Scalar>(static_cast<Unsigned>(value) << shift));
				EXPECT_EQ(right[i], value >> shift);
			}
		}
		EXPECT_EQ((a ^ a), Deep::Int32x4::s_Replicate(0));
		EXPECT_EQ((a & ~a), Deep::Int32x4::s_Replicate(0));
		EXPECT_EQ((a | ~a), Deep::Int32x4::s_Replicate(static_cast<Scalar>(~Unsigned{ 0 })));
	}
	for (Deep::uint32 mask = 0; mask < 16; ++mask) {
		Deep::Int32x4 a;
		for (size_t i = 0; i < 4; ++i)
			a[i] = static_cast<Scalar>((mask & (1u << i)) ? (Unsigned{ 1 } << 31) : 1);
		EXPECT_EQ(a.ToBooleanBitMask(), mask);
		EXPECT_EQ(a, a);
		auto b = a;
		b[0] ^= 1;
		EXPECT_NE(a, b);
	}
}

TEST(Int32x4, ArithmeticAndSelect) {
	Deep::Int32x4 a{ 12, 24, 36, 48 };
	Deep::Int32x4 b{ 3, 4, 6, 8 };
	auto sum = a + b;
	auto difference = a - b;
	auto product = a * b;
	auto quotient = a / b;
	for (size_t i = 0; i < 4; ++i) {
		EXPECT_EQ(sum[i], a[i] + b[i]);
		EXPECT_EQ(difference[i], a[i] - b[i]);
		EXPECT_EQ(product[i], a[i] * b[i]);
		EXPECT_EQ(quotient[i], a[i] / b[i]);
		EXPECT_EQ(static_cast<Deep::int32>((a * Deep::int32{ 2 })[i]), a[i] * 2);
		EXPECT_EQ(static_cast<Deep::int32>((Deep::int32{ 2 } * a)[i]), 2 * a[i]);
		EXPECT_EQ(static_cast<Deep::int32>((a / Deep::int32{ 2 })[i]), a[i] / 2);
		EXPECT_EQ(static_cast<Deep::int32>(a.ToFloat()[i]), static_cast<Deep::float32>(a[i]));
	}
	EXPECT_EQ(Deep::Int32x4::s_Min(a, b), b);
	EXPECT_EQ(Deep::Int32x4::s_Max(a, b), a);
	for (Deep::uint32 mask = 0; mask < 16; ++mask) {
		Deep::Int32x4 control;
		for (size_t i = 0; i < 4; ++i)
			control[i] = (mask & (1u << i)) ? Deep::int32(0x80000000) : 1;
		auto selected = Deep::Int32x4::s_Select(a, b, control);
		for (size_t i = 0; i < 4; ++i)
			EXPECT_EQ(selected[i], (mask & (1u << i)) ? b[i] : a[i]);
	}
}

TEST(Int32x4, ConstructorsAndConstants) {
	constexpr auto constant = Deep::Int32x4::Constexpr(1, 2, 3, 4);
	static_assert(constant.m_values[0] == 1 && constant.m_values[3] == 4);
	Deep::Int32x2 low{ 1, 2 };
	Deep::Int32x2 high{ 3, 4 };
	Deep::Int32x4 combined{ low, high };
	Deep::Int32x4 extended{ low };
	EXPECT_EQ(combined, (Deep::Int32x4{ 1, 2, 3, 4 }));
	EXPECT_EQ(extended, (Deep::Int32x4{ 1, 2, 0, 0 }));
	EXPECT_EQ(Deep::Int32x2{ combined }, low);
	Deep::Int32x4 native{ combined.m_internal };
	EXPECT_EQ(native, combined);
	combined[3] = Deep::int32{ 9 };
	EXPECT_EQ(combined.w, 9);
}
