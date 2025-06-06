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
#include "env_interface.hpp"
#include "constants.hpp"
#include "iospecs.hpp"

namespace core 
{
    namespace env 
    {
        constexpr int Off = 0;
        constexpr int Sustain = 3;
        constexpr int Segments = 6; 
        constexpr int Forms = 4;
    };

    class ENV: public Module<float>
    {
        private:
            static int idc;  
            float theta = 0.0f;                 // Change in value_scale
            unsigned delta = 0;
            unsigned time[env::Segments];
            float level[env::Segments];
            float curve[env::Segments];

        public:
            const int id = 0;

            float time_multiplier; 
            uint  departed;                     // Current sample
            int   stage = 0;                    // Current stage
            void  start();          
            void  next_stage();
            void  jump(int);                    // Jump to stage N 
            void  reset();
            float iterate();
            void  process() noexcept override;
            float value_scale = 1.0f;
            bool  freerun = true;
            ENV();
           ~ENV() = default;
    };

/******************************************************************************************************************************
*   Easing functions
*   t = Time - Amount of time that has passed since the beginning.
*   b = Beginning value - The starting point of the transition.
*   c = Change in value - The amount of change needed to go from starting point to end point.
*   d = Duration - Amount of time the transition will take.
******************************************************************************************************************************/
constexpr float fLinear(float t, const float b, const float c, const float d)
{
        return c * t / d + b;
}

constexpr float fCubicIn(float t, const float b, const float c, const float d)
{
        t /= d;
        return c * t * t * t + b;
}

constexpr float fCubicOut(float t, const float b, const float c, const float d)
{
        t /= d;
        t--;
        return c * (t * t * t + 1.0f) + b;
};

constexpr float fCubicIO(float t, const float b, const float c, const float d)
{
        t /= (d * 0.5f);
        if (t < 1.0f) return c * 0.5f * t * t * t + b;
        t -= 2;
        return c * 0.5f * (t * t * t + 2.0f) + b;
}

inline float (*ease[])(float, const float, const float, const float) = 
{ 
    fLinear,
    fCubicOut,
    fCubicIn,
    fCubicIO
};



};
