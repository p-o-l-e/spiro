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
#include "node.hpp"
#include "rtr_interface.hpp"

namespace core 
{
    using namespace rtr;
    int RTR::idc = 0;
    
    void RTR::process() noexcept
    {
        Point3D<float> a 
        { 
            (*icv[cvi::ax]) + 
            (*icv[cvi::bx]),
            (*icv[cvi::ay]) +
            (*icv[cvi::by]),
            (*icv[cvi::az]) +
            (*icv[cvi::bz]) 
        };

        float x = ccv[ctl::x]->load() + pi * (*icv[cvi::cvx]);
        float y = ccv[ctl::y]->load() + pi * (*icv[cvi::cvy]);
        float z = ccv[ctl::z]->load() + pi * (*icv[cvi::cvz]);

        q.from_euler(x, y, z);
        q.rotate_vector(a.x, a.y, a.z);

        ocv[cvo::ax] = a.x;
        ocv[cvo::ay] = a.y;
        ocv[cvo::az] = a.z;
        ocv[cvo::bx] = a.x;
        ocv[cvo::by] = a.y;
        ocv[cvo::bz] = a.z;

    }

    RTR::RTR(): Module(idc, &rtr::descriptor), id(idc++)
    { 
    };

}
