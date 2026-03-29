#pragma once

#include "Deep/Net/PacketReader.h"
#include "Deep/Bit.h"

DEEP_NAMESPACE_BEGIN

PacketReader::PacketReader(const uint8* in_data) :
	m_head(in_data), m_data(in_data) {}

uint8 PacketReader::ReadUInt8() {
	uint8 value = *m_head;
	m_head += sizeof value;
	return value;
}
uint16 PacketReader::ReadUInt16() {
	uint16 value = *reinterpret_cast<const uint16*>(m_head);
	m_head += sizeof value;
	return ntoh(value);
}
uint32 PacketReader::ReadUInt32() {
	uint32 value = *reinterpret_cast<const uint32*>(m_head);
	m_head += sizeof value;
	return ntoh(value);
}
uint64 PacketReader::ReadUInt64() {
	uint64 value = *reinterpret_cast<const uint64*>(m_head);
	m_head += sizeof value;
	return ntoh(value);
}

int16 PacketReader::ReadInt16() {
	int16 value = *reinterpret_cast<const int16*>(m_head);
	m_head += sizeof value;
	return ntoh(value);
}
int32 PacketReader::ReadInt32() {
	int32 value = *reinterpret_cast<const int32*>(m_head);
	m_head += sizeof value;
	return ntoh(value);
}
int64 PacketReader::ReadInt64() {
	int64 value = *reinterpret_cast<const int64*>(m_head);
	m_head += sizeof value;
	return ntoh(value);
}

float32 PacketReader::ReadFloat16() {
	uint16 value = *reinterpret_cast<const uint16*>(m_head);
	m_head += sizeof value;
	return HalfToFloat(ntoh(value));
}
float32 PacketReader::ReadFloat32() {
	uint32 value = *reinterpret_cast<const uint32*>(m_head);
	m_head += sizeof value;
	return AsFloat(ntoh(value));
}

Vec3 PacketReader::ReadVec3() {
	return Vec3{ ReadFloat32(), ReadFloat32(), ReadFloat32() };
}
Vec3 PacketReader::ReadHalfVec3() {
	return Vec3{ ReadFloat16(), ReadFloat16(), ReadFloat16() };
}

DEEP_NAMESPACE_END
