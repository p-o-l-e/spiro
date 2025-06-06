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

#include "env.hpp"
#include "interface/env_interface.hpp"
namespace core 
{
    using namespace env; 
    namespace settings 
    {
        unsigned env_time_max_ms = 120 /* seconds */ * 1000;
        float env_time_multiplier = 1;
        void reset_time_multiplier()
        {
            env_time_multiplier = float(env_time_max_ms) * 1000.0f / float(sample_rate);
        }
    };

int ENV::idc = 0;

void ENV::start()
{
    stage = 1;
    departed = 0;
    for(int i = 0; i < SEGMENTS; ++i)
    {
        value[i] = ccv[ctl::aa + i]->load() * value_scale;
        time[i]  = ccv[ctl::at + i]->load() * time_multiplier * time_scale->load();
        curve[i] = ccv[ctl::af + i]->load();
    }
    theta = value[stage] - value[stage - 1];
    delta = time[stage] - time[stage - 1];
}

void ENV::reset()
{
    stage    = 0;
    departed = 0;
    for(int i = 0; i < SEGMENTS; ++i)
    {
        time[i]  = 0;
        value[i] = 0.0f;
        curve[i] = 0.0f;
    }
    settings::reset_time_multiplier();
}

void ENV::next_stage()
{
    stage++;
    departed = 0;
    if  (stage >= SEGMENTS)  stage = OFF;
    else
    {
        theta = value[stage] - value[stage - 1];
        delta = time[stage] - time[stage - 1];
    }
}

void ENV::jump(int target)
{
    departed = 0;
    stage = target;
    value[stage - 1] = out.load();
    theta = value[stage] - value[stage - 1];
    delta = time[stage] - time[stage - 1];
}

float ENV::iterate()
{
    if(stage > 0)
    {
        out.store(ease[(int)curve[stage]](float(departed), value[stage - 1], theta, float(delta)));
        departed++;
        if (departed >= delta) next_stage();
        if (std::isnan(out.load())) out.store(0.0f);
        return out.load();
    }
    return 0.0f;
}

void ENV::process() noexcept
{
    // if(stage <= 0) start();
    // iterate();
}

void ENV::generate(float* data, int width)
{
    reset();
    start();
    for(int i = 0; i < width; ++i) data[i] = iterate();
    regenerate = false;
}


core::ENV::ENV(): id(idc++), Module(idc, &env::descriptor[0])
{
    reset();
}


};
