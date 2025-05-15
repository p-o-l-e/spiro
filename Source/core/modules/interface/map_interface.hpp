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
        namespace cso 
        {
            const int cc { 5 };
            const int ic { 2 };
            const int oc { 3 };

            struct ctl { enum { tune, warp, amp, form, options    }; };             // Controls
            struct cvi { enum { fm, warp                          }; };             // CV in
            struct cvo { enum { x, y, z                           }; };             // CV out

            const std::string prefix    { "cso"                                             };
            
            const core::Control set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  51.00f, 238.00f,  16.00f,  16.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  12.00f, 238.00f,  16.00f,  16.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const core::Control set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  31.00f, 268.00f,  16.00f,  16.00f }, "x"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  31.00f, 298.00f,  16.00f,  16.00f }, "y"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  31.00f, 328.00f,  16.00f,  16.00f }, "z"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };

            const core::Control set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  14.00f,  32.00f,  48.00f,  48.00f }, "tune"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
                { Control::type::slider   , {  14.00f, 108.00f,  48.00f,  48.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
                { Control::type::slider   , {  22.00f, 177.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.00f, 2.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::push     , {  58.00f,   5.00f,  12.00f,  12.00f }, "options", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, 0x00000000  },
            };
            
            const Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 352.0f };
            
            const core::Descriptor descriptor  
            { 
                core::map::module::type::cso, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i, set_o, set_c }, 
                &constrain
            };
        }
    }
}

