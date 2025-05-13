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
        namespace cro 
        {
            const int cc { 10 };
            const int ic {  0 };
            const int oc {  0 };

            struct ctl { enum { sa, sb, sc, sd, se, sf, sg, sh, menu, scope   }; };              // Controls
            struct cvi { enum {                                               }; };              // CV in
            struct cvo { enum {                                               }; };              // CV out

            const std::string prefix    { "cro"                                  };
            
            const core::Control set_i[ic]
            {
            };
            
            const core::Control set_o[oc]
            {
            // -- TYPE ---------------------------- X ------ Y ------ W ------ H ------ ID ------- MIN -- MAX -- DEF -- SKEW - STEP -- RAD - SYM -- FLAG --------
            };

            const core::Control set_c[cc]
            {
                { Control::type::momentary, {   8.00f,  70.00f,  16.00f,  16.00f }, "soft_a" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::momentary, {   8.00f, 100.00f,  16.00f,  16.00f }, "soft_b" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::momentary, {   8.00f, 130.00f,  16.00f,  16.00f }, "soft_c" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::momentary, {   8.00f, 160.00f,  16.00f,  16.00f }, "soft_d" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },

                { Control::type::momentary, {  85.00f, 220.00f,  16.00f,  16.00f }, "soft_e" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::momentary, { 115.00f, 220.00f,  16.00f,  16.00f }, "soft_f" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::momentary, { 145.00f, 220.00f,  16.00f,  16.00f }, "soft_g" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
                { Control::type::momentary, { 175.00f, 220.00f,  16.00f,  16.00f }, "soft_h" , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x00000000  },
               
                { Control::type::radio    , {  10.00f, 226.00f,  12.00f,  12.00f }, "menu"   , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x000000FF  },
                { Control::type::radio    , { 205.00f, 226.00f,  12.00f,  12.00f }, "scope"  , 0.00f, 1.00f, 0.00f, 0.50f, 0.000f, 0x00, false, 0x000000FF  },
            };
            
            const Rectangle<float> constrain { 0.0f, 0.0f, 247.0f, 270.0f };
            
            const core::Descriptor descriptor
            {
                core::map::module::type::cro, 
                { &ic, &oc, &cc },
                &prefix, 
                { set_i, set_o, set_c }, 
                &constrain
            };
        }
    }
}

