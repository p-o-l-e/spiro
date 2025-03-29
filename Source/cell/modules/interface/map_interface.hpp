#pragma once
#include "descriptor.hxx"
#include <string>

namespace cell 
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace map 
        {
            const int cc { 5 };
            const int ic { 2 };
            const int oc { 3 };
            const int mc { 2 };

            struct ctl { enum { tune, warp, amp, form, options    }; };             // Controls
            struct cvi { enum { fm, warp                          }; };             // CV in
            struct cvo { enum { x, y, z                           }; };             // CV out

            const std::string prefix    { "map"                                             };
            
            const cell::control_t set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  51.00f, 238.00f,  16.00f,  16.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  12.00f, 238.00f,  16.00f,  16.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const cell::control_t set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  31.00f, 268.00f,  16.00f,  16.00f }, "x"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  31.00f, 298.00f,  16.00f,  16.00f }, "y"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  31.00f, 328.00f,  16.00f,  16.00f }, "z"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };

            const cell::control_t set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::slider   , {  14.00f,  32.00f,  48.00f,  48.00f }, "tune"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
                { control_t::type_t::slider   , {  14.00f, 108.00f,  48.00f,  48.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000A  },
                { control_t::type_t::slider   , {  22.00f, 177.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.00f, 2.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::push     , {  58.00f,   5.00f,  12.00f,  12.00f }, "options", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, 0x00000000  },
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 352.0f };
            
            const cell::descriptor_t descriptor  
            { 
                module_type::map, 
                &ic, &oc, &cc, &mc,
                &prefix, 
                set_i, 
                set_o, 
                set_c, 
                &constrain
            };
        }
    }
}

