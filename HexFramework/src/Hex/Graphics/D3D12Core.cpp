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
//  File Name: D3D12Core.cpp
//  Date File Created: 08/30/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------
#include "D3D12Core.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace hex::graphics::core
{

namespace
{
comptr<IDXGIFactory4> dxgi_factory;
comptr<ID3D12Device>  main_device;


#ifdef _DEBUG

void log_output_display_modes(IDXGIOutput* output, DXGI_FORMAT format)
{
    u32           count{};
    constexpr u32 flags{};

    output->GetDisplayModeList(format, flags, &count, nullptr);

    std::vector<DXGI_MODE_DESC> modes{ count };
    output->GetDisplayModeList(format, flags, &count, &modes[0]);

    for (auto& m : modes)
    {
        const u32 n = m.RefreshRate.Numerator;
        const u32 d = m.RefreshRate.Denominator;
        const f32 r = (f32)n / d;

        std::wstring text = L"Width = " + std::to_wstring(m.Width) + L" " + L"Height = " + std::to_wstring(m.Height) + L" " +
                            L"Refresh = " + std::to_wstring(r) + L" Hz\n";
        OutputDebugString(text.c_str());
    }
}

void log_adapter_outputs(IDXGIAdapter* adapter)
{
    u32          n{};
    IDXGIOutput* output{ nullptr };
    while (adapter->EnumOutputs(n, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugString(text.c_str());

        log_output_display_modes(output, default_backbuffer_format);

        RELEASE_COM(output);
        ++n;
    }
}

void log_adapters()
{
    u32                        n{};
    IDXGIAdapter*              adapter{ nullptr };
    std::vector<IDXGIAdapter*> adapters{};
    while (dxgi_factory->EnumAdapters(n, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring str = L"***Adapter: ";
        str += desc.Description;
        str += L"\n";

        OutputDebugString(str.c_str());

        adapters.push_back(adapter);

        ++n;
    }

    for (u32 i = 0; i < adapters.size(); ++i)
    {
        log_adapter_outputs(adapters[i]);
        RELEASE_COM(adapters[i]);
    }
}
#endif

} // anonymous namespace

bool initialize()
{
#ifdef _DEBUG
    comptr<ID3D12Debug> debug_controller;
    DX_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
    debug_controller->EnableDebugLayer();
#endif

    DX_CALL(CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory)));

    const HRESULT hw_result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&main_device));

    if (FAILED(hw_result))
    {
        comptr<IDXGIAdapter> warp_adapter;
        DX_CALL(dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&warp_adapter)));
        DX_CALL(D3D12CreateDevice(warp_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&main_device)));
    }


#ifdef _DEBUG
    log_adapters();
#endif

    return true;
}

} // namespace hex::graphics