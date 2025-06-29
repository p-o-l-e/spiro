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
#include "modmatrix.hpp"
#include "modules/env.hpp"
#include "modules/node.hpp"
#include "modules/vco.hpp"
#include "rack.hpp"
#include "setup/midi.h"
#include <atomic>
#include <set>
#include <cstdint>
#include <functional>


namespace core 
{
    class Spiro
    { 
        public:
            struct stereo { enum { l, r }; };
        private:
            uint8_t note[settings::poly];
            int voiceIterator = 1;
            std::set<int> active;                   // Active voices
            std::set<int> whitelist;                // Active modules
            std::set<int> blacklist;                // Always ON modules
            int* activeOutputs;
            Module<float>* mixer; 
            Module<float>* com;
            ENV* envelope[4];
            VCO* oscillator[4];
            void noteOn (uint8_t, uint8_t);
            void noteOff(uint8_t);
            void resetVoice(int);

        public:
            const Grid* grid;
            Rack rack;
            Patchbay* bay = nullptr;
            std::atomic<float> out[2];                       // LR Output
            void midiMessage(uint8_t, uint8_t, uint8_t);
            void process() noexcept;
            void addConnection(int pos) noexcept;
            void removeConnection(int pos) noexcept;
            Spiro(const Grid*);
           ~Spiro();
    };
};
