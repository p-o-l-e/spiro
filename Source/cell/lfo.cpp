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

#include "lfo.h"

namespace cell{

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
