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
        namespace rtr 
        {
            const int cc { 3 };
            const int ic { 9 };
            const int oc { 6 };

            struct ctl { enum { x, y, z                                   }; };             // Controls
            struct cvi { enum { ax, ay, az, bx, by, bz, cvx, cvy, cvz     }; };             // CV in
            struct cvo { enum { ax, ay, az, bx, by, bz                    }; };             // CV out

            const std::string prefix    { "rtr"                                             };
            
            const core::control_t set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  15.00f,  28.00f,  16.00f,  16.00f }, "ax"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  53.00f,  28.00f,  16.00f,  16.00f }, "ay"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  91.00f,  28.00f,  16.00f,  16.00f }, "az"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  15.00f,  58.00f,  16.00f,  16.00f }, "bx"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  53.00f,  58.00f,  16.00f,  16.00f }, "by"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  91.00f,  58.00f,  16.00f,  16.00f }, "bz"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  15.00f, 148.00f,  16.00f,  16.00f }, "cvx"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  53.00f, 148.00f,  16.00f,  16.00f }, "cvy"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  91.00f, 148.00f,  16.00f,  16.00f }, "cvz"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const core::control_t set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  34.00f,  88.00f,  16.00f,  16.00f }, "ax"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  72.00f,  88.00f,  16.00f,  16.00f }, "ay"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , { 110.00f,  88.00f,  16.00f,  16.00f }, "az"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  34.00f, 118.00f,  16.00f,  16.00f }, "bx"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  72.00f, 118.00f,  16.00f,  16.00f }, "by"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , { 110.00f, 118.00f,  16.00f,  16.00f }, "bz"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },

            };

            const core::control_t set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::slider   , {   8.00f, 187.00f,  32.00f,  32.00f }, "x"      , 0.00f,   tao, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000E  },
                { control_t::type_t::slider   , {  46.00f, 187.00f,  32.00f,  32.00f }, "y"      , 0.00f,   tao, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000E  },
                { control_t::type_t::slider   , {  84.00f, 187.00f,  32.00f,  32.00f }, "z"      , 0.00f,   tao, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000E  },
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f, 128.0f, 326.0f };
            
            const core::descriptor_t descriptor  
            { 
                core::map::module::type::rtr, 
                &ic, &oc, &cc,
                &prefix, 
                set_i, 
                set_o, 
                set_c, 
                &constrain
            };
        }
    }
}

