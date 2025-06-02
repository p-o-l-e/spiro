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
    namespace lfo 
    {
        constexpr int cc { 5 };
        constexpr int ic { 2 };
        constexpr int oc { 2 };

        struct ctl { enum { octave, delta, amp, form, options }; };             // Controls
        struct cvi { enum { fm, am                            }; };             // CV in
        struct cvo { enum { a, b                              }; };             // CV out

        constexpr std::string prefix    { "lfo"                      };
        
        constexpr core::Control set_i[ic]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN --- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::input    , {  12.00f,  66.00f,  16.00f,  16.00f }, "fm"     , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::input    , {  50.00f,  66.00f,  16.00f,  16.00f }, "am"     , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };
        
        constexpr core::Control set_o[oc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN --- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::output   , {  30.00f,   6.00f,  16.00f,  16.00f }, "a"      , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::output   , {  30.00f,  36.00f,  16.00f,  16.00f }, "b"      , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
        };

        constexpr core::Control set_c[cc]
        {
        // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN --- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "octave" , 0.000f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
            { Control::type::slider   , {  22.00f, 106.00f,  32.00f,  32.00f }, "delta"  , 0.001f, 1.00f, 0.01f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            { Control::type::slider   , {  22.00f, 166.00f,  32.00f,  32.00f }, "amp"    , 0.000f, 1.00f, 0.00f, 0.20f, 0.001f, 0x00, false, map::flag::B  },
            { Control::type::parameter, {   0.00f,   0.00f,   0.00f,   0.00f }, "form"   , 0.000f, 2.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
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
    }
}

