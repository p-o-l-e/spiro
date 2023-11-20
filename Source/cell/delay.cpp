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


#include "delay.hpp"

namespace cell {

    delay::delay()
    {
        init(interface::vcd::ctrls, interface::vcd::ins, interface::vcd::outs);
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
