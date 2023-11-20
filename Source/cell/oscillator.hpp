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

#pragma once

#include <cmath>
#include "constants.hpp"
#include "utility.hpp"
#include "delay.hpp"
#include "envelopes.hpp"
#include "iospecs.hpp"
#include "scales.h"
#include "node.h"

#include <iostream>
#include <complex>
#include <string>
#include <map>


namespace cell 
{
    namespace settings
    {
        const int poly { 8 };
    }

    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace vco 
        {
            const int ctrls  { 9 };
            const int ins    { 5 };
            const int outs   { 1 };

            enum class ctrl { octave, detune, pll, pwm, fm, am, amp, form, freerun  };
            enum class in   {         detune, pll, pwm, fm, am                      };
            enum class out  {         main                                          };

            const std::string prefix {"vco_"};

            const std::map<interface::vco::ctrl, std::string> ctrl_postfix
            {
                { ctrl::octave, "_ctrl_octave"   },
                { ctrl::detune, "_ctrl_detune"   },
                { ctrl::pll,    "_ctrl_pll"      },
                { ctrl::pwm,    "_ctrl_pwm"      },
                { ctrl::fm,     "_ctrl_fm"       },
                { ctrl::am,     "_ctrl_am"       },
                { ctrl::amp,    "_ctrl_amp"      }
            };

            const std::map<interface::vco::in, std::string> in_postfix
            {
                { in::detune,   "_in_detune" },
                { in::pll,      "_in_pll"    },
                { in::pwm,      "_in_pwm"    },
                { in::fm,       "_in_fm"     },
                { in::am,       "_in_am"     }
            };

            const std::map<interface::vco::out, std::string> out_postfix
            {
                { out::main,    "_out_main"  }
            };
            
            const std::map<interface::vco::ctrl, point<float>> ctrl_constraints
            {
                { ctrl::octave, { 0.00f, 0.000f }},
                { ctrl::detune, { 0.25f, 0.175f }},
                { ctrl::pll,    { 0.75f, 0.175f }},
                { ctrl::pwm,    { 0.25f, 0.425f }},
                { ctrl::fm,     { 0.75f, 0.425f }},
                { ctrl::am,     { 0.25f, 0.625f }},
                { ctrl::amp,    { 0.75f, 0.625f }}

            };

            const std::map<interface::vco::in, point<float>> in_constraints
            {
                { in::detune, { 0.375f, 0.8f }},
                { in::pll,    { 0.625f, 0.8f }},
                { in::pwm,    { 0.125f, 0.8f }},
                { in::fm,     { 0.875f, 0.8f }},
                { in::am,     { 0.250f, 0.9f }},
            };

            const std::map<interface::vco::out, point<float>> out_constraints
            {
                { out::main,  { 0.750f, 0.9f }},
            };
            
            const float wh_ratio { 0.5f };         // Width/Height 


        }
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

        public:

            static int idc;                                 // ID counter
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

