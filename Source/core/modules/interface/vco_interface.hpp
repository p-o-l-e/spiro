#pragma once
#include "descriptor.hxx"
#include <string>
#include <string_view>

namespace core 
{
    /**********************************************************************************************************************
    * 
    *  Descriptor
    * 
    **********************************************************************************************************************/
    namespace vco 
    {
        constexpr int cc { 10 };
        constexpr int ic {  5 };
        constexpr int oc {  1 };

        struct ctl { enum { octave, detune, pll, pwm, fm, am, amp, form, mode, options  }; };              // Controls
        struct cvi { enum {         detune, pll, pwm, fm, am                            }; };              // CV in
        struct cvo { enum {         main                                                }; };              // CV out

        constexpr std::string prefix { "vco" };
        
        constexpr core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF - SKEW -- STEP - RAD -- SYM ------ FLAG ----------
            { Control::type::input    , {  49.00f, 239.00f,  16.00f,  16.00f }, "detune" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000          },
            { Control::type::input    , {  87.00f, 239.00f,  16.00f,  16.00f }, "pll"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000          },
            { Control::type::input    , {  11.00f, 239.00f,  16.00f,  16.00f }, "pwm"    , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000          },
            { Control::type::input    , { 125.00f, 239.00f,  16.00f,  16.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000          },
            { Control::type::input    , {  30.00f, 269.00f,  16.00f,  16.00f }, "am"     , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000          },
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF - SKEW -- STEP - RAD -- SYM ------ FLAG ----------
            { Control::type::output   , { 106.00f, 269.00f,  16.00f,  16.00f }, "a"      , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000          },
        };

        constexpr core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF - SKEW -- STEP - RAD -- SYM ------ FLAG ----------
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "octave" , 0.00f, 5.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000          },
            { Control::type::slider   , {  14.00f,  32.00f,  48.00f,  48.00f }, "detune" , 0.00f, 1.00f, 0.50f, 1.00f, 0.001f, 0x00, true , map::flag::A        },
            { Control::type::slider   , {  90.00f,  32.00f,  48.00f,  48.00f }, "pll"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::A        },
            { Control::type::slider   , {  14.00f, 108.00f,  48.00f,  48.00f }, "pwm"    , 0.00f, 1.00f, 0.50f, 1.30f, 0.001f, 0x00, true , map::flag::A        },
            { Control::type::slider   , {  90.00f, 108.00f,  48.00f,  48.00f }, "fm"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::A        },
            { Control::type::slider   , {  22.00f, 177.00f,  32.00f,  32.00f }, "am"     , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B        },
            { Control::type::slider   , {  98.00f, 177.00f,  32.00f,  32.00f }, "amp"    , 0.00f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B        },
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.00f, 2.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000          },
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "mode"   , 0.00f, 2.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000          },
            { Control::type::button   , { 136.00f,   5.00f,  12.00f,  12.00f }, "options", 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, map::flag::radio    },
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f, 152.0f, 292.0f };
        
        constexpr core::Descriptor descriptor  
        { 
            core::map::module::type::vco, 
            { 
                &ic, 
                &oc, 
                &cc 
            },
            &prefix, 
            { 
                set_i, 
                set_o, 
                set_c 
            }, 
            &constrain
        };

       /**********************************************************************************************************************
        * 
        *  Options
        * 
        **********************************************************************************************************************/
        constexpr std::string_view parameterId[]    = { "FORM  :",
                                                        "MODE  :",
                                                        "OCTAVE:" };
        constexpr Options::type parameterType[]     = { Options::Choice, Options::Choice, Options::Integer };
        constexpr std::string_view waveforms[]      = { "SINE", "SQUARE", "HEXAGON" };
        constexpr std::string_view modes[]          = { "MONO", "FREERUN", "POLY" };
        constexpr const std::string_view* const choice[] = { waveforms, modes };
        constexpr uint8_t parameterPosition[] = { static_cast<uint8_t>(ctl::form), static_cast<uint8_t>(ctl::mode), static_cast<uint8_t>(ctl::octave) };
        constexpr Options options 
        { 
            "OSCILLATOR", 
            parameterId, 
            parameterType,
            parameterPosition, 
            3, 
            choice 
        };
    }
}

