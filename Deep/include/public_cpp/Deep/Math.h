/**
 * Math.h
 *
 * Math uses Left-Hand coordinate system so quaternions rotate counter-clockwise.
 * All types use float32, double precision (float64) is not supported.
 */

// NOTE(randomuserhi): Math uses Left-Hand coordinate system so quaternions rotate counter-clockwise ->
//                     https://gamedev.stackexchange.com/questions/87612/quaternion-rotation-clockwise-or-counter-clockwise

#pragma once

#include "Deep/Math/Ops.h"       // IWYU pragma: export
#include "Deep/Math/Constants.h" // IWYU pragma: export
#include "Deep/Simd.h"           // IWYU pragma: export
#include "Deep/Math/Vec2.h"      // IWYU pragma: export
#include "Deep/Math/Vec2i.h"     // IWYU pragma: export
#include "Deep/Math/Vec3.h"      // IWYU pragma: export
#include "Deep/Math/Vec3i.h"     // IWYU pragma: export
#include "Deep/Math/Vec4.h"      // IWYU pragma: export
#include "Deep/Math/Vec4i.h"     // IWYU pragma: export
#include "Deep/Math/Quat.h"      // IWYU pragma: export
#include "Deep/Math/Mat4x4.h"    // IWYU pragma: export
#include "Deep/Math/Float2.h"    // IWYU pragma: export
#include "Deep/Math/Float3.h"    // IWYU pragma: export
#include "Deep/Math/Float4.h"    // IWYU pragma: export
#include "Deep/Math/Int2.h"      // IWYU pragma: export
#include "Deep/Math/Int3.h"      // IWYU pragma: export
#include "Deep/Math/Int4.h"      // IWYU pragma: export
