/**
 * Packet
 */

#pragma once

#include <Deep.h>
#include <Deep/Math.h>
#include <Deep/Bit.h>

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#include <vector>
DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

struct DEEP_EXPORT PacketWriter {
    //

    Deep_Inline PacketWriter();
    Deep_Inline explicit PacketWriter(size_t in_size);

    //

    // TODO(randomuserhi): https://stackoverflow.com/a/51615364/9642458
    Deep_Inline const uint8* data() const;
    Deep_Inline size_t size() const;

    Deep_Inline void WriteUInt8(uint8 in_byte);
    Deep_Inline void WriteBytes(const uint8* in_bytes, size_t in_numBytes);
    Deep_Inline void WriteUInt16(uint16 in_value);
    Deep_Inline void WriteInt16(int16 in_value);
    Deep_Inline void WriteUInt32(uint32 in_value);
    Deep_Inline void WriteInt32(int32 in_value);
    Deep_Inline void WriteUInt64(uint64 in_value);
    Deep_Inline void WriteInt64(int64 in_value);
    Deep_Inline void WriteFloat32(float32 in_value);
    Deep_Inline void WriteVec3(Vec3 in_value);
    void Write(Quat in_value);

    Deep_Inline void WriteFloat16(float32 in_value);
    Deep_Inline void WriteHalfVec3(Vec3 in_value);
    void WriteHalfQuat(Quat in_value);

    Deep_Inline void Clear();

    //

private:
    //

    std::vector<uint8> m_buffer;

    //
};

DEEP_NAMESPACE_END

#include "PacketWriter.inl"