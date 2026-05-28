/**
 * Packet
 */

#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

// NOTE(randomuserhi): There are no bound checks on the data buffer being read
// TODO(randomuserhi): Add bound checks - only in use with asserts
struct PacketReader {
	//

	inline PacketReader(const uint8* in_data);

	//

	inline uint8 ReadUInt8();

	inline uint16 ReadUInt16();
	inline uint32 ReadUInt32();
	inline uint64 ReadUInt64();

	inline int16 ReadInt16();
	inline int32 ReadInt32();
	inline int64 ReadInt64();

	inline float32 ReadFloat16();
	inline float32 ReadFloat32();

	inline Vec3 ReadVec3();
	DEEP_EXPORT Quat ReadQuaternion();

	inline Vec3 ReadHalfVec3();
	DEEP_EXPORT Quat ReadHalfQuaternion();

	//

	const uint8* m_head;

	//

private:
	DEEP_PRIVATE_TESTABLE

	//

	const uint8* const m_data;

	//
};

DEEP_NAMESPACE_END

#include "PacketReader.inl" // IWYU pragma: export
