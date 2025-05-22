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


#include "vco.hpp"
#include "../setup/constants.hpp"
#include "../utility/utility.hpp"

#include "../setup/iospecs.hpp"
#include "../setup/scales.h"

namespace core
{
    using namespace vco;
    int vco_t::idc = 0;

    void vco_t::set_delta(const unsigned& voice)
    { 
        int n = note[voice] + 12 * roundf(ccv[ctl::octave]->load() * 10.0f);
        freq[voice]  = chromatic[n];
        delta[voice] = chromatic[n] * tao / settings::sample_rate; 
        set_fine(voice);
    }

    void vco_t::set_fine(const unsigned& voice)
    {
        float range   = (freq[voice] * chromatic_ratio - freq[voice] / chromatic_ratio) * tao / settings::sample_rate;
        delta[voice] += (ccv[ctl::detune]->load() + icv[cvi::detune]->load() - 0.5f) * range * 2.0f;
    }

    inline float vco_t::tomisawa(const int& voice)
    {
        float oa = cosf(phase[voice] + eax[0][voice]);
        eax[0][voice] = (oa + eax[0][voice]) * 0.5f;

        float pw =  icv[cvi::pwm] == &zero ? 
            (0.5f - ccv[ctl::pwm]->load()) * tao * 0.98f - pi :
            (0.5f - ccv[ctl::pwm]->load() + icv[cvi::pwm]->load());  

        float ob = cosf(phase[voice] + eax[1][voice] + pw);
        eax[1][voice] = (ob + eax[1][voice]) * 0.5f;
        return oa - ob;
    }

    inline float vco_t::pulse(const int& voice)
    {
        float pw =  icv[cvi::pwm] == &zero ? 
            (0.5f - ccv[ctl::pwm]->load()) * 2.0f :
            (0.5f - ccv[ctl::pwm]->load() + icv[cvi::pwm]->load()) * 2.0f;    

        return fPulse(phase[voice], pw, 0.0001f);
    }

    inline float vco_t::hexagon(const int& voice)
    {
        float pw = icv[cvi::pwm] == &zero ? 
            (0.5f - ccv[ctl::pwm]->load()) * pi :
            (0.5f - ccv[ctl::pwm]->load() + icv[cvi::pwm]->load()) * pi;

            float feed = (fTriangle(phase[voice], 0.001f) * fSquare(phase[voice] + pw, 0.001f))/pi + (pi * 0.5f - fabsf(pw)) * 0.25f;
        return feed * (pi - fabsf(pw));
    }


    void vco_t::process()
    {
        float accu = 0.0f;
        
        if(ccv[ctl::freerun]->load() > 0.5f)
        {
            set_delta(0);

            float fm = powf(ccv[ctl::fm]->load(), 3.0f);

            phase[0] += (delta[0] + icv[cvi::fm]->load() * fm);
            if(phase[0] >= pi) phase[0] -= tao;  

            accu = (this->*form[(int)ccv[ctl::form]->load()])(0);


            if(icv[cvi::pll] != &zero)
            {
                float f = powf(ccv[ctl::pll]->load(), 3.0f);
                phase[0] += fPLL(accu, icv[cvi::pll]->load()) * f;
            }
            if(icv[cvi::am] != &zero)
            {
                accu = xfade(accu * icv[cvi::am]->load(), accu, ccv[ctl::am]->load());
            }
            accu *= ccv[ctl::amp]->load();
            ocv[cvo::main].store(accu);
        }
    }


    

    void vco_t::reset()
    {
        init(id, &vco::descriptor);
        for(int i = 0; i < settings::poly; ++i)
        {
            phase[i]    = 0;
            delta[i]    = 0;
            eax[0][i]   = 0.0f;
            eax[1][i]   = 0.0f;
            eax[2][i]   = 0.0f;
            note[i]     = 36;
        }
    }


    vco_t::vco_t()
    {
        id = ++idc;
        reset();
    }

    vco_t::~vco_t() = default;


 
}; // Namespace
