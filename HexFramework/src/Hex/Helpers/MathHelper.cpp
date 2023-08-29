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
//  File Name: MathHelper.cpp
//  Date File Created: 08/29/2023
//  Author: Matt
//
//  ------------------------------------------------------------------------------
#include "MathHelper.h"


#include "RandomHelper.h"

using namespace DirectX;

namespace hex::math
{

f32 angle_from_xy(f32 x, f32 y)
{
    f32 theta{};
    // Quadrant I or IV
    if (x >= 0.f)
    {
        theta = atanf(y / x);
        if (theta < 0.f)
        {
            theta += two_pi;
        }
    } else // Quadrant II or III
    {
        theta = atanf(y / x) + pi;
    }

    return theta;
}

vec random_unit_vec3()
{
    const vec one = XMVectorSet(1.f, 1.f, 1.f, 1.f);

    // Keep trying until a point on/in the hemisphere is selected
    while (true)
    {
        const vec v = XMVectorSet(random::get_float(-1.f, 1.f), random::get_float(-1.f, 1.f), random::get_float(-1.f, 1.f), 0.f);

        if (XMVector3Greater(XMVector3LengthSq(v), one))
        {
            continue;
        }

        return XMVector3Normalize(v);
    }
}

vec random_hemisphere_unit_vec3(vec n)
{
    const vec one  = XMVectorSet(1.f, 1.f, 1.f, 1.f);
    const vec zero = XMVectorZero();

    // Keep trying until a point on/in the hemisphere is selected
    while (true)
    {
        const vec v = XMVectorSet(random::get_float(-1.f, 1.f), random::get_float(-1.f, 1.f), random::get_float(-1.f, 1.f), 0.f);

        if (XMVector3Greater(XMVector3LengthSq(v), one) || XMVector3Less(XMVector3Dot(n, v), zero))
        {
            continue;
        }

        return XMVector3Normalize(v);
    }
}

} // namespace hex::math