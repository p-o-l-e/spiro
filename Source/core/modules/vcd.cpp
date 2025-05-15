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

namespace core {

    delay::delay()
    {
        init(interface::vcd::ctrls, interface::vcd::ins, interface::vcd::outs, module_type::vcd, 0);
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

        for(int i = 0; i < interface::vcd::ctrls; ++i)  ctrl[i] = &zero;
        for(int i = 0; i < interface::vcd::ins; ++i)    in[i]   = &zero;
        for(int i = 0; i < interface::vcd::outs; ++i)   out[i].store(0.0f);
    }

    delay::~delay() {}

    void delay::process()
    {
        if (departed >= length) departed = 0;
        float time = in[static_cast<int>(interface::vcd::in::time)]->load() 
                   + ctrl[static_cast<int>(interface::vcd::ctrl::time)]->load();


        if      (time > 1.0f) time = 1.0f;
        else if (time < 0.1f) time = 0.1f;


        time = psf.process(time);
        

        float input = in[static_cast<int>(interface::vcd::in::a)]->load()  +
                      in[static_cast<int>(interface::vcd::in::b)]->load()  +
                      in[static_cast<int>(interface::vcd::in::c)]->load()  +
                      in[static_cast<int>(interface::vcd::in::d)]->load();


        int f = departed - roundf(fabsf(time) * tmax);
        if (f < 0) f += length;


        float feedback = in[static_cast<int>(interface::vcd::in::feed)]->load()  
                       + ctrl[static_cast<int>(interface::vcd::ctrl::feed)]->load();
        if      (feedback > 1.0f) feedback = 1.0f;
        else if (feedback < 0.0f) feedback = 0.0f;

        float accu = data[departed] = input + (data[f] * feedback);

        apf.a = fabsf(eax - time);
        accu = apf.process(accu);
        eax = time;
        departed++;

        out[static_cast<int>(interface::vcd::out::a)].store(accu);
        out[static_cast<int>(interface::vcd::out::b)].store(accu);
        out[static_cast<int>(interface::vcd::out::c)].store(accu);
        out[static_cast<int>(interface::vcd::out::d)].store(accu);
    }
};
