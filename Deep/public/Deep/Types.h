/**
 * Deep Types
 */

#pragma once

#include <cstdint>

namespace Deep {

enum class byte : unsigned char {};

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using float16 = uint16_t;

using float32 = float;
using float64 = double;

} // namespace Deep

#ifndef DEEP_NO_TYPE_ALIASES

using byte = Deep::byte;

using int8 = Deep::int8;
using int16 = Deep::int16;
using int32 = Deep::int32;
using int64 = Deep::int64;

using uint8 = Deep::uint8;
using uint16 = Deep::uint16;
using uint32 = Deep::uint32;
using uint64 = Deep::uint64;

using float16 = Deep::float16;

using float32 = Deep::float32;
using float64 = Deep::float64;

#endif
