/**
 * Packet
 */

#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <vector>
DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

template<std::endian in_Endian>
struct PacketWriter {
	static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big,
	              "Mixed-endian architectures are not supported.");

public:
	inline PacketWriter();
	inline explicit PacketWriter(size_t in_size);

	//

	Deep_ForceInline const uint8* m_Data() const;
	Deep_ForceInline size_t m_Size() const;

	// The `in_bytes` buffer must not alias or overlap with the internal writers buffer.
	//
	// NOTE(randomuserhi): This writes raw bytes to the buffer and does not account
	//                     for endianess. For ASCII strings or formats where data only
	//                     spans 1 byte, this works fine:
	//                     https://stackoverflow.com/questions/63974853/explaining-lack-of-endianness-as-it-applies-to-a-string
	inline void WriteBytes(const uint8* in_bytes, size_t in_numBytes);

	inline void WriteUInt8(uint8 in_byte);
	inline void WriteUInt16(uint16 in_value);
	inline void WriteInt16(int16 in_value);
	inline void WriteUInt32(uint32 in_value);
	inline void WriteInt32(int32 in_value);
	inline void WriteUInt64(uint64 in_value);
	inline void WriteInt64(int64 in_value);
	inline void WriteFloat32(float32 in_value);
	inline void WriteVec3(Vec3 in_value);
	DEEP_EXPORT void Write(Quat in_value);

	inline void WriteFloat16(float32 in_value);
	inline void WriteHalfVec3(Vec3 in_value);
	DEEP_EXPORT void WriteHalfQuat(Quat in_value);

	inline void Clear();

	//

private:
	DEEP_PRIVATE_TESTABLE

	//

	std::vector<uint8> m_buffer;

	//
};

extern template struct PacketWriter<std::endian::big>;
extern template struct PacketWriter<std::endian::little>;

DEEP_NAMESPACE_END

#include "./PacketWriter.inl" // IWYU pragma: export
