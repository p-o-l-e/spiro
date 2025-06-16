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
#include "vco.hpp"
#include <iostream>
namespace core {
using namespace env; 

int ENV::idc = 0;

inline float linearToLog(float value) noexcept
{
    return std::pow(value, 2.0f);
}

void ENV::start(float velocity, int v) noexcept
{
    stage[v] = ADSR::Attack;
    hold[v] = true;
    departed[v] = 0;
    for(int i = 0; i < env::Segments - 1; ++i)
    {
        node[i + 1][v].L = linearToLog(ccv[ctl::aa + i]->load()) * value_scale * velocity;
        node[i + 1][v].T = ccv[ctl::at + i]->load() * ccv[ctl::scale]->load() * core::settings::sample_rate;
        node[i + 1][v].F = ccv[ctl::af + i]->load();
    }
    theta[v] = node[stage[v]][v].L - node[stage[v] - 1][v].L;
    delta[v] = node[stage[v]][v].T - node[stage[v] - 1][v].T;

    onStart(v);
}

void ENV::next_stage(int v) noexcept
{
    ++stage[v];
    departed[v] = 0;
    if(stage[v] >= ADSR::Finish) 
    {
        onFinish(v);
        stage[v] = ADSR::Start;
    }
    else
    {
        theta[v] = node[stage[v]][v].L - node[stage[v] - 1][v].L;
        delta[v] = node[stage[v]][v].T - node[stage[v] - 1][v].T;
    }
}

void ENV::jump(int target, int v) noexcept
{
    if(target > stage[v])
    {
        departed[v] = 0;
        stage[v] = target;
        // node[stage[v] - 1][v].L = ocv[env::cvo::a].load();
        node[stage[v] - 1][v].L = pin[v];

        theta[v] = node[stage[v]][v].L - node[stage[v] - 1][v].L;
        delta[v] = node[stage[v]][v].T - node[stage[v] - 1][v].T;
    }
}

void ENV::iterate(int v) noexcept
{
    if(stage[v] > ADSR::Start)
    {
        if(stage[v] == ADSR::Sustain && hold[v])
        {
        }
        else
        {
            pin[v] =
            (
                ease[(int)node[stage[v]][v].F]
                (
                    (float)departed[v],
                    node[stage[v] - 1][v].L,
                    theta[v],
                    (float)delta[v]
                )
            );
            departed[v]++;
            if (departed[v] >= delta[v]) next_stage(v);
            ocv[env::cvo::a].store(pin[v]);
        }
        // if (std::isnan(ocv[env::cvo::a].load())) ocv[env::cvo::a].store(0.0f);
        // return ocv[env::cvo::a].load();
    }
    ocv[env::cvo::a].store(0.0f);

}

void ENV::process() noexcept
{
    for(int voice = 0; voice < settings::poly; ++voice) 
    {
        if(gate[voice]) 
        {
            iterate(voice);
        }
    }
}

core::ENV::ENV(): id(idc++), Module(idc, &env::descriptor[0])
{
    for(int v = 0; v < settings::poly; ++v)
    {
        stage[v] = 0;
        departed[v] = 0;
        for(int i = 0; i < env::Segments; ++i)
        {
            node[i][v].T = 0;
            node[i][v].L = 0;
            node[i][v].F = 0;
            gate[v] = false;
        }
    }
}


};
