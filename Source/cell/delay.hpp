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
#include "utility.hpp"
#include "filters.hpp"
#include "iospecs.hpp"
#include "node.h"
#include <atomic>
#include <map>
#include <memory>

namespace cell {

    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace vcd
        {
            const int ctrls  { 2 };
            const int ins    { 6 };
            const int outs   { 4 };

            enum class ctrl { time, feed };
            enum class in   { time, feed, a, b, c, d };
            enum class out  { a, b, c, d };

            const std::string prefix {"vcd_"};
            const std::map<interface::vcd::ctrl, std::string> ctrl_postfix
            {
                { ctrl::time,   "_ctrl_time" },
                { ctrl::feed,   "_ctrl_feed" },
            };
            const std::map<interface::vcd::in, std::string> in_postfix
            {
                { in::time,     "_in_time" },
                { in::feed,     "_in_feed" },
                { in::a,        "_in_a" },
                { in::b,        "_in_b" },
                { in::c,        "_in_c" },
                { in::d,        "_in_d" }
            };
            const std::map<interface::vcd::out, std::string> out_postfix
            {
                { out::a, "_out_a" },
                { out::b, "_out_b" },
                { out::c, "_out_c" },
                { out::d, "_out_d" }
            };
        }
    }

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

