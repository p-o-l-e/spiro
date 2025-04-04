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
        namespace vca 
        {
            const int cc { 1 };
            const int ic { 3 };
            const int oc { 2 };
            const int vc { 2 }; // Variants

            struct ctl { enum { amp                               }; };              // Controls
            struct cvi { enum { a, b, amp                         }; };              // CV in
            struct cvo { enum { a, b                              }; };              // CV out

            const std::string prefix    { "vca"                      };
            
            const core::control_t set_i[vc][ic]
            {
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::socket   , {  12.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                    { control_t::type_t::socket   , {  12.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                    { control_t::type_t::socket   , {  12.00f, 126.00f,  16.00f,  16.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                },
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::socket   , {  50.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                    { control_t::type_t::socket   , {  50.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                    { control_t::type_t::socket   , {  50.00f, 126.00f,  16.00f,  16.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                }
            };
            
            const core::control_t set_o[vc][oc]
            {
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::socket   , {  12.00f,  66.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                    { control_t::type_t::socket   , {  12.00f,  96.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                },
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::socket   , {  50.00f,  66.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                    { control_t::type_t::socket   , {  50.00f,  96.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                }
            };

            const core::control_t set_c[vc][cc]
            {
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::slider   , {  22.00f, 166.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                },
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::slider   , {  22.00f, 226.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                }
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 304.0f };
            
            const core::descriptor_t descriptor[vc]  
            { 
                core::descriptor_t {
                    module_type::vca, 
                    &ic, &oc, &cc,
                    &prefix, 
                    set_i[0], 
                    set_o[0], 
                    set_c[0], 
                    &constrain
                },
                core::descriptor_t {
                    module_type::vca, 
                    &ic, &oc, &cc,
                    &prefix, 
                    set_i[1], 
                    set_o[1], 
                    set_c[1], 
                    &constrain
                }
            };
        }
    }
}

