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
    int RTR::idc = 0;

    void RTR::process()
    {
        Point3D<float> lcr
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
        q.rotate_vector(lcr.x, lcr.y, lcr.z);

        ocv[cvo::ax].store(lcr.x);
        ocv[cvo::ay].store(lcr.y);
        ocv[cvo::az].store(lcr.z);
        
        ocv[cvo::bx].store(lcr.x);
        ocv[cvo::by].store(lcr.y);
        ocv[cvo::bz].store(lcr.z);

    }

    RTR::RTR(): id(++idc)
    { 
        init(cc, ic, oc, map::module::rtr, id);
    };
    RTR::~RTR() {};

}
