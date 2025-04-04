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

#include "lfo.hpp"

namespace cell
{
    using namespace cell::interface;

    int lfo_t::idc = 0;

    void lfo_t::process()
    {
        float o = (this->*form[(int)ctrl[lfo::ctl::form]->load()])();
        out[lfo::cvo::a].store(o);
        out[lfo::cvo::b].store(o);
    }

    float lfo_t::sine()
    {
        phase += (*ctrl[lfo::ctl::delta] + fabsf(*in[lfo::cvi::fm])) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return cosf(phase) * ctrl[lfo::ctl::amp]->load() * (in[lfo::cvi::am] == &zero ? 1.0f : in[lfo::cvi::am]->load());
    }

    float lfo_t::ramp()
    {
        phase += (*ctrl[lfo::ctl::delta] + fabsf(*in[lfo::cvi::fm])) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return atanf(tanf(phase * 0.5f)) * ctrl[lfo::ctl::amp]->load() * (in[lfo::cvi::am] == &zero ? 1.0f : in[lfo::cvi::am]->load());
    }

    float lfo_t::saw()
    {
        phase += ( *ctrl[lfo::ctl::delta] + fabsf(*in[lfo::cvi::fm]) ) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return atanf(tanf(pi - phase * 0.5f)) * ctrl[lfo::ctl::amp]->load() * (in[lfo::cvi::am] == &zero ? 1.0f : in[lfo::cvi::am]->load());
    }

    float lfo_t::square()
    {
        phase += ( *ctrl[lfo::ctl::delta] + fabsf(*in[lfo::cvi::fm]) ) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return (phase > 0.0f ? 1.0f : 0.0f) * ctrl[lfo::ctl::amp]->load() * (in[lfo::cvi::am] == &zero ? 1.0f : in[lfo::cvi::am]->load());
    }

    float lfo_t::triangle()
    {
        phase += ( *ctrl[lfo::ctl::delta] + fabsf(*in[lfo::cvi::fm]) ) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return tan(sin(phase)) * ctrl[lfo::ctl::amp]->load() * (in[lfo::cvi::am] == &zero ? 1.0f : in[lfo::cvi::am]->load()) * 0.65f;
    }

    void lfo_t::reset()
    {
        phase = 0.0f;
        for(int i = 0; i < lfo::cc; ++i) ctrl[i] = &zero;
        for(int i = 0; i < lfo::ic; ++i) in[i] = &zero;
        for(int i = 0; i < lfo::oc; ++i) out[i].store(0.0f);
    }

    lfo_t::lfo_t(): id(++idc)
    {
        init(id, &lfo::descriptor);
        reset();
    };
}
