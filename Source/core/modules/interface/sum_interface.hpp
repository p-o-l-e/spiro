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
    namespace sum 
    {
        const int cc { 0 };
        const int ic { 2 };
        const int oc { 2 };
        const int vc { 1 };

        struct ctl { enum {                                           }; };              // Controls
        struct cvi { enum { a, b                                      }; };              // CV in
        struct cvo { enum { a, b                                      }; };              // CV out

        const std::string prefix    { "sum"                              };
        
        const core::Control set_i[vc][ic]
        {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            {
                { Control::type::socket   , {  12.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  12.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };
        
        const core::Control set_o[vc][oc]
        {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            {
                { Control::type::socket   , {  50.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::socket   , {  50.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };

        const core::Control set_c[vc][cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
        };
        
        const Rectangle<float> constrain { 0.0f, 0.0f,  76.0f,  59.0f };
        
        const core::Descriptor descriptor[vc]
        { 
            core::Descriptor {
                core::map::module::type::sum, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i[0], set_o[0], set_c[0] }, 
                &constrain
            }
        };
    }
}

