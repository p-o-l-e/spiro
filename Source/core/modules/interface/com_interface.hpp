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
        const int cc { 0 };
        const int ic { 0 };
        const int oc { 2 };

        struct ctl { enum {                                           }; };              // Controls
        struct cvi { enum {                                           }; };              // CV in
        struct cvo { enum { p_wheel, m_wheel                          }; };              // CV out

        const std::string prefix    { "com"                              };
        
        const core::Control set_i[ic]
        {
        };
        
        const core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  31.00f,   6.00f,  16.00f,  16.00f }, "p_wheel", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  31.00f,  36.00f,  16.00f,  16.00f }, "m_wheel", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        const core::Control set_c[cc]
        {
        };
        
        const Rectangle<float> constrain { 0.0f, 0.0f,  76.0f,  89.0f };
        
        const core::Descriptor descriptor
        {
            core::map::module::type::snh, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };
    }
}

