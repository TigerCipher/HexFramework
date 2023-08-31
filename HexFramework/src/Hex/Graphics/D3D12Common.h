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
//  File Name: D3D12Common.h
//  Date File Created: 08/30/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------

#pragma once

#include "Hex/Common.h"
#include "D3D12Debug.h"
#include "Hex/Helpers/MathHelper.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

template<typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

#ifndef RELEASE_COM
    #define RELEASE_COM(x)                                                                                                       \
        if (x)                                                                                                                   \
        {                                                                                                                        \
            x->Release();                                                                                                        \
            x = nullptr;                                                                                                         \
        }
#endif