#pragma once
#include "descriptor.hxx"
#include <string>

namespace core 
{
    namespace interface 
    {
        /**********************************************************************************************************************
        * 
        *  Descriptor
        * 
        **********************************************************************************************************************/
        namespace fuse 
        {
            const int cc { 0 };
            const int ic { 0 };
            const int oc { 0 };

            struct ctl { enum {                                           }; };              // Controls
            struct cvi { enum {                                           }; };              // CV in
            struct cvo { enum {                                           }; };              // CV out

            const std::string prefix    { "fuse"                             };
            
            const core::control_t set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            };
            
            const core::control_t set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            };

            const core::control_t set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f,   0.0f,   0.0f };
            
            const core::Descriptor descriptor  
            { 
                core::map::module::type::fuse, 
                &ic, &oc, &cc,
                &prefix, 
                set_i, 
                set_o, 
                set_c, 
                &constrain
            };
        }
    }
}

