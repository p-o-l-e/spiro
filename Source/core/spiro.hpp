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

#pragma once

#include <iostream>
#include <cstdlib>

#include "containers.hpp"
#include "iospecs.hpp"
#include "utility.hpp"
#include "modmatrix.hpp"
#include "interface.h"
#include "module_headers.hpp"
#include "rack.hpp"
#include "grid.hpp"

#ifdef DEBUG_MODE
    #include <iostream>
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
    #define LOG(x)
#endif

namespace core {


class Spiro
{ 
    private:
        void note_on (uint8_t, uint8_t);
        void note_off(uint8_t);

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
        Rack      rack;
        patchbay*   bay = nullptr;
        float       out[2];                       // LR Output
        void midi_message(uint8_t, uint8_t, uint8_t);
        void        connect_bus();
        void        process();
        void        reset();
        Spiro(const Descriptor**);
       ~Spiro();
};

};
