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


#include "Timer.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace hex
{

timer::timer()
{
    i64 counts_per_sec{};
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&counts_per_sec));
    m_seconds_per_count = 1.0 / static_cast<f64>(counts_per_sec);
}

f32 timer::total_time() const
{
    if (m_stopped)
    {
        return static_cast<f32>((m_stop_time - m_paused_time - m_base_time) * m_seconds_per_count);
    }

    return static_cast<f32>((m_current_time - m_paused_time - m_base_time) * m_seconds_per_count);
}

void timer::reset()
{
    i64 curr_time{};
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr_time));

    m_base_time = curr_time;
    m_prev_time = curr_time;
    m_stop_time = 0;
    m_stopped   = false;
}

void timer::start()
{
    i64 start_time{};
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time));

    if (m_stopped)
    {
        m_paused_time += start_time - m_stop_time;
        m_prev_time = start_time;
        m_stop_time = 0;
        m_stopped   = false;
    }
}

void timer::stop()
{
    if (m_stopped)
        return;

    i64 curr_time{};
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr_time));
    m_stop_time = curr_time;
    m_stopped   = true;
}

void timer::tick()
{
    if (m_stopped)
    {
        m_delta = 0.0;
        return;
    }

    i64 curr_time{};
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr_time));
    m_current_time = curr_time;

    m_delta = static_cast<f64>(m_current_time - m_prev_time) * m_seconds_per_count;

    m_prev_time = m_current_time;


    if (m_delta < 0.0)
    {
        m_delta = 0.0;
    }
}

} // namespace hex