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
        namespace env 
        {
            const int cc { 16 };
            const int ic {  0 };
            const int oc {  1 };
            const int vc {  4 }; // Variants

            struct ctl { enum { at, ht, dt, st, rt, aa, ha, da, sa, ra, af, hf, df, sf, rf, scale }; };              // Controls
            struct cvi { enum {                                                                   }; };              // CV in
            struct cvo { enum { a                                                                 }; };              // CV out

            const std::string prefix    { "env" };
            
            const core::control_t set_i[ic]
            {
            };
            
            const core::control_t set_o[vc][oc]
            {
                {
                // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                    { control_t::type_t::socket   , {  88.00f,   7.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                },
                {
                    { control_t::type_t::socket   , { 126.00f,   7.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                },
                {
                    { control_t::type_t::socket   , { 164.00f,   7.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                },
                {
                    { control_t::type_t::socket   , { 202.00f,   7.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                }
            };

            const core::control_t set_c[cc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "at"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "ht"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "dt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "st"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "rt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },

                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "aa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "ha"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "da"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "sa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },
                { control_t::type_t::slider   , {   0.00f,   0.00f,   0.00f,   0.00f }, "ra"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C  },

                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "af"     , 0.00f, 4.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "hf"     , 0.00f, 4.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "df"     , 0.00f, 4.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sf"     , 0.00f, 4.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000  },
                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rf"     , 0.00f, 4.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000  },

                { control_t::type_t::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.00f, 8.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x00000000  },
            };
            
            const rectangle<float> constrain { 0.0f, 0.0f, 228.0f, 215.0f };
            
            const core::descriptor_t descriptor[vc] 
            { 
                {
                    core::map::module::type::env, 
                    &ic, &oc, &cc,
                    &prefix, 
                    set_i, 
                    set_o[0], 
                    set_c, 
                    &constrain
                },
                {
                    core::map::module::type::env, 
                    &ic, &oc, &cc,
                    &prefix, 
                    set_i, 
                    set_o[1], 
                    set_c, 
                    &constrain
                },
                {
                    core::map::module::type::env, 
                    &ic, &oc, &cc,
                    &prefix, 
                    set_i, 
                    set_o[2], 
                    set_c, 
                    &constrain
                },
                {
                    core::map::module::type::env, 
                    &ic, &oc, &cc,
                    &prefix, 
                    set_i, 
                    set_o[3], 
                    set_c, 
                    &constrain
                },

            };
        }
    }
}

