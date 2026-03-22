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
#include "setup/midi.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>


namespace core
{
    void Spiro::process() noexcept
    {
        for(const auto o: whitelist) rack.process(o);
        out[stereo::l].store(mixer->ocv[stereo::l].load());
        out[stereo::r].store(mixer->ocv[stereo::r].load());
    }

    Spiro::Spiro(const Grid* grid): grid(grid), rack(grid), oscn(grid->count(map::module::vco))
    {
        envelope    = new ENV*[oscn];
        oscillator  = new VCO*[oscn];
        mixer = rack.at(map::module::type::mix, 0);
        com   = rack.at(map::module::type::com, 0);
        blacklist.emplace(rack.index(map::module::type::mix, 0));
        whitelist.emplace(rack.index(map::module::type::mix, 0));

        activeOutputs = new int[grid->sectors];
        for(int i = 0; i < grid->sectors; ++i) activeOutputs[i] = 0;

        for(int i = 0; i < oscn; ++i) 
        {
            blacklist.emplace(rack.index(map::module::type::env, i));
            whitelist.emplace(rack.index(map::module::type::env, i));
        

            envelope[i]   = dynamic_cast<ENV*>(rack.at(map::module::type::env, i));
            oscillator[i] = dynamic_cast<VCO*>(rack.at(map::module::type::vco, i));
            
            envelope[i]->onStart = [this, i](int voice)
            {
                oscillator[i]->note[voice] = note[voice];
                oscillator[i]->gate[voice] = true;
                envelope[i]->gate[voice]   = true;
                envelope[i]->hold[voice]   = true;
                active.emplace(voice);
            };

            envelope[i]->onFinish = [this, i](int voice) 
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

    Spiro::~Spiro()
    {
        delete[] activeOutputs;
        delete[] oscillator;
        delete[] envelope;
    }

    void Spiro::noteOn(uint8_t msb, uint8_t lsb)
    {
        if(++voiceIndex >= settings::poly) voiceIndex = 1;
        for(auto voice: active) 
        {
            if(note[voice] == msb) 
            {
                voiceIndex = voice;   // On duplicate
                break;
            }
        }

        note[voiceIndex] = msb;
        for(int i = 0; i < oscn; ++i)
        {
            if(oscillator[i]->mode() != VCO::Poly) 
            {
                envelope[i]->start((float)lsb/(float)0x7F, VCO::Mono);
                oscillator[i]->note[VCO::Mono] = msb;
                note[VCO::Mono] = msb;
            }
            else 
            {
                envelope[i]->start((float)lsb/(float)0x7F, voiceIndex);
            }
        }
    }

    void Spiro::noteOff(uint8_t msb)
    {
        std::vector<int> to_process(active.begin(), active.end());
        for (auto voice : to_process) 
        {
            if (note[voice] == msb) 
            {
                for (int i = 0; i < oscn; ++i) 
                {
                    envelope[i]->hold[voice] = false;
                    if (oscillator[i]->mode() == VCO::Freerun) 
                    {
                        envelope[i]->jump(ENV::Release, voice);
                        envelope[i]->next_stage(voice);
                    }
                }
            }
        }    
    }

    void Spiro::midiMessage(uint8_t status, uint8_t msb, uint8_t lsb)
    {
        switch(status & 0xF0) 
        {
            case MidiMessage::NOTE_ON:
                lsb ? noteOn (msb, lsb) : noteOff(msb);
                break;

            case MidiMessage::NOTE_OFF:
                noteOff(msb);
                break;            
            
            case MidiMessage::PITCH_BEND:
                com->ocv[com::cvo::p_wheel].store(float((lsb | (msb << 7)) - 8192.0f) / 8192.0f);
                break;

            case MidiMessage::CONTROL_CHANGE:
                if(msb == 1)  // Mod Wheel (CC #1)
                {
                    com->ocv[com::cvo::m_wheel].store(lsb / 127.0f * 2.0f - 1.0f);
                }
                break;

            default:
                break;
        }
    }

    void Spiro::addConnection(int moduleIndex) noexcept
    {
        ++activeOutputs[moduleIndex];
        auto max = grid->sector[moduleIndex].descriptor->cv[map::cv::o];
        if(activeOutputs[moduleIndex] > *max)[[unlikely]] activeOutputs[moduleIndex] = *max;
        whitelist.emplace(moduleIndex);
    }
    
    void Spiro::removeConnection(int moduleIndex) noexcept
    {
        if(--activeOutputs[moduleIndex] < 0)[[unlikely]] activeOutputs[moduleIndex] = 0;
        if(!blacklist.contains(moduleIndex))[[likely]]
        {
            if(!activeOutputs[moduleIndex]) 
                whitelist.erase(moduleIndex);
        }
    }
}

