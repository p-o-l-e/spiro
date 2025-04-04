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
        namespace vcf 
        {
            const int cc { 2 };
            const int ic { 5 };
            const int oc { 3 };

            struct ctl { enum { cutoff, Q                                 }; };              // Controls
            struct cvi { enum { a, b, c, cutoff, Q                        }; };              // CV in
            struct cvo { enum { lp, bp, hp                                }; };              // CV out

            const std::string prefix    { "vcf"                            };
            
            const core::control_t set_i[ic]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  12.00f,  36.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  12.00f,  66.00f,  16.00f,  16.00f }, "b"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  12.00f,  96.00f,  16.00f,  16.00f }, "c"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  12.00f, 126.00f,  16.00f,  16.00f }, "cutoff" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  50.00f, 126.00f,  16.00f,  16.00f }, "Q"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };
            
            const core::control_t set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::socket   , {  50.00f,  36.00f,  16.00f,  16.00f }, "lp"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  50.00f,  66.00f,  16.00f,  16.00f }, "bp"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::socket   , {  50.00f,  96.00f,  16.00f,  16.00f }, "hp"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            };

            const core::control_t set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::slider   , {  22.00f, 167.00f,  32.00f,  32.00f }, "cutoff" , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
                { control_t::type_t::slider   , {  22.00f, 228.00f,  32.00f,  32.00f }, "Q"      , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, 0x0000000B  },
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 304.0f };
            
            const core::descriptor_t descriptor  
            { 
                module_type::vcf, 
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


