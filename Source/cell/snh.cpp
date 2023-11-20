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

void snh_t::process()
{
    if(in[static_cast<int>(interface::snh::in::time)] == &zero)
    {
        if (t > (scale * float(settings::sample_rate) / 1000.0f))
        {
            t = 0.0f;
            out[static_cast<int>(interface::snh::out::a)].store(in[static_cast<int>(interface::snh::in::a)]->load() 
                                                              + in[static_cast<int>(interface::snh::in::b)]->load());
        }
        t += (1.0f - ctrl[static_cast<int>(interface::snh::ctrl::time)]->load() * 0.99f);
    }
    else
    {
        if (t > (scale * float(settings::sample_rate) / 1000.0f))
        {
            t = 0.0f;
            out[static_cast<int>(interface::snh::out::a)].store(in[static_cast<int>(interface::snh::in::a)]->load() 
                                                              + in[static_cast<int>(interface::snh::in::b)]->load());
        }
        t += ((1.0f - ctrl[static_cast<int>(interface::snh::ctrl::time)]->load() * 0.99f)
                    * fabsf(in[static_cast<int>(interface::snh::in::time)]->load()));
    }
}

void snh_t::reset()
{
    t     = 0.0f;
    value = 0.0f;
    scale = 40.0f;
}

snh_t::snh_t()
{
    init(interface::snh::ctrls, interface::snh::ins, interface::snh::outs);
    reset();
}


}; // Namespace
