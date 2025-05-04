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

#include "spiro.hpp"
#include <cstdint>
#include <iostream>

namespace core
{
    using namespace interface;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Destination buffer setter //////////////////////////////////////////////////////////////////////////////////////////////////
    void spiro_t::process()
    {
        for(int o = 0; o < rack.bus.blueprint.mc; ++o) rack.process(o);

        out[stereo::l].store(rack.at(core::map::module::type::mix, 0)->ocv[mix::cvo::l]);
        out[stereo::r].store(rack.at(core::map::module::type::mix, 0)->ocv[mix::cvo::r]);
        
        out[stereo::l].store(dcb[0].process(out[stereo::l].load()));
        out[stereo::r].store(dcb[1].process(out[stereo::l].load()));
    }

    void spiro_t::connect_bus()
    {
        #ifdef DEBUG 
            std::cout<<"-- Connecting bus...\n";
        #endif

        for(int o = 0; o < rack.bus.blueprint.mc; ++o)
        {
            for(int i = 0; i < *rack.at(o)->descriptor->cc; ++i)
            {
                auto hash = rack.bus.blueprint.get_hash(core::map::parameter::type::cc, i);
                // rack.connect_pin_o(hash,  rack.bus.pin_c(hash));
                
                rack.bus.pin_c(0);
            }
            for(int i = 0; i < *rack.at(o)->descriptor->ic; ++i)
            {
                auto hash = rack.bus.blueprint.get_hash(core::map::parameter::type::ic, i);
                // rack.connect_pin_i(hash, bay->io[rack.bus.blueprint.get_index(hash)].com);
            }
            for(int i = 0; i < *rack.at(o)->descriptor->oc; ++i)
            {
                auto hash = rack.bus.blueprint.get_hash(core::map::parameter::type::oc, i);
                // rack.connect_pin_o(hash, bay->io[rack.bus.blueprint.get_index(hash)].data);
            }
        }

        #ifdef DEBUG
            std::cout<<"-- Bus connected...\n";
        #endif
    }

    spiro_t::spiro_t(const descriptor_t* d): rack(d)
    {
        connect_bus();
        #ifdef DEBUG
            std::cout<<"-- Core initialized...\n";
        #endif
    }



    spiro_t::~spiro_t()
    {
    }

    void spiro_t::note_on(uint8_t msb, uint8_t lsb)
    {

    }

    void spiro_t::note_off(uint8_t msb)
    {

    }

    void spiro_t::midi_message(uint8_t status, uint8_t msb, uint8_t lsb)
    {
        switch(status&0xF0) 
        {
            case 0x80: // Note off
                note_off(msb);
                break;

            case 0x90: // Note on
                if(lsb) note_on (msb, lsb);
                else    note_off(msb);
                break;

            case 0xB0: // Control change
                // controlChange(msb, lsb);
                break;

            case 0xD0: // Channel aftertouch
                // This maps the pressure value to a parabolic curve starting at
                // 0.0 (position 0) up to 1.61 (position 127).
                // pressure = 0.0001f * float(msb * msb);
                break;

            // Pitch bend
            case 0xE0:
                // The pitch wheel can shift the tone up or down by 2 semitones.
                // pitchBend = std::exp(-0.000014102f * float(msb + 128 * lsb - 8192));
                break;

            default:
                break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
