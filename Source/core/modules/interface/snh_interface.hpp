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
    namespace snh 
    {
        constexpr int cc { 1 };
        constexpr int ic { 3 };
        constexpr int oc { 1 };
        constexpr int vc { 2 }; // Variants

        struct ctl { enum { time                                      }; };              // Controls
        struct cvi { enum { time, a, b                                }; };              // CV in
        struct cvo { enum { a                                         }; };              // CV out

        constexpr std::string prefix    { "snh"                              };
        
        constexpr core::Control set_i[vc][ic]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::input    , {  11.00f, 127.00f,  16.00f,  16.00f }, "time"   , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  11.00f,  37.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  11.00f,  67.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::input    , {  49.00f, 127.00f,  16.00f,  16.00f }, "time"   , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  49.00f,  37.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  49.00f,  67.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };
        
        constexpr core::Control set_o[vc][oc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::output   , {  11.00f,  97.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::output   , {  49.00f,  97.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };

        constexpr core::Control set_c[vc][cc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  22.00f, 166.00f,  32.00f,  32.00f }, "time"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  22.00f, 226.00f,  32.00f,  32.00f }, "time"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            }
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 304.0f };
        
        constexpr core::Descriptor descriptor[vc]
        {
            {
                core::map::module::type::snh, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i[0], set_o[0], set_c[0] }, 
                &constrain
            },
            {
                core::map::module::type::snh, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i[1], set_o[1], set_c[1] }, 
                &constrain
            }
        };
    }
}

