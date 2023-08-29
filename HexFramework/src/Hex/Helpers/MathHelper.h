// ------------------------------------------------------------------------------
//
// HexFramework
//    Copyright 2023 Matthew Rogers
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
// File Name: MathHelper
// Date File Created: 08/29/2023
// Author: Matt
//
// ------------------------------------------------------------------------------
#pragma once

#include "Hex/Types.h"

#include <cmath>
#include <limits>
#include <DirectXMath.h>

using vec  = DirectX::XMVECTOR;
using vec2 = DirectX::XMFLOAT2;
using vec3 = DirectX::XMFLOAT3;
using vec4 = DirectX::XMFLOAT4;

using mat  = DirectX::XMMATRIX;
using mat3 = DirectX::XMFLOAT3X3;
using mat4 = DirectX::XMFLOAT4X4;

namespace hex::math
{
constexpr f32 pi                 = 3.1415926535f;
constexpr f32 two_pi             = pi * 2.0f;
constexpr f32 half_pi            = pi / 2.0f;
constexpr f32 infinity           = std::numeric_limits<f32>::infinity();
constexpr f32 neg_infinity       = -infinity;
constexpr f32 epsilon            = 0.001f;
constexpr f32 deg2rad_multiplier = pi / 180.0f;
constexpr f32 rad2deg_multiplier = 180.0f / pi;
constexpr f32 nan                = std::numeric_limits<f32>::quiet_NaN();

template<primitive_type T>
constexpr T min(T a, T b)
{
    return a < b ? a : b;
}

template<primitive_type T>
constexpr T max(T a, T b)
{
    return a < b ? b : a;
}

template<primitive_type T>
constexpr T clamp(T value, T lower, T upper)
{
    return min(upper, max(lower, value));
}

// standard abs is not constexpr until C++23 -- this engine is C++20
template<primitive_type T>
constexpr T abs(T x)
{
    return x < 0 ? -x : x;
}

constexpr bool near_equal(f32 x, f32 y, f32 tolerance = epsilon)
{
    return abs(x - y) <= tolerance;
}

constexpr bool near_zero(f32 val, f32 tolerance = epsilon)
{
    return abs(val) <= tolerance;
}

constexpr f32 sqrt_newton(f32 x, f32 current, f32 previous)
{
    return near_equal(current, previous) ? current : sqrt_newton(x, 0.5f * (current + x / current), current);
}

constexpr f32 sqrt(f32 x)
{
    return x >= 0 && x < infinity ? sqrt_newton(x, x, 0.f) : nan;
}

constexpr f32 ToRadians(f32 degrees)
{
    return degrees * deg2rad_multiplier;
}

constexpr f32 ToDegrees(f32 radians)
{
    return radians * rad2deg_multiplier;
}

constexpr bool is_power_of_two(u64 value)
{
    return value != 0 && (value & (value - 1)) == 0;
}

constexpr f32 lerp(f32 a, f32 b, f32 f)
{
    return a + f * (b - a);
}

// Returns polar angle of the point (x, y) in [0, two_pi)
f32 angle_from_xy(f32 x, f32 y);

vec random_unit_vec3();
vec random_hemisphere_unit_vec3(vec n);

} // namespace hex::math
