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
    namespace vcf 
    {
        constexpr int cc { 2 };
        constexpr int ic { 5 };
        constexpr int oc { 3 };

        struct ctl { enum { cutoff, Q                                 }; };              // Controls
        struct cvi { enum { a, b, c, cutoff, Q                        }; };              // CV in
        struct cvo { enum { lp, bp, hp                                }; };              // CV out

        constexpr std::string prefix    { "vcf"                            };
        
        constexpr core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::input    , {  11.00f,  37.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  11.00f,  67.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  11.00f,  97.00f,  16.00f,  16.00f }, "c"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  11.00f, 127.00f,  16.00f,  16.00f }, "cutoff" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  49.00f, 127.00f,  16.00f,  16.00f }, "Q"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  49.00f,  37.00f,  16.00f,  16.00f }, "lp"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  49.00f,  67.00f,  16.00f,  16.00f }, "bp"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  49.00f,  97.00f,  16.00f,  16.00f }, "hp"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        constexpr core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::slider   , {  22.00f, 167.00f,  32.00f,  32.00f }, "cutoff" , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            { Control::type::slider   , {  22.00f, 228.00f,  32.00f,  32.00f }, "Q"      , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 304.0f };
        
        constexpr core::Descriptor descriptor  
        { 
            core::map::module::type::vcf, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}


