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
#include "constants.hpp"
#include "node.hpp"
#include "utility.hpp"

#include "iospecs.hpp"
#include "scales.h"
#include "vco_interface.hpp"
#include <iostream>

namespace core
{
    using namespace vco;
    int VCO::idc = 0;

    inline float getFrequency(int n) 
    {
        constexpr float A4 = 440.0f;
        return A4 * std::pow(2.0f, n / 12.0f);
    }

    void VCO::set_delta(const unsigned& voice)
    { 
        int n = note[voice] + 12 * ccv[ctl::octave]->load();

        if(n < chroma_n)[[likely]]
        {
            freq[voice]  = chromatic[n];
            delta[voice] = chromatic[n] * tao / settings::sample_rate; 
        }
        else[[unlikely]] 
        {
            auto f = getFrequency(n);
            freq[voice]  = f;
            delta[voice] = f * tao / settings::sample_rate;
        }
        set_fine(voice);
    }

    void VCO::set_fine(const unsigned& voice)
    {
        float range   = (freq[voice] * chromatic_ratio - freq[voice] / chromatic_ratio) * tao / settings::sample_rate;
        delta[voice] += (ccv[ctl::detune]->load() + icv[cvi::detune]->load() - 0.5f) * range * 2.0f;
    }

    inline float VCO::tomisawa(const int& voice)
    {
        float oa = cosf(phase[voice] + mem[0][voice]);
        mem[0][voice] = (oa + mem[0][voice]) * 0.5f;

        float pw =  icv[cvi::pwm] == &zero ? 
            (0.5f - ccv[ctl::pwm]->load()) * tao * 0.98f - pi :
            (0.5f - ccv[ctl::pwm]->load() + icv[cvi::pwm]->load());  

        float ob = cosf(phase[voice] + mem[1][voice] + pw);
        mem[1][voice] = (ob + mem[1][voice]) * 0.5f;
        return oa - ob;
    }

    inline float VCO::pulse(const int& voice)
    {
        float pw =  icv[cvi::pwm] == &zero ? 
            (0.5f - ccv[ctl::pwm]->load()) * 2.0f :
            (0.5f - ccv[ctl::pwm]->load() + icv[cvi::pwm]->load()) * 2.0f;    

        return fPulse(phase[voice], pw, 0.0001f);
    }

    inline float VCO::hexagon(const int& voice)
    {
        float pw = icv[cvi::pwm] == &zero ? 
            (0.5f - ccv[ctl::pwm]->load()) * pi :
            (0.5f - ccv[ctl::pwm]->load() + icv[cvi::pwm]->load()) * pi;

            float feed = (fTriangle(phase[voice], 0.001f) * fSquare(phase[voice] + pw, 0.001f))/pi + (pi * 0.5f - fabsf(pw)) * 0.25f;
        return feed * (pi - fabsf(pw));
    }

    void VCO::process() noexcept
    {
        if(ccv[ctl::mode]->load() < 1.0f) // Mono
        {
            set_delta(0);

            float fm = powf(ccv[ctl::fm]->load(), 3.0f);

            phase[0] += (delta[0] + icv[cvi::fm]->load() * fm);
            if(phase[0] >= pi) phase[0] -= tao;  

            float accu = (this->*form[(int)ccv[ctl::form]->load()])(0);

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
            accu *= gate[0];
            ocv[cvo::main].store(accu);
        }
        else // if(ccv[ctl::mode]->load() < 2.0f) // Poly
        {
            float accu = 0.0;
            float fm  = powf(ccv[ctl::fm]->load(),  3.0f) * icv[cvi::fm]->load();
            float pll = powf(ccv[ctl::pll]->load(), 3.0f);

            for(int i = 0; i < settings::poly; ++i)
            {
                if(gate[i])
                {
                    set_delta(i);
                    phase[i] += (delta[i] + fm);
                    if(phase[i] >= pi) phase[i] -= tao;  

                    auto current = (this->*form[(int)ccv[ctl::form]->load()])(i);

                    if(icv[cvi::pll] != &zero)
                    {
                        phase[i] += fPLL(current, icv[cvi::pll]->load()) * pll;
                    }
                    if(icv[cvi::am] != &zero)
                    {
                        current = xfade(current * icv[cvi::am]->load(), current, ccv[ctl::am]->load());
                    }
                    current *= ccv[ctl::amp]->load();
                    current *= *pin[i];
                    accu += current;
                                    
                }
            }
            ocv[cvo::main].store(accu);

        }
    }

    VCO::Mode VCO::mode() const noexcept
    {
        return static_cast<VCO::Mode>(ccv[ctl::mode]->load());
    }

    void VCO::reset()
    {
        for(int i = 0; i < settings::poly; ++i)
        {
            phase[i]    = 0;
            delta[i]    = 0;
            mem[0][i]   = 0.0f;
            mem[1][i]   = 0.0f;
            mem[2][i]   = 0.0f;
            note[i]     = 36;
            gate[i]     = false;
        }
    }

    VCO::VCO(): id(idc++), Module(idc, &vco::descriptor)
    {
        reset();
    }

    VCO::~VCO() = default;
 
}; // Namespace
