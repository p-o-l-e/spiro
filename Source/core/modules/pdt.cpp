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
#include "pdt.hpp"
#include "interface/pdt_interface.hpp"
#include "node.hpp"
#include "pdt_interface.hpp"

namespace core {
    
    int PDT::idc = 0;

    void PDT::process() noexcept
    {
        bool  o = false;
        float s = 1.0f;
        for(int i = 0; i < pdt::ic; ++i)
        {
            if(icv[i] == &zero) continue;
            else 
            {
                s *= icv[i]->load();
                o = true;
            }
        }
        ocv[0].store( o ? s : 0.0f);
    };

    PDT::PDT(): id(idc++), Module(idc, &pdt::descriptor)
    { 
    };
}
