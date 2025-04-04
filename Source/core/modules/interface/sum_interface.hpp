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
        namespace sum 
        {
            const int cc { 0 };
            const int ic { 2 };
            const int oc { 2 };

            struct ctl { enum {                                           }; };              // Controls
            struct cvi { enum { a, b                                      }; };              // CV in
            struct cvo { enum { a, b                                      }; };              // CV out

            const std::string prefix    { "sum"                              };
            
            const core::control_t set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  12.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  12.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const core::control_t set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  50.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  50.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };

            const core::control_t set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f,  76.0f,  59.0f };
            
            const core::descriptor_t descriptor  
            { 
                module_type::sum, 
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

