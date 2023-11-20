/*****************************************************************************************************************************
* ISC License
*
* Copyright (c) 2023 POLE
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
* REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
* AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
* INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
* OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
******************************************************************************************************************************/

#pragma once

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <atomic>
#include "constants.hpp"
#include "iospecs.hpp"

namespace cell {
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



class envelope
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
        void  generate(float*, int);        // Compute envelope to given array
        envelope();
       ~envelope();
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

inline float (*formEnvelope[])(float, float, float, float) = 
{ 
    fLinear,
    fCubicOut,
    fCubicIn,
    fCubicIO
};



};
