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
//  File Name: RandomHelper.cpp
//  Date File Created: 08/29/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------
#include "RandomHelper.h"

#include <cassert>
#include <random>

namespace hex::random
{
namespace
{
std::mt19937 generator{};  // NOLINT(cert-msc51-cpp)
bool         initialized{ false };
} // anonymous namespace

void initialize()
{
    std::random_device rd{};
    seed(rd());
    initialized = true;
}

void seed(u32 seed)
{
    generator.seed(seed);
}

f32  get_float(f32 min, f32 max)
{
    assert(initialized);
    std::uniform_real_distribution<f32> dist{min, max};
    return dist(generator);
}

i32  get_int(i32 min, i32 max)
{
    assert(initialized);
    std::uniform_int_distribution<i32> dist{min, max};
    return dist(generator);
}

} // namespace hex::random