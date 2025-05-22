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
        const int cc { 0 };
        const int ic { 3 };
        const int oc { 1 };

        struct ctl { enum {                                   }; };             // Controls
        struct cvi { enum { a, b, c                           }; };             // CV in
        struct cvo { enum { x                                 }; };             // CV out

        const std::string prefix    { "pdt"                                             };
        
        const core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::socket   , {  12.00f,   7.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::socket   , {  50.00f,   7.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::socket   , {  88.00f,   7.00f,  16.00f,  16.00f }, "c"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        const core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::socket   , { 126.00f,   7.00f,  16.00f,  16.00f }, "x"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        const core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
        };
        
        const Rectangle<float> constrain { 0.0f, 0.0f, 152.0f,  30.0f };
        
        const core::Descriptor descriptor  
        { 
            core::map::module::type::pdt, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}

