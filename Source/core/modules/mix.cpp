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
#include "mix.hpp"
#include "mix_interface.hpp"
#include "node.hpp"
#include "utility.hpp"
#include "primitives.hpp"

namespace core 
{
    using namespace mix;
    int MIX::idc = 0;

    void MIX::process() noexcept
    {
        Point3D<float> a 
        { 
            icv[cvi::l]->load(), 
            icv[cvi::c]->load(), 
            icv[cvi::r]->load() 
        };
        float lc = ccv[ctl::alpha]->load() + icv[cvi::alpha]->load();
        float cr = ccv[ctl::theta]->load() + icv[cvi::theta]->load();
        Point2D<float> lr = c3c2(a, lc, cr);
        ocv[cvo::l].store(lr.x * ccv[ctl::amp]->load());
        ocv[cvo::r].store(lr.y * ccv[ctl::amp]->load());
    }

    MIX::MIX(): id(idc++), Module(idc, &mix::descriptor)
    {
    }
}
