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
//  File Name: Timer.h
//  Date File Created: 08/29/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------

#pragma once

#include "Hex/Common.h"

namespace hex
{

class timer
{
public:
    timer();

    f32 total_time() const;

    constexpr f32 delta() const { return (f32) m_delta; }

    void reset();
    void start();
    void stop();
    void tick();

private:
    f64 m_seconds_per_count{};
    f64 m_delta{ -1.0 };

    i64 m_base_time{};
    i64 m_paused_time{};
    i64 m_stop_time{};
    i64 m_prev_time{};
    i64 m_current_time{};

    bool m_stopped{};
};

} // namespace hex
