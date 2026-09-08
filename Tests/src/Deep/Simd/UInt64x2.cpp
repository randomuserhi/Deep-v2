#include "Tests.h"

#include "Deep/Simd/UInt64x2.h"
#include <limits>

TEST(UInt64x2, Shuffle) {
	Deep::UInt64x2 a{ 0x123456789abcdef0ull, 0xfedcba9876543210ull };
	EXPECT_EQ((Deep::UInt64x2::s_Shuffle<0, 1>(a)), a);
	EXPECT_EQ((Deep::UInt64x2::s_Shuffle<1, 0>(a)), (Deep::UInt64x2{ a.y, a.x }));
	EXPECT_EQ((Deep::UInt64x2::s_Shuffle<0, 0>(a)), (Deep::UInt64x2{ a.x, a.x }));
	EXPECT_EQ((Deep::UInt64x2::s_Shuffle<1, 1>(a)), (Deep::UInt64x2{ a.y, a.y }));
}

TEST(UInt64x2, LanesMasksAndShifts) {
	using Scalar = Deep::uint64;
	using Unsigned = Deep::uint64;
	const Scalar values[] = { 0, 1, std::numeric_limits<Scalar>::max(), std::numeric_limits<Scalar>::min() };
	for (Scalar value : values) {
		auto a = Deep::UInt64x2::s_Replicate(value);
		EXPECT_EQ(Deep::UInt64x2::s_Equals(a, a).ToBooleanBitMask(), 3u);
		for (Deep::int32 shift = 0; shift < 64; ++shift) {
			auto left = a << shift;
			auto right = a >> shift;
			for (size_t i = 0; i < 2; ++i) {
				EXPECT_EQ(left[i], static_cast<Scalar>(static_cast<Unsigned>(value) << shift));
				EXPECT_EQ(right[i], value >> shift);
			}
		}
		EXPECT_EQ((a ^ a), Deep::UInt64x2::s_Replicate(0));
		EXPECT_EQ((a & ~a), Deep::UInt64x2::s_Replicate(0));
		EXPECT_EQ((a | ~a), Deep::UInt64x2::s_Replicate(static_cast<Scalar>(~Unsigned{ 0 })));
	}
	for (Deep::uint32 mask = 0; mask < 4; ++mask) {
		Deep::UInt64x2 a;
		for (size_t i = 0; i < 2; ++i)
			a[i] = static_cast<Scalar>((mask & (1u << i)) ? (Unsigned{ 1 } << 63) : 1);
		EXPECT_EQ(a.ToBooleanBitMask(), mask);
		EXPECT_EQ(a, a);
		auto b = a;
		b[0] ^= 1;
		EXPECT_NE(a, b);
	}
}

TEST(UInt64x2, FullWidthConversion) {
	Deep::UInt64x2 a{ static_cast<Deep::uint64>(0x123456789abcdef0ull), static_cast<Deep::uint64>(0xfedcba9876543210ull) };
	EXPECT_EQ(Deep::UInt64x2{ Deep::Int64x2{ a } }, a);
}
