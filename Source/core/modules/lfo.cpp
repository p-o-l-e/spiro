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

namespace core
{
    using namespace core::interface;

    int lfo_t::idc = 0;

    void lfo_t::process()
    {
        float o = (this->*form[(int)ccv[lfo::ctl::form]->load()])();
        ocv[lfo::cvo::a].store(o);
        ocv[lfo::cvo::b].store(o);
    }

    float lfo_t::sine()
    {
        phase += (*ccv[lfo::ctl::delta] + fabsf(*icv[lfo::cvi::fm])) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return cosf(phase) * ccv[lfo::ctl::amp]->load() * (icv[lfo::cvi::am] == &zero ? 1.0f : icv[lfo::cvi::am]->load());
    }

    float lfo_t::ramp()
    {
        phase += (*ccv[lfo::ctl::delta] + fabsf(*icv[lfo::cvi::fm])) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return atanf(tanf(phase * 0.5f)) * ccv[lfo::ctl::amp]->load() * (icv[lfo::cvi::am] == &zero ? 1.0f : icv[lfo::cvi::am]->load());
    }

    float lfo_t::saw()
    {
        phase += ( *ccv[lfo::ctl::delta] + fabsf(*icv[lfo::cvi::fm]) ) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return atanf(tanf(pi - phase * 0.5f)) * ccv[lfo::ctl::amp]->load() * (icv[lfo::cvi::am] == &zero ? 1.0f : icv[lfo::cvi::am]->load());
    }

    float lfo_t::square()
    {
        phase += ( *ccv[lfo::ctl::delta] + fabsf(*icv[lfo::cvi::fm]) ) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return (phase > 0.0f ? 1.0f : 0.0f) * ccv[lfo::ctl::amp]->load() * (icv[lfo::cvi::am] == &zero ? 1.0f : icv[lfo::cvi::am]->load());
    }

    float lfo_t::triangle()
    {
        phase += ( *ccv[lfo::ctl::delta] + fabsf(*icv[lfo::cvi::fm]) ) * tao / settings::sample_rate;
        if(phase > pi) phase -= tao;
        return tan(sin(phase)) * ccv[lfo::ctl::amp]->load() * (icv[lfo::cvi::am] == &zero ? 1.0f : icv[lfo::cvi::am]->load()) * 0.65f;
    }

    void lfo_t::reset()
    {
        phase = 0.0f;
        for(int i = 0; i < lfo::cc; ++i) ccv[i] = &zero;
        for(int i = 0; i < lfo::ic; ++i) icv[i] = &zero;
        for(int i = 0; i < lfo::oc; ++i) ocv[i].store(0.0f);
    }

    lfo_t::lfo_t(): id(++idc)
    {
        init(id, &interface::lfo::descriptor);
        reset();
    };
}
