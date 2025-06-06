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
#include "modules/interface/descriptor.hxx"
#include "setup/midi.h"


namespace core
{
    void Spiro::process() noexcept
    {
        for(int o = 0; o < grid->sectors; ++o) rack.process(o);

        out[stereo::l].store(mixer->ocv[stereo::l].load());
        out[stereo::r].store(mixer->ocv[stereo::r].load());
    }

    Spiro::Spiro(const Grid* grid): grid(grid), rack(grid)
    {
        mixer = rack.at(map::module::type::mix, 0);
        for(int i = 0; i < 4; ++i) envelope[i] = dynamic_cast<ENV*>(rack.at(map::module::type::env, i));
    }

    void Spiro::note_on(uint8_t msb, uint8_t lsb)
    {
        for(int i = 0; i < 4; ++i) envelope[i]->start();
    }

    void Spiro::note_off(uint8_t msb)
    {

    }

    void Spiro::midi_message(uint8_t status, uint8_t msb, uint8_t lsb)
    {
        switch(status & 0xF0) 
        {
            case MidiMessage::NOTE_OFF:
                note_off(msb);
                break;

            case MidiMessage::NOTE_ON:
                lsb ? note_on (msb, lsb) : note_off(msb);
                break;

            default:
                break;
        }
    }
}

