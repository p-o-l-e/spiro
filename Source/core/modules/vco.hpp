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

#pragma once

#include <cmath>
#include <cstdint>
#include <complex>
#include <set>
#include "node.hpp"

namespace core
{
    namespace settings
    {
        const int poly { 32 };
    }

    /**************************************************************************************************************************
    * 
    *  VCO
    * 
    **************************************************************************************************************************/
    class VCO: public Module<float>
    {   
        private:
            float phase[settings::poly];                // Current phase
            float delta[settings::poly];                // Phase increment
            float mem[3][settings::poly];               // Feedback memory
            float tomisawa(const int&);
            float pulse(const int&);
            float hexagon(const int&);
            float (VCO::*form[3])(const int&) = 
            { 
                &VCO::tomisawa, 
                &VCO::pulse, 
                &VCO::hexagon 
            };
            static int idc;                             // ID counter
  
        public:
            enum Mode { Mono, Freerun, Poly };
            Mode mode() const noexcept;
            const float* pin[settings::poly];
            int id;                                     // Unique VCO id
            float freq[settings::poly];                 // Frequency
            uint8_t note[settings::poly];               // Triggered note
            bool gate[settings::poly];
            std::set<int> active{}; 
            void process() noexcept override;
            void set_delta(const unsigned&);
            void set_fine(const unsigned&);
            
            void reset();
            VCO();
           ~VCO();
    };



    /**************************************************************************************************************************
    *
    *  Waveforms
    *
    **************************************************************************************************************************/
    inline float fPLL(const float& carrier, const float& modulator)
    {
        std::complex<float> fI = exp( std::complex<float> (0.0, 1.0) * carrier);
        std::complex<float> fO = exp( std::complex<float> (0.0, 1.0) * modulator);
        float fE = std::arg( fI * std::conj(fO) );
        return fE;
    }

    constexpr float fTriangle(const float& x, const float& f) noexcept
    {
        return acosf((1.0f - f) * cosf(x)) - pi * 0.5f; 
    }

    constexpr float fSquare(const float& x, const float& f) noexcept
    {
        return atanf(cos(x)/f);
    }

    constexpr float fRamp(const float& x) noexcept
    {
        return atanf(tanf(x))/(pi * 0.5f);
    }

    constexpr float fPulse(const float& phase, const float& pw, const float& f) noexcept
    {
        return atanf((cosf(phase) + pw)/f) / (pi * 0.5f);
    }


}; // Namespace

