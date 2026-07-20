#pragma once

#include "./PacketReader.h"

#include "Deep/Memory.h"
#include "Deep/Bit.h"

DEEP_NAMESPACE_BEGIN

// TODO(randomuserhi): Address nullptr `in_data` and 0 `in_size`.
//                     `HasBytesRemaining` has undefined behaviour in that case `nullptr + 0`.

template<std::endian in_Endian>
PacketReader<in_Endian>::PacketReader(const uint8* in_data, size_t in_size) :
	m_head{ in_data }, m_tail{ in_data + in_size }, m_data{ in_data }, m_size{ in_size } {}

template<std::endian in_Endian>
bool PacketReader<in_Endian>::HasBytesRemaining(size_t in_count) const {
	return in_count <= static_cast<size_t>(m_tail - m_head);
}

template<std::endian in_Endian>
uint8 PacketReader<in_Endian>::ReadUInt8() {
	uint8 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	value = *m_head;
	m_head += sizeof value;
	return value;
}

template<std::endian in_Endian>
uint16 PacketReader<in_Endian>::ReadUInt16() {
	uint16 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return WireToHost<in_Endian>(value);
}

template<std::endian in_Endian>
uint32 PacketReader<in_Endian>::ReadUInt32() {
	uint32 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return WireToHost<in_Endian>(value);
}

template<std::endian in_Endian>
uint64 PacketReader<in_Endian>::ReadUInt64() {
	uint64 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return WireToHost<in_Endian>(value);
}

template<std::endian in_Endian>
int16 PacketReader<in_Endian>::ReadInt16() {
	int16 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return WireToHost<in_Endian>(value);
}

template<std::endian in_Endian>
int32 PacketReader<in_Endian>::ReadInt32() {
	int32 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return WireToHost<in_Endian>(value);
}

template<std::endian in_Endian>
int64 PacketReader<in_Endian>::ReadInt64() {
	int64 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return WireToHost<in_Endian>(value);
}

template<std::endian in_Endian>
float32 PacketReader<in_Endian>::ReadFloat16() {
	uint16 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return HalfToFloat(WireToHost<in_Endian>(value));
}

template<std::endian in_Endian>
float32 PacketReader<in_Endian>::ReadFloat32() {
	uint32 value;
	Deep_Assert(HasBytesRemaining(sizeof value), "Reached end of byte buffer.");
	Memcpy(&value, m_head, sizeof value);
	m_head += sizeof value;
	return AsFloat(WireToHost<in_Endian>(value));
}

template<std::endian in_Endian>
Vec3 PacketReader<in_Endian>::ReadVec3() {
	Deep_Assert(HasBytesRemaining(sizeof(float32) * 3), "Reached end of byte buffer.");
	return Vec3{ ReadFloat32(), ReadFloat32(), ReadFloat32() };
}

template<std::endian in_Endian>
Vec3 PacketReader<in_Endian>::ReadHalfVec3() {
	Deep_Assert(HasBytesRemaining(sizeof(float16) * 3), "Reached end of byte buffer.");
	return Vec3{ ReadFloat16(), ReadFloat16(), ReadFloat16() };
}

DEEP_NAMESPACE_END
