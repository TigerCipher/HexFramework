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
comptr<IDXGIFactory7>       dxgi_factory;
comptr<ID3D12Device>        main_device;
constexpr D3D_FEATURE_LEVEL min_feature_level = D3D_FEATURE_LEVEL_11_0;

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
        const f32 r = (f32) n / d;

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

        release(output);
        ++n;
    }
}

IDXGIAdapter* determine_main_adapter()
{
    LOG_INFO("Selecting best adapter");
    u32           n{};
    IDXGIAdapter* adapter{ nullptr };
    while (dxgi_factory->EnumAdapterByGpuPreference(n, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) !=
           DXGI_ERROR_NOT_FOUND)
    {
        // Pick the first that features minimum feature level
        if (SUCCEEDED(D3D12CreateDevice(adapter, min_feature_level, __uuidof(ID3D12Device), nullptr)))
        {
            DXGI_ADAPTER_DESC desc;
            adapter->GetDesc(&desc);

            char buf[128];
            wcstombs(buf, desc.Description, 128);
            LOG_DEBUG("Adapter: {}", buf);
            LOG_DEBUG("Vendor ID: 0x{:0>8X}", desc.VendorId);
            LOG_DEBUG("Device ID: 0x{:0>8X}", desc.DeviceId);
            LOG_DEBUG("Video Memory: {:.2f} GB", desc.DedicatedVideoMemory / 1024.0 / 1024.0 / 1024.0);
            LOG_DEBUG("Shared Memory: {:.2f} GB", desc.SharedSystemMemory / 1024.0 / 1024.0 / 1024.0);


            log_adapter_outputs(adapter);
            LOG_INFO("Adapter selected");
            return adapter;
        }


        ++n;

        release(adapter);
    }

    return nullptr;
}
#endif


D3D_FEATURE_LEVEL get_max_feature_level(IDXGIAdapter* adapter)
{
    constexpr D3D_FEATURE_LEVEL feat_levels[4] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_12_1,
    };

    D3D12_FEATURE_DATA_FEATURE_LEVELS feat_level_info;
    feat_level_info.NumFeatureLevels        = _countof(feat_levels);
    feat_level_info.pFeatureLevelsRequested = feat_levels;

    comptr<ID3D12Device> device;
    DX_CALL(D3D12CreateDevice(adapter, min_feature_level, IID_PPV_ARGS(&device)));
    DX_CALL(device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &feat_level_info, sizeof(feat_level_info)));
    LOG_INFO("Max supported feature level found");
    return feat_level_info.MaxSupportedFeatureLevel;
}

} // anonymous namespace

bool initialize()
{
    LOG_INFO("Initializing Direct3D 12");
#ifdef _DEBUG
    LOG_DEBUG("Enabling the debug layer");
    comptr<ID3D12Debug> debug_controller;
    DX_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
    debug_controller->EnableDebugLayer();
#endif

    DX_CALL(CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory)));

    comptr<IDXGIAdapter> main_adapter;
    main_adapter.Attach(determine_main_adapter());
    if (!main_adapter)
    {
        return false;
    }

    LOG_TRACE("Obtaining max feature level for chosen adapter");
    const D3D_FEATURE_LEVEL max_feature_level = get_max_feature_level(main_adapter.Get());
    assert(max_feature_level >= min_feature_level);
    if (max_feature_level < min_feature_level)
    {
        return false;
    }
    const HRESULT hw_result = D3D12CreateDevice(nullptr, max_feature_level, IID_PPV_ARGS(&main_device));

    if (FAILED(hw_result))
    {
        LOG_WARN("Failed to create d3d12 device, falling back to warp device");
        comptr<IDXGIAdapter> warp_adapter;
        DX_CALL(dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&warp_adapter)));
        DX_CALL(D3D12CreateDevice(warp_adapter.Get(), min_feature_level, IID_PPV_ARGS(&main_device)));
    }
    LOG_INFO("d3d12 device created");

#ifdef _DEBUG
    {
        comptr<ID3D12InfoQueue> info_queue;
        DX_CALL(main_device->QueryInterface(IID_PPV_ARGS(&info_queue)));
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
    }
#endif


    LOG_INFO("Direct3D 12 initialized");
    return true;
}

} // namespace hex::graphics::core