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
    namespace vca 
    {
        constexpr int cc { 1 };
        constexpr int ic { 3 };
        constexpr int oc { 2 };
        constexpr int vc { 2 }; // Variants

        struct ctl { enum { amp                               }; };              // Controls
        struct cvi { enum { a, b, amp                         }; };              // CV in
        struct cvo { enum { a, b                              }; };              // CV out

        constexpr std::string prefix    { "vca"                      };
        
        constexpr core::Control set_i[vc][ic]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::input    , {  12.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  12.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  12.00f, 126.00f,  16.00f,  16.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::input   , {  50.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input   , {  50.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input   , {  50.00f, 126.00f,  16.00f,  16.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };
        
        constexpr core::Control set_o[vc][oc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::output   , {  12.00f,  66.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::output   , {  12.00f,  96.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::output   , {  50.00f,  66.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::output   , {  50.00f,  96.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };

        constexpr core::Control set_c[vc][cc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  22.00f, 166.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  22.00f, 226.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            }
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 304.0f };
        
        constexpr core::Descriptor descriptor[vc]  
        { 
            core::Descriptor {
                core::map::module::type::vca, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i[0], set_o[0], set_c[0] }, 
                &constrain
            },
            core::Descriptor {
                core::map::module::type::vca, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i[1], set_o[1], set_c[1] }, 
                &constrain
            }
        };
    }
}

