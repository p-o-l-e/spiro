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

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
namespace core {
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace lfo
        {
            const int ctrls  { 4 };
            const int ins    { 2 };
            const int outs   { 2 };

            enum class ctrl { octave, delta, amp, form };
            enum class in   { fm, am };
            enum class out  {  a,  b };

            const std::string prefix {"lfo_"};
            const std::map<interface::lfo::ctrl, std::string> ctrl_postfix
            {
                { ctrl::octave, "_ctrl_octave"   },
                { ctrl::delta,  "_ctrl_delta"    },
                { ctrl::amp,    "_ctrl_amp"      }
            };
            const std::map<interface::lfo::in, std::string> in_postfix
            {
                { in::fm, "_in_fm" },
                { in::am, "_in_am" }
            };
            const std::map<interface::lfo::out, std::string> out_postfix
            {
                { out::a, "_out_a" },
                { out::b, "_out_b" }
            };
        }
    }

class lfo_t: public module
{
    private:
        static int idc;                                 // ID counter
        float sine();
        float ramp();
        float saw();
        float square();
        float triangle();

        float (lfo_t::*form[5])() = 
        { 
            &lfo_t::sine,
            &lfo_t::square,
            &lfo_t::ramp,
            &lfo_t::saw,
            &lfo_t::triangle
        };

        float phase = 0.0f;                                 // Current phase


    public:
        const int id = 0;
        void process() override;
        void reset();
        lfo_t();
       ~lfo_t(){};
}; 


inline const char* wforms_lfo[]     = { "SINE", "SQUARE", "RAMP", "SAW", "TRIANGLE" };


};
