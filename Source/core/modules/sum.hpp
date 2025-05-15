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
#include "node.hpp"
#include "constants.hpp"

namespace core
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace sum
        {
            const int ctrls  { 0 };
            const int ins    { 2 };
            const int outs   { 2 };

            enum class ctrl { };
            enum class in   { a, b };
            enum class out  { a, b };

        }
    }

    using namespace interface::sum;

    class sigma: public module
    {
        private:
            static int idc;

        public:
            int id;
            void process() override
            {
                out[0].store(in[0]->load() + in[1]->load());
                out[1].store(out[0]);
            };


            sigma(): id(++idc)
            { 
                init(interface::sum::ctrls, interface::sum::ins, interface::sum::outs, module_type::sigma, id);
            };
           ~sigma() {};
};
}
