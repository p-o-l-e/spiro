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
#include "vcd.hpp"

namespace core 
{
    using namespace interface::vcd;
    int delay::idc = 0;

    delay::delay(): id(++idc)
    {
        init(cc, ic, oc, module_type::vcd, 0);
        reset();
    }

    void delay::reset() 
    {
        psf.reset(10.0f);
        apf.a   = 0.6f;
        length  = settings::sample_rate/4;
        tmax    = length/2;
        eax     = 0.0f;
        departed  = 0;
        data = std::make_unique<float[]>(length);
        for (uint i = 0; i < length; i++)  data.get()[i] = 0.0f;

        for(int i = 0; i < cc; ++i) ctrl[i] = &zero;
        for(int i = 0; i < ic; ++i) in[i]   = &zero;
        for(int i = 0; i < oc; ++i) out[i].store(0.0f);
    }

    delay::~delay() {}

    void delay::process()
    {
        if (departed >= length) departed = 0;
        float time = in[cvi::time]->load() + ctrl[ctl::time]->load();

        if      (time > 1.0f) time = 1.0f;
        else if (time < 0.1f) time = 0.1f;

        time = psf.process(time);
        
        float input = in[cvi::a]->load() +
                      in[cvi::b]->load() +
                      in[cvi::c]->load() +
                      in[cvi::d]->load();

        int f = departed - roundf(fabsf(time) * tmax);
        if (f < 0) f += length;

        float feedback = in[cvi::feed]->load() + ctrl[ctl::feed]->load();
        if      (feedback > 1.0f) feedback = 1.0f;
        else if (feedback < 0.0f) feedback = 0.0f;

        float accu = data[departed] = input + (data[f] * feedback);

        apf.a = fabsf(eax - time);
        accu = apf.process(accu);
        eax = time;
        departed++;

        out[cvo::a].store(accu);
        out[cvo::b].store(accu);
        out[cvo::c].store(accu);
        out[cvo::d].store(accu);
    }
};
