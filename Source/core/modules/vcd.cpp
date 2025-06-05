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
#include "node.hpp"
#include "vcd_interface.hpp"

namespace core 
{
    using namespace vcd;
    int vcd_t::idc = 0;

    vcd_t::vcd_t(): id(idc++), Module(idc, &vcd::descriptor)
    {
        reset();
    }

    void vcd_t::reset() 
    {
        psf.reset(10.0f);
        apf.a   = 0.6f;
        length  = settings::sample_rate/4;
        tmax    = length/2;
        eax     = 0.0f;
        departed  = 0;
        data = std::make_unique<float[]>(length);
        for (uint i = 0; i < length; i++)  data.get()[i] = 0.0f;

        for(int i = 0; i < cc; ++i) ccv[i] = &zero;
        for(int i = 0; i < ic; ++i) icv[i] = &zero;
        for(int i = 0; i < oc; ++i) ocv[i].store(0.0f);
    }

    vcd_t::~vcd_t() {}

    void vcd_t::process() noexcept
    {
        if (departed >= length) departed = 0;
        float time = icv[cvi::time]->load() + ccv[ctl::time]->load();

        if      (time > 1.0f) time = 1.0f;
        else if (time < 0.1f) time = 0.1f;


        time = psf.process(time);
        

        float input = icv[cvi::a]->load()  +
                      icv[cvi::b]->load()  +
                      icv[cvi::c]->load()  +
                      icv[cvi::d]->load();


        int f = departed - roundf(fabsf(time) * tmax);
        if (f < 0) f += length;

        float feedback = icv[cvi::feed]->load() + ccv[ctl::feed]->load();
        if      (feedback > 1.0f) feedback = 1.0f;
        else if (feedback < 0.0f) feedback = 0.0f;

        float accu = data[departed] = input + (data[f] * feedback);

        apf.a = fabsf(eax - time);
        accu = apf.process(accu);
        eax = time;
        departed++;

        ocv[cvo::a].store(accu);
        ocv[cvo::b].store(accu);
        ocv[cvo::c].store(accu);
        ocv[cvo::d].store(accu);
    }
};
