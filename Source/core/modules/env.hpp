/*****************************************************************************************************************************
* Copyright (c) 2022-2025 POLE
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
******************************************************************************************************************************/

#pragma once

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <atomic>
#include "node.hpp"
#include "constants.hpp"
#include "iospecs.hpp"

namespace core {

    namespace settings {
        extern unsigned env_time_max_ms;
        extern float env_time_multiplier;
        void reset_time_multiplier();
    };

#define OFF             0 
#define SUS             3
#define SEGMENTS        6 
#define env_curves_n    4

struct breakpoint
{
    std::atomic<float>* time  = &zero;
    std::atomic<float>* value = &zero;
    std::atomic<float>* curve = &zero;
};



class env_t: public Module
{
    private:
        float theta = 0.0f;                 // Change in value_scale
        unsigned delta = 0;
    public:
        breakpoint  node[SEGMENTS];
        unsigned    time[SEGMENTS];
        float      value[SEGMENTS];
        float      curve[SEGMENTS];
        float time_multiplier = settings::env_time_multiplier; 
        uint  departed;                     // Current sample
        std::atomic<float> out;             // Current output
        int   stage = 0;                    // Current stage
        void  start();          
        void  next_stage();
        void  jump(int);                    // Jump to stage N 
        void  reset();
        float iterate();
        std::atomic<float>* time_scale = &one;
        float value_scale = (1.0f / 100.0f);
        bool  regenerate = false;
        bool  freerun = true;
        void  generate(float*, int);        // Compute env_t to given array
        env_t();
       ~env_t();
};


////////////////////////////////////////////////////////////////////////////////////////////
// t = Time - Amount of time that has passed since the beginning.
// b = Beginning value - The starting point of the transition.
// c = Change in value - The amount of change needed to go from starting point to end point.
// d = Duration - Amount of time the transition will take.
////////////////////////////////////////////////////////////////////////////////////////////
constexpr float fLinear(float t, float b, float c, float d)
{
        return c * t / d + b;
}
////////////////////////////////////////////////////////////////////////////////////////////
constexpr float fCubicIn(float t, float b, float c, float d)
{
        t /= d;
        return c*t*t*t + b;
}
////////////////////////////////////////////////////////////////////////////////////////////
constexpr float fCubicOut(float t, float b, float c, float d)
{
        t /= d;
        t--;
        return c * (t*t*t + 1.0f) + b;
};
////////////////////////////////////////////////////////////////////////////////////////////
constexpr float fCubicIO(float t, float b, float c, float d)
{
        t /= (d * 0.5f);
        if (t < 1.0f) return c * 0.5f * t*t*t + b;
        t -= 2;
        return c * 0.5f * (t*t*t + 2.0f) + b;
}

inline float (*formenv_t[])(float, float, float, float) = 
{ 
    fLinear,
    fCubicOut,
    fCubicIn,
    fCubicIO
};



};
