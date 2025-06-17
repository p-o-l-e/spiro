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
    namespace com 
    {
        constexpr int cc { 0 };
        constexpr int ic { 0 };
        constexpr int oc { 2 };

        struct cvo { enum { p_wheel, m_wheel                          }; };              // CV out

        constexpr std::string prefix { "com" };
        
        constexpr core::Control set_i[ic]
        {
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  31.00f,   6.00f,  16.00f,  16.00f }, "p_wheel", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  31.00f,  36.00f,  16.00f,  16.00f }, "m_wheel", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        constexpr core::Control set_c[cc]
        {
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f,  89.0f };
        
        constexpr core::Descriptor descriptor
        {
            core::map::module::type::com, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}

