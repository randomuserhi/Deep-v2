#include "Tests.h"

#include "Deep/Bit.h"
#include <limits>

TEST(Bit, CountsAndPowersOfTwo) {
	auto check = [&]<typename T>() {
		constexpr int width = std::numeric_limits<T>::digits;
		EXPECT_EQ(Deep::CountSetBits(T{ 0 }), 0);
		EXPECT_EQ(Deep::CountTrailingZeros(T{ 0 }), width);
		EXPECT_FALSE(Deep::IsPowerOf2(T{ 0 }));
		for (int bit = 0; bit < width; ++bit) {
			T value = static_cast<T>(T{ 1 } << bit);
			EXPECT_TRUE(Deep::IsPowerOf2(value));
			EXPECT_EQ(Deep::CountSetBits(value), 1);
			EXPECT_EQ(Deep::CountTrailingZeros(value), bit);
			EXPECT_EQ(Deep::CountSetBits(static_cast<T>(value - 1)), bit);
			if (bit > 0) EXPECT_FALSE(Deep::IsPowerOf2(static_cast<T>(value | 1)));
		}
		EXPECT_EQ(Deep::CountSetBits(std::numeric_limits<T>::max()), width);
	};
	check.operator()<Deep::uint8>();
	check.operator()<Deep::uint16>();
	check.operator()<Deep::uint32>();
	check.operator()<Deep::uint64>();
}

TEST(Bit, Rotations) {
	static_assert(Deep::RotateLeft(Deep::uint32{ 0x12345678 }, 8) == 0x34567812);
	static_assert(Deep::RotateRight(Deep::uint32{ 0x12345678 }, 8) == 0x78123456);
	auto check = [&]<typename T>() {
		constexpr int width = std::numeric_limits<T>::digits;
		T value = static_cast<T>(0x81a5f03c12345678ull);
		for (int shift = -2 * width; shift <= 2 * width; ++shift) {
			EXPECT_EQ(Deep::RotateRight(Deep::RotateLeft(value, shift), shift), value);
			EXPECT_EQ(Deep::RotateLeft(value, shift), Deep::RotateRight(value, -shift));
			EXPECT_EQ(Deep::RotateLeft(value, shift), Deep::RotateLeft(value, shift + width));
		}
	};
	check.operator()<Deep::uint8>();
	check.operator()<Deep::uint16>();
	check.operator()<Deep::uint32>();
	check.operator()<Deep::uint64>();
}

TEST(Bit, Endianness) {
	static_assert(Deep::ReverseEndianness(Deep::uint16{ 0x1234 }) == 0x3412);
	static_assert(Deep::ReverseEndianness(Deep::uint32{ 0x12345678 }) == 0x78563412);
	static_assert(Deep::ReverseEndianness(Deep::uint64{ 0x0123456789abcdef }) == 0xefcdab8967452301);
	auto check = [&]<typename T>() {
		const T values[] = { 0, 1, std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
		for (T value : values) {
			EXPECT_EQ(Deep::ReverseEndianness(Deep::ReverseEndianness(value)), value);
			EXPECT_EQ(Deep::WireToHost<std::endian::big>(Deep::HostToWire<std::endian::big>(value)), value);
			EXPECT_EQ(Deep::WireToHost<std::endian::little>(Deep::HostToWire<std::endian::little>(value)), value);
			EXPECT_EQ(Deep::HostToWire<std::endian::native>(value), value);
		}
	};
	check.operator()<Deep::int8>();
	check.operator()<Deep::uint8>();
	check.operator()<Deep::int16>();
	check.operator()<Deep::uint16>();
	check.operator()<Deep::int32>();
	check.operator()<Deep::uint32>();
	check.operator()<Deep::int64>();
	check.operator()<Deep::uint64>();
}

TEST(Bit, FloatRepresentation) {
	const Deep::uint32 values[] = { 0, 0x80000000, 0x3f800000, 0xbf800000, 0x7f800000, 0xff800000, 0x7fc12345 };
	for (Deep::uint32 bits : values)
		EXPECT_EQ(Deep::AsUInt(Deep::AsFloat(bits)), bits);
}

TEST(Bit, HalfPrecisionValues) {
	struct Case {
		Deep::uint16 m_bits;
		float m_value;
	};
	// Deep's half format uses exponent 31 for finite values rather than infinity/NaN.
	const Case cases[] = { { 0, 0.0f },      { 0x8000, -0.0f },    { 0x3c00, 1.0f }, { 0xc000, -2.0f },
		                   { 0x3800, 0.5f }, { 0x0400, 0x1p-14f }, { 1, 0x1p-24f },  { 0x7fff, 131008.0f } };
	for (const Case& test : cases) {
		EXPECT_EQ(Deep::AsUInt(Deep::HalfToFloat(test.m_bits)), Deep::AsUInt(test.m_value));
		EXPECT_EQ(Deep::FloatToHalf(test.m_value), test.m_bits);
	}
}

TEST(Bit, AlignmentAndContainer) {
	alignas(64) Deep::uint8 bytes[128]{};
	EXPECT_TRUE(Deep::IsAligned(bytes, 64));
	EXPECT_TRUE(Deep::IsAligned(bytes + 64, 64));
	EXPECT_FALSE(Deep::IsAligned(bytes + 1, 64));
	EXPECT_EQ((Deep::AssumeAligned<Deep::uint8, 64>(bytes)), bytes);
	struct Container {
		int m_first;
		double m_second;
	};
	Container value{ 3, 4.0 };
	EXPECT_EQ(Deep::ContainerOf(&value.m_first, &Container::m_first), &value);
	EXPECT_EQ(Deep::ContainerOf(&value.m_second, &Container::m_second), &value);
}
