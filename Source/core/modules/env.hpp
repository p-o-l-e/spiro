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

#include <set>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <functional>
#include <atomic>
#include <memory>
#include "node.hpp"
#include "env_interface.hpp"
#include "constants.hpp"
#include "iospecs.hpp"
#include "vco.hpp"

namespace core 
{
    namespace env 
    {
        constexpr int Segments = 6; 
        constexpr int Forms = 4;

        template <typename Real>
        struct Node 
        {
            uint T;
            Real L;
            Real F;
        };
    };

    class ENV: public Module<float>
    {
        public:
            enum ADSR { Start, Attack, Decay, Sustain, Release, Finish };
            std::function<void(int)> onStart;
            std::function<void(int)> onFinish;
        private:
            static int idc;  
            float theta[settings::poly]{};                      // Change in value_scale
            uint delta[settings::poly]{};                       // Time delta
            float time_multiplier; 
            uint departed[settings::poly]{};                    // Current sample
            int stage[settings::poly]{};                        // Current stage
            env::Node<float> node[env::Segments][settings::poly];
            void next_stage(int) noexcept;
            void iterate(int) noexcept;

        public:
            float pin[settings::poly] {};
            bool gate[settings::poly] {};                            // Active voice
            bool hold[settings::poly] {};
            const int id = 0;
            void start(float, int) noexcept;          
            void jump(int, int) noexcept;                       // Jump to stage N 
            void process() noexcept override;
            float value_scale = 1.0f;
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
constexpr float fLinear(float t, float b, float c, float d)
{
        return c * t / d + b;
}

constexpr float fCubicIn(float t, float b, float c, float d)
{
        t /= d;
        return c * t * t * t + b;
}

constexpr float fCubicOut(float t, float b, float c, float d)
{
        t /= d;
        t--;
        return c * (t * t * t + 1.0f) + b;
};

constexpr float fCubicIO(float t, float b, float c, float d)
{
        t /= (d * 0.5f);
        if (t < 1.0f) return c * 0.5f * t * t * t + b;
        t -= 2;
        return c * 0.5f * (t * t * t + 2.0f) + b;
}

inline float (*ease[])(float, float, float, float) = 
{ 
    fLinear,
    fCubicOut,
    fCubicIn,
    fCubicIO
};



};
