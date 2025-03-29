/*****************************************************************************************************************************
* Copyright (c) 2022-2023 POLE
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

#include "snh.h"

namespace cell {

int snh_t::idc = 0;

void snh_t::process()
{
    if(in[interface::snh::cvi::time] == &zero)
    {
        if (t > (scale * float(settings::sample_rate) / 1000.0f))
        {
            t = 0.0f;
            out[interface::snh::cvo::a].store(in[interface::snh::cvi::a]->load() + in[interface::snh::cvi::b]->load());
        }
        t += (1.0f - ctrl[interface::snh::ctl::time]->load() * 0.99f);
    }
    else
    {
        if (t > (scale * float(settings::sample_rate) / 1000.0f))
        {
            t = 0.0f;
            out[interface::snh::cvo::a].store(in[interface::snh::cvi::a]->load() + in[interface::snh::cvi::b]->load());
        }
        t += ((1.0f - ctrl[interface::snh::ctl::time]->load() * 0.99f) * fabsf(in[interface::snh::cvi::time]->load()));
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
    init(id, &interface::snh::descriptor[0]);
    reset();
}


}; // Namespace
