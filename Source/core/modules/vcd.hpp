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

#include "containers.hpp"
#include "utility.hpp"
#include "vcd.hpp"
#include "iospecs.hpp"
#include "node.hpp"
#include <atomic>
#include <map>
#include <memory>

namespace core {

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

