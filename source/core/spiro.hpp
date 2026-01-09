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
#include "modules/node.hpp"
#include "modules/vco.hpp"
#include "modules/env.hpp"
#include "rack.hpp"
#include <atomic>
#include <unordered_set>
#include <flat_set>
#include <cstdint>

namespace core 
{
    class Spiro
    { 
        public:
            struct stereo { enum { l, r }; }; 
            const Grid* grid;
            Rack rack;

        private:
            uint8_t note[settings::poly];
            int voiceIndex = 1;
            std::unordered_set<int> active;             // Active voices
            std::flat_set<int> whitelist;               // Active modules
            std::flat_set<int> blacklist;               // Always ON modules
            int* activeOutputs;
            Module<float>* mixer; 
            Module<float>* com;
            const int oscn;
            ENV** envelope;
            VCO** oscillator;
            void noteOn (uint8_t, uint8_t);
            void noteOff(uint8_t);
            void resetVoice(int);

        public:
            Patchbay* bay = nullptr;
            std::atomic<float> out[2];                       // LR Output
            void midiMessage(uint8_t, uint8_t, uint8_t);
            void process() noexcept;
            void addConnection(int pos) noexcept;
            void removeConnection(int pos) noexcept;
            Spiro operator=(Spiro) = delete;
            Spiro(const Spiro&) = delete;
            Spiro(const Grid*);
           ~Spiro();
    };
};
