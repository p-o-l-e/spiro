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
#include <cmath>
#include "constants.hpp"
#include "iospecs.hpp"
#include "node.hpp"

#include <map>

namespace core {

    class LFO: public Module
    {
        private:
            static int idc;                                 // ID counter
            float sine();
            float ramp();
            float saw();
            float square();
            float triangle();

            float (LFO::*form[5])() = 
            { 
                &LFO::sine,
                &LFO::square,
                &LFO::ramp,
                &LFO::saw,
                &LFO::triangle
            };

            float phase = 0.0f;                                 // Current phase

        public:
            const int id = 0;
            void process() override;
            void reset();
            LFO();
           ~LFO(){};
    }; 

    inline const char* wforms_lfo[]     = { "SINE", "SQUARE", "RAMP", "SAW", "TRIANGLE" };

};
