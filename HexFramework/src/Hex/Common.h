//  ------------------------------------------------------------------------------
//
//  HexFramework
//     Copyright 2023 Matthew Rogers
//
//     Licensed under the Apache License, Version 2.0 (the "License");
//     you may not use this file except in compliance with the License.
//     You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
//     Unless required by applicable law or agreed to in writing, software
//     distributed under the License is distributed on an "AS IS" BASIS,
//     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//     See the License for the specific language governing permissions and
//     limitations under the License.
//
//  File Name: Common.h
//  Date File Created: 08/29/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------

#pragma once

#include <cassert>

#include <unordered_map>
#include <vector>

#include "Types.h"

#ifdef _DEBUG
    #define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
    #define DBG_NEW new
#endif

#define SAFE_DELETE(x)                                                                                                           \
    delete (x);                                                                                                                  \
    (x) = nullptr

constexpr auto operator""_KB(const u64 x)
{
    return x * 1024u;
}

constexpr auto operator""_MB(const u64 x)
{
    return x * 1024u * 1024u;
}

constexpr auto operator""_GB(const u64 x)
{
    return x * 1024u * 1024u * 1024u;
}