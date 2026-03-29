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

struct DEEP_EXPORT PacketWriter {
	//

	inline PacketWriter();
	inline explicit PacketWriter(size_t in_size);

	//

	// TODO(randomuserhi): https://stackoverflow.com/a/51615364/9642458
	inline const uint8* data() const;
	inline size_t size() const;

	inline void WriteUInt8(uint8 in_byte);
	inline void WriteBytes(const uint8* in_bytes, size_t in_numBytes);
	inline void WriteUInt16(uint16 in_value);
	inline void WriteInt16(int16 in_value);
	inline void WriteUInt32(uint32 in_value);
	inline void WriteInt32(int32 in_value);
	inline void WriteUInt64(uint64 in_value);
	inline void WriteInt64(int64 in_value);
	inline void WriteFloat32(float32 in_value);
	inline void WriteVec3(Vec3 in_value);
	void Write(Quat in_value);

	inline void WriteFloat16(float32 in_value);
	inline void WriteHalfVec3(Vec3 in_value);
	void WriteHalfQuat(Quat in_value);

	inline void Clear();

	//

private:
	//

	std::vector<uint8> m_buffer;

	//
};

DEEP_NAMESPACE_END

#include "PacketWriter.inl" // IWYU pragma: export
