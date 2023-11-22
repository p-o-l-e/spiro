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

#pragma once

#include <iostream>
#include <cstdlib>
#include <map>

#include "containers.hpp"
#include "iospecs.hpp"
#include "filters.hpp"
#include "envelopes.hpp"
#include "utility.hpp"
#include "oscillator.hpp"
#include "modmatrix.hpp"
#include "interface.h"
#include "chaos.hpp"
#include "rotor.h"
#include "sigma.h"
#include "product.h"
#include "mixer.h"
#include "lfo.h"
#include "snh.h"
#include "vca.h"

namespace cell {


class spawner
{  
    public:
        bus_connector bus;
        oscillator  vco[settings::osc_n];
        envelope    env[settings::env_n];
        vcf_t       vcf[settings::vcf_n];
        vca_t       vca[settings::vca_n];
        rotor       rot[settings::rtr_n];
        delay       vcd[settings::vcd_n];
        mixer       mix[settings::mix_n];
        map_t       chs[settings::map_n];
        snh_t       snh[settings::snh_n];
        lfo_t       lfo[settings::lfo_n];
        sigma       sum[settings::sum_n];
        product     pct[settings::pct_n];
        dcb_t       dcb[2];
        rack_t      rack;
        patchbay*   bay = nullptr;
        float       out[2];                       // LR Output
        void        connect_bus();
        void        process();
        void        reset();
        spawner();
       ~spawner();
};

};
