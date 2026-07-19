/**
 * Packet
 */

#include "Deep/Net/PacketWriter.h"
#include "Deep/Math.h"

DEEP_NAMESPACE_BEGIN

template<std::endian in_Endian>
void PacketWriter<in_Endian>::Write(Quat in_Quat) {
	m_buffer.reserve(m_buffer.size() + sizeof(uint8) + sizeof(float32) * 3);

	float32 largest = Abs(in_Quat.x);
	uint8 i = 0;
	if (float32 temp = Abs(in_Quat.y); temp > largest) {
		largest = temp;
		i = 1;
	}
	if (float32 temp = Abs(in_Quat.z); temp > largest) {
		largest = temp;
		i = 2;
	}
	if (float32 temp = Abs(in_Quat.w); temp > largest) {
		largest = temp;
		i = 3;
	}

	WriteUInt8(i);
	switch (i) {
	case 0:
		if (in_Quat.x >= 0) {
			WriteFloat32(in_Quat.y);
			WriteFloat32(in_Quat.z);
			WriteFloat32(in_Quat.w);
		} else {
			WriteFloat32(-in_Quat.y);
			WriteFloat32(-in_Quat.z);
			WriteFloat32(-in_Quat.w);
		}
		break;
	case 1:
		if (in_Quat.y >= 0) {
			WriteFloat32(in_Quat.x);
			WriteFloat32(in_Quat.z);
			WriteFloat32(in_Quat.w);
		} else {
			WriteFloat32(-in_Quat.x);
			WriteFloat32(-in_Quat.z);
			WriteFloat32(-in_Quat.w);
		}
		break;
	case 2:
		if (in_Quat.z >= 0) {
			WriteFloat32(in_Quat.x);
			WriteFloat32(in_Quat.y);
			WriteFloat32(in_Quat.w);
		} else {
			WriteFloat32(-in_Quat.x);
			WriteFloat32(-in_Quat.y);
			WriteFloat32(-in_Quat.w);
		}
		break;
	case 3:
		if (in_Quat.w >= 0) {
			WriteFloat32(in_Quat.x);
			WriteFloat32(in_Quat.y);
			WriteFloat32(in_Quat.z);
		} else {
			WriteFloat32(-in_Quat.x);
			WriteFloat32(-in_Quat.y);
			WriteFloat32(-in_Quat.z);
		}
		break;
	}
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteHalfQuat(Quat in_Quat) {
	m_buffer.reserve(m_buffer.size() + sizeof(uint8) + sizeof(float16) * 3);

	float32 largest = Abs(in_Quat.x);
	uint8 i = 0;
	if (float32 temp = Abs(in_Quat.y); temp > largest) {
		largest = temp;
		i = 1;
	}
	if (float32 temp = Abs(in_Quat.z); temp > largest) {
		largest = temp;
		i = 2;
	}
	if (float32 temp = Abs(in_Quat.w); temp > largest) {
		largest = temp;
		i = 3;
	}

	WriteUInt8(i);
	switch (i) {
	case 0:
		if (in_Quat.x >= 0) {
			WriteFloat16(in_Quat.y);
			WriteFloat16(in_Quat.z);
			WriteFloat16(in_Quat.w);
		} else {
			WriteFloat16(-in_Quat.y);
			WriteFloat16(-in_Quat.z);
			WriteFloat16(-in_Quat.w);
		}
		break;
	case 1:
		if (in_Quat.y >= 0) {
			WriteFloat16(in_Quat.x);
			WriteFloat16(in_Quat.z);
			WriteFloat16(in_Quat.w);
		} else {
			WriteFloat16(-in_Quat.x);
			WriteFloat16(-in_Quat.z);
			WriteFloat16(-in_Quat.w);
		}
		break;
	case 2:
		if (in_Quat.z >= 0) {
			WriteFloat16(in_Quat.x);
			WriteFloat16(in_Quat.y);
			WriteFloat16(in_Quat.w);
		} else {
			WriteFloat16(-in_Quat.x);
			WriteFloat16(-in_Quat.y);
			WriteFloat16(-in_Quat.w);
		}
		break;
	case 3:
		if (in_Quat.w >= 0) {
			WriteFloat16(in_Quat.x);
			WriteFloat16(in_Quat.y);
			WriteFloat16(in_Quat.z);
		} else {
			WriteFloat16(-in_Quat.x);
			WriteFloat16(-in_Quat.y);
			WriteFloat16(-in_Quat.z);
		}
		break;
	}
}

template struct DEEP_EXPORT PacketWriter<std::endian::big>;
template struct DEEP_EXPORT PacketWriter<std::endian::little>;

DEEP_NAMESPACE_END
