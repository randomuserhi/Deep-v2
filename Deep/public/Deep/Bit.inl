#pragma once

#include "./Bit.h"

#include <bit>
#include <limits>

DEEP_NAMESPACE_BEGIN

template<typename T, size_t in_alignment>
constexpr inline T* AssumeAligned(T* in_ptr) noexcept {
#if defined(DEEP_COMPILER_CLANG) || defined(DEEP_COMPILER_GCC)
	return static_cast<T*>(__builtin_assume_aligned(in_ptr, in_alignment));
#else
	return in_ptr;
#endif
}

template<class To, class From>
constexpr inline To BitCast(const From& in_value) {
	return std::bit_cast<To>(in_value);
}

template<typename Container, typename Member>
constexpr inline Container* ContainerOf(Member* in_memberAddr, Member Container::* in_memberPtr) noexcept {
	static_assert(std::is_standard_layout<Container>(), "Container type must be of standard layout.");
	const char* base = reinterpret_cast<const char*>(&(reinterpret_cast<Container*>(0)->*in_memberPtr));
	const char* memberAddr = reinterpret_cast<const char*>(in_memberAddr);
	return reinterpret_cast<Container*>(memberAddr - base);
}

template<typename T>
constexpr bool IsPowerOf2(T in_value) {
	return in_value != 0 && (in_value & (in_value - 1)) == 0;
}

template<typename T>
inline bool IsAligned(T in_pointer, uint64 in_alignment) {
	static_assert(std::is_pointer<T>(), "Expected type T to be a pointer.");
	Deep_Assert(IsPowerOf2(in_alignment), "Alignment should be a power of 2.");
	return (BitCast<std::uintptr_t>(in_pointer) & (in_alignment - 1)) == 0;
}

template<typename T>
constexpr inline int32 CountTrailingZeros(T in_value) {
	return std::countr_zero<T>(in_value);
}

template<typename T>
constexpr inline int32 CountSetBits(T in_value) {
	return std::popcount<T>(in_value);
}

template<std::integral T>
constexpr inline int32 NumBits() {
	return std::numeric_limits<T>::digits;
}

template<std::unsigned_integral T>
constexpr T RotateLeft(const T in_value, const int32 in_offset) noexcept {
	return std::rotl(in_value, in_offset);
}

template<std::unsigned_integral T>
constexpr T RotateRight(const T in_value, const int32 in_offset) noexcept {
	return std::rotr(in_value, in_offset);
}

constexpr uint16 ReverseEndianness(const uint16 in_value) noexcept {
	return static_cast<uint16>((in_value >> 8) | (in_value << 8));
}

constexpr uint32 ReverseEndianness(const uint32 in_value) noexcept {
	return RotateRight(in_value & 0x00FF00FFu, 8) | RotateLeft(in_value & 0xFF00FF00u, 8);
}

constexpr uint64 ReverseEndianness(const uint64 in_value) noexcept {
	return (static_cast<uint64>(ReverseEndianness(static_cast<uint32>(in_value))) << 32)
	       | static_cast<uint64>(ReverseEndianness(static_cast<uint32>(in_value >> 32)));
}

constexpr int16 ReverseEndianness(const int16 in_value) noexcept {
	return static_cast<int16>(ReverseEndianness(static_cast<uint16>(in_value)));
}

constexpr int32 ReverseEndianness(const int32 in_value) noexcept {
	return static_cast<int32>(ReverseEndianness(static_cast<uint32>(in_value)));
}

constexpr int64 ReverseEndianness(const int64 in_value) noexcept {
	return static_cast<int64>(ReverseEndianness(static_cast<uint64>(in_value)));
}

uint32 AsUInt(const float32 in_value) {
	return BitCast<const uint32>(in_value);
}
float32 AsFloat(const uint32 in_value) {
	return BitCast<const float32>(in_value);
}

float32 HalfToFloat(const uint16 in_value) {      // IEEE-754 16-bit floating-point format (without infinity): 1-5-10,
	                                              // exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
	const uint32 e = (in_value & 0x7C00u) >> 10u; // exponent
	const uint32 m = (in_value & 0x03FFu) << 13u; // mantissa
	const uint32 v =
		AsUInt(static_cast<float32>(m)) >> 23u; // evil log2 bit hack to count leading zeros in denormalized format
	return AsFloat((in_value & 0x8000u) << 16u | (e != 0u) * ((e + 112u) << 23u | m)
	               | ((e == 0u) & (m != 0u))
	                     * ((v - 37u) << 23u | ((m << (150u - v)) & 0x007FE000u))); // sign : normalized : denormalized
}
float16 FloatToHalf(const float32 in_value) {        // IEEE-754 16-bit floating-point format (without infinity): 1-5-10,
	                                                 // exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
	const uint32 b = AsUInt(in_value) + 0x00001000u; // round-to-nearest-even: add last bit after truncated mantissa
	const uint32 e = (b & 0x7F800000u) >> 23u;       // exponent
	const uint32 m = b & 0x007FFFFFu; // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator
	                                  // flag - initial rounding
	return static_cast<float16>((b & 0x80000000u) >> 16u | (e > 112u) * ((((e - 112u) << 10u) & 0x7C00u) | m >> 13u)
	                            | ((e < 113u) & (e > 101u)) * ((((0x007FF000u + m) >> (125u - e)) + 1u) >> 1u)
	                            | (e > 143u) * 0x7FFFu); // sign : normalized : denormalized : saturate
}

template<std::endian WireEndian, c_Integer T>
constexpr inline T HostToWire(T in_value) noexcept {
	if constexpr (WireEndian == std::endian::native)
		return in_value;
	else
		return ReverseEndianness(in_value);
}

template<std::endian WireEndian, c_Integer T>
constexpr inline T WireToHost(T in_value) noexcept {
	return HostToWire<WireEndian>(in_value);
}

DEEP_NAMESPACE_END
