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
        constexpr int cc { 0 };
        constexpr int ic { 2 };
        constexpr int oc { 2 };
        constexpr int vc { 1 };

        struct ctl { enum {                                           }; };              // Controls
        struct cvi { enum { a, b                                      }; };              // CV in
        struct cvo { enum { a, b                                      }; };              // CV out

        constexpr std::string prefix    { "sum"                              };
        
        constexpr core::Control set_i[vc][ic]
        {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            {
                { Control::type::input    , {  12.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::input    , {  12.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }  
        };  
          
        constexpr core::Control set_o[vc][oc]
        {  
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            {
                { Control::type::output   , {  50.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::output   , {  50.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };

        constexpr core::Control set_c[vc][cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f,  59.0f };
        
        constexpr core::Descriptor descriptor[vc]
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

