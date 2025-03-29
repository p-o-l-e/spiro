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

#include "containers.hpp"
#include "interface/vcd_interface.hpp"
#include "utility.hpp"
#include "filters.hpp"
#include "iospecs.hpp"
#include "node.h"
#include <atomic>
#include <map>
#include <memory>

namespace cell {

    class delay: public module
    {
        private:
            onepole psf;
            allpass apf;
            std::unique_ptr<float[]> data;
            float tmax;
            float eax;
            int   length;
            int   departed;

        public:
            void process() override;
            void reset();
            delay();
           ~delay();
    };


};

