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
#include "vca.hpp"

namespace core 
{
    using namespace interface::vca;
    int vca_t::idc = 0;

            void vca_t::process()
            {
                float v = ctrl[ctl::amp]->load() + in[ctl::amp]->load();
                if      (v < 0.0f) v = 0.0f;
                else if (v > 1.0f) v = 1.0f;
                float o = (in[cvi::a]->load() + in[cvi::b]->load()) * v; 
                out[cvo::a].store(o);
                out[cvo::b].store(o);

            };

            vca_t::vca_t(): id(++idc)
            {  
                init(cc, ic, oc, module_type::vca, 0);
            };
            vca_t::~vca_t() {};
    
}
