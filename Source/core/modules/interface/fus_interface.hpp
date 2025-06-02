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
        constexpr int cc { 0 };
        constexpr int ic { 0 };
        constexpr int oc { 0 };

        struct ctl { enum {                                           }; };              // Controls
        struct cvi { enum {                                           }; };              // CV in
        struct cvo { enum {                                           }; };              // CV out

        constexpr std::string prefix    { "fuse"                             };
        
        constexpr core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
        };

        constexpr core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
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

