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
        VCO vco[settings::osc_n];
        EGM env[settings::env_n];
        VCF vcf[settings::vcf_n];
        VCA vca[settings::vca_n];
        RTR rot[settings::rtr_n];
        VCD vcd[settings::vcd_n];
        MIX mix[settings::mix_n];
        CSO chs[settings::map_n];
        SNH snh[settings::snh_n];
        LFO lfo[settings::lfo_n];
        SUM sum[settings::sum_n];
        PDT pct[settings::pct_n];
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
