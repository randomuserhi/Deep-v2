/**
 * Packet
 */

#include <Deep/Net/PacketReader.h>
#include <Deep/Math.h>

DEEP_NAMESPACE_BEGIN

Quat PacketReader::ReadQuaternion() {
    uint8 i = ReadUInt8();
    Quat q{};
    switch (i) {
    case 0:
        q.y = ReadFloat32();
        q.z = ReadFloat32();
        q.w = ReadFloat32();
        q.x = Sqrt(Clamp01(1.0f - q.y * q.y - q.z * q.z - q.w * q.w));
        break;
    case 1:
        q.x = ReadFloat32();
        q.z = ReadFloat32();
        q.w = ReadFloat32();
        q.y = Sqrt(Clamp01(1.0f - q.x * q.x - q.z * q.z - q.w * q.w));
        break;
    case 2:
        q.x = ReadFloat32();
        q.y = ReadFloat32();
        q.w = ReadFloat32();
        q.z = Sqrt(Clamp01(1.0f - q.x * q.x - q.y * q.y - q.w * q.w));
        break;
    case 3:
        q.x = ReadFloat32();
        q.y = ReadFloat32();
        q.z = ReadFloat32();
        q.w = Sqrt(Clamp01(1.0f - q.x * q.x - q.y * q.y - q.z * q.z));
        break;
    }
    return q;
}

Quat PacketReader::ReadHalfQuaternion() {
    uint8 i = ReadUInt8();
    Quat q{};
    switch (i) {
    case 0:
        q.y = ReadFloat16();
        q.z = ReadFloat16();
        q.w = ReadFloat16();
        q.x = Sqrt(Clamp01(1.0f - q.y * q.y - q.z * q.z - q.w * q.w));
        break;
    case 1:
        q.x = ReadFloat16();
        q.z = ReadFloat16();
        q.w = ReadFloat16();
        q.y = Sqrt(Clamp01(1.0f - q.x * q.x - q.z * q.z - q.w * q.w));
        break;
    case 2:
        q.x = ReadFloat16();
        q.y = ReadFloat16();
        q.w = ReadFloat16();
        q.z = Sqrt(Clamp01(1.0f - q.x * q.x - q.y * q.y - q.w * q.w));
        break;
    case 3:
        q.x = ReadFloat16();
        q.y = ReadFloat16();
        q.z = ReadFloat16();
        q.w = Sqrt(Clamp01(1.0f - q.x * q.x - q.y * q.y - q.z * q.z));
        break;
    }
    return q;
}

DEEP_NAMESPACE_END