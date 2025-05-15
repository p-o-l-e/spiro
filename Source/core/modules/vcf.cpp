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

#include "vcf.hpp"

namespace core 
{
    using namespace interface::vcf;
    int vcf_t::idc = 0;

    vcf_t::vcf_t(): id(++idc) 
    { 
        init(cc, ic, oc, module_type::vcf, id);
        for(int i = 0; i < interface::vcf::ic; ++i) in[i] = &zero;
        for(int i = 0; i < interface::vcf::cc; ++i) ctrl[i] = &zero;

        reset(); 
    }

    void vcf_t::reset()
    {
        iceq[0]  = 0.0f;
        iceq[1]  = 0.0f;

        g = 0.0f;
        k = 0.0f;
        a = 0.0f;
        b = 0.0f;
    }

    void vcf_t::process()
    {
        float cutoff = ctrl[ctl::cutoff]->load() + in[cvi::cutoff]->load();
        if      (cutoff < 0.0f) cutoff = 0.0f;
        else if (cutoff > 1.0f) cutoff = 1.0f;

        float Q = ctrl[ctl::Q]->load() + in[ctl::Q]->load();
        if      (Q < 0.0f) Q = 0.0f;
        else if (Q > 1.0f) Q = 1.0f;

        g = powf(cutoff, 5) * 0.6f + 0.01f;
        k = (1.0f - Q * Q * 0.9f) + 0.01f;
        a = 1.0f / (1.0f + g * (g + k));
        b = g * a;

        float is = in[cvi::a]->load() + in[cvi::b]->load() + in[cvi::c]->load();
        float va = a * iceq[0] + b * (is - iceq[1]);
        float vb = iceq[1] + g * va;
        iceq[0]  = 2.0f * va - iceq[0];
        iceq[1]  = 2.0f * vb - iceq[1];

        out[cvo::lp].store(vb);
        out[cvo::bp].store(va);
        out[cvo::hp].store(is - k * va - vb);
    }
};
