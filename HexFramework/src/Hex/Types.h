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
// File Name: Types
// Date File Created: 08/29/2023
// Author: Matt
//
// ------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <memory>

using ulong = unsigned long;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;


constexpr u8  u8_invalid_id  = 0xffui8;
constexpr u16 u16_invalid_id = 0xffffui16;
constexpr u32 u32_invalid_id = 0xffff'ffffui32;
constexpr u64 u64_invalid_id = 0xffff'ffff'ffff'ffffui64;

using f32 = float;
using f64 = double;

template<typename T, typename... Args>
concept constructible_from_args = std::constructible_from<T, Args...>;

template<typename T>
concept primitive_type = std::is_arithmetic_v<T>;

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T>
using weak = std::weak_ptr<T>;

template<typename T, typename... Args>
    requires constructible_from_args<T, Args...>
constexpr scope<T> create_scope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
    requires constructible_from_args<T, Args...>
constexpr ref<T> create_ref(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}