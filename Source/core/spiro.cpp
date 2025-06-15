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
#include "modules/vco.hpp"
#include "setup/midi.h"
#include <cmath>
#include <iostream>


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
        for(int i = 0; i < 4; ++i) 
        {
            envelope[i]   = dynamic_cast<ENV*>(rack.at(map::module::type::env, i));
            oscillator[i] = dynamic_cast<VCO*>(rack.at(map::module::type::vco, i));

            envelope[i]->onStart = [=](int voice)
            {
                if(oscillator[i]->mode() == VCO::Mono) voice = 0;
                else oscillator[i]->note[voice] = note[voice];
                oscillator[i]->gate[voice] = true;
                envelope[i]->gate[voice]   = true;
                envelope[i]->hold[voice]   = true;
                active.emplace(voice);
            };

            envelope[i]->onFinish = [=](int voice) 
            {
                oscillator[i]->gate[voice] = false;
                envelope[i]->gate[voice]   = false;
                active.erase(voice);
            };
            for(int voice = 0; voice < settings::poly; ++voice)
            {
                oscillator[i]->pin[voice] = &envelope[i]->pin[voice];
            }
        }
    }

    void Spiro::noteOn(uint8_t msb, uint8_t lsb)
    {
        if(++voiceIterator >= settings::poly) voiceIterator = 0;
        for(auto voice: active) 
        {
            if(note[voice] == msb) 
            {
                std::cout<<"Duplicate found: "<<std::hex<<(int)msb<<" : "<<voice<<"\n";
                voiceIterator = voice;   // On duplicate
                break;
            }
        }

        note[voiceIterator] = msb;
        for(int i = 0; i < 4; ++i)
        {
            if(oscillator[i]->mode() == VCO::Mono) 
            {
                envelope[i]->start((float)lsb/(float)0x7F, 0);
                oscillator[i]->note[0] = msb;
            }
            else envelope[i]->start((float)lsb/(float)0x7F, voiceIterator);
        }

        std::cout<<"Note  ON: ";
        for(auto voice: active) std::cout<<voice<<" ";
        std::cout<<"\n";
    }

    void Spiro::noteOff(uint8_t msb)
    {
        std::cout<<"Note OFF: ";
        for(auto voice: active) std::cout<<voice<<" ";
        std::cout<<"\n";

        for(auto voice: active)
        {
            if(note[voice] == msb)
            {
                auto on_hold = voice;
                for(int i = 0; i < 4; ++i) 
                {
                    envelope[i]->hold[on_hold] = false;
                    if(oscillator[i]->mode() == VCO::Mono) oscillator[i]->gate[0] = false;
                }
                break;
            }
        }

    }

    void Spiro::midiMessage(uint8_t status, uint8_t msb, uint8_t lsb)
    {
        std::cout<<"MidiMessage: "<<std::hex<<(int)status<<" : "<<(int)msb<<" : "<<(int)lsb<<"\n";
        switch(status & 0xF0) 
        {
            case MidiMessage::NOTE_OFF:
                noteOff(msb);
                break;

            case MidiMessage::NOTE_ON:
                lsb ? noteOn (msb, lsb) : noteOff(msb);
                break;

            default:
                break;
        }
    }
}

