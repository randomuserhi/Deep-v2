#pragma once

#include "Deep.h"
#include "Deep/Concepts.h"

#include <concepts>
#include <bit>

DEEP_SUPPRESS_WARNINGS_STD_BEGIN
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	#include <immintrin.h>
#endif

DEEP_SUPPRESS_WARNINGS_STD_END

DEEP_NAMESPACE_BEGIN

template<typename T, size_t in_alignment>
constexpr inline T* AssumeAligned(T* in_ptr) noexcept;

template<class To, class From>
constexpr inline To BitCast(const From& in_value);

// Gets address of the container from the address of one of its members provided its member pointer.
//
// Example:
//
// struct A {
//     int m_a;
//     int m_b;
// };
//
// A obj = {};
// int* objA = &obj.m_b;
// A* objFromMember = ContainerOf(objA, &A::m_a);
//
// Deep_Assert(&obj == objFromMember);
//
template<typename Container, typename Member>
constexpr inline Container* ContainerOf(Member* in_memberAddr, Member Container::* in_memberPtr) noexcept;

// Check if value is a power of 2.
template<typename T>
constexpr bool IsPowerOf2(T in_value);

// Check if the given pointer is aligned to the specified alignment
template<typename T>
inline bool IsAligned(T in_pointer, uint64 in_alignment);

// Compute number of trailing zero bits (how many low bits are zero)
template<typename T>
constexpr inline int32 CountTrailingZeros(T in_value);

template<typename T>
constexpr inline int32 CountSetBits(T in_value);

template<std::integral T>
constexpr inline int32 NumBits();

template<std::unsigned_integral T>
constexpr inline T RotateLeft(const T in_value, const int32 in_offset) noexcept;

template<std::unsigned_integral T>
constexpr inline T RotateRight(const T in_value, const int32 in_offset) noexcept;

constexpr inline uint16 ReverseEndianness(const uint16 in_value) noexcept;
constexpr inline uint32 ReverseEndianness(const uint32 in_value) noexcept;
constexpr inline uint64 ReverseEndianness(const uint64 in_value) noexcept;
constexpr inline int16 ReverseEndianness(const int16 in_value) noexcept;
constexpr inline int32 ReverseEndianness(const int32 in_value) noexcept;
constexpr inline int64 ReverseEndianness(const int64 in_value) noexcept;

Deep_ForceInline uint32 AsUInt(const float32 in_value);
Deep_ForceInline float32 AsFloat(const uint32 in_value);

// IEEE-754 16-bit floating-point format (without infinity): 1-5-10,
// exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
inline float32 HalfToFloat(const uint16 in_value);

// IEEE-754 16-bit floating-point format (without infinity): 1-5-10,
// exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
inline float16 FloatToHalf(const float32 in_value);

template<std::endian WireEndian, c_Integer T>
constexpr inline T HostToWire(T in_value) noexcept;

template<std::endian WireEndian, c_Integer T>
constexpr inline T WireToHost(T in_value) noexcept;

DEEP_NAMESPACE_END

#include "./Bit.inl" // IWYU pragma: export
