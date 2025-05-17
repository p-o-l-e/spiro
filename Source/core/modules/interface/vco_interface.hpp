#pragma once
#include "descriptor.hxx"
#include <string>

namespace core 
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace vco 
        {
            const int cc { 10 };
            const int ic {  5 };
            const int oc {  1 };

            struct ctl { enum { octave, detune, pll, pwm, fm, am, amp, form, freerun, options }; };              // Controls
            struct cvi { enum {         detune, pll, pwm, fm, am                              }; };              // CV in
            struct cvo { enum {         main                                                  }; };              // CV out

            const std::string prefix    { "vco"                                                 };
            
            const core::Control set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  50.00f, 238.00f,  16.00f,  16.00f }, "detune" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  88.00f, 238.00f,  16.00f,  16.00f }, "pll"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  12.00f, 238.00f,  16.00f,  16.00f }, "pwm"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , { 126.00f, 238.00f,  16.00f,  16.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  31.00f, 268.00f,  16.00f,  16.00f }, "am"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const core::Control set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , { 107.00f, 268.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };

            const core::Control set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "octave" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::slider   , {  14.00f,  32.00f,  48.00f,  48.00f }, "detune" , 0.00f, 1.00f, 0.50f, 1.00f, 0.001f, 0x00, true , 0x0000000A  },
                { Control::type::slider   , {  90.00f,  32.00f,  48.00f,  48.00f }, "pll"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
                { Control::type::slider   , {  14.00f, 108.00f,  48.00f,  48.00f }, "pwm"    , 0.00f, 1.00f, 0.50f, 1.30f, 0.001f, 0x00, true , 0x0000000A  },
                { Control::type::slider   , {  90.00f, 108.00f,  48.00f,  48.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
                { Control::type::slider   , {  22.00f, 177.00f,  32.00f,  32.00f }, "am"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                { Control::type::slider   , {  98.00f, 177.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.00f, 2.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "freerun", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::push     , { 130.00f,   5.00f,  12.00f,  12.00f }, "options", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, 0x00000000  },
            };
            
            const Rectangle<float> constrain { 0.0f, 0.0f, 152.0f, 292.0f };
            
            const core::Descriptor descriptor  
            { 
                core::map::module::type::vco, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i, set_o, set_c }, 
                &constrain
            };
        }
    }
}

