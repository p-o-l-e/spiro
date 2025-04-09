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

#include "rtr.hpp"

namespace core 
{
    using namespace interface::rtr;
    
    void rtr_t::process()
    {
        point3d<float> a 
        { 
            icv[cvi::ax]->load() + 
            icv[cvi::bx]->load(),
            icv[cvi::ay]->load() +
            icv[cvi::by]->load(),
            icv[cvi::az]->load() +
            icv[cvi::bz]->load() 
        };

        float x = ccv[ctl::x]->load() + pi * icv[cvi::cvx]->load();
        float y = ccv[ctl::y]->load() + pi * icv[cvi::cvy]->load();
        float z = ccv[ctl::z]->load() + pi * icv[cvi::cvz]->load();

        q.from_euler(x, y, z);
        q.rotate_vector(a.x, a.y, a.z);

        ocv[cvo::ax].store(a.x);
        ocv[cvo::ay].store(a.y);
        ocv[cvo::az].store(a.z);
        ocv[cvo::bx].store(a.x);
        ocv[cvo::by].store(a.y);
        ocv[cvo::bz].store(a.z);

    }

    rtr_t::rtr_t(): id(++idc)
    { 
        init(id, &interface::rtr::descriptor);
    };

    int rtr_t::idc = 0;
}
