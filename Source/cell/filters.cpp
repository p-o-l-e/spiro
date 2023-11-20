/*****************************************************************************************************************************
* ISC License
*
* Copyright (c) 2023 POLE
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
* REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
* AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
* INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
* LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
* OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE.
*
* Based on: https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
******************************************************************************************************************************/

#include "filters.hpp"

namespace cell {

    using namespace interface::vcf;

    vcf_t::vcf_t() 
    { 
        init(interface::vcf::ctrls, interface::vcf::ins, interface::vcf::outs);
        for(int i = 0; i < interface::vcf::ins; ++i) in[i] = &zero;
        for(int i = 0; i < interface::vcf::ctrls; ++i) ctrl[i] = &zero;

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
        float cutoff = ctrl[static_cast<int>(ctrl::cutoff)]->load() + in[static_cast<int>(in::cutoff)]->load();
        if      (cutoff < 0.0f) cutoff = 0.0f;
        else if (cutoff > 1.0f) cutoff = 1.0f;

        float Q = ctrl[static_cast<int>(ctrl::Q)]->load() + in[static_cast<int>(ctrl::Q)]->load();
        if      (Q < 0.0f) Q = 0.0f;
        else if (Q > 1.0f) Q = 1.0f;

        g = powf(cutoff, 5) * 0.6f + 0.01f;
        k = (1.0f - Q * Q * 0.9f) + 0.01f;
        a = 1.0f / (1.0f + g * (g + k));
        b = g * a;

        float is = in[static_cast<int>(in::a)]->load() + in[static_cast<int>(in::b)]->load() + in[static_cast<int>(in::c)]->load();
        float va = a * iceq[0] + b * (is - iceq[1]);
        float vb = iceq[1] + g * va;
        iceq[0]  = 2.0f * va - iceq[0];
        iceq[1]  = 2.0f * vb - iceq[1];

        out[static_cast<int>(out::lp)].store(vb);
        out[static_cast<int>(out::bp)].store(va);
        out[static_cast<int>(out::hp)].store(is - k * va - vb);
    }
};
