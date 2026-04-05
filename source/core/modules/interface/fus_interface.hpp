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
    namespace fuse 
    {
        constexpr unsigned cc { 1 };
        constexpr unsigned ic { 1 };
        constexpr unsigned oc { 1 };

        struct ctl { enum id {                                           }; };              // Controls
        struct cvi { enum id {                                           }; };              // CV in
        struct cvo { enum id {                                           }; };              // CV out

        constexpr std::string_view prefix    { "fuse"                             };
        
        const core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::dummy   , {   0.00f,   0.00f,   0.00f,   0.00f }, "dummy"   , 0.00f, 0.00f, 0.00f, 0.00f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        const core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::dummy   , {   0.00f,   0.00f,   0.00f,   0.00f }, "dummy"   , 0.00f, 0.00f, 0.00f, 0.00f, 0.000f, 0x00, false, 0x00000000  },
        };

        const core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::dummy   , {   0.00f,   0.00f,   0.00f,   0.00f }, "dummy"   , 0.00f, 0.00f, 0.00f, 0.00f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,   0.0f,   0.0f };
        
        constexpr core::Descriptor descriptor  
        { 
            core::map::module::type::off, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}

