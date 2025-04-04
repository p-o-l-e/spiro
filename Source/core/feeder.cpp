/*****************************************************************************************************************************
* Copyright (c) 2022-2023 POLE
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


#include "feeder.hpp"

namespace cell {

void feeder::reset()
{
    standby  = false;
}

feeder::feeder()
{
    reset();
}

feeder::~feeder()
{
}

void feeder::note_on(uint8_t msb, uint8_t lsb)
{
    for(int i = 0; i < settings::osc_n; ++i)
    {
        for(int j = 0; j < settings::poly; ++j)
        {
            if(renderer.vco[i].gate[j] == false) // Voice is free
            {
                renderer.vco[i].gate[j] = true;
                renderer.vco[i].note[j] = msb;
                renderer.vco[i].velo[j] = float(lsb) / 127.0f;
                renderer.vco[i].set_delta(j);
                renderer.vco[i].env[j].start();
                break;
            }
        }
    }
}

void feeder::note_off(uint8_t msb)
{
    for(int i = 0; i < settings::osc_n; ++i)
    {
        for(int j = 0; j < settings::poly; ++j)
        {
            if(renderer.vco[i].note[j] == msb)
            {
                renderer.vco[i].env[j].jump(SUS);
                renderer.vco[i].release[j] = true;
            }
        }
    }
}

void feeder::midi_message(uint8_t status, uint8_t msb, uint8_t lsb)
{
    switch(status&0xF0) 
    {
        // Note off
        case 0x80:
            note_off(msb);
            break;

        // Note on
        case 0x90:

            if(lsb) note_on (msb, lsb);
            else    note_off(msb);
            break;

        // Control change
        case 0xB0:
            // controlChange(msb, lsb);
            break;

        // Channel aftertouch
        case 0xD0:
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
