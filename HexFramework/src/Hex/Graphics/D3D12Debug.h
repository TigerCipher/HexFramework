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
// File Name: D3D12Debug
// Date File Created: 08/30/2023
// Author: Matt
//
// ------------------------------------------------------------------------------

#pragma once

#include "Hex/Common.h"
#include "Hex/Helpers/StringHelper.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <string>

#include <comdef.h>

namespace hex::graphics
{

class d3d12_exception
{
public:
    HRESULT      error_code{ S_OK };
    std::wstring function_name{};
    std::wstring filename{};
    i32          line_number{};

    d3d12_exception() = default;
    d3d12_exception(HRESULT hr, std::wstring function_name, std::wstring filename, i32 line_number) :
        error_code{ hr }, function_name{ std::move(function_name) }, filename{ std::move(filename) }, line_number{ line_number }
    {}

    std::wstring what() const
    {
        const _com_error   err{ error_code };
        const std::wstring msg = err.ErrorMessage();
        return function_name + L" failed in " + filename + L"; line " + std::to_wstring(line_number) + L"; error: " + msg;
    }
};

} // namespace hex::graphics


#ifndef DX_CALL
    #define DX_CALL(x)                                                                                                           \
        {                                                                                                                        \
            HRESULT      hr_ = (x);                                                                                              \
            std::wstring fn  = utl::to_wstring(__FILE__);                                                                        \
            if (FAILED(hr_))                                                                                                     \
            {                                                                                                                    \
                throw d3d12_exception{ hr_, L#x, fn, __LINE__ };                                                                 \
            }                                                                                                                    \
        }
#endif