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

#include "envelopes.hpp"
namespace cell {

namespace settings {
        unsigned env_time_max_ms = 120 /* seconds */ * 1000;
        float env_time_multiplier = 1;
        void reset_time_multiplier()
        {
            env_time_multiplier = float(env_time_max_ms) * 1000.0f / float(sample_rate);
        }
    };

void envelope::start()
{
    stage = 1;
    departed = 0;
    for(int i = 0; i < SEGMENTS; ++i)
    {
        value[i] = node[i].value->load() * value_scale;
        time[i]  = node[i].time->load() * time_multiplier * time_scale->load();
        curve[i] = node[i].curve->load();
    }
    theta = value[stage] - value[stage - 1];
    delta = time[stage] - time[stage - 1];
}

void envelope::reset()
{
    stage    = 0;
    departed = 0;
    for(int i = 0; i < SEGMENTS; ++i)
    {
        time[i]  = 0;
        value[i] = 0.0f;
        curve[i] = 0.0f;
    }
    settings::reset_time_multiplier();
}

void envelope::next_stage()
{
    stage++;
    departed = 0;
    if  (stage >= SEGMENTS)  stage = OFF;
    else
    {
        theta = value[stage] - value[stage - 1];
        delta = time[stage] - time[stage - 1];
    }
}

void envelope::jump(int to)
{
    departed = 0;
    stage = to;
    value[stage - 1] = out.load();
    theta = value[stage] - value[stage - 1];
    delta = time[stage] - time[stage - 1];
}

float envelope::iterate()
{
    if(stage > 0)
    {
        out.store(formEnvelope[(int)curve[stage]](float(departed), value[stage - 1], theta, float(delta)));
        departed++;
        if (departed >= delta) next_stage();
        if (std::isnan(out.load())) out.store(0.0f);
        return out.load();
    }
    return 0.0f;
}

void envelope::generate(float* data, int width)
{
    reset();
    start();
    for(int i = 0; i < width; ++i) data[i] = iterate();
    regenerate = false;
}


cell::envelope::envelope()
{
    reset();
}

cell::envelope::~envelope()
{
}




////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
};
