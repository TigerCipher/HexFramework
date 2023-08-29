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
//  File Name: RandomHelper.h
//  Date File Created: 08/29/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------

#pragma once

#include "Hex/Types.h"

namespace hex::random
{

void initialize();
void seed(u32 seed);

f32 get_float(f32 min = 0.f, f32 max = 1.f);
i32 get_int(i32 min, i32 max);

}