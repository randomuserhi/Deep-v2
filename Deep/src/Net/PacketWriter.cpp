/**
 * Packet
 */

#include "Deep/Net/PacketWriter.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

void PacketWriter::Write(Quat roQuat) {
    m_buffer.reserve(m_buffer.size() + sizeof(uint8) + sizeof(float32) * 3);

    float32 largest = roQuat.x;
    uint8 i = 0;
    if (roQuat.y > largest) {
        largest = roQuat.y;
        i = 1;
    }
    if (roQuat.z > largest) {
        largest = roQuat.z;
        i = 2;
    }
    if (roQuat.w > largest) {
        largest = roQuat.w;
        i = 3;
    }

    WriteUInt8(i);
    switch (i) {
    case 0:
        if (roQuat.x >= 0) {
            WriteFloat32(roQuat.y);
            WriteFloat32(roQuat.z);
            WriteFloat32(roQuat.w);
        } else {
            WriteFloat32(-roQuat.y);
            WriteFloat32(-roQuat.z);
            WriteFloat32(-roQuat.w);
        }
        break;
    case 1:
        if (roQuat.y >= 0) {
            WriteFloat32(roQuat.x);
            WriteFloat32(roQuat.z);
            WriteFloat32(roQuat.w);
        } else {
            WriteFloat32(-roQuat.x);
            WriteFloat32(-roQuat.z);
            WriteFloat32(-roQuat.w);
        }
        break;
    case 2:
        if (roQuat.z >= 0) {
            WriteFloat32(roQuat.x);
            WriteFloat32(roQuat.y);
            WriteFloat32(roQuat.w);
        } else {
            WriteFloat32(-roQuat.x);
            WriteFloat32(-roQuat.y);
            WriteFloat32(-roQuat.w);
        }
        break;
    case 3:
        if (roQuat.w >= 0) {
            WriteFloat32(roQuat.x);
            WriteFloat32(roQuat.y);
            WriteFloat32(roQuat.z);
        } else {
            WriteFloat32(-roQuat.x);
            WriteFloat32(-roQuat.y);
            WriteFloat32(-roQuat.z);
        }
        break;
    }
}
void PacketWriter::WriteHalfQuat(Quat roQuat) {
    m_buffer.reserve(m_buffer.size() + sizeof(uint8) + sizeof(float16) * 3);

    float32 largest = roQuat.x;
    uint8 i = 0;
    if (roQuat.y > largest) {
        largest = roQuat.y;
        i = 1;
    }
    if (roQuat.z > largest) {
        largest = roQuat.z;
        i = 2;
    }
    if (roQuat.w > largest) {
        largest = roQuat.w;
        i = 3;
    }

    WriteUInt8(i);
    switch (i) {
    case 0:
        if (roQuat.x >= 0) {
            WriteFloat16(roQuat.y);
            WriteFloat16(roQuat.z);
            WriteFloat16(roQuat.w);
        } else {
            WriteFloat16(-roQuat.y);
            WriteFloat16(-roQuat.z);
            WriteFloat16(-roQuat.w);
        }
        break;
    case 1:
        if (roQuat.y >= 0) {
            WriteFloat16(roQuat.x);
            WriteFloat16(roQuat.z);
            WriteFloat16(roQuat.w);
        } else {
            WriteFloat16(-roQuat.x);
            WriteFloat16(-roQuat.z);
            WriteFloat16(-roQuat.w);
        }
        break;
    case 2:
        if (roQuat.z >= 0) {
            WriteFloat16(roQuat.x);
            WriteFloat16(roQuat.y);
            WriteFloat16(roQuat.w);
        } else {
            WriteFloat16(-roQuat.x);
            WriteFloat16(-roQuat.y);
            WriteFloat16(-roQuat.w);
        }
        break;
    case 3:
        if (roQuat.w >= 0) {
            WriteFloat16(roQuat.x);
            WriteFloat16(roQuat.y);
            WriteFloat16(roQuat.z);
        } else {
            WriteFloat16(-roQuat.x);
            WriteFloat16(-roQuat.y);
            WriteFloat16(-roQuat.z);
        }
        break;
    }
}

DEEP_NAMESPACE_END