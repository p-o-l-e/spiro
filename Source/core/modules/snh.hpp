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
#include <string>
#include <cmath>
#include <map>
#include <atomic>
#include "constants.hpp"
#include "iospecs.hpp"
#include "node.hpp"

namespace core {


    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace snh
        {
            const int ctrls  { 1 };
            const int ins    { 3 };
            const int outs   { 1 };

            enum class ctrl { time };
            enum class in   { time, a, b };
            enum class out  { a };

            const std::string prefix {"snh_"};
            const std::map<interface::snh::ctrl, std::string> ctrl_postfix
            {
                { ctrl::time, "_ctrl_time" }
            };
            const std::map<interface::snh::in, std::string> in_postfix
            {
                { in::a, "_in_a" },
                { in::b, "_in_b" }
            };
            const std::map<interface::snh::out, std::string> out_postfix
            {
                { out::a, "_out_a" },
            };
        }
    }

    class snh_t: public module
    {
        private:
            static int idc;
            float t     = 0.0f;
            float value = 0.0f;
            float scale = 40.0f;

        public:
            int id;
            void process() override;
            void reset();

            snh_t();
           ~snh_t() {};
    };

}
