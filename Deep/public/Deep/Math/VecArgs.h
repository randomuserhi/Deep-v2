#pragma once

#include "Deep.h"

DEEP_NAMESPACE_BEGIN

struct Vec2i;
using Arg_Vec2i = const Vec2i;

struct Vec2;
using Arg_Vec2 = const Vec2;

struct Vec3i;
using Arg_Vec3i = const Vec3i;

struct Vec3;
using Arg_Vec3 = const Vec3;

struct Vec4i;
using Arg_Vec4i = const Vec4i;

struct Vec4;
using Arg_Vec4 = const Vec4;

struct Quat;
using Arg_Quat = const Quat;

struct Mat4;
using Arg_Mat4 = const Mat4&;

DEEP_NAMESPACE_END