#pragma once

#include "./PacketWriter.h"

#include "Deep/Bit.h"
#include "Deep/Memory.h"

DEEP_NAMESPACE_BEGIN

template<std::endian in_Endian>
PacketWriter<in_Endian>::PacketWriter() {}

template<std::endian in_Endian>
PacketWriter<in_Endian>::PacketWriter(size_t in_size) {
	m_buffer.reserve(in_size);
}

template<std::endian in_Endian>
const uint8* PacketWriter<in_Endian>::m_Data() const {
	return m_buffer.data();
}

template<std::endian in_Endian>
size_t PacketWriter<in_Endian>::m_Size() const {
	return m_buffer.size();
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteUInt8(uint8 in_byte) {
	m_buffer.push_back(in_byte);
}

// NOTE(randomuserhi): This writes raw bytes to the buffer and does not account
//                     for endianess. For ASCII strings or formats where data only
//                     spans 1 byte, this works fine:
//                     https://stackoverflow.com/questions/63974853/explaining-lack-of-endianness-as-it-applies-to-a-string
template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteBytes(const uint8* in_bytes, size_t in_numBytes) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + in_numBytes);
	Memcpy(m_buffer.data() + old, in_bytes, in_numBytes);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteUInt16(uint16 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = HostToWire<in_Endian>(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteInt16(int16 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = HostToWire<in_Endian>(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteUInt32(uint32 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = HostToWire<in_Endian>(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteInt32(int32 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = HostToWire<in_Endian>(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteUInt64(uint64 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = HostToWire<in_Endian>(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteInt64(int64 in_value) {
	size_t old = m_buffer.size();
	m_buffer.resize(m_buffer.size() + sizeof in_value);
	in_value = HostToWire<in_Endian>(in_value);
	Memcpy(m_buffer.data() + old, &in_value, sizeof in_value);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteFloat32(float32 in_value) {
	WriteUInt32(BitCast<uint32>(in_value));
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteFloat16(float32 in_value) {
	float16 half = FloatToHalf(in_value);
	WriteUInt16(half);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteVec3(Vec3 in_value) {
	WriteFloat32(in_value.x);
	WriteFloat32(in_value.y);
	WriteFloat32(in_value.z);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::WriteHalfVec3(Vec3 in_value) {
	WriteFloat16(in_value.x);
	WriteFloat16(in_value.y);
	WriteFloat16(in_value.z);
}

template<std::endian in_Endian>
void PacketWriter<in_Endian>::Clear() {
	m_buffer.clear();
}

DEEP_NAMESPACE_END
