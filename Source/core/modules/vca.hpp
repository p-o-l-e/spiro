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

#include "constants.hpp"
#include "node.hpp"

namespace core
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace vca
        {
            const int ctrls  { 1 };
            const int ins    { 3 };
            const int outs   { 2 };

            enum class ctrl { amp };
            enum class in   { a, b, amp };
            enum class out  { a, b };

        }
    }


    class vca_t: public module
    {
        private:
            static int idc;

        public:
            const int id = 0;
            void process() override
            {
                float v = ctrl[static_cast<int>(interface::vca::ctrl::amp)]->load() + in[static_cast<int>(interface::vca::ctrl::amp)]->load();
                if      (v < 0.0f) v = 0.0f;
                else if (v > 1.0f) v = 1.0f;
                float o = (in[static_cast<int>(interface::vca::in::a)]->load() + in[static_cast<int>(interface::vca::in::b)]->load()) * v; 
                out[static_cast<int>(interface::vca::out::a)].store(o);
                out[static_cast<int>(interface::vca::out::b)].store(o);

            };

            vca_t(): id(++idc)
            {  
                init(interface::vca::ctrls, interface::vca::ins, interface::vca::outs, module_type::vca, 0);
            };
           ~vca_t() {};
    };

}
