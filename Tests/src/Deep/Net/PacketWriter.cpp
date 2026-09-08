#include "Tests.h"

#include "Deep/Net/PacketWriter.h"
#include "Deep/Net/PacketReader.h"

TEST(PacketWriter, IntegerWireBytes) {
	auto check = [&]<std::endian in_endian>() {
		Deep::PacketWriter<in_endian> writer{ 1 };
		EXPECT_EQ(writer.m_Size(), 0);
		writer.WriteUInt8(0xab);
		writer.WriteUInt16(0x1234);
		writer.WriteUInt32(0x56789abc);
		writer.WriteUInt64(0x0123456789abcdefull);
		writer.WriteInt16(-2);
		writer.WriteInt32(-3);
		writer.WriteInt64(-4);
		const Deep::uint8 big[] = { 0xab, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
			                        0xff, 0xfe, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc };
		const Deep::uint8 little[] = { 0xab, 0x34, 0x12, 0xbc, 0x9a, 0x78, 0x56, 0xef, 0xcd, 0xab,
			                           0x89, 0x67, 0x45, 0x23, 0x01, 0xfe, 0xff, 0xfd, 0xff, 0xff,
			                           0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
		EXPECT_EQ(writer.m_Size(), sizeof big);
		for (size_t i = 0; i < sizeof big; ++i)
			EXPECT_EQ(writer.m_Data()[i], in_endian == std::endian::big ? big[i] : little[i]);
	};
	check.operator()<std::endian::big>();
	check.operator()<std::endian::little>();
}

TEST(PacketWriter, RawBytesAndClear) {
	auto check = [&]<std::endian in_endian>() {
		Deep::PacketWriter<in_endian> writer;
		const Deep::uint8 bytes[] = { 0, 0xff, 0x12, 0x80 };
		writer.WriteBytes(bytes, sizeof bytes);
		writer.WriteBytes(bytes, 0);
		writer.WriteBytes(bytes, sizeof bytes);
		EXPECT_EQ(writer.m_Size(), 8);
		for (size_t i = 0; i < 8; ++i)
			EXPECT_EQ(writer.m_Data()[i], bytes[i % 4]);
		writer.Clear();
		EXPECT_EQ(writer.m_Size(), 0);
		writer.WriteUInt8(42);
		EXPECT_EQ(writer.m_Size(), 1);
		EXPECT_EQ(writer.m_Data()[0], 42);
	};
	check.operator()<std::endian::big>();
	check.operator()<std::endian::little>();
}

TEST(PacketWriter, FloatAndVectorRoundTrip) {
	auto check = [&]<std::endian in_endian>() {
		Deep::PacketWriter<in_endian> writer;
		const Deep::uint32 bits[] = { 0, 0x80000000, 0x3f800000, 0xc0200000, 0x7f800000, 0xff800000, 0x7fc12345 };
		for (Deep::uint32 value : bits)
			writer.WriteFloat32(Deep::AsFloat(value));
		Deep::Vec3 vector{ 1.5f, -2.0f, 0.25f };
		writer.WriteVec3(vector);
		writer.WriteHalfVec3(vector);
		writer.WriteFloat16(-4.0f);
		EXPECT_EQ(writer.m_Size(), sizeof bits + 12 + 6 + 2);
		Deep::PacketReader<in_endian> reader{ writer.m_Data(), writer.m_Size() };
		for (Deep::uint32 value : bits)
			EXPECT_EQ(Deep::AsUInt(reader.ReadFloat32()), value);
		EXPECT_EQ(reader.ReadVec3(), vector);
		EXPECT_EQ(reader.ReadHalfVec3(), vector);
		EXPECT_EQ(reader.ReadFloat16(), -4.0f);
		EXPECT_FALSE(reader.HasBytesRemaining(1));
	};
	check.operator()<std::endian::big>();
	check.operator()<std::endian::little>();
}

TEST(PacketWriter, CompressedQuaternionBranches) {
	auto check = [&]<std::endian in_endian>() {
		for (size_t largest = 0; largest < 4; ++largest) {
			for (float sign : { -1.0f, 1.0f }) {
				Deep::Quat input{ 0.1f, -0.2f, 0.3f, -0.1f };
				input[largest] = sign;
				input.Normalize();
				Deep::PacketWriter<in_endian> writer;
				writer.Write(input);
				writer.WriteHalfQuat(input);
				writer.WriteUInt8(0xab);
				EXPECT_EQ(writer.m_Size(), 21);
				EXPECT_EQ(writer.m_Data()[0], largest);
				EXPECT_EQ(writer.m_Data()[13], largest);
				Deep::PacketReader<in_endian> reader{ writer.m_Data(), writer.m_Size() };
				auto full = reader.ReadQuaternion();
				auto half = reader.ReadHalfQuaternion();
				for (size_t i = 0; i < 4; ++i) {
					EXPECT_TRUE(Deep::Abs(full[i] - input[i] * sign) < 0.00001f);
					EXPECT_TRUE(Deep::Abs(half[i] - input[i] * sign) < 0.001f);
				}
				EXPECT_EQ(reader.ReadUInt8(), 0xab);
				EXPECT_FALSE(reader.HasBytesRemaining(1));
			}
		}
	};
	check.operator()<std::endian::big>();
	check.operator()<std::endian::little>();
}
