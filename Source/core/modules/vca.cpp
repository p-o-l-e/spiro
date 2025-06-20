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
#include "node.hpp"
#include "vca_interface.hpp"

namespace core 
{
    using namespace vca;

    int VCA::idc = 0;
    
    inline float sigmoid_amp(float x) 
    {
        return 0.5f * (std::tanh(2.0f * x) + 1.0f);
    }

    void VCA::process() noexcept
    {
        float v = 0.0f;
        if(icv[cvi::amp] == &zero)
        {
            v = ccv[ctl::amp]->load();
        }
        else 
        {
            v = ccv[ctl::amp]->load() * sigmoid_amp(icv[cvi::amp]->load());
        }
        float o = (icv[cvi::a]->load() + icv[cvi::b]->load()) * v; 
        ocv[cvo::a].store(o);
        ocv[cvo::b].store(o);
    };

    VCA::VCA(): id(idc++), Module(idc, &vca::descriptor[0])
    {  
    };
}
