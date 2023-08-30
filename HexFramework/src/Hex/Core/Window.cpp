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
//  File Name: Window.cpp
//  Date File Created: 08/29/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------
#include "Window.h"

#include "Core.h"

namespace hex::window
{

namespace
{
bool         minimized{};
bool         maximized{};
bool         resizing{};
bool         fullscreen{};
bool         paused{};
i32          client_width{};
i32          client_height{};
HINSTANCE    hinst{};
HWND         window_handle{};
std::wstring window_title{};


LRESULT CALLBACK message_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
        if (LOWORD(wparam) == WA_INACTIVE)
        {
            paused = true;
            core::get_timer().stop();
        } else
        {
            paused = false;
            core::get_timer().start();
        }
        return 0;
    case WM_SIZE:
        client_width  = LOWORD(lparam);
        client_height = HIWORD(lparam);
        if (true /*TODO should only do this if the d3d device is valid*/)
        {
            if (wparam == SIZE_MINIMIZED)
            {
                paused    = true;
                minimized = true;
                maximized = false;
            } else if (wparam == SIZE_MAXIMIZED)
            {
                paused    = false;
                minimized = false;
                maximized = true;
                // on_resize();
            } else if (wparam == SIZE_RESTORED)
            {
                if (minimized)
                {
                    paused    = false;
                    minimized = false;
                    // on_resize();
                } else if (maximized)
                {
                    paused    = false;
                    maximized = false;
                    //on_resize();
                } else if (resizing)
                {
                    // do nothing
                } else
                {
                    //on_resize();
                }
            }
        }
        return 0;
    case WM_ENTERSIZEMOVE:
        paused   = true;
        resizing = true;
        core::get_timer().stop();
        return 0;
    case WM_EXITSIZEMOVE:
        paused   = false;
        resizing = false;
        core::get_timer().start();
        // on_resize();
        return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    case WM_MENUCHAR: return MAKELRESULT(0, MNC_CLOSE); // makes it so alt-enter doesn't beep
    case WM_GETMINMAXINFO:
        // makes it so we can't make the window too small
        ((MINMAXINFO*) lparam)->ptMinTrackSize.x = 200;
        ((MINMAXINFO*) lparam)->ptMinTrackSize.y = 200;
        return 0;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN: // on_mouse_down(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)); return 0;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP: //on_mouse_up(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)); return 0;
    case WM_MOUSEMOVE: //on_mouse_move(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)); return 0;
        return 0;
    case WM_KEYUP:
        if (wparam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }

        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

} // anonymous namespace

bool initialize(const std::wstring& title, i32 width, i32 height)
{
    window_title = title;
    hinst        = GetModuleHandle(nullptr);
    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = message_proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hinst;
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = L"MainWnd";

    if (!RegisterClass(&wc))
    {
        MessageBox(nullptr, L"Failed to register window class", nullptr, 0);
        return false;
    }

    client_width  = width;
    client_height = height;

    RECT r = { 0, 0, width, height };
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
    i32 w = r.right - r.left;
    i32 h = r.bottom - r.top;

    window_handle = CreateWindow(L"MainWnd", title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h, nullptr,
                                 nullptr, hinst, nullptr);
    if (!window_handle)
    {
        MessageBox(nullptr, L"Failed to create window", nullptr, 0);
        return false;
    }

    ShowWindow(window_handle, SW_SHOW);
    UpdateWindow(window_handle);
    return true;
}

void      shutdown() {}
HINSTANCE instance()
{
    return hinst;
}
HWND handle()
{
    return window_handle;
}
bool is_fullscreen()
{
    return fullscreen;
}
bool is_resizing()
{
    return resizing;
}
bool is_minimized()
{
    return minimized;
}
bool is_maximized()
{
    return maximized;
}

bool is_paused()
{
    return paused;
}

i32 width()
{
    return client_width;
}
i32 height()
{
    return client_height;
}
f32 aspect_ratio()
{
    return static_cast<f32>(client_width) / client_height;
}

std::wstring title()
{
    return window_title;
}


} // namespace hex::window