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
#include "constants.hpp"
#include "iospecs.hpp"
#include "node.hpp"
#include "snh_interface.hpp"

namespace core 
{
    using namespace snh;

int SNH::idc = 0;

void SNH::process() noexcept
{
    const float epsilon = 1.0f / settings::sample_rate;
    const float t_scale = scale * (float(settings::sample_rate) / 1000.0f);
    const float base = 1.0f - std::pow(ccv[ctl::time]->load(), 1.5f) * 0.995f;
    const float factor = icv[cvi::time] != &zero ? base * fabsf(icv[cvi::time]->load()) : base;

    if (t > t_scale + epsilon)
    {
        t = 0.0f;
        ocv[cvo::a].store(icv[cvi::a]->load() + icv[cvi::b]->load());
    }

    t += factor;
}

void SNH::reset()
{
    t     = 0.0f;
    value = 0.0f;
    scale = 40.0f;
}

SNH::SNH(): id(idc++), Module(idc, &snh::descriptor[0])
{
    reset();
}


}; // Namespace
