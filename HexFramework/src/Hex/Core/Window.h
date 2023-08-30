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
// File Name: Window
// Date File Created: 08/29/2023
// Author: Matt
//
// ------------------------------------------------------------------------------
#pragma once

#include "Hex/Common.h"

#include "Hex/Helpers/Timer.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <string>

namespace hex::window
{

bool initialize(const std::wstring& title, i32 width, i32 height);
void shutdown();

HINSTANCE instance();
HWND      handle();
bool      is_fullscreen();
bool      is_resizing();
bool      is_minimized();
bool      is_maximized();
bool      is_paused();

i32 width();
i32 height();
f32 aspect_ratio();

timer& get_timer();

void calculate_frame_stats();

} // namespace hex::window
