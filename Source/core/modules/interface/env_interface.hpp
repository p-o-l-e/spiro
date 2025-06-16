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
    namespace env 
    {
        constexpr int cc { 17 };
        constexpr int ic {  0 };
        constexpr int oc {  1 };
        constexpr int vc {  4 }; // Variants

        struct ctl { enum { at, ht, dt, st, rt, aa, ha, da, sa, ra, af, hf, df, sf, rf, scale, select }; };         // Controls
        struct cvi { enum {     }; };                                                                               // CV in
        struct cvo { enum { a   }; };                                                                               // CV out
        
        constexpr int idxl[] { ctl::aa, ctl::ha, ctl::da, ctl::sa, ctl::ra };
        constexpr int idxt[] { ctl::at, ctl::ht, ctl::dt, ctl::st, ctl::rt };
        constexpr int idxf[] { ctl::af, ctl::hf, ctl::df, ctl::sf, ctl::rf };

        constexpr std::string prefix    { "env" };
        
        constexpr core::Control set_i[ic]
        {
        };
        
        constexpr core::Control set_o[vc][oc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::output   , {  87.00f,   8.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
                { Control::type::output   , { 125.00f,   8.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
                { Control::type::output   , { 163.00f,   8.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            },
            {
                { Control::type::output   , { 201.00f,   8.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            }
        };

        constexpr core::Control set_c[vc][cc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ---- ID ------- MIN -- MAX -- DEF - SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "at"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ht"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "dt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "st"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "aa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ha"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "da"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ra"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "af"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "hf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "df"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },

                { Control::type::slider,    {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.33f, 9.99f, 1.00f, 0.50f, 0.010f, 0x00, false, map::flag::fader    },
                { Control::type::button,    {  89.00f,  56.00f,  12.00f,  12.00f }, "select" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xEE, false, map::flag::radio    },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ---- ID ------- MIN -- MAX -- DEF - SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "at"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ht"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "dt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "st"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "aa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ha"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "da"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ra"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "af"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "hf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "df"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },

                { Control::type::slider,    {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.33f, 9.99f, 1.00f, 0.50f, 0.010f, 0x00, false, map::flag::fader    },
                { Control::type::button,    { 127.00f,  56.00f,  12.00f,  12.00f }, "select" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xEE, false, map::flag::radio    },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ---- ID ------- MIN -- MAX -- DEF - SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "at"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ht"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "dt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "st"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "aa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ha"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "da"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ra"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "af"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "hf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "df"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },

                { Control::type::slider,    {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.33f, 9.99f, 1.00f, 0.50f, 0.010f, 0x00, false, map::flag::fader    },
                { Control::type::button,    { 165.00f,  56.00f,  12.00f,  12.00f }, "select" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xEE, false, map::flag::radio    },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ---- ID ------- MIN -- MAX -- DEF - SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "at"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ht"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "dt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "st"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rt"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "aa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ha"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "da"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sa"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "ra"     , 0.00f, 1.00f, 0.00f, 1.00f, 0.001f, 0x00, false, 0x0000000C          },

                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "af"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "hf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "df"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "sf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "rf"     , 0.00f, 3.00f, 0.00f, 1.00f, 1.000f, 0x00, false, 0x00000000          },

                { Control::type::slider,    {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.33f, 9.99f, 1.00f, 0.50f, 0.010f, 0x00, false, map::flag::fader    },
                { Control::type::button,    { 203.00f,  56.00f,  12.00f,  12.00f }, "select" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xEE, false, map::flag::radio    },
            },

        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f, 228.0f, 215.0f };
        
        constexpr core::Descriptor descriptor[vc] 
        { 
            {
                core::map::module::type::env, 
                { 
                    &ic, 
                    &oc, 
                    &cc 
                },
                &prefix, 
                { 
                    set_i, 
                    set_o[0], 
                    set_c[0] 
                }, 
                &constrain
            },
            {
                core::map::module::type::env, 
                { 
                    &ic, 
                    &oc, 
                    &cc 
                },
                &prefix, 
                { 
                    set_i, 
                    set_o[1], 
                    set_c[1] 
                }, 
                &constrain                
            },
            {
                core::map::module::type::env, 
                { 
                    &ic, 
                    &oc, 
                    &cc 
                },
                &prefix, 
                { 
                    set_i, 
                    set_o[2], 
                    set_c[2] 
                }, 
                &constrain                
            },
            {
                core::map::module::type::env, 
                { 
                    &ic, 
                    &oc, 
                    &cc 
                },
                &prefix, 
                { 
                    set_i, 
                    set_o[3], 
                    set_c[3] 
                }, 
                &constrain                
            },
        };
    }
}

