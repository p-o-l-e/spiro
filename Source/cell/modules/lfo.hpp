/*****************************************************************************************************************************
* ISC License
*
* Copyright (c) 2023 POLE
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
* REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
* AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
* INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
* OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
******************************************************************************************************************************/

#pragma once
#include <cmath>
#include "constants.hpp"
#include "interface/lfo_interface.hpp"
#include "iospecs.hpp"
#include "node.h"

#include <map>

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
namespace cell 
{
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
