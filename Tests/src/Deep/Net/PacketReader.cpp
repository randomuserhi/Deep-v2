#include "Tests.h"

#include "Deep/Net/PacketReader.h"
#include <limits>

TEST(PacketReader, BigEndianFixture) {
	const Deep::uint8 bytes[] = { 0xee, 0x12, 0x34, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		                          0xff, 0xfe, 0x80, 0,    0,    0,    0x80, 0,    0,    0,    0,    0,    0,    0 };
	Deep::PacketReader<std::endian::big> reader{ bytes, sizeof bytes };
	EXPECT_EQ(reader.ReadUInt8(), 0xee);
	EXPECT_EQ(reader.ReadUInt16(), 0x1234);
	EXPECT_EQ(reader.ReadUInt32(), 0x89abcdefu);
	EXPECT_EQ(reader.ReadUInt64(), 0x0123456789abcdefull);
	EXPECT_EQ(reader.ReadInt16(), -2);
	EXPECT_EQ(reader.ReadInt32(), std::numeric_limits<Deep::int32>::min());
	EXPECT_EQ(reader.ReadInt64(), std::numeric_limits<Deep::int64>::min());
	EXPECT_EQ(reader.m_head, reader.m_tail);
}

TEST(PacketReader, LittleEndianFixture) {
	const Deep::uint8 bytes[] = { 0xee, 0x34, 0x12, 0xef, 0xcd, 0xab, 0x89, 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,
		                          0xfe, 0xff, 0,    0,    0,    0x80, 0,    0,    0,    0,    0,    0,    0,    0x80 };
	Deep::PacketReader<std::endian::little> reader{ bytes, sizeof bytes };
	EXPECT_EQ(reader.ReadUInt8(), 0xee);
	EXPECT_EQ(reader.ReadUInt16(), 0x1234);
	EXPECT_EQ(reader.ReadUInt32(), 0x89abcdefu);
	EXPECT_EQ(reader.ReadUInt64(), 0x0123456789abcdefull);
	EXPECT_EQ(reader.ReadInt16(), -2);
	EXPECT_EQ(reader.ReadInt32(), std::numeric_limits<Deep::int32>::min());
	EXPECT_EQ(reader.ReadInt64(), std::numeric_limits<Deep::int64>::min());
	EXPECT_EQ(reader.m_head, reader.m_tail);
}

TEST(PacketReader, RemainingBytes) {
	const Deep::uint8 bytes[] = { 1, 2, 3, 4 };
	Deep::PacketReader<std::endian::big> reader{ bytes, sizeof bytes };
	EXPECT_TRUE(reader.HasBytesRemaining(4));
	EXPECT_FALSE(reader.HasBytesRemaining(5));
	EXPECT_FALSE(reader.HasBytesRemaining(std::numeric_limits<size_t>::max()));
	EXPECT_EQ(reader.ReadUInt16(), 0x0102);
	EXPECT_TRUE(reader.HasBytesRemaining(2));
	EXPECT_FALSE(reader.HasBytesRemaining(3));
	EXPECT_EQ(reader.ReadUInt16(), 0x0304);
	EXPECT_TRUE(reader.HasBytesRemaining(0));
	EXPECT_FALSE(reader.HasBytesRemaining(1));
	Deep::PacketReader<std::endian::big> empty{ bytes, 0 };
	EXPECT_TRUE(empty.HasBytesRemaining(0));
	EXPECT_FALSE(empty.HasBytesRemaining(1));
}

TEST(PacketReader, FloatWireFixture) {
	const Deep::uint8 bytes[] = { 0x3f, 0xc0, 0, 0, 0xc0, 0, 0, 0, 0x3e, 0x80, 0, 0, 0x3e, 0, 0xc0, 0, 0x34, 0 };
	Deep::PacketReader<std::endian::big> reader{ bytes, sizeof bytes };
	EXPECT_EQ(reader.ReadVec3(), (Deep::Vec3{ 1.5f, -2.0f, 0.25f }));
	EXPECT_EQ(reader.ReadHalfVec3(), (Deep::Vec3{ 1.5f, -2.0f, 0.25f }));
	EXPECT_EQ(reader.m_head, reader.m_tail);
}
