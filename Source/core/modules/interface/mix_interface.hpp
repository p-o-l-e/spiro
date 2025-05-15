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
        namespace mix 
        {
            const int cc { 3 };
            const int ic { 5 };
            const int oc { 2 };

            struct ctl { enum { alpha, theta, amp,                }; };              // Controls
            struct cvi { enum { l, c, r, alpha, theta             }; };             // CV in
            struct cvo { enum { l, r                              }; };             // CV out

            const std::string prefix    { "mix"                                             };
            
            const core::Control set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  32.00f,  13.00f,  16.00f,  16.00f }, "l"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  32.00f,  43.00f,  16.00f,  16.00f }, "c"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  32.00f,  73.00f,  16.00f,  16.00f }, "r"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {   2.00f,  28.00f,  16.00f,  16.00f }, "alpha"  , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {   2.00f,  58.00f,  16.00f,  16.00f }, "theta"  , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const core::Control set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::pin      , {    0.0f,    0.0f,    0.0f,    0.0f }, "l"      , 0.00f, 0.00f, 0.00f, 0.00f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::pin      , {    0.0f,    0.0f,    0.0f,    0.0f }, "r"      , 0.00f, 0.00f, 0.00f, 0.00f, 0.000f, 0x00, false, 0x00000000  },
            };

            const core::Control set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  67.00f,  17.00f,  32.00f,  32.00f }, "alpha"  , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000E  },
                { Control::type::slider   , {  67.00f,  53.00f,  32.00f,  32.00f }, "theta"  , 0.00f, 1.00f, 0.50f, 1.00f, 0.001f, 0x00, false, 0x0000000E  },
                { Control::type::slider   , {  57.00f, 179.00f,  48.00f,  48.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
            };
            
            const Rectangle<float> constrain { 0.0f, 0.0f, 119.0f, 326.0f };
            
            const core::Descriptor descriptor  
            { 
                core::map::module::type::mix, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i, set_o, set_c }, 
                &constrain
            };
        }
    }
}

