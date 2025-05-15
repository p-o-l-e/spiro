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

#include "lfo.hpp"

namespace core{

int lfo_t::idc = 0;

void lfo_t::process()
{
    float o = (this->*form[(int)ctrl[static_cast<int>(interface::lfo::ctrl::form)]->load()])();
    out[static_cast<int>(interface::lfo::out::a)].store(o);
    out[static_cast<int>(interface::lfo::out::b)].store(o);
}

float lfo_t::sine()
{
    phase += (
                    *ctrl[static_cast<int>(interface::lfo::ctrl::delta)] +
                    fabsf(*in[static_cast<int>(interface::lfo::in::fm)])
             ) 
             * tao / settings::sample_rate;

    if(phase > pi) phase -= tao;
    return
        cosf(phase) * 
        ctrl[static_cast<int>(interface::lfo::ctrl::amp)]->load() * 
        (in[static_cast<int>(interface::lfo::in::am)] == &zero ? 1.0f : in[static_cast<int>(interface::lfo::in::am)]->load());
}

float lfo_t::ramp()
{
    phase += (
                    *ctrl[static_cast<int>(interface::lfo::ctrl::delta)] + 
                    fabsf(*in[static_cast<int>(interface::lfo::in::fm)])
             ) 
             * tao / settings::sample_rate;

    if(phase > pi) phase -= tao;
    return
        atanf(tanf(phase * 0.5f)) * 
        ctrl[static_cast<int>(interface::lfo::ctrl::amp)]->load() * 
        (in[static_cast<int>(interface::lfo::in::am)] == &zero ? 1.0f : in[static_cast<int>(interface::lfo::in::am)]->load());
}

float lfo_t::saw()
{
    phase += (
                    *ctrl[static_cast<int>(interface::lfo::ctrl::delta)] + 
                    fabsf(*in[static_cast<int>(interface::lfo::in::fm)])
             ) 
             * tao / settings::sample_rate;
    if(phase > pi) phase -= tao;
    return
        atanf(tanf(pi - phase * 0.5f)) * 
        ctrl[static_cast<int>(interface::lfo::ctrl::amp)]->load() * 
        (in[static_cast<int>(interface::lfo::in::am)] == &zero ? 1.0f : in[static_cast<int>(interface::lfo::in::am)]->load());
}

float lfo_t::square()
{
    phase += (
                    *ctrl[static_cast<int>(interface::lfo::ctrl::delta)] + 
                    fabsf(*in[static_cast<int>(interface::lfo::in::fm)])
             ) 
             * tao / settings::sample_rate;
    if(phase > pi) phase -= tao;
    return 
        (phase > 0.0f ? 1.0f : 0.0f) * 
        ctrl[static_cast<int>(interface::lfo::ctrl::amp)]->load() * 
        (in[static_cast<int>(interface::lfo::in::am)] == &zero ? 1.0f : in[static_cast<int>(interface::lfo::in::am)]->load());
}

float lfo_t::triangle()
{
    phase += (
                    *ctrl[static_cast<int>(interface::lfo::ctrl::delta)] + 
                    fabsf(*in[static_cast<int>(interface::lfo::in::fm)])
             ) 
             * tao / settings::sample_rate;
    if(phase > pi) phase -= tao;
    return
        tan(sin(phase)) * 
        ctrl[static_cast<int>(interface::lfo::ctrl::amp)]->load() * 
        (in[static_cast<int>(interface::lfo::in::am)] == &zero ? 1.0f : in[static_cast<int>(interface::lfo::in::am)]->load()) * 
        0.65f;
}

void lfo_t::reset()
{
    phase = 0.0f;
    for(int i = 0; i < interface::lfo::ctrls; ++i)  ctrl[i] = &zero;
    for(int i = 0; i < interface::lfo::ins; ++i)    in[i]   = &zero;
    for(int i = 0; i < interface::lfo::outs; ++i)   out[i].store(0.0f);
}

lfo_t::lfo_t(): id(++idc)
{
    init(interface::lfo::ctrls, interface::lfo::ins, interface::lfo::outs, module_type::lfo, id);
    reset();
};


}
