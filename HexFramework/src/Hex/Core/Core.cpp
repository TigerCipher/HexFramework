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
//  File Name: Core.cpp
//  Date File Created: 08/30/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------
#include "Core.h"

#include "Window.h"

namespace hex::core
{

namespace
{
timer main_timer{};
bool  initialized{};

void calculate_frame_stats()
{
    static i32 frame_count{};
    static f32 time_elapsed{};

    frame_count++;

    if (core::get_timer().total_time() - time_elapsed >= 1.f)
    {
        const f32 fps  = (f32) frame_count;
        const f32 mspf = 1000.f / fps;

        const std::wstring fps_str  = std::to_wstring(fps);
        const std::wstring mspf_str = std::to_wstring(mspf);

        const std::wstring title = window::title() + L"    FPS: " + fps_str + L"    mspf: " + mspf_str;
        SetWindowText(window::handle(), title.c_str());

        frame_count = 0;
        time_elapsed += 1.0f;
    }
}


} // anonymous namespace

bool initialize(std::wstring_view title, i32 width, i32 height)
{
    if (!window::initialize(std::wstring{ title }, width, height))
    {
        return false;
    }

    initialized = true;
    return true;
}

timer& get_timer()
{
    return main_timer;
}

i32 run()
{
    if (!initialized)
        return -1;
    MSG  msg{};
    bool running = true;
    main_timer.reset();
    while (running)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            running &= (msg.message != WM_QUIT);
        } else
        {
            main_timer.tick();
            if (!window::is_paused())
            {
                calculate_frame_stats();
                // Update and render
            } else
            {
                Sleep(100);
            }
        }
    }

    return (i32)msg.wParam;
}

} // namespace hex::core