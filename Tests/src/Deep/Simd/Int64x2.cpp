#include "Tests.h"

#include "Deep/Simd/Int64x2.h"
#include <limits>

TEST(Int64x2, Equality) {
	Deep::Int64x2 a{ 0, 0 };
	Deep::Int64x2 b{ 0, 0 };
	EXPECT_EQ(a, b);
}

TEST(Int64x2, Shift) {
	TEST_CASE(ShiftLeft) {
		Deep::Int64x2 a{ Deep::int64{ 3 }, Deep::int64{ 6 } };
		a <<= 10;
		EXPECT_EQ(a.x, Deep::int64{ 3 } << 10);
		EXPECT_EQ(a.y, Deep::int64{ 6 } << 10);
	}

	TEST_CASE(ShiftRight) {
		Deep::Int64x2 a{ Deep::int64{ 0xacacbacacaca }, Deep::int64{ 0xfeffefefefefef } };
		a >>= 10;
		EXPECT_EQ(a.x, Deep::int64{ 0xacacbacacaca } >> 10);
		EXPECT_EQ(a.y, Deep::int64{ 0xfeffefefefefef } >> 10);
	}
}

TEST(Int64x2, LanesMasksAndShifts) {
	using Scalar = Deep::int64;
	using Unsigned = Deep::uint64;
	const Scalar values[] = { 0, 1, std::numeric_limits<Scalar>::max(), std::numeric_limits<Scalar>::min() };
	for (Scalar value : values) {
		auto a = Deep::Int64x2::s_Replicate(value);
		EXPECT_EQ(Deep::Int64x2::s_Equals(a, a).ToBooleanBitMask(), 3u);
		for (Deep::int32 shift = 0; shift < 64; ++shift) {
			auto left = a << shift;
			auto right = a >> shift;
			for (size_t i = 0; i < 2; ++i) {
				EXPECT_EQ(left[i], static_cast<Scalar>(static_cast<Unsigned>(value) << shift));
				EXPECT_EQ(right[i], value >> shift);
			}
		}
		EXPECT_EQ((a ^ a), Deep::Int64x2::s_Replicate(0));
		EXPECT_EQ((a & ~a), Deep::Int64x2::s_Replicate(0));
		EXPECT_EQ((a | ~a), Deep::Int64x2::s_Replicate(static_cast<Scalar>(~Unsigned{ 0 })));
	}
	for (Deep::uint32 mask = 0; mask < 4; ++mask) {
		Deep::Int64x2 a;
		for (size_t i = 0; i < 2; ++i)
			a[i] = static_cast<Scalar>((mask & (1u << i)) ? (Unsigned{ 1 } << 63) : 1);
		EXPECT_EQ(a.ToBooleanBitMask(), mask);
		EXPECT_EQ(a, a);
		auto b = a;
		b[0] ^= 1;
		EXPECT_NE(a, b);
	}
}

TEST(Int64x2, FullWidthConversion) {
	Deep::Int64x2 a{ static_cast<Deep::int64>(0x123456789abcdef0ull), static_cast<Deep::int64>(0xfedcba9876543210ull) };
	EXPECT_EQ(Deep::Int64x2{ Deep::UInt64x2{ a } }, a);
}
