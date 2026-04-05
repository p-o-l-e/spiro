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
    namespace pdt 
    {
        constexpr unsigned cc { 1 };
        constexpr unsigned ic { 3 };
        constexpr unsigned oc { 1 };

        struct ctl { enum id {                                   }; };             // Controls
        struct cvi { enum id { a, b, c                           }; };             // CV in
        struct cvo { enum id { x                                 }; };             // CV out

        constexpr std::string_view prefix    { "pdt"                                             };
        
        const core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::input    , {  11.00f,   8.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  49.00f,   8.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  85.00f,   8.00f,  16.00f,  16.00f }, "c"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        const core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , { 125.00f,   8.00f,  16.00f,  16.00f }, "x"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        const core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::dummy   , {   0.00f,   0.00f,   0.00f,   0.00f }, "dummy"   , 0.00f, 0.00f, 0.00f, 0.00f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f, 152.0f,  30.0f };
        
        constexpr core::Descriptor descriptor  
        { 
            core::map::module::type::pdt, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}

