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
#include <iostream>
namespace core {
using namespace env; 

int ENV::idc = 0;

inline float linearToLog(float value) 
{
    return std::pow(value, 2.0f);
}

void ENV::start()
{
    stage = 1;
    departed = 0;
    for(int i = 0; i < env::Segments; ++i)
    {
        level[i] = linearToLog(ccv[ctl::aa + i]->load()) * value_scale;
        time[i]  = ccv[ctl::at + i]->load() * ccv[ctl::scale]->load() * core::settings::sample_rate * 5.0f;
        curve[i] = ccv[ctl::af + i]->load();
    }
    theta = level[stage] - level[stage - 1];
    delta = time[stage] - time[stage - 1];
}

void ENV::reset()
{
    stage    = 0;
    departed = 0;
    for(int i = 0; i < env::Segments; ++i)
    {
        time[i]  = 0;
        level[i] = 0.0f;
        curve[i] = 0.0f;
    }
}

void ENV::next_stage()
{
    stage++;
    departed = 0;
    if  (stage >= env::Segments)  stage = env::Off;
    else
    {
        theta = level[stage] - level[stage - 1];
        delta = time[stage] - time[stage - 1];
    }
}

void ENV::jump(int target)
{
    departed = 0;
    stage = target;
    level[stage - 1] = ocv[env::cvo::a].load();
    theta = level[stage] - level[stage - 1];
    delta = time[stage] - time[stage - 1];
}

float ENV::iterate()
{
    if(stage > 0)
    {
        ocv[env::cvo::a].store(ease[(int)curve[stage]](float(departed), level[stage - 1], theta, float(delta)));
        departed++;
        if (departed >= delta) next_stage();
        if (std::isnan(ocv[env::cvo::a].load())) ocv[env::cvo::a].store(0.0f);
        return ocv[env::cvo::a].load();
    }
    return 0.0f;
}

void ENV::process() noexcept
{
    iterate();
}

core::ENV::ENV(): id(idc++), Module(idc, &env::descriptor[0])
{
    reset();
}


};
