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
    namespace cro 
    {
        constexpr int cc { 12 };
        constexpr int ic {  0 };
        constexpr int oc {  0 };

        struct ctl { enum { sa, sb, sc, sd, se, sf, sg, sh, menu, scope, type, scale    }; };              // Controls
        struct cvi { enum {                                                             }; };              // CV in
        struct cvo { enum {                                                             }; };              // CV out

        constexpr std::string prefix { "cro" };
        
        constexpr core::Control set_i[ic]
        {
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ----- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
        };

        constexpr core::Control set_c[cc]
        {
            { Control::type::button,    {   7.00f,  73.00f,  16.00f,  16.00f }, "soft_a" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
            { Control::type::button,    {   7.00f, 103.00f,  16.00f,  16.00f }, "soft_b" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
            { Control::type::button,    {   7.00f, 133.00f,  16.00f,  16.00f }, "soft_c" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
            { Control::type::button,    {   7.00f, 163.00f,  16.00f,  16.00f }, "soft_d" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },

            { Control::type::button,    {  78.00f, 222.00f,  16.00f,  16.00f }, "soft_e" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
            { Control::type::button,    { 108.00f, 222.00f,  16.00f,  16.00f }, "soft_f" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
            { Control::type::button,    { 138.00f, 222.00f,  16.00f,  16.00f }, "soft_g" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
            { Control::type::button,    { 168.00f, 222.00f,  16.00f,  16.00f }, "soft_h" , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, map::flag::momentary    },
           
            { Control::type::button,    {  12.00f, 226.00f,  12.00f,  12.00f }, "menu"   , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, map::flag::radio        },
            { Control::type::button,    { 204.00f, 226.00f,  12.00f,  12.00f }, "scope"  , 0.00f,  1.00f, 0.00f, 0.50f, 0.000f, 0xFF, false, map::flag::radio        },

            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "type"   , 0.00f,  1.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000              },
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "scale"  , 0.00f, 10.00f, 0.00f, 0.50f, 1.000f, 0x00, false, 0x00000000              },

        };
        
        constexpr Rectangle<float> constrain { 0.0f, 0.0f, 247.0f, 270.0f };
        
        constexpr core::Descriptor descriptor
        {
            core::map::module::type::cro, 
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
        constexpr std::string_view parameterId[]    = { "TYPE   :",
                                                        "SCALE  :" };
        constexpr Options::type parameterType[]     = { Options::Choice, Options::Integer };
        constexpr std::string_view modes[]          = { "2D", "3D" };
        constexpr const std::string_view* const choice[] = { modes };
        constexpr uint8_t parameterPosition[] = { static_cast<uint8_t>(ctl::type), static_cast<uint8_t>(ctl::scale) };
        constexpr Options options 
        { 
            "SCOPE", 
            parameterId, 
            parameterType,
            parameterPosition, 
            2, 
            choice 
        };

    }
}

