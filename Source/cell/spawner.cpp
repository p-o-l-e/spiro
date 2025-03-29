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

#include "spawner.hpp"
#include "chaos.hpp"
#include "delay.hpp"
#include "filters.hpp"
#include "interface.h"
#include "lfo.h"
#include "mixer.h"
#include "node.h"
#include "oscillator.hpp"
#include "product.h"
#include "rotor.h"
#include "sigma.h"
#include "snh.h"
#include "vca.h"
#include <cstdint>
#include <iostream>

using namespace cell::interface;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destination buffer setter //////////////////////////////////////////////////////////////////////////////////////////////////
void cell::spawner::process()
{
    for(int e = 0; e < settings::env_n; ++e)
    {
        if(env[e].stage == OFF) env[e].start(); 
        env[e].iterate();
    }

    for(int e = 0; e < rack.size(); ++e) rack.process(e);

    out[0] = dcb[0].process(mix[0].out[0]);
    out[1] = dcb[1].process(mix[0].out[1]);
}

void cell::spawner::connect_bus()
{
    #ifdef DEBUG 
    std::cout<<"-- Connecting bus...\n";
    #endif

    int p = 0;
    for(int e = 0; e < settings::lfo_n; ++e) rack.bind(&lfo[e], p++);
    for(int e = 0; e < settings::snh_n; ++e) rack.bind(&snh[e], p++);
    for(int e = 0; e < settings::vca_n; ++e) rack.bind(&vca[e], p++);
    for(int e = 0; e < settings::map_n; ++e) rack.bind(&chs[e], p++);
    for(int e = 0; e < settings::osc_n; ++e) rack.bind(&vco[e], p++);
    for(int e = 0; e < settings::vcf_n; ++e) rack.bind(&vcf[e], p++);
    for(int e = 0; e < settings::vcd_n; ++e) rack.bind(&vcd[e], p++);
    for(int e = 0; e < settings::rtr_n; ++e) rack.bind(&rot[e], p++);
    for(int e = 0; e < settings::sum_n; ++e) rack.bind(&sum[e], p++);
    for(int e = 0; e < settings::pdt_n; ++e) rack.bind(&pdt[e], p++);
    for(int e = 0; e < settings::mix_n; ++e) rack.bind(&mix[e], p++);

    for(int o = 0; o < rack.registry->size(); ++o)
    {
        for(int i = 0; i < rack.at(o)->controls(); ++i)
        {
            uint32_t hash = rack.registry->get_hash(parameter_type::cc, i);
            rack.connect_pin_o(hash, bus.port_pin(hash));
        }
        for(int i = 0; i < rack.at(o)->inputs(); ++i)
        {
            uint32_t hash = rack.registry->get_hash(parameter_type::ic, i);
            rack.connect_pin_i(hash, bay->io[rack.registry->get_index(hash)].com);
        }
        for(int i = 0; i < rack.at(o)->outputs(); ++i)
        {
            uint32_t hash = rack.registry->get_hash(parameter_type::oc, i);
            rack.connect_pin_o(hash, bay->io[rack.registry->get_index(hash)].data);
        }
    }

   
    for(int i = 0; i < settings::osc_n; ++i)
    {
        for(int j = 0; j < settings::poly; ++j)
        {
            for(int e = 0; e < SEGMENTS; ++e)
            {
                vco[i].env[j].node[e].time     = env[i].node[e].time;
                vco[i].env[j].node[e].value    = env[i].node[e].value;
                vco[i].env[j].node[e].curve    = env[i].node[e].curve;
                vco[i].env[j].time_scale       = env[i].time_scale;
            }
        }
    }

    #ifdef DEBUG
    std::cout<<"-- Bus connected...\n";
    #endif
}

cell::spawner::spawner()
{
    #ifdef DEBUG
    std::cout<<"-- Spawner initialized...\n";
    #endif
}



cell::spawner::~spawner()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
