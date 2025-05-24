#pragma once
#include "descriptor.hxx"
#include <string>

namespace core 
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
        
        const core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::input    , {  15.00f,  28.00f,  16.00f,  16.00f }, "ax"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  53.00f,  28.00f,  16.00f,  16.00f }, "ay"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  91.00f,  28.00f,  16.00f,  16.00f }, "az"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  15.00f,  58.00f,  16.00f,  16.00f }, "bx"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  53.00f,  58.00f,  16.00f,  16.00f }, "by"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  91.00f,  58.00f,  16.00f,  16.00f }, "bz"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  15.00f, 148.00f,  16.00f,  16.00f }, "cvx"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  53.00f, 148.00f,  16.00f,  16.00f }, "cvy"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  91.00f, 148.00f,  16.00f,  16.00f }, "cvz"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        const core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  34.00f,  88.00f,  16.00f,  16.00f }, "ax"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  72.00f,  88.00f,  16.00f,  16.00f }, "ay"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , { 110.00f,  88.00f,  16.00f,  16.00f }, "az"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  34.00f, 118.00f,  16.00f,  16.00f }, "bx"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  72.00f, 118.00f,  16.00f,  16.00f }, "by"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , { 110.00f, 118.00f,  16.00f,  16.00f }, "bz"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },

        };

        const core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::slider   , {   8.00f, 187.00f,  32.00f,  32.00f }, "x"      , 0.00f,   tao, 0.00f, 1.00f, 0.001f, 0x00, false, map::flag::encoder  },
            { Control::type::slider   , {  46.00f, 187.00f,  32.00f,  32.00f }, "y"      , 0.00f,   tao, 0.00f, 1.00f, 0.001f, 0x00, false, map::flag::encoder  },
            { Control::type::slider   , {  84.00f, 187.00f,  32.00f,  32.00f }, "z"      , 0.00f,   tao, 0.00f, 1.00f, 0.001f, 0x00, false, map::flag::encoder  },
        };
        
        const Rectangle<float> constrain { 0.0f, 0.0f, 128.0f, 326.0f };
        
        const core::Descriptor descriptor  
        { 
            core::map::module::type::rtr, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}

