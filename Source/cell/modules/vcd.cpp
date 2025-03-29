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
        init(0, &interface::vcd::descriptor);
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

        for(int i = 0; i < interface::vcd::cc; ++i)  ctrl[i] = &zero;
        for(int i = 0; i < interface::vcd::ic; ++i)    in[i] = &zero;
        for(int i = 0; i < interface::vcd::oc; ++i)   out[i].store(0.0f);
    }

    delay::~delay() {}

    void delay::process()
    {
        if (departed >= length) departed = 0;
        float time = in[interface::vcd::cvi::time]->load() + ctrl[interface::vcd::ctl::time]->load();

        if      (time > 1.0f) time = 1.0f;
        else if (time < 0.1f) time = 0.1f;


        time = psf.process(time);
        

        float input = in[interface::vcd::cvi::a]->load()  +
                      in[interface::vcd::cvi::b]->load()  +
                      in[interface::vcd::cvi::c]->load()  +
                      in[interface::vcd::cvi::d]->load();


        int f = departed - roundf(fabsf(time) * tmax);
        if (f < 0) f += length;

        float feedback = in[interface::vcd::cvi::feed]->load() + ctrl[interface::vcd::ctl::feed]->load();
        if      (feedback > 1.0f) feedback = 1.0f;
        else if (feedback < 0.0f) feedback = 0.0f;

        float accu = data[departed] = input + (data[f] * feedback);

        apf.a = fabsf(eax - time);
        accu = apf.process(accu);
        eax = time;
        departed++;

        out[interface::vcd::cvo::a].store(accu);
        out[interface::vcd::cvo::b].store(accu);
        out[interface::vcd::cvo::c].store(accu);
        out[interface::vcd::cvo::d].store(accu);
    }
};
