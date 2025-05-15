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
#include "vco_interface.hpp"
#include "constants.hpp"
#include "utility.hpp"
// #include "delay.hpp"
#include "env.hpp"
#include "iospecs.hpp"
#include "scales.h"
#include "node.hpp"

#include <cstdint>
#include <iostream>
#include <complex>
#include <string>
#include <map>


namespace core 
{
    namespace settings
    {
        const int poly { 8 };
    }

    
    /**************************************************************************************************************************
    * 
    *  VCO
    * 
    **************************************************************************************************************************/
    class oscillator: public module
    {   
        private:
            float   phase   [settings::poly];               // Current phase
            float   delta   [settings::poly];               // Phase increment
            float   eax[3]  [settings::poly];               // Feedback memory

            float tomisawa(const int&);
            float pulse(const int&);
            float hexagon(const int&);
            float (oscillator::*form[3])(const int&) = 
            { 
                &oscillator::tomisawa, 
                &oscillator::pulse, 
                &oscillator::hexagon 
            };
            static int idc;                                 // ID counter
  
        public:
            int id;                                         // Unique oscillator id
            envelope env    [settings::poly];
        
            float   freq    [settings::poly];               // Frequency
            bool    gate    [settings::poly];               // Gate ON ?
            bool    release [settings::poly];               // Released ?
            uint8_t note    [settings::poly];               // Triggered note
            float   velo    [settings::poly];               // Velocity

            void process() override;
        
            void set_delta(const unsigned&);
            void set_fine(const unsigned&);

            void reset();
            oscillator();
           ~oscillator();
    };


    inline const char* wforms_vco[] = { "SINE", "SQUARE", "HEXAGON" };



    /**************************************************************************************************************************
    * 
    *  Map generators
    * 
    **************************************************************************************************************************/
    std::map<std::string, std::atomic<float>*> vco_create_controls_map(oscillator* o);
    std::map<std::string, std::atomic<float>*> vco_create_inputs_map(oscillator* o);
    std::map<std::string, std::atomic<float>*> vco_create_outputs_map(oscillator* o);
    


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

