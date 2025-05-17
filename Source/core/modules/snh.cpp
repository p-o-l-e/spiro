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

#include "snh.hpp"

namespace core 
{
    using namespace interface::snh;
    int snh_t::idc = 0;

    void snh_t::process()
    {
        if(icv[cvi::time] == &zero)
        {
            if (t > (scale * float(settings::sample_rate) / 1000.0f))
            {
                t = 0.0f;
                ocv[cvo::a].store(icv[cvi::a]->load() + icv[cvi::b]->load());
            }
            t += (1.0f - ccv[ctl::time]->load() * 0.99f);
        }
        else
        {
            if (t > (scale * float(settings::sample_rate) / 1000.0f))
            {
                t = 0.0f;
                ocv[cvo::a].store(icv[cvi::a]->load() + icv[cvi::b]->load());
            }
            t += ((1.0f - ccv[ctl::time]->load() * 0.99f) * fabsf(icv[cvi::time]->load()));
        }
    }

    void snh_t::reset()
    {
        t     = 0.0f;
        value = 0.0f;
        scale = 40.0f;
    }

    snh_t::snh_t(): id(++idc)
    {
        init(cc, ic, oc, map::module::snh, id);
        reset();
    }


}; // Namespace
