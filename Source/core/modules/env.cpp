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

void ENV::start(float velocity, int v)
{
    stage[v] = 1;
    departed[v] = 0;
    for(int i = 0; i < env::Segments; ++i)
    {
        point[i][v].L = linearToLog(ccv[ctl::aa + i]->load()) * value_scale * velocity;
        point[i][v].T = ccv[ctl::at + i]->load() * ccv[ctl::scale]->load() * core::settings::sample_rate * 5.0f;
        point[i][v].F = ccv[ctl::af + i]->load();
    }
    theta[v] = point[stage[v]][v].L - point[stage[v] - 1][v].L;
    delta[v] = point[stage[v]][v].T - point[stage[v] - 1][v].T;
}

void ENV::reset(int v)
{
    stage[v] = 0;
    departed[v] = 0;
    for(int i = 0; i < env::Segments; ++i)
    {
        point[i][v].T = 0;
        point[i][v].L = 0;
        point[i][v].F = 0;
    }
}

void ENV::next_stage(int v)
{
    ++stage[v];
    departed[v] = 0;
    if(stage[v] >= env::Segments) stage[v] = env::Off;
    else
    {
        theta[v] = point[stage[v]][v].L - point[stage[v] - 1][v].L;
        delta[v] = point[stage[v]][v].T - point[stage[v] - 1][v].T;
    }
}

void ENV::jump(int target, int v)
{
    departed[v] = 0;
    stage[v] = target;
    point[stage[v] - 1][v].L = ocv[env::cvo::a].load();
    theta[v] = point[stage[v]][v].L - point[stage[v] - 1][v].L;
    delta[v] = point[stage[v]][v].T - point[stage[v] - 1][v].T;
}

float ENV::iterate(int v)
{
    if(stage[v] > 0)
    {
        ocv[env::cvo::a].store
        (
            ease[(int)point[stage[v]][v].F]
            (
                (float)departed[v],
                point[stage[v] - 1][v].L,
                theta[v],
                (float)delta[v]
            )
        );
        departed[v]++;
        if (departed[v] >= delta[v]) next_stage(v);
        if (std::isnan(ocv[env::cvo::a].load())) ocv[env::cvo::a].store(0.0f);
        return ocv[env::cvo::a].load();
    }
    return 0.0f;
}

void ENV::process() noexcept
{
    iterate(0);
}

core::ENV::ENV(): id(idc++), Module(idc, &env::descriptor[0])
{
    reset(0);
}


};
