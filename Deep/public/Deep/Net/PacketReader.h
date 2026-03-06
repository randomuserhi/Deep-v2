/**
 * Packet
 */

#pragma once

#include "Deep.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

// NOTE(randomuserhi): There are no bound checks on the data buffer being read
// TODO(randomuserhi): Add bound checks - only in use with asserts
struct DEEP_EXPORT PacketReader {
    //

    Deep_Inline PacketReader(const uint8* in_data);

    //

    Deep_Inline uint8 ReadUInt8();

    Deep_Inline uint16 ReadUInt16();
    Deep_Inline uint32 ReadUInt32();
    Deep_Inline uint64 ReadUInt64();

    Deep_Inline int16 ReadInt16();
    Deep_Inline int32 ReadInt32();
    Deep_Inline int64 ReadInt64();

    Deep_Inline float32 ReadFloat16();
    Deep_Inline float32 ReadFloat32();

    Deep_Inline Vec3 ReadVec3();
    Quat ReadQuaternion();

    Deep_Inline Vec3 ReadHalfVec3();
    Quat ReadHalfQuaternion();

    //

    const uint8* m_head;

    //

private:
    //

    const uint8* const m_data;

    //
};

DEEP_NAMESPACE_END

#include "PacketReader.inl" // IWYU pragma: export
