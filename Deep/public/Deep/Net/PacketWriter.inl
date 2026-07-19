#pragma once

#include "./PacketWriter.h"

#include "Deep/Bit.h"
#include "Deep/Memory.h"

DEEP_NAMESPACE_BEGIN

PacketWriter::PacketWriter() {}
PacketWriter::PacketWriter(size_t in_size) {
	m_buffer.reserve(in_size);
}

const uint8* PacketWriter::m_Data() const {
	return m_buffer.data();
}
size_t PacketWriter::m_Size() const {
	return m_buffer.size();
}

void PacketWriter::WriteUInt8(uint8 in_byte) {
	m_buffer.push_back(in_byte);
}

// NOTE(randomuserhi): This writes raw bytes to the buffer and does not account
//                     for endianess. For ASCII strings or formats where data only
//                     spans 1 byte, this works fine:
//                     https://stackoverflow.com/questions/63974853/explaining-lack-of-endianness-as-it-applies-to-a-string
void PacketWriter::WriteBytes(const uint8* in_bytes, size_t in_numBytes) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + in_numBytes);
	Memcpy(m_buffer.data() + old, in_bytes, in_numBytes);
}

void PacketWriter::WriteUInt16(uint16 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = hton(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}
void PacketWriter::WriteInt16(int16 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = hton(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

void PacketWriter::WriteUInt32(uint32 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = hton(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}
void PacketWriter::WriteInt32(int32 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = hton(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

void PacketWriter::WriteUInt64(uint64 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = hton(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}
void PacketWriter::WriteInt64(int64 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = hton(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

void PacketWriter::WriteFloat32(float32 in_value) {
	WriteUInt32(BitCast<uint32>(in_value));
}
void PacketWriter::WriteFloat16(float32 in_value) {
	float16 half = FloatToHalf(in_value);
	WriteUInt16(half);
}

void PacketWriter::WriteVec3(Vec3 in_value) {
	WriteFloat32(in_value.x);
	WriteFloat32(in_value.y);
	WriteFloat32(in_value.z);
}
void PacketWriter::WriteHalfVec3(Vec3 in_value) {
	WriteFloat16(in_value.x);
	WriteFloat16(in_value.y);
	WriteFloat16(in_value.z);
}

void PacketWriter::Clear() {
	m_buffer.clear();
}

DEEP_NAMESPACE_END
