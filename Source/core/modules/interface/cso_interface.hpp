#pragma once
#include "descriptor.hxx"
#include <cstdint>
#include <string>
#include <string_view>

namespace core 
{
    /**********************************************************************************************************************
    * 
    *  Descriptor
    * 
    **********************************************************************************************************************/
    namespace cso 
    {
        constexpr int cc { 5 };
        constexpr int ic { 2 };
        constexpr int oc { 3 };
        constexpr int vc { 2 };

        struct ctl { enum { tune, warp, amp, form, options    }; };             // Controls
        struct cvi { enum { fm, warp                          }; };             // CV in
        struct cvo { enum { x, y, z                           }; };             // CV out

        constexpr std::string prefix { "cso" };
        
        constexpr core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::input    , {  49.00f, 239.00f,  16.00f,  16.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  10.00f, 239.00f,  16.00f,  16.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  30.00f, 269.00f,  16.00f,  16.00f }, "x"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  30.00f, 299.00f,  16.00f,  16.00f }, "y"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  30.00f, 329.00f,  16.00f,  16.00f }, "z"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        constexpr core::Control set_c[vc][cc]
        {
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  14.00f,  32.00f,  48.00f,  48.00f }, "tune"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::A        },
                { Control::type::slider   , {  14.00f, 108.00f,  48.00f,  48.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::A        },
                { Control::type::slider   , {  22.00f, 177.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B        },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.00f, 3.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::button   , { 120.00f,   5.00f,  12.00f,  12.00f }, "options", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, map::flag::radio    },
            },
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
                { Control::type::slider   , {  14.00f,  32.00f,  48.00f,  48.00f }, "tune"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::A        },
                { Control::type::slider   , {  14.00f, 108.00f,  48.00f,  48.00f }, "warp"   , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::A        },
                { Control::type::slider   , {  22.00f, 177.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B        },
                { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.00f, 3.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000          },
                { Control::type::button   , {  60.00f,   5.00f,  12.00f,  12.00f }, "options", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, map::flag::radio    },
            }
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 352.0f };

        constexpr core::Descriptor descriptor[vc]
        { 
            core::Descriptor 
            {
                core::map::module::type::cso, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i, set_o, set_c[0] }, 
                &constrain
            },
            core::Descriptor 
            {
                core::map::module::type::cso, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i, set_o, set_c[1] }, 
                &constrain
            }
        };
       
       /**********************************************************************************************************************
        * 
        *  Options
        * 
        **********************************************************************************************************************/
        constexpr std::string_view parameterId[]    = { "FORM:" };
        constexpr Options::type parameterType[]     = { Options::Choice };
        constexpr std::string_view waveforms[]      = { "SPROTT", "HELMHOLZ", "HALVORSEN", "3SCROLL" };
        constexpr const std::string_view* const choice[] = { waveforms };
        constexpr uint8_t parameterPosition[] = { static_cast<uint8_t>(ctl::form) };
        constexpr Options options 
        { 
            "DYNAMIC SYSTEM", 
            parameterId, 
            parameterType,
            parameterPosition, 
            1, 
            choice 
        };
    }
}

