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
        const int cc { 1 };
        const int ic { 3 };
        const int oc { 2 };
        const int vc { 2 }; // Variants

        struct ctl { enum { amp                               }; };              // Controls
        struct cvi { enum { a, b, amp                         }; };              // CV in
        struct cvo { enum { a, b                              }; };              // CV out

        const std::string prefix    { "vca"                      };
        
        const core::Control set_i[vc][ic]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  12.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  12.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  12.00f, 126.00f,  16.00f,  16.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  50.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  50.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  50.00f, 126.00f,  16.00f,  16.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };
        
        const core::Control set_o[vc][oc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  12.00f,  66.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  12.00f,  96.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::socket   , {  50.00f,  66.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  50.00f,  96.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };

        const core::Control set_c[vc][cc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  22.00f, 166.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  22.00f, 226.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
            }
        };
        
        const Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 304.0f };
        
        const core::Descriptor descriptor[vc]  
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

