/*****************************************************************************************************************************
* Copyright (c) 2022-2023 POLE
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Based on: https://github.com/Takemonist/catmull_rom
******************************************************************************************************************************/

#pragma once
#include <cmath>
#include "containers.hpp"

namespace cell {

    constexpr point<float> interpolate_bezier(const point<float>& p0, const point<float>& p1, const point<float>& p2, const point<float>& p3, const float& t) noexcept // t always lies between 0 and 1
    {
        return point<float>
        {
            powf(1-t, 3) * p0.x + 3 * t * powf(1 - t, 2) * p1.x + 3 * t * t * (1 - t) * p2.x + powf(t, 3) * p3.x,
            powf(1-t, 3) * p0.y + 3 * t * powf(1 - t, 2) * p1.y + 3 * t * t * (1 - t) * p2.y + powf(t, 3) * p3.y
        };
    }

};
