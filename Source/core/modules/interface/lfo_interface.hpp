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
    namespace lfo 
    {
        constexpr int cc { 6 };
        constexpr int ic { 2 };
        constexpr int oc { 2 };

        struct ctl { enum { octave, delta, amp, form, scale, options    }; };             // Controls
        struct cvi { enum { fm, am                                      }; };             // CV in
        struct cvo { enum { a, b                                        }; };             // CV out

        constexpr std::string prefix { "lfo" };
        
        constexpr core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN --- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::input    , {  11.00f,  67.00f,  16.00f,  16.00f }, "fm"     , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  49.00f,  67.00f,  16.00f,  16.00f }, "am"     , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN --- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  30.00f,   7.00f,  16.00f,  16.00f }, "a"      , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  30.00f,  37.00f,  16.00f,  16.00f }, "b"      , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        constexpr core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN --- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "octave" , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000         },
            { Control::type::slider   , {  22.00f, 106.00f,  32.00f,  32.00f }, "delta"  , 0.001f, 1.00f, 0.01f, 0.20f, 0.001f, 0x00, false, map::flag::B       },
            { Control::type::slider   , {  22.00f, 166.00f,  32.00f,  32.00f }, "amp"    , 0.000f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B       },
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.000f, 4.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000         },
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.000f, 4.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000         },
            { Control::type::button   , {  60.00f, 227.00f,  12.00f,  12.00f }, "options", 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, map::flag::radio   },
        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f,  76.0f, 244.0f };
        
        constexpr core::Descriptor descriptor  
        { 
            core::map::module::type::lfo, 
            { &ic, &oc, &cc },
            &prefix, 
            { set_i, set_o, set_c }, 
            &constrain
        };

       /**********************************************************************************************************************
        * 
        *  Options
        * 
        **********************************************************************************************************************/
        constexpr std::string_view parameterId[]    = { "FORM :", 
                                                        "SCALE:" };
        constexpr Options::type parameterType[]     = { Options::Choice, Options::Choice };
        constexpr std::string_view waveforms[]      = { "SINE", "SQUARE", "RAMP", "SAW", "TRIANGLE" };
        constexpr std::string_view scales[]         = { "1X", "2X", "3X", "4X", "5X" };
        constexpr const std::string_view* const choice[] = { waveforms, scales };
        constexpr uint8_t parameterPosition[] = { static_cast<uint8_t>(ctl::form), static_cast<uint8_t>(ctl::scale) };
        constexpr Options options 
        { 
            "LFO", 
            parameterId, 
            parameterType,
            parameterPosition, 
            2, 
            choice 
        };
    }
}

