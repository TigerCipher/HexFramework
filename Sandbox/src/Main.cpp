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
// File Name: Main
// Date File Created: 08/29/2023
// Author: Matt
//
// ------------------------------------------------------------------------------

#pragma comment(lib, "HexFramework.lib")

#include <Windows.h>
#include "Hex/Common.h"
#include "Hex/Core/Core.h"
#include "Hex/Graphics/D3D12Core.h"


using namespace hex;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    i32 status = -1;
    try
    {
        core::initialize(L"Sandbox", 1000, 800);
        status = core::run();
    } catch (graphics::d3d12_exception& e)
    {
        MessageBox(nullptr, e.what().c_str(), L"HR Failed", MB_OK);
        return -1;
    }
    return status;
}