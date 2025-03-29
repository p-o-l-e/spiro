#/*****************************************************************************************************************************
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
*
* Based on: https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
******************************************************************************************************************************/
#pragma once

#include "iospecs.hpp"
#include "constants.hpp"
#include "interface/vcf_interface.hpp"
#include "node.h"
#include <cmath>
#include <atomic>


namespace cell {

    class vcf_t: public module
    {
        private:
            static int idc;
            float iceq[2];
            float g;
            float k;
            float a;
            float b;

        public:
            const int id;
            void process() override;
            void reset();
            vcf_t();
           ~vcf_t() {};
    };

};
